#include <topaz/assets/sound.h>



TSO_SCRIPT_API_FN(console_api__enable) {
    topazScriptManager_t * mgr = context;

    topazConsole_t * console = topaz_context_get_console(mgr->ctx);
    topaz_console_attach_script(console, script);
    topaz_console_enable(console, TRUE);

    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(console_api__print_message) {
    TSO_ARG_0;
    TSO_ARG_1;
    topazScriptManager_t * mgr = context;
    topazConsole_t * c = topaz_context_get_console(mgr->ctx);
    topaz_console_print_message(c, topaz_script_object_as_string(arg0), topaz_script_object_as_int(arg1));
    TSO_NO_RETURN;    
}

typedef struct {
    topazScript_t * script;
    topazScript_Object_t * fn;
} ConsoleListenerData;

static void script_listener(
    topazConsole_t * c,
    const topazString_t * line,
    topazConsole_MessageType_t type,	
    void * userdata
) {
    ConsoleListenerData * data = userdata;
    topazScript_Object_t * args[] = {
        topaz_script_object_from_string(data->script, line),
        topaz_script_object_from_int(data->script, type)
    };
    topaz_script_object_reference_call(
        data->fn,    
        TOPAZ_ARRAY_CAST(args, topazScript_Object_t *, 2)        
    );
    topaz_script_object_destroy(args[0]);
    topaz_script_object_destroy(args[1]);
}

TSO_SCRIPT_API_FN(console_api__add_listener) {
    TSO_ARG_0;
    topazScriptManager_t * mgr = context;
    topazConsole_t * c = topaz_context_get_console(mgr->ctx);
    topazScript_Object_t * o = topaz_script_object_from_object(script, arg0);
    topaz_script_object_reference_ref(o);

    ConsoleListenerData * data = calloc(1, sizeof(ConsoleListenerData));
    data->fn = o;
    data->script = script;

    int id = topaz_console_add_listener(c, script_listener, data);
    return topaz_script_object_from_int(script, id);
}

TSO_SCRIPT_API_FN(console_api__remove_listener) {
    TSO_ARG_0;
    topazScriptManager_t * mgr = context;
    topazConsole_t * c = topaz_context_get_console(mgr->ctx);
    int id = topaz_script_object_as_int(arg0);
    
    topaz_console_remove_listener(c, id);
    TSO_NO_RETURN;        
}



TSO_SCRIPT_API_FN(console_api__run) {
    TSO_ARG_0;
    topazScriptManager_t * mgr = context;
    topazConsole_t * c = topaz_context_get_console(mgr->ctx);
    
    topazString_t * str = topaz_console_run(c, topaz_script_object_as_string(arg0));
    topazScript_Object_t * out = topaz_script_object_from_string(script, str);
    topaz_string_destroy(str);
    return out;
}

TSO_SCRIPT_API_FN(console_api__command_context_create) {    
    topazConsole_CommandContext_t * c = topaz_console_command_context_create();
    TSO_OBJECT_NEW_VALUE(c, TSO_OBJECT_ID__CONSOLECOMMANDCONTEXT, NULL, NULL);
    TSO_OBJECT_KEEP_REF(c);
    return object;
}

TSO_SCRIPT_API_FN(console_api__command_context_destroy) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazConsole_CommandContext_t * , TSO_OBJECT_ID__CONSOLECOMMANDCONTEXT);   
    topaz_console_command_context_destroy(native);
    TSO_NO_RETURN;
}



typedef struct {
    topazScript_t * script;
    topazScript_Object_t * addArg;
    topazScript_Object_t * doAction;
} ConsoleCommandBridgeData;

static topazString_t * script_console_context_command_bridge(
    topazConsole_t * console, 
    void * callbackData, 
    const topazArray_t * args,
    const topazString_t * fullCommand
) {
    ConsoleCommandBridgeData * bridge = callbackData;
    
    uint32_t i;
    uint32_t len = topaz_array_get_size(args);
    for(i = 0; i < len; ++i) {
        topazScript_Object_t * arg = topaz_script_object_from_string(bridge->script, topaz_array_at(args, topazString_t*, i));
        topaz_script_object_reference_call(
            bridge->addArg,
            TOPAZ_ARRAY_CAST(&arg, topazScript_Object_t *, 1)
        );
        topaz_script_object_destroy(arg);
    }
    
    topazScript_Object_t * full = topaz_script_object_from_string(bridge->script, fullCommand);   
    topazScript_Object_t * obj = topaz_script_object_reference_call(
        bridge->doAction,
        TOPAZ_ARRAY_CAST(&full, topazScript_Object_t *, 1)
    );
    topaz_script_object_destroy(full);
    topazString_t * out = topaz_string_clone(topaz_script_object_as_string(obj));
    topaz_script_object_destroy(obj);
    return out;
}


TSO_SCRIPT_API_FN(console_api__command_context_add_command) {
    TSO_ARG_0;    
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazConsole_CommandContext_t *, TSO_OBJECT_ID__CONSOLECOMMANDCONTEXT);   
    
    if (!((topaz_script_object_reference_get_feature_mask(arg2) & topazScript_Object_Feature_Callable) &&
          (topaz_script_object_reference_get_feature_mask(arg3) & topazScript_Object_Feature_Callable))) {
        TSO_NO_RETURN;          
    }
    ConsoleCommandBridgeData * data = calloc(1, sizeof(ConsoleCommandBridgeData));
    data->script = script;
    data->addArg = topaz_script_object_from_object(script, arg2);
    data->doAction = topaz_script_object_from_object(script, arg3);

        
    topaz_console_command_context_add_command(
        native,
        topaz_script_object_as_string(arg1),
        script_console_context_command_bridge,
        data
    );

    TSO_NO_RETURN;    
}

TSO_SCRIPT_API_FN(console_api__command_context_set_default_handler) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazConsole_CommandContext_t *, TSO_OBJECT_ID__CONSOLECOMMANDCONTEXT);   
    if (!((topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Callable) &&
          (topaz_script_object_reference_get_feature_mask(arg2) & topazScript_Object_Feature_Callable))) {
        TSO_NO_RETURN;          
    }
    ConsoleCommandBridgeData * data = calloc(1, sizeof(ConsoleCommandBridgeData));
    data->script = script;
    data->addArg = topaz_script_object_from_object(script, arg1);
    data->doAction = topaz_script_object_from_object(script, arg2);


        
    topaz_console_command_context_set_default_handler(
        native,
        script_console_context_command_bridge,
        data
    );

    TSO_NO_RETURN;    
}

TSO_SCRIPT_API_FN(console_api__command_context_set_prompt) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazConsole_CommandContext_t  *, TSO_OBJECT_ID__CONSOLECOMMANDCONTEXT);   
    
    topaz_console_command_context_set_prompt(
        native,
        topaz_script_object_as_string(arg1)
    );
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(console_api__push_command_context) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazConsole_CommandContext_t  *, TSO_OBJECT_ID__CONSOLECOMMANDCONTEXT);   
    
    topazScriptManager_t * mgr = context;
    topazConsole_t * c = topaz_context_get_console(mgr->ctx);

    
    topaz_console_push_command_context(
        c,
        native
    );
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(console_api__pop_command_context) {    
    topazScriptManager_t * mgr = context;
    topazConsole_t * c = topaz_context_get_console(mgr->ctx);

    
    topaz_console_pop_command_context(
        c
    );
    TSO_NO_RETURN;
}





static void add_refs__console_api(topazScript_t * script, topazScriptManager_t * context) {
    // member functions
    TS_MAP_NATIVE_FN("topaz_console__enable", console_api__enable, 2);
    TS_MAP_NATIVE_FN("topaz_console__print_message", console_api__print_message, 2);
    TS_MAP_NATIVE_FN("topaz_console__add_listener", console_api__add_listener, 1);
    TS_MAP_NATIVE_FN("topaz_console__remove_listener", console_api__remove_listener, 1);
    TS_MAP_NATIVE_FN("topaz_console__run", console_api__run, 1);
    TS_MAP_NATIVE_FN("topaz_console__command_context_create", console_api__command_context_create, 0);
    TS_MAP_NATIVE_FN("topaz_console__command_context_destroy", console_api__command_context_destroy, 1);
    TS_MAP_NATIVE_FN("topaz_console__command_context_add_command", console_api__command_context_add_command, 4);
    TS_MAP_NATIVE_FN("topaz_console__command_context_set_default_handler", console_api__command_context_set_default_handler, 3);
    TS_MAP_NATIVE_FN("topaz_console__command_context_set_prompt", console_api__command_context_set_prompt, 2);
    TS_MAP_NATIVE_FN("topaz_console__push_command_context", console_api__push_command_context, 1);
    TS_MAP_NATIVE_FN("topaz_console__pop_command_context", console_api__pop_command_context, 0);
    

    

}
