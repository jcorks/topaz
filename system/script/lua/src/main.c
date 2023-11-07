/*
Copyright (c) 2020, Johnathan Corkery. (jcorkery@umich.edu)
All rights reserved.

This file is part of the topaz project (https://github.com/jcorks/topaz)
topaz was released under the MIT License, as detailed below.



Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is furnished 
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall
be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.


*/




#include "backend.h"
#include <topaz/system.h>
#include <topaz/version.h>
#include <topaz/containers/array.h>
#include <topaz/modules/console.h>
#include <topaz/topaz.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef TOPAZDC_DEBUG
    #include <assert.h>
#endif
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"


void PLOG(topaz_t * t, topazString_t * str) {
    topaz_console_print_message(
        topaz_context_get_console(t),
        str,
        topazConsole_MessageType_Warning
    );
    topaz_string_destroy(str);
}


void PWARN(topaz_t * t, topazString_t * str) {
    topaz_console_print_message(
        topaz_context_get_console(t),
        str,
        topazConsole_MessageType_Warning
    );
    topaz_string_destroy(str);
}


void PERROR(topaz_t * t, topazString_t * str) {
    topazConsole_t * console = topaz_context_get_console(t);
    topaz_console_enable(console, TRUE);

    topaz_console_print_message(
        topaz_context_get_console(t),
        str,
        topazConsole_MessageType_Error
    );
    topaz_string_destroy(str);
}

typedef struct {
    // all TOPAZLUA * instances active.
    topazArray_t * instances;

} TOPAZLUABackend;


static uint32_t stashPool = 1;


typedef struct TOPAZLUA TOPAZLUA;

// The tag is the data that "follows" a script object and is unique 
// to that object. Since you can have multiple object references 
// refer to the same object, the tag for that object should always be
// the same for all those references.
//
typedef struct {
    
    // used for set/get native
    void * nativeData;

    // also used for set/get native
    int tagID;

    // reference counting for when to lock the object to prevent garbage collection
    int refCount;

    // the source context
    TOPAZLUA * ctx;

    // populated if the object was created from C
    topaz_script_native_function cfinalizer;

    void * cfinalizerData;

    // used for finding the tag in the global stash
    uint32_t stashID;
} TOPAZLUAObjectTag;

struct TOPAZLUA {
    lua_State * lua;
    topazScript_t * script;
    // used for object reference creation
    int lastObjectReference;
    topaz_t * ctx;



    //// debugging

    // whether debugging has started
    int isDebugging;


    // Whether the debug state is waiting for the initial cooperation message 
    // to be processed.
    int debugReachedInit;

    // sent commands t
    topazArray_t * lastCommand;
    
    // string args for commands in order.
    topazArray_t * lastCommandArgs;


    // state of the defbug
    topazScript_DebugState_t debugState;

    // queue of breakpoints requested
    // Maps the "duktape" index to the lua id
    topazArray_t * debugBreakpoints;

    // queue of breakpoints requested to be removed.
    topazArray_t * debugRemoveBreakpoint;

    // queue of notifications to send to the script instance.
    // theyre queued until ALL intermediate steps are finished, allowing 
    // the script context to rely on the debug state properly. 
    topazArray_t * debugQueuedNotifications;

    int debugPaused;

    // for temporary string operations.
    topazString_t * spfString;
    
    // placeholder tag for functions, which all use the same tag.
    // Bare minimum to work.
    TOPAZLUAObjectTag functiontag;
    
    // ".lua"
    topazString_t * ext;
    
    topazString_t * lastErrorTrace;
};





// when notifications are prepared, they are stored intermediately 
// in in this.
typedef struct {
    // relecant command
    int command;

    // the result to deliver to the script instance
    topazString_t * result;

} DebugNotification;
 

static void clear_debug_state(topazScript_DebugState_t * s) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(s->callstack);
    for(i = 0; i < len; ++i) {
        topazScript_CallstackFrame_t * frame = &topaz_array_at(s->callstack, topazScript_CallstackFrame_t, i);
        topaz_string_destroy((topazString_t*)frame->filename);
        topaz_string_destroy((topazString_t*)frame->functionName);
        topaz_string_destroy((topazString_t*)frame->entityName);
    }
    topaz_array_set_size(s->callstack, 0);
}


static void debug_state_add_frame(topazScript_DebugState_t * s,
    int lineNumber,
    const char * filename,
    const char * functionName
) {
    topazScript_CallstackFrame_t frame;
    frame.lineNumber   = lineNumber;
    frame.filename     = topaz_string_create_from_c_str("%s", filename);
    frame.functionName = topaz_string_create_from_c_str("%s", functionName);
    frame.entityName   = topaz_string_create();

    topaz_array_push(s->callstack, frame);
}


static void topaz_lua_populate_stack(TOPAZLUA * ctx) {
    clear_debug_state(&ctx->debugState);
    
    lua_Debug dbg;
    int level = 0;
    while(lua_getstack(ctx->lua, level, &dbg)) {
        lua_getinfo(ctx->lua, "nSl", &dbg);
        debug_state_add_frame(
            &ctx->debugState,
            dbg.currentline,
            dbg.source,
            dbg.name
        );
        level++;
    }
}


static void topaz_lua_save_stacktrace(TOPAZLUA * ctx) {
    if (ctx->lastErrorTrace) {
        topaz_string_destroy(ctx->lastErrorTrace);
    }

    luaL_traceback(ctx->lua, ctx->lua, "", 1);
    ctx->lastErrorTrace = topaz_string_create_from_c_str("%s", lua_tostring(ctx->lua, -1));
    lua_pop(ctx->lua, 1);    
}


static int topaz_lua_object_finalizer(lua_State * l) {
    TOPAZLUAObjectTag * tag = NULL;
    tag = lua_touserdata(l, lua_upvalueindex(1)); 
    lua_pop(l, 1);


    #ifdef TOPAZDC_DEBUG
        assert(tag);
    #endif

    // if this is even happening, the global stash entry was already removed.

    // call c finalizer, which is why this is here mostly
    if (tag->cfinalizer)
        tag->cfinalizer(
                tag->ctx->script, 
                NULL, 
                tag->cfinalizerData
        );
    free(tag);
    return 0;
}


// gets the current debug traceback
static topazString_t * topaz_lua_get_traceback(TOPAZLUA * t) {
    #ifdef TOPAZDC_DEBUG
        int stackSize = lua_gettop(t->lua);
    #endif

    topazString_t * out = t->lastErrorTrace;
    if (out) 
        t->lastErrorTrace = NULL;
    else {
        luaL_traceback(t->lua, t->lua, "", 1);
        out = topaz_string_create_from_c_str("%s", lua_tostring(t->lua, -1));
        lua_pop(t->lua, 1);    
    }
    #ifdef TOPAZDC_DEBUG 
        assert(lua_gettop(t->lua) == stackSize);
    #endif
    return out;
}



// handler for the error on pcall, which is 
// called right when the error occurs.
static int topaz_lua_pcall__on_error(lua_State * l) {
    TOPAZLUA * ctx = lua_touserdata(l, lua_upvalueindex(1));
    topaz_lua_save_stacktrace(ctx);
    return 1; // returns the error object.
}

// wrapper for pcall that always retrieves the backtrace on error.
static int topaz_lua_pcall__prep(TOPAZLUA * ctx) {
    lua_pushlightuserdata(ctx->lua, ctx);
    lua_pushcclosure(ctx->lua, topaz_lua_pcall__on_error, 1);
}

static int topaz_lua_pcall(TOPAZLUA * ctx) {
    int result = lua_pcall(ctx->lua, 0, 1, -2);
    return result; // do we need to pop the closure?
}


// assumes object to receive prop is already at the top of the stack
// ideally sets the property as a read-only value
static void topaz_lua_set_private_prop(lua_State * l, const char * pName, void * val) {
    lua_pushlightuserdata(l, val);
    lua_setfield(l, -2, pName);
}


// Retrieves the data placed within the lua obejct as a pointer.
// Null if none. Assumes the source object is the top.
static void * topaz_lua_get_private_prop(lua_State * l, const char * pName) {
    lua_getfield(l, -1, pName);
    void * out = lua_touserdata(l, -1);
    lua_pop(l, 1);
    return out;
}


// Gets the object tag from the top object
static TOPAZLUAObjectTag * topaz_lua_object_get_tag_from_top(TOPAZLUA * ctx) {
    #ifdef TOPAZDC_DEBUG
        int stackSize = lua_gettop(ctx->lua);
    #endif

    if (lua_isfunction(ctx->lua, -1)) {
        // TODO: save;
        // right now, a new tag
        // is created for every function as a sort of transient object, as 
        // the function cannot refer back to the finalizer very simply 
        // as can be done with tables. So, a finalizer is set to 
        // clean this up after.

        TOPAZLUAObjectTag * out = calloc(1, sizeof(TOPAZLUAObjectTag));
        out->stashID = stashPool++;
        out->ctx = ctx;        

        // special one-off finalizer for function
        lua_newtable(ctx->lua);
        lua_pushlightuserdata(ctx->lua, out);
        lua_pushcclosure(ctx->lua, topaz_lua_object_finalizer, 1);
        lua_setfield(ctx->lua, -2, "__gc");
        lua_setmetatable(ctx->lua, -2);
        return out;
    }
    // uhh?? unsafe if someone puts a ___tz prop by hand in a js context.
    // think of a different way before release, thanks
    TOPAZLUAObjectTag * tag = topaz_lua_get_private_prop(ctx->lua, "___tz");

    #ifdef TOPAZDC_DEBUG 
        assert(lua_gettop(ctx->lua) == stackSize);
    #endif
    return tag;
}

// assigns the pointer to the tag as a read-only property string 
// for the object itself.
static TOPAZLUAObjectTag * topaz_lua_top_object_set_tag(TOPAZLUA * ctx) {
    #ifdef TOPAZDC_DEBUG
        int stackSize = lua_gettop(ctx->lua);
    #endif

    TOPAZLUAObjectTag * out = calloc(1, sizeof(TOPAZLUAObjectTag));
    out->stashID = stashPool++;
    topaz_lua_set_private_prop(ctx->lua, "___tz", out);


    #ifdef TOPAZDC_DEBUG  
        assert(lua_gettop(ctx->lua) == stackSize);
    #endif

    return out;
}


// prevents garbage collection by assigning a property to the global stash object. (the top is assumed to be the object)
//
static void topaz_lua_object_keep_reference(TOPAZLUA * ctx, TOPAZLUAObjectTag * tag) {
    #ifdef TOPAZDC_DEBUG
        int stackSize = lua_gettop(ctx->lua);
    #endif
    

    // lua uses the registry for this purpose
    lua_pushvalue(ctx->lua, -1);
    topaz_string_clear(ctx->spfString);
    topaz_string_concat_printf(ctx->spfString, "%d", tag->stashID);
    lua_setfield(ctx->lua, LUA_REGISTRYINDEX, topaz_string_get_c_str(ctx->spfString));

    #ifdef TOPAZDC_DEBUG 
        assert(lua_gettop(ctx->lua) == stackSize);
    #endif
}


// removes the key in the global stash, which leaves the object 
// open to garbage collection sweeps if it is no longer used / accessible.
static void topaz_lua_object_unkeep_reference(TOPAZLUA * ctx, TOPAZLUAObjectTag * tag) {
    #ifdef TOPAZDC_DEBUG
        int stackSize = lua_gettop(ctx->lua);
    #endif

    lua_pushnil(ctx->lua);
    topaz_string_clear(ctx->spfString);
    topaz_string_concat_printf(ctx->spfString, "%d", tag->stashID);
    lua_setfield(ctx->lua, LUA_REGISTRYINDEX, topaz_string_get_c_str(ctx->spfString));


    #ifdef TOPAZDC_DEBUG 
        assert(lua_gettop(ctx->lua) == stackSize);
    #endif

}




static void topaz_lua_object_push_to_top_from_tag(TOPAZLUAObjectTag * tag) {
    topaz_string_clear(tag->ctx->spfString);
    topaz_string_concat_printf(tag->ctx->spfString, "%d", tag->stashID);
    lua_getfield(tag->ctx->lua, LUA_REGISTRYINDEX, topaz_string_get_c_str(tag->ctx->spfString));


    #ifdef TOPAZDC_DEBUG
        if (lua_isnil(tag->ctx->lua, -1)) {
            assert(!"topaz_lua_object_push_to_top_from_tag() resulted in UNDEFINED. This shouldn't happen!\n");
        }
    #endif
}





// fetches/creates an ObjectTag for a given object
static void * topaz_lua_top_object_wrap(void * ctxSrc) {
    TOPAZLUA * ctx = ctxSrc;
    #ifdef TOPAZDC_DEBUG
        int stackSize = lua_gettop(ctx->lua);
    #endif
    
    TOPAZLUAObjectTag * tag = topaz_lua_object_get_tag_from_top(ctx);

    if (!tag) {
        // create a new tag and bind that tag to this object using a hidden prop
        tag = topaz_lua_top_object_set_tag(ctx);   
        tag->ctx = ctx;

 

        // Sets the standard finalizer function.
        lua_newtable(ctx->lua);
        lua_pushlightuserdata(ctx->lua, tag);
        lua_pushcclosure(ctx->lua, topaz_lua_object_finalizer, 1);
        lua_setfield(ctx->lua, -2, "__gc");
        lua_setmetatable(ctx->lua, -2);
    }

    if (tag->refCount == 0) { 
        // make sure that a reference to this object exists outside of the pure 
        // js context. This allows the C context to keep the object around.
        // in the case that the C tag has persisted when vulnerable to garbage collection,
        // the ref count might be 0 despite the tag existing before. THats why this check 
        // is for tag->ref == 0 rather than just creation
        topaz_lua_object_keep_reference(ctxSrc, tag);
    }



    #ifdef TOPAZDC_DEBUG 
        assert(lua_gettop(ctx->lua) == stackSize);
    #endif

    return tag;
} 



// creates a new topaz script object from a value on the duk stack
static topazScript_Object_t * topaz_lua_stack_object_to_tso(TOPAZLUA * ctx, int index) {
    #ifdef TOPAZDC_DEBUG
        int stackSize = lua_gettop(ctx->lua);
    #endif
    
    topazScript_Object_t * o = NULL;
    switch(lua_type(ctx->lua, index)) {
      case LUA_TBOOLEAN:
        o = topaz_script_object_from_int(ctx->script, lua_toboolean(ctx->lua, index));
        break;
      case LUA_TNUMBER:    
        o = topaz_script_object_from_number(ctx->script, lua_tonumber(ctx->lua, index));
        break;

      case LUA_TSTRING:
        o = topaz_script_object_from_string(ctx->script, TOPAZ_STR_CAST(lua_tostring(ctx->lua, index)));
        break;

      case LUA_TTABLE: 
      case LUA_TFUNCTION:{
        lua_pushvalue(ctx->lua, index);
        o = topaz_script_object_wrapper(ctx->script, topaz_lua_top_object_wrap(ctx));        
        lua_pop(ctx->lua, 1);
        break;
      }

      default:;
        o = topaz_script_object_undefined(ctx->script);
        break;
    }


    #ifdef TOPAZDC_DEBUG 
        assert(lua_gettop(ctx->lua) == stackSize);
    #endif
    return o;
}



// pushes the given object as a duktape object
// on the top of the stack.
static void topaz_lua_object_push_tso(TOPAZLUA * ctx, topazScript_Object_t * o) {
    switch(topaz_script_object_get_type(o)) {

      case topazScript_Object_Type_Integer:
        lua_pushinteger(ctx->lua, topaz_script_object_as_int(o));
        break;

      case topazScript_Object_Type_Number:
        lua_pushnumber(ctx->lua, topaz_script_object_as_number(o));
        break;

      case topazScript_Object_Type_String:
        lua_pushstring(ctx->lua, topaz_string_get_c_str(topaz_script_object_as_string(o)));
        break;

      case topazScript_Object_Type_Reference: {
        TOPAZLUAObjectTag * tag = topaz_script_object_get_api_data(o);
        topaz_lua_object_push_to_top_from_tag(tag);
        break;
      }
      default:
        lua_pushnil(ctx->lua);
        break;        
    }
}


/*

static duk_ret_t topaz_lua_get_internal(duk_context * js) {
    #ifdef TOPAZDC_DEBUG
        int stackSize = duk_get_top(js);
    #endif

    // retrieve tag and actual getter
    duk_push_current_function(js);
    TOPAZLUAObjectTag * tag = topaz_lua_get_private_prop(js, "___tz");
    topaz_script_native_function getReal = (topaz_script_native_function)topaz_lua_get_private_prop(js, "___tzfn");
    duk_pop(js);

    // We need a representative object for the 'this' to which the 
    // getter belongs
    topaz_lua_object_push_to_top_from_tag(tag);
    topazScript_Object_t * self = topaz_lua_stack_object_to_tso(tag->ctx, -1);
    duk_pop(js);

    // actually call the native getter
    topazScript_Object_t * res = getReal(
        tag->ctx->script,
        TOPAZ_ARRAY_CAST(&self, topazScript_Object_t *, 1),
        NULL //????
    );

    #ifdef TOPAZDC_DEBUG 
        assert(duk_get_top(js) == stackSize);
    #endif

    // finally, push the tso result as a duk object
    if (res) {
        topaz_lua_object_push_tso(tag->ctx, res);
    } else {
        duk_push_undefined(tag->ctx->js);
    }

    topaz_script_object_destroy(self);
    topaz_script_object_destroy(res);
    return 1;
}

static duk_ret_t topaz_lua_set_internal(duk_context * js) {
    // we assume that get_top == 2 since its a setter
    duk_push_current_function(js);
    TOPAZLUAObjectTag * tag = topaz_lua_get_private_prop(js, "___tz");
    topaz_script_native_function setReal = (topaz_script_native_function)topaz_lua_get_private_prop(js, "___tzfn");
    duk_pop(js);

    topazScript_Object_t * args[2];


    // We need a representative object for the 'this' to which the 
    // getter belongs
    topaz_lua_object_push_to_top_from_tag(tag);
    args[0] = topaz_lua_stack_object_to_tso(tag->ctx, -1);
    duk_pop(js);

    // arg 0 is always the value in question
    args[1] = topaz_lua_stack_object_to_tso(tag->ctx, 0);


    topazScript_Object_t * result = setReal(
        tag->ctx->script,
        TOPAZ_ARRAY_CAST(args, topazScript_Object_t *, 2),
        NULL //???????????
    );

    #ifdef TOPAZDC_DEBUG
        assert(!result);
    #endif

    
    topaz_script_object_destroy(args[0]);
    topaz_script_object_destroy(args[1]);

    return 0;
}




*/
static int topaz_lua_native_function_internal(lua_State * l) {

    uint32_t nargsProvided = lua_gettop(l);


    // retrieve tag and actual getter
    TOPAZLUA * ctx                      = lua_touserdata(l, lua_upvalueindex(1));
    topaz_script_native_function fnReal = lua_touserdata(l, lua_upvalueindex(2));
    void * fnData                       = lua_touserdata(l, lua_upvalueindex(3));
    uint8_t nargs                        = (uint8_t)lua_touserdata(l, lua_upvalueindex(4));

    int argsTotal = nargs > nargsProvided ? nargs : nargsProvided;

    // convert all args to tso
    uint32_t i;
    topazScript_Object_t * args[argsTotal];
    for(i = 0; i < nargsProvided; ++i) {
        args[i] = topaz_lua_stack_object_to_tso(ctx, i+1);
    }
    for(; i < nargs; ++i) {
        args[i] = topaz_script_object_undefined(ctx->script);
    }   

    // actually call the native fn
    topazScript_Object_t * res = fnReal(
        ctx->script,
        args,
        fnData
    );

    #ifdef TOPAZDC_DEBUG 
        assert(lua_gettop(l) == nargs);
    #endif

    // finally, push the tso result as a lua object 
    if (res) {
        topaz_lua_object_push_tso(ctx, res);
        topaz_script_object_destroy(res);
    } else {
        lua_pushnil(l);
    }

    for(i = 0; i < argsTotal; ++i) {
        topaz_script_object_destroy(args[i]);
    }
    return 1;
}

/*

static topazString_t * topaz_lua_stack_where(TOPAZLUA * ctx) {
    topazString_t * str = topaz_string_create();

    if (duk_is_error(ctx->js, -2)) {
        duk_get_prop_string(ctx->js, -2, "lineNumber");
        topaz_string_concat_printf(str, "ERROR @ line %d", duk_to_int(ctx->js, -1));
        duk_pop(ctx->js);

        duk_get_prop_string(ctx->js, -2, "fileName");
        topaz_string_concat_printf(str, " in \"%s\"\n", duk_to_string(ctx->js, -1));
        duk_pop(ctx->js);

    }



    int level = -1;    

    for(;;) {
        #ifdef TOPAZDC_DEBUG
            int stackSize = duk_get_top(ctx->js);
        #endif


        duk_inspect_callstack_entry(ctx->js, level);
        if (duk_is_undefined(ctx->js, -1)) 
            break;

        if (level == -1)
            topaz_string_concat_printf(str, "@ innermost :");
        else 
            topaz_string_concat_printf(str, "@ L%d :", (level*-1)-1);

            


        duk_get_prop_string(ctx->js, -1, "function");
        duk_get_prop_string(ctx->js, -1, "name");
        topaz_string_concat_printf(str, "In call %s()", duk_to_string(ctx->js, -1));
        duk_pop(ctx->js);
        duk_pop(ctx->js);

        duk_get_prop_string(ctx->js, -1, "lineNumber");
        int lineNum = duk_to_int(ctx->js, -1);
        if (lineNum)
            topaz_string_concat_printf(str, ", line [%d]", lineNum);

        duk_pop(ctx->js);
        duk_pop(ctx->js);

        topaz_string_concat_printf(str, "\n");
        level--;
 
        #ifdef TOPAZDC_DEBUG 
            assert(duk_get_top(ctx->js) == stackSize);
        #endif

    }
    return str;
}


static void topaz_lua_fatal(void * udata, const char *msg) {
    TOPAZLUA * ctx = udata;
    PERROR(ctx->ctx, topaz_string_create_from_c_str("TOPAZ-DUKTAPE FATAL ERROR: %s\n", msg));

    topazString_t * str = topaz_lua_stack_where(ctx);
    PERROR(ctx->ctx, str);
    fflush(stdout);
}









*/





static void * topaz_lua_create(topazScript_t * scr, topaz_t * ctx) {
    TOPAZLUA * out = calloc(1, sizeof(TOPAZLUA));
    out->debugState.callstack = topaz_array_create(sizeof(topazScript_CallstackFrame_t));
    out->ctx = ctx;
    out->debugBreakpoints = topaz_array_create(sizeof(int));
    out->debugRemoveBreakpoint = topaz_array_create(sizeof(int));
    out->debugQueuedNotifications = topaz_array_create(sizeof(DebugNotification));
    out->spfString = topaz_string_create();
    out->lua = luaL_newstate();
    out->functiontag.ctx = out;
    out->ext = topaz_string_create_from_c_str("lua");
    luaL_openlibs(out->lua);
    out->script = scr;

    TOPAZLUABackend * backend = topaz_system_backend_get_user_data(topaz_script_get_backend(scr));
    topaz_array_push(backend->instances, out);
    return out;
} 

static void topaz_lua_destroy(topazScript_t * scr, void * data) {
    TOPAZLUABackend * backend = topaz_system_backend_get_user_data(topaz_script_get_backend(scr));
    uint32_t i;
    uint32_t len = topaz_array_get_size(backend->instances);
    for(i = 0; i < len; ++i) {
        if (topaz_array_at(backend->instances, TOPAZLUA*, i) == data) {
            topaz_array_remove(backend->instances, i);
            break;
        }
    }


    TOPAZLUA * ctx = data;
    
    // TODO: destroy ALL objects??
    lua_close(ctx->lua);
}

static int topaz_lua_map_native_function(
    topazScript_t * script, 
    void * data, 
    
    const topazString_t * pname, 
    uint8_t args,
    topaz_script_native_function fn, 
    void * userData

) {
    TOPAZLUA * ctx = data;
    #ifdef TOPAZDC_DEBUG
        int stackSize = lua_gettop(ctx->lua);
    #endif
 


    lua_pushlightuserdata(ctx->lua, ctx);
    lua_pushlightuserdata(ctx->lua, (void*)fn);
    lua_pushlightuserdata(ctx->lua, userData);
    lua_pushlightuserdata(ctx->lua, (void*)args);
    lua_pushcclosure(ctx->lua, topaz_lua_native_function_internal, 4);


    lua_setglobal(ctx->lua, topaz_string_get_c_str(pname));

    #ifdef TOPAZDC_DEBUG 
        assert(lua_gettop(ctx->lua) == stackSize);
    #endif

    return 1;
}


static void topaz_lua_report_top_error(TOPAZLUA * ctx) {
    topazString_t * str = topaz_string_create();
    topaz_string_concat_printf(str, "An error occurred: %s\n", lua_tostring(ctx->lua, -1));
    topazString_t * err = topaz_lua_get_traceback(ctx);
    topaz_string_concat(str, err);
    topaz_string_destroy(err);
    PERROR(ctx->ctx, str);
    fflush(stdout);
}

static topazScript_Object_t * topaz_lua_expression(
    topazScript_t * script, 
    void * data,
    const topazString_t * expr
) {
    TOPAZLUA * ctx = data;
    topazScript_Object_t * out;
    topaz_lua_pcall__prep(ctx);
    if (luaL_loadstring(ctx->lua, topaz_string_get_c_str(expr)) != LUA_OK) {
        topaz_lua_report_top_error(ctx);
        out = topaz_script_object_undefined(ctx->script);    
        lua_pop(ctx->lua, 1);
    } else {
        if (topaz_lua_pcall(ctx) != LUA_OK) {
            topaz_lua_report_top_error(ctx);
            fflush(stdout);        
        } else {
            out = topaz_lua_stack_object_to_tso(
                ctx,
                -1
            );
        }
    }

    lua_pop(ctx->lua, 1);
    return out;
}

static void topaz_lua_throw_error(
    topazScript_t * script, 
    void * data,
    const topazArray_t * args
) {
    TOPAZLUA * ctx = data;
    

    topazString_t * str = topaz_string_create();
    topaz_string_concat_printf(
        str,
        "topaz threw an error: %s",
        topaz_array_get_size(args) ?
            topaz_string_get_c_str(topaz_script_object_as_string(topaz_array_at(args, topazScript_Object_t *, 0)))
        :
            "Internal native error."
    );
    
    // This shouldnt happen, but if it does, things will be confusing
        
    topaz_lua_save_stacktrace(ctx);
    
    lua_pushstring(ctx->lua, topaz_string_get_c_str(str));
    lua_error(ctx->lua);
    topaz_string_destroy(str);
}



typedef struct {
    int sent;
    const topazString_t * src;
} LUAREADERSTATE;

static const char * topaz_lua_run__reader(
    lua_State * lue,
    void * data,
    size_t * size
) {
    LUAREADERSTATE * src = data;
    if (src->sent) {
        *size = 0;
        return NULL;
    }
    *size = topaz_string_get_length(src->src);
    src->sent = 1;
    return topaz_string_get_c_str(src->src);
}


void topaz_lua_run(
    topazScript_t * script, 
    void * data, 
    
    const topazString_t * sourceName, 
    const topazString_t * sourceData) {

    TOPAZLUA * ctx = data;
    #ifdef TOPAZDC_DEBUG
        int stackSize = lua_gettop(ctx->lua);
    #endif
    LUAREADERSTATE reader;
    reader.src = sourceData;
    reader.sent = 0;
    topaz_lua_pcall__prep(ctx);
    if (lua_load(
        ctx->lua,
        topaz_lua_run__reader,
        &reader,
        topaz_string_get_c_str(sourceName),
        NULL
    )!= LUA_OK) {
        topaz_lua_report_top_error(ctx);
        lua_pop(ctx->lua, 1);
    } else {            
        if (topaz_lua_pcall(ctx) != LUA_OK) {
            topaz_lua_report_top_error(ctx);
        }
    }
    lua_pop(ctx->lua, 1); // throw away result of last statement.
    #ifdef TOPAZDC_DEBUG 
        assert(lua_gettop(ctx->lua) == stackSize);
    #endif
}

topazScript_Object_t * topaz_lua_create_empty_object(
    topazScript_t * script,
    void * data,

    topaz_script_native_function cleanup,
    void * cleanupData

) {
    TOPAZLUA * ctx = data;
    #ifdef TOPAZDC_DEBUG
        int stackSize = lua_gettop(ctx->lua);
    #endif


    // create the new object
    lua_newtable(ctx->lua);

    // create raw tso
    topazScript_Object_t * out = topaz_lua_stack_object_to_tso(ctx, -1);

    // get the tag so we can add the finalizer
    TOPAZLUAObjectTag * tag = topaz_lua_object_get_tag_from_top(ctx);

    tag->cfinalizer = cleanup;
    tag->cfinalizerData = cleanupData;

    lua_pop(ctx->lua, 1);

    #ifdef TOPAZDC_DEBUG 
        assert(lua_gettop(ctx->lua) == stackSize);
    #endif

    return out;
}


void topaz_lua_bootstrap(topazScript_t * script, void * n) {
    #include "bootstrap_bytes"

    #ifdef TOPAZDC_DEBUG
        topaz_script_run(
            script,
            TOPAZ_STR_CAST("TOPAZ_BOOTSTRAP.lua"),
            TOPAZ_STR_CAST((char*)bootstrap_bytes)
        );
    #else
        topaz_lua_run(
            script, n,
            TOPAZ_STR_CAST("[internal]"),
            TOPAZ_STR_CAST((char*)bootstrap_bytes)
        );
    #endif
}
























 

static void * topaz_lua_object_reference_create_from_reference(topazScript_Object_t * o, void * ctxSrc, topazScript_Object_t * from, void * fromData) {
    TOPAZLUA * ctx = ctxSrc;
    #ifdef TOPAZDC_DEBUG
        int stackSize = lua_gettop(ctx->lua);
    #endif
    topaz_lua_object_push_to_top_from_tag(fromData);
    void * outTag = topaz_lua_top_object_wrap(ctxSrc);

    lua_pop(ctx->lua, 1);
    #ifdef TOPAZDC_DEBUG 
        assert(lua_gettop(ctx->lua) == stackSize);
    #endif
    return outTag;
}




static void topaz_lua_object_reference_destroy(topazScript_Object_t * o, void * tagSrc) {
    // the finalizer handlers destruction for us, 
    // so there isnt much to do here.
    // (topaz will unref for us)
}



static int topaz_lua_object_reference_get_feature_mask(topazScript_Object_t * o, void * tagSrc) {
    TOPAZLUAObjectTag * t = tagSrc;
    lua_State * ctx = t->ctx->lua;
    #ifdef TOPAZDC_DEBUG
        int stackSize = lua_gettop(ctx);
    #endif

    topaz_lua_object_push_to_top_from_tag(t);
    int out = 0; // all objects in js can have properties
    if (lua_isfunction(ctx, -1)) {
        out |= topazScript_Object_Feature_Callable;
    } 

    if (lua_istable(ctx, -1)) {
        out |= topazScript_Object_Feature_Map;
        out |= topazScript_Object_Feature_Array;
        
        int calltype = luaL_getmetafield(ctx, -1, "__call"); 
        if (calltype == LUA_TFUNCTION) {
            out |= topazScript_Object_Feature_Callable;
        }
        if (calltype != LUA_TNIL) {
            lua_pop(ctx, 1);
        }
    }
    lua_pop(ctx, 1);

    #ifdef TOPAZDC_DEBUG 
        assert(lua_gettop(ctx) == stackSize);
    #endif
    return out;
}



static void * topaz_lua_object_reference_get_native_data(topazScript_Object_t * o, int * tagID, void * tagSrc) {
    TOPAZLUAObjectTag * t = tagSrc;
    *tagID = t->tagID;
    return t->nativeData;
}

static void topaz_lua_object_reference_set_native_data(topazScript_Object_t * o, void * nativeData, int tagID, void * tagSrc) {
    TOPAZLUAObjectTag * t = tagSrc;
    t->tagID = tagID;
    t->nativeData = nativeData;
}



static void topaz_lua_object_reference_ref(topazScript_Object_t * o, void * data) {
    TOPAZLUAObjectTag * tag = data;
    // when 0, will unkeep
    tag->refCount++;
}

static void topaz_lua_object_reference_unref(topazScript_Object_t * o, void * data) {
    TOPAZLUAObjectTag * tag = data;
    // when 0, will unkeep
    tag->refCount--;

    if (tag->refCount == 0) {
        #ifdef TOPAZDC_DEBUG
            int stackSize = lua_gettop(tag->ctx->lua);
        #endif

        topaz_lua_object_push_to_top_from_tag(tag);
        topaz_lua_object_unkeep_reference(tag->ctx, tag);
        lua_pop(tag->ctx->lua, 1); 

        #ifdef TOPAZDC_DEBUG 
            assert(lua_gettop(tag->ctx->lua) == stackSize);
        #endif
    }

}




static topazScript_Object_t * topaz_lua_object_reference_call(topazScript_Object_t * o, const topazArray_t * args, void * data) {
    TOPAZLUAObjectTag * tag = data;
    TOPAZLUA * ctx = tag->ctx;
    #ifdef TOPAZDC_DEBUG
        int stackSize = lua_gettop(tag->ctx->lua);
    #endif

    lua_pushlightuserdata(ctx->lua, ctx);
    lua_pushcclosure(ctx->lua, topaz_lua_pcall__on_error, 1);


    // assumes callable
    topaz_lua_object_push_to_top_from_tag(tag);


    // duk calling
    uint32_t i = 0;
    uint32_t len = topaz_array_get_size(args);
    for(; i < len; ++i) {
        topazScript_Object_t * o = topaz_array_at(args, topazScript_Object_t *, i);
        topaz_lua_object_push_tso(
            tag->ctx, 
            o
        );
    }


    int result = lua_pcall(ctx->lua, len, 1, -(len+2));
    if (result != LUA_OK) {
        topaz_lua_report_top_error(ctx);    
    }

    
    
    topazScript_Object_t * out = topaz_lua_stack_object_to_tso(
        tag->ctx,
        -1
    );
    lua_pop(tag->ctx->lua, 1);

    #ifdef TOPAZDC_DEBUG 
        assert(lua_gettop(tag->ctx->lua) == stackSize);
    #endif
 
    return out;
}


static topazScript_Object_t * topaz_lua_object_reference_array_get_nth(
    topazScript_Object_t * oSrc, 
    int index,  
    void * tagSrc
) {

    TOPAZLUAObjectTag * tag = tagSrc;
    #ifdef TOPAZDC_DEBUG
        int stackSize = lua_gettop(tag->ctx->lua);
    #endif

    topaz_lua_object_push_to_top_from_tag(tag);
    lua_geti(tag->ctx->lua, -1, index+1);
    topazScript_Object_t * o = topaz_lua_stack_object_to_tso(tag->ctx, -1);
    lua_pop(tag->ctx->lua, 2);

    #ifdef TOPAZDC_DEBUG 
        assert(lua_gettop(tag->ctx->lua) == stackSize);
    #endif

    return o;
}


static int topaz_lua_object_reference_array_get_count(topazScript_Object_t * o, void * tagSrc) {
    TOPAZLUAObjectTag * tag = tagSrc;
    #ifdef TOPAZDC_DEBUG
        int stackSize = lua_gettop(tag->ctx->lua);
    #endif

    topaz_lua_object_push_to_top_from_tag(tag);
    lua_len(tag->ctx->lua, -1);
    int len = lua_tointeger(tag->ctx->lua, -1);
    lua_pop(tag->ctx->lua, 2); 

    #ifdef TOPAZDC_DEBUG 
        assert(lua_gettop(tag->ctx->lua) == stackSize);
    #endif
    return len;
}

static topazScript_Object_t * topaz_lua_object_reference_map_get_property(
    topazScript_Object_t * o, 
    const topazString_t * prop, 
    void * tagSrc
) {
    TOPAZLUAObjectTag * tag = tagSrc;
    #ifdef TOPAZDC_DEBUG
        int stackSize = lua_gettop(tag->ctx->lua);
    #endif

    topaz_lua_object_push_to_top_from_tag(tag);
    lua_getfield(tag->ctx->lua, -1, topaz_string_get_c_str(prop));
    topazScript_Object_t * out = topaz_lua_stack_object_to_tso(tag->ctx, -1);
    lua_pop(tag->ctx->lua, 2);


    #ifdef TOPAZDC_DEBUG 
        assert(lua_gettop(tag->ctx->lua) == stackSize);
    #endif

    return out;
}

void topaz_lua_object_reference_to_string(
    topazScript_Object_t * o, 
    topazString_t * str,
    void * tagSrc) {
    
    TOPAZLUAObjectTag * tag = tagSrc;

    #ifdef TOPAZDC_DEBUG
        int stackSize = lua_gettop(tag->ctx->lua);
    #endif

    topaz_lua_object_push_to_top_from_tag(tag);
    const char * strC = lua_tostring(tag->ctx->lua, -1);
    topaz_string_concat_printf(str, "%s", strC ? strC : "[not coercable to string]");
    lua_pop(tag->ctx->lua, 1);


    #ifdef TOPAZDC_DEBUG 
        assert(lua_gettop(tag->ctx->lua) == stackSize);
    #endif
}

/*
// SUPPORT TRANS WRITES

static const char * dvalue_to_string(duk_dvalue * dv) {
    static char bufOut[64];

	if (!dv) {
		return "NULL";
	}

	switch (dv->tag) {
	case DUK_DVALUE_EOM:
        return "EOM";

	case DUK_DVALUE_REQ:
        return "REQ";

	case DUK_DVALUE_REP:
        return "REP";

	case DUK_DVALUE_ERR:
        return "ERR";

	case DUK_DVALUE_NFY:
        return "NFY";

	case DUK_DVALUE_INTEGER: {
		sprintf(bufOut, "%d", dv->i);
		return bufOut;
    }

	case DUK_DVALUE_STRING:
        return (const char*)dv->buf;        

	case DUK_DVALUE_BUFFER:
        return "[Buffer]";

	case DUK_DVALUE_UNUSED:
        return "undefined";

	case DUK_DVALUE_UNDEFINED:
        return "undefined";

	case DUK_DVALUE_NULL:
        return "null";

	case DUK_DVALUE_TRUE:
        return "true";

	case DUK_DVALUE_FALSE:
        return "false";

	case DUK_DVALUE_NUMBER:
		if (fpclassify(dv->d) == FP_ZERO) {
			if (signbit(dv->d)) {
                return "-0";
			} else {
                return "0";
			}
		} else {
			sprintf(bufOut, "%lg", dv->d);
            return bufOut;
		}

	case DUK_DVALUE_OBJECT:
        return "[Object]";

	case DUK_DVALUE_POINTER:
        return "[Pointer]";

	case DUK_DVALUE_LIGHTFUNC:
        return "[L-Function]";

	case DUK_DVALUE_HEAPPTR:
        return "[H-Pointer]";

	default:;
	}
    return "[Unknown]";

}

static void topaz_lua_trans_received(duk_trans_dvalue_ctx * ctxT, duk_dvalue * dv) {
    //printf("dvalue received: %s\n", &bufferSrc[0]);
    TOPAZLUA * ctx = ctxT->userData;
    if (!ctx->debugOneOff) {
        // always skip the very first pause notif.
        if (!strcmp(dvalue_to_string(dv), "EOM")) {
            ctx->debugOneOff = 1;
        }
        return;
    }
    char * cpy = strdup(dvalue_to_string(dv));
    topaz_array_push(ctx->pendingMessages, cpy);
    #ifdef TOPAZDC_DEBUG
        //printf("RECEIVED DEBUG MSG: %s\n", cpy);
        //fflush(stdout);
    #endif
}

static void topaz_lua_trans_handshake(duk_trans_dvalue_ctx * ctx, const char * handshake) {
    //printf("handshake: %s\n", handshake);
    fflush(stdout);

}

static void topaz_lua_trans_detached(duk_trans_dvalue_ctx * ctx) {
    //printf("was detached!\n");
    fflush(stdout);

}


// commands

#define DUKTAPE_COMMAND__GET_CALLSTACK 256

static void topaz_lua_trans_command__pause(duk_trans_dvalue_ctx * ctxT) {
    TOPAZLUA * ctx = ctxT->userData;

    duk_trans_dvalue_send_req(ctxT);
    duk_trans_dvalue_send_integer(ctxT, 0x12);  
    duk_trans_dvalue_send_eom(ctxT);
    //printf("sent pause request\n");

    topazScript_DebugCommand_t c = topazScript_DebugCommand_Pause;
    topaz_array_push(ctx->lastCommand, c);    
}



static void topaz_lua_trans_command__resume(duk_trans_dvalue_ctx * ctxT) {
    TOPAZLUA * ctx = ctxT->userData;

    duk_trans_dvalue_send_req(ctxT);
    duk_trans_dvalue_send_integer(ctxT, 0x13);  
    duk_trans_dvalue_send_eom(ctxT);
    //printf("sent resume request\n");

    topazScript_DebugCommand_t c = topazScript_DebugCommand_Resume;
    topaz_array_push(ctx->lastCommand, c);    
}

static void topaz_lua_trans_command__step_into(duk_trans_dvalue_ctx * ctxT) {
    TOPAZLUA * ctx = ctxT->userData;

    duk_trans_dvalue_send_req(ctxT);
    duk_trans_dvalue_send_integer(ctxT, 0x14);  
    duk_trans_dvalue_send_eom(ctxT);
    //printf("sent step into request\n");


    topazScript_DebugCommand_t c = topazScript_DebugCommand_StepInto;
    topaz_array_push(ctx->lastCommand, c);    
}



static void topaz_lua_trans_command__step_over(duk_trans_dvalue_ctx * ctxT) {
    TOPAZLUA * ctx = ctxT->userData;

    duk_trans_dvalue_send_req(ctxT);
    duk_trans_dvalue_send_integer(ctxT, 0x15);  
    duk_trans_dvalue_send_eom(ctxT);
    //printf("sent step over request\n");

    topazScript_DebugCommand_t c = topazScript_DebugCommand_StepOver;
    topaz_array_push(ctx->lastCommand, c);    
}

static void topaz_lua_trans_command__add_breakpoint(duk_trans_dvalue_ctx * ctxT, const topazString_t * filename, int line) {
    TOPAZLUA * ctx = ctxT->userData;

    duk_trans_dvalue_send_req(ctxT);
    duk_trans_dvalue_send_integer(ctxT, 0x18);  
    duk_trans_dvalue_send_string(ctxT, topaz_string_get_c_str(filename));
    duk_trans_dvalue_send_integer(ctxT, line);
    duk_trans_dvalue_send_eom(ctxT);
    //printf("sent add break request\n");

    topazScript_DebugCommand_t c = topazScript_DebugCommand_AddBreakpoint;
    topaz_array_push(ctx->lastCommand, c);    
}

static void topaz_lua_trans_command__delete_breakpoint(duk_trans_dvalue_ctx * ctxT, int index) {
    TOPAZLUA * ctx = ctxT->userData;

    // assumes that we're keeping track of the debug indices correctly.
    if (index >= 0 && index < topaz_array_get_size(ctx->debugBreakpoints)) {
        int id = topaz_array_at(ctx->debugBreakpoints, int, index);
        topaz_array_remove(ctx->debugBreakpoints, index);
        topaz_array_push(ctx->debugRemoveBreakpoint, id);
    }


    duk_trans_dvalue_send_req(ctxT);
    duk_trans_dvalue_send_integer(ctxT, 0x19);  
    duk_trans_dvalue_send_integer(ctxT, index);  
    duk_trans_dvalue_send_eom(ctxT);
    //printf("sent breakpoint delete request %d\n", index);

    topazScript_DebugCommand_t c = topazScript_DebugCommand_RemoveBreakpoint;
    topaz_array_push(ctx->lastCommand, c);    

}

static void topaz_lua_trans_command__eval(duk_trans_dvalue_ctx * ctxT, const topazString_t * eval, int callstackLevel) {
    TOPAZLUA * ctx = ctxT->userData;

    duk_trans_dvalue_send_req(ctxT);
    duk_trans_dvalue_send_integer(ctxT, 0x1e);  
    duk_trans_dvalue_send_integer(ctxT, callstackLevel);
    topazString_t * command = topaz_string_create_from_c_str("Topaz.objectToString(");
    topaz_string_concat(command, eval);
    topaz_string_concat(command, TOPAZ_STR_CAST(")"));
    duk_trans_dvalue_send_string(ctxT, topaz_string_get_c_str(command));
    topaz_string_destroy(command);
    duk_trans_dvalue_send_eom(ctxT);
    //printf("sent eval request\n");


    topazScript_DebugCommand_t c = topazScript_DebugCommand_ScopedEval;
    topaz_array_push(ctx->lastCommand, c);    
}

static void topaz_lua_trans_command__get_call_stack(duk_trans_dvalue_ctx * ctxT) {
    TOPAZLUA * ctx = ctxT->userData;

    duk_trans_dvalue_send_req(ctxT);
    duk_trans_dvalue_send_integer(ctxT, 0x1c);  
    duk_trans_dvalue_send_eom(ctxT);

    int c = DUKTAPE_COMMAND__GET_CALLSTACK;
    topaz_array_push(ctx->lastCommand, c);    
}    








static void clear_debug_state(topazScript_DebugState_t * s) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(s->callstack);
    for(i = 0; i < len; ++i) {
        topazScript_CallstackFrame_t * frame = &topaz_array_at(s->callstack, topazScript_CallstackFrame_t, i);
        topaz_string_destroy((topazString_t*)frame->filename);
        topaz_string_destroy((topazString_t*)frame->functionName);
        topaz_string_destroy((topazString_t*)frame->entityName);
    }
    topaz_array_set_size(s->callstack, 0);
}

static void debug_state_add_frame(topazScript_DebugState_t * s,
    int lineNumber,
    const char * filename,
    const char * functionName
) {
    topazScript_CallstackFrame_t frame;
    frame.lineNumber   = lineNumber;
    frame.filename     = topaz_string_create_from_c_str("%s", filename);
    frame.functionName = topaz_string_create_from_c_str("%s", functionName);
    frame.entityName   = topaz_string_create();

    topaz_array_push(s->callstack, frame);
}
*/
// queues a notification. Ownership of the string 
static void topaz_lua_debug_queue_notify(TOPAZLUA * ctx, int cmd, topazString_t * str) {
    DebugNotification n;
    n.command = cmd;
    n.result = str;
    topaz_array_push(ctx->debugQueuedNotifications, n);
}

static void topaz_lua_push_notify(TOPAZLUA * ctx) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(ctx->debugQueuedNotifications);
    if (!len) return;
    for(i = 0; i < len; ++i) {
        topaz_script_notify_command(
            ctx->script, 
            topaz_array_at(ctx->debugQueuedNotifications, DebugNotification, i).command,            
            topaz_array_at(ctx->debugQueuedNotifications, DebugNotification, i).result  
        );
        //topaz_string_destroy(topaz_array_at(ctx->debugQueuedNotifications, DebugNotification, i).result);
    }
    topaz_array_set_size(ctx->debugQueuedNotifications, 0);
    #ifdef TOPAZDC_DEBUG
        //printf("Pushed %d notifications to the script instance.\n", (int)len);
    #endif
}



#define ARRPOP(__T__,__TYPE__) (topaz_array_at(__T__, __TYPE__, topaz_array_get_size(__T__)-1));topaz_array_set_size(__T__, topaz_array_get_size(__T__)-1);


static void topaz_lua_debug_cooperate(TOPAZLUA * ctx, int block) {
   if (topaz_array_get_size(ctx->lastCommand)) {
        topazScript_DebugCommand_t command = ARRPOP(ctx->lastCommand, topazScript_DebugCommand_t);



        // notify from debugger (status)
        switch((int)command) {
          case topazScript_DebugCommand_ScopedEval: {
            topazString_t * level  = ARRPOP(ctx->lastCommandArgs, topazString_t *);
            topazString_t * str    = ARRPOP(ctx->lastCommandArgs, topazString_t *);

            topazString_t * result = NULL;
            topaz_lua_pcall__prep(ctx);
            if (luaL_loadstring(ctx->lua, topaz_string_get_c_str(str)) != LUA_OK) {
                topaz_lua_report_top_error(ctx);
                result = topaz_string_create_from_c_str("<error>");
                lua_pop(ctx->lua, 1);
            } else {
                if (topaz_lua_pcall(ctx) != LUA_OK) {
                    topaz_lua_report_top_error(ctx);
                    result = topaz_string_create_from_c_str("<error>");
                } else {
                    result = topaz_string_create_from_c_str("%s", lua_tostring(ctx->lua, -1));
                }
            }
            lua_pop(ctx->lua, 1);
            
        
            topaz_string_destroy(level);
            topaz_string_destroy(str);
            
            topaz_lua_debug_queue_notify(
                ctx,
                topazScript_DebugCommand_ScopedEval,
                result
            );
            
            
            break;
          }

          default:
            //topaz_lua_trans_command__get_call_stack(ctx);
            //printf("Received %d messages.\n", messagesLen);
            fflush(stdout);
            
        }

    
    }
}

static void topaz_lua_debug_send_command(
    topazScript_t * scr, 
    void * data, 
    int cmd, 
    const topazString_t * arg
);


static void topaz_lua_pause_trap(topazScript_t * src, TOPAZLUA * ctx) {
    if (ctx->debugPaused) return;
    ctx->debugPaused = TRUE;
    topaz_context_pause(ctx->ctx);
    topaz_script_notify_command(
        src,
        topazScript_DebugCommand_Pause,
        topaz_string_create()
    );
    topaz_lua_get_traceback(ctx);
    while(topaz_context_is_paused(ctx->ctx)) {
        topaz_lua_debug_cooperate(ctx, 0);
        topaz_context_iterate(ctx->ctx);
        topaz_context_frame_end(ctx->ctx);
    }
    ctx->debugPaused = FALSE;
}

static topazScript_Object_t * topaz_lua_global_method_debugger(
    topazScript_t * src, 
    topazScript_Object_t ** args, 
    void * fnData
) {
    TOPAZLUA * ctx = fnData;

    /*
    topaz_lua_debug_send_command(
        src,
        fnData,
        topazScript_DebugCommand_Pause,
        TOPAZ_STR_CAST("")
    );
    */
    topaz_script_notify_command(src, topazScript_DebugCommand_Pause, TOPAZ_STR_CAST(""));
    topaz_lua_pause_trap(src, ctx);
    return topaz_script_object_undefined(src);
}


static void topaz_lua_debug_start(topazScript_t * scr, void * data) {
    TOPAZLUA * ctx = data;
    ctx->isDebugging = TRUE;

    ctx->lastCommand     = topaz_array_create(sizeof(topazScript_DebugCommand_t));
    ctx->lastCommandArgs = topaz_array_create(sizeof(topazString_t *));


    topaz_lua_map_native_function(
        scr, 
        ctx, 


        TOPAZ_STR_CAST("debugger"),
        0,
        topaz_lua_global_method_debugger,                    
        ctx
    );
    

}

static void topaz_lua_cooperate_update(topazSystem_Backend_t * backend, void * data) {
    TOPAZLUABackend * b = data;
    uint32_t i;
    for(i = 0; i < topaz_array_get_size(b->instances); ++i) {
        TOPAZLUA * ctx = topaz_array_at(b->instances, TOPAZLUA *, i);
        if (ctx->isDebugging) {
            topaz_lua_debug_cooperate(ctx, 0);
            while(topaz_array_get_size(ctx->lastCommand))
                topaz_lua_debug_cooperate(ctx, 0);

            topaz_lua_push_notify(ctx);
        }
    }    
}

static void topaz_lua_light_compile_expression__char_ok(
    int * srcIter,
    topazString_t * src,
    
    topazString_t * working
) {
    int ch = topaz_string_get_char(src, *srcIter);
    topaz_string_append_char(working, ch);
    (*srcIter) ++;
}

static void topaz_lua_light_compile_expression__consume_string__single(
    int * srcIter,
    topazString_t * src,
    
    topazString_t * working
) {
    topaz_lua_light_compile_expression__char_ok(srcIter, src, working);
    int i;
    for(i = *srcIter; i < topaz_string_get_length(src); ++i) {
        int ch = topaz_string_get_char(src, i);
        topaz_lua_light_compile_expression__char_ok(srcIter, src, working);
        if (ch == '\\') {
            if (i + 1 >= topaz_string_get_length(src)) continue;
            if (topaz_string_get_char(src, i+1) == '\'') {
                topaz_lua_light_compile_expression__char_ok(srcIter, src, working);                
                i++;
            }            
        }
        
        if (ch == '\'') {
            break;
        }        
    }
}


static void topaz_lua_light_compile_expression__consume_string__double(
    int * srcIter,
    topazString_t * src,
    
    topazString_t * working
) {
    topaz_lua_light_compile_expression__char_ok(srcIter, src, working);
    int i;
    for(i = *srcIter; i < topaz_string_get_length(src); ++i) {
        int ch = topaz_string_get_char(src, i);
        topaz_lua_light_compile_expression__char_ok(srcIter, src, working);
        if (ch == '\\') {
            if (i + 1 >= topaz_string_get_length(src)) continue;
            if (topaz_string_get_char(src, i+1) == '"') {
                topaz_lua_light_compile_expression__char_ok(srcIter, src, working);                
                i++;
            }            
        }
        
        if (ch == '"') {
            break;
        }        
    }
}


static topazString_t * topaz_lua_light_compile_expression__extract_var_name(
    int * srcIter,
    topazString_t * src,
    
    topazString_t * working
) {
    topazString_t * varname = topaz_string_create();
    int i;
    for(i = *srcIter; i < topaz_string_get_length(src); ++i) {
        int ch = topaz_string_get_char(src, i);
        if (i == 0) {
            if (!isalpha(ch)) {
                // whoops, not a lua identifier/name
                return varname;            
            }
            
            topaz_string_append_char(varname, ch);
            (*srcIter) ++;
        } else {
            if (!(isalnum(ch) || ch == '_')) {
                // end lua identifier/name
                return varname;                        
            } else {
                topaz_string_append_char(varname, ch);
                (*srcIter) ++;            
            }        
        }
    }
    return varname;
}
static void topaz_lua_light_compile_expression__consume_str_index(
    int * srcIter,
    topazString_t * src,
    
    topazString_t * working
) {
    topaz_lua_light_compile_expression__char_ok(srcIter, src, working);
    int i;
    for(i = *srcIter; i < topaz_string_get_length(src); ++i) {
        int ch = topaz_string_get_char(src, i);
        if (isspace(ch)) {
            topaz_lua_light_compile_expression__char_ok(srcIter, src, working);        
        } else {
            break;
        }
    }
    
    
    topazString_t * str = topaz_lua_light_compile_expression__extract_var_name(srcIter, src, working);
    topaz_string_concat(working, str);
    topaz_string_destroy(str);
}


static void topaz_lua_light_compile_expression__wrap_varname(
    int debugLevel,
    int * srcIter,
    topazString_t * src,
    
    topazString_t * working
) {
    topazString_t * name = topaz_lua_light_compile_expression__extract_var_name(srcIter, src, working);
    topazString_t * namewrapped = topaz_string_create_from_c_str(
        "(Topaz.Lua.getLocal('%s', %d))",
        topaz_string_get_c_str(name),
        debugLevel + 3 
    );
    topaz_string_concat(working, namewrapped);
    topaz_string_destroy(namewrapped);
    topaz_string_destroy(name);
}    




static topazString_t * topaz_lua_light_compile_expression(
    int debugLevel,
    const char * orig
) {
    topazString_t * working = topaz_string_create();
    topazString_t * src = topaz_string_create_from_c_str("%s", orig);
    
    int srcIter = 0;
    while(srcIter < topaz_string_get_length(src)) {
        int ch = topaz_string_get_char(src, srcIter);
        
        switch(ch) {
            case '\'':
                topaz_lua_light_compile_expression__consume_string__single(&srcIter, src, working);
                continue;
                break;
            case '"':
                topaz_lua_light_compile_expression__consume_string__double(&srcIter, src, working);
                continue;
                break;

                
            case '.':
            case ':':
                topaz_lua_light_compile_expression__consume_str_index(&srcIter, src, working);
                continue;
                break;
            default:;
        }
                
        if (isalpha(ch)) {
            topaz_lua_light_compile_expression__wrap_varname(debugLevel, &srcIter, src,  working);
        } else {
            topaz_lua_light_compile_expression__char_ok(&srcIter, src, working);        
        }
    }
    
    

    topazString_t * output = topaz_string_create_from_c_str(
        "return(Topaz.Lua.objectToString(%s))",
        topaz_string_get_c_str(working)
    );
    topaz_string_destroy(working);
    topaz_string_destroy(src);
    return output;
}

void topaz_lua_debug_send_command(
    topazScript_t * scr, 
    void * data, 
    int cmd, 
    const topazString_t * arg
) {
    TOPAZLUA * ctx = data;
    switch(cmd) {
      case topazScript_DebugCommand_Pause: {
        topaz_lua_pause_trap(scr, ctx);        
        break;    
      }
      case topazScript_DebugCommand_Resume:
        topaz_context_resume(ctx->ctx);
        topaz_script_notify_command(
            scr,
            topazScript_DebugCommand_Resume,
            topaz_string_create()
        );
        //topaz_lua_trans_command__resume(ctx->trans_ctx);
        break;    

      case topazScript_DebugCommand_StepInto:
        //topaz_lua_trans_command__step_into(ctx->trans_ctx);
        break;

      case topazScript_DebugCommand_StepOver:
        //topaz_lua_trans_command__step_over(ctx->trans_ctx);
        break;

      case topazScript_DebugCommand_AddBreakpoint: {
        /*
        topazString_t * str = topaz_string_clone(arg);
        const topazString_t * iter = topaz_string_chain_start(str, TOPAZ_STR_CAST(":"));
        topazString_t * filename = topaz_string_clone(iter);
        iter = topaz_string_chain_proceed(str);        
        int lineNumber = atoi(topaz_string_get_c_str(iter));
                
        topaz_lua_trans_command__add_breakpoint(ctx->trans_ctx, filename, lineNumber);
        break;
        */
      }

      case topazScript_DebugCommand_RemoveBreakpoint: {
        /*
        int id = atoi(topaz_string_get_c_str(arg));
        uint32_t i;
        uint32_t len = topaz_array_get_size(ctx->debugBreakpoints);
        for(i = 0; i < len; ++i) {
            if (topaz_array_at(ctx->debugBreakpoints, int, i) == id) {
                topaz_lua_trans_command__delete_breakpoint(ctx->trans_ctx, i);
                return;
            }
        } 
        topaz_lua_trans_command__delete_breakpoint(ctx->trans_ctx, -1);
        break;
        */
      }

      case topazScript_DebugCommand_ScopedEval: {
        topazString_t * str = topaz_string_clone(arg);
        topazString_t * number = topaz_string_clone(topaz_string_chain_start(str, TOPAZ_STR_CAST(":")));
        int scope = atoi(topaz_string_get_c_str(number));

        topazString_t * command = topaz_lua_light_compile_expression(
            scope,
            topaz_string_get_c_str(topaz_string_get_substr(
                str, 
                topaz_string_get_length(number)+1, // skip the :
                topaz_string_get_length(str)-1
            ))            
        );


 
        topazScript_DebugCommand_t c = topazScript_DebugCommand_ScopedEval;
        topaz_array_push(ctx->lastCommand, c); 
        topaz_array_push(ctx->lastCommandArgs, command);
        topaz_array_push(ctx->lastCommandArgs, number);
        topaz_string_destroy(str);
        break;
      }
      default:;
    }
}




const topazScript_DebugState_t * topaz_lua_debug_get_state(topazScript_t * scr, void * data) {
    TOPAZLUA * ctx = data;
    topaz_lua_populate_stack(ctx);
    return &ctx->debugState;
}



const topazString_t * topaz_lua_file_extension(topazScript_t * src, void * data) {
    TOPAZLUA * ctx = data;
    return ctx->ext;
} 

static void NOOP(){};

void topaz_system_script_lua__backend(
    topazSystem_t *          system, 
    topazSystem_Backend_t *  backend, 
    topazScriptAPI_t      *  api
) {
    TOPAZLUABackend * b = calloc(1, sizeof(TOPAZLUABackend));
    b->instances = topaz_array_create(sizeof(TOPAZLUA*));
    topaz_system_backend_bind(
        backend,
        // name
        TOPAZ_STR_CAST("Lua"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("https://lua.org/"),

        // desc 
        TOPAZ_STR_CAST("Bindings to use the ubiquitous lua scripting language."),



        // on step 
        NULL,
        
        // on step late 
        topaz_lua_cooperate_update,
        
        // on draw 
        NULL,

        // on draw late
        NULL,



        // backend callback user data
        b,


        // API version 
        TOPAZ__VERSION__MAJOR,
        TOPAZ__VERSION__MINOR,
        TOPAZ__VERSION__MICRO
    );


    api->objectAPI.object_reference_create_from_reference = topaz_lua_object_reference_create_from_reference;
    api->objectAPI.object_reference_destroy = topaz_lua_object_reference_destroy;
    api->objectAPI.object_reference_get_feature_mask = topaz_lua_object_reference_get_feature_mask;
    api->objectAPI.object_reference_get_native_data = topaz_lua_object_reference_get_native_data;
    api->objectAPI.object_reference_set_native_data = topaz_lua_object_reference_set_native_data;
    api->objectAPI.object_reference_ref = topaz_lua_object_reference_ref;
    api->objectAPI.object_reference_unref = topaz_lua_object_reference_unref;
    api->objectAPI.object_reference_call = topaz_lua_object_reference_call;
    api->objectAPI.object_reference_array_get_nth = topaz_lua_object_reference_array_get_nth;
    api->objectAPI.object_reference_array_get_count = topaz_lua_object_reference_array_get_count;
    api->objectAPI.object_reference_map_get_property = topaz_lua_object_reference_map_get_property;
    api->objectAPI.object_reference_to_string = topaz_lua_object_reference_to_string;

    api->script_create = topaz_lua_create;
    api->script_destroy = topaz_lua_destroy;
    api->script_map_native_function = topaz_lua_map_native_function;
    api->script_run = topaz_lua_run;
    api->script_expression = topaz_lua_expression;
    api->script_create_empty_object = topaz_lua_create_empty_object;
    api->script_throw_error = topaz_lua_throw_error;
    api->script_bootstrap = topaz_lua_bootstrap;
    api->script_file_extension = topaz_lua_file_extension;


    api->script_debug_start = topaz_lua_debug_start;
    api->script_debug_send_command = topaz_lua_debug_send_command;
    api->script_debug_get_state = topaz_lua_debug_get_state;
}





