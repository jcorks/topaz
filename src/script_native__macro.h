
// utility function for script errors
static void script_error(topazScript_t * src, const char * message) {
    topazScript_Object_t * messageObj = topaz_script_object_from_string(src, TOPAZ_STR_CAST(message));
    topaz_script_emit_event(
        src,
        topaz_Script_Event_OnError,
        TOPAZ_ARRAY_CAST(&messageObj, topazScript_Object_t *, 1)
    );
    topaz_script_object_destroy(messageObj);
}


static void * script_create_native_object(topazScript_t * script, topaz_script_native_function create, void ** nativeRef) {
    topazScript_Object_t * obj = create(script, TOPAZ_ARRAY_CAST(NULL, void*, 0), NULL);
    int unused;
    *nativeRef = topaz_script_object_reference_get_native_data(obj, &unused);
    return obj;
}



// main macro for defining functions
#define TSO_SCRIPT_API_FN(__name__) static topazScript_Object_t * __name__(topazScript_t * script, const topazArray_t * args, void * userData)


// macro for adding a property.
// requires:
// - TSO_OBJECT_NEW to be called within the same scope
#define TSO_PROP_ADD(__s__, __os__, __og__) topaz_script_object_reference_extendable_add_property(object, TOPAZ_STR_CAST(__s__), __os__, __og__)

// Creates a new object to be modified in the current scope.
// __native__ is the variable to that contains the native object 
// __tag__ is the unique ID tag for this type of object. this is varified unpon functions calls.
// __remover__ is the native function to call when the object reference is destroyed in the script context.
#define TSO_OBJECT_NEW(__native__, __tag__, __remover__) topazScript_Object_t * object = topaz_script_create_empty_object(script, __remover__, NULL); topaz_script_object_reference_set_native_data(object, __native__, __tag__); 

// Creates an object from a different context and returns its object and native ref.
#define TSO_OBJECT_INSTANTIATE(__nativecreate__, __native__) script_create_native_object(script, __nativecreate__, (void**)&__native__);

// Tells the scripting engine to prevent garbage collection on this newly created objectobject.
#define TSO_OBJECT_KEEP topaz_script_object_reference_ref(object);

// Tells the scripting engine that it can garbage collect arg0 if desired.
#define TSO_OBJECT_UNKEEP topaz_script_object_reference_unref(arg0);


// Ensures that, past this point, AT LEAST the given number of args is 
// passed to this function. If not, an error is "thrown"
#define TSO_ASSERT_ARG_COUNT(__count__) if (topaz_array_get_size(args) < __count__){script_error(script, "Insufficient arguments given to native function."); return topaz_script_object_undefined(script);}

// Retrieves and stores the given args
#define TSO_ARG_0 topazScript_Object_t * arg0 = topaz_array_at(args, topazScript_Object_t *, 0);
#define TSO_ARG_1 topazScript_Object_t * arg1 = topaz_array_at(args, topazScript_Object_t *, 1);
#define TSO_ARG_2 topazScript_Object_t * arg2 = topaz_array_at(args, topazScript_Object_t *, 2);
#define TSO_ARG_3 topazScript_Object_t * arg3 = topaz_array_at(args, topazScript_Object_t *, 3);

// Retrieves argN as a native object of the given type.
// If it is not the given type, an error is "thrown".
#define TSO_NATIVIZE(__type__, __assertTag__) int tag__;__type__ native = topaz_script_object_reference_get_native_data(arg0, &tag__); if (tag__ != __assertTag__) {script_error(script, "Native object type mismatch."); return topaz_script_object_undefined(script);}
#define TSO_NATIVIZE_1(__type__, __assertTag__) int tag1__;__type__ native1 = topaz_script_object_reference_get_native_data(arg1, &tag1__); if (tag1__ != __assertTag__) {script_error(script, "Native object type mismatch."); return topaz_script_object_undefined(script);}
#define TSO_NATIVIZE_2(__type__, __assertTag__) int tag2__;__type__ native2 = topaz_script_object_reference_get_native_data(arg2, &tag2__); if (tag2__ != __assertTag__) {script_error(script, "Native object type mismatch."); return topaz_script_object_undefined(script);}
#define TSO_NATIVIZE_3(__type__, __assertTag__) int tag3__;__type__ native3 = topaz_script_object_reference_get_native_data(arg3, &tag3__); if (tag3__ != __assertTag__) {script_error(script, "Native object type mismatch."); return topaz_script_object_undefined(script);}

// Default return value.
#define TSO_NO_RETURN return NULL;

// macro for assigning a native function
#define TS_MAP_NATIVE_FN(__name__, __fn__) topaz_script_map_native_function(script, TOPAZ_STR_CAST(__name__), __fn__, NULL)

