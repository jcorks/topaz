/// Since rngs dont have a "managed life" on their own, we wont set 
#include <topaz/rng.h>


static uint64_t rng_str_to_seed(const topazString_t * str) {
    uint64_t seed = 0;
    uint8_t * iter = (uint8_t*)&seed;    
    uint32_t i;
    uint32_t len = topaz_string_get_length(str);
    for(i = 0; i < len; ++i) {
        iter[i%sizeof(uint64_t)] += topaz_string_get_char(str, i);
    }
    return seed;
}


TSO_SCRIPT_API_FN(rng_api__set_seed) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazRNG_t *, TSO_OBJECT_ID__RNG);

    topaz_rng_set_seed(native, rng_str_to_seed(topaz_script_object_as_string(arg1)));        
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(rng_api__next_int) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazRNG_t *, TSO_OBJECT_ID__RNG);

    return topaz_script_object_from_int(script, topaz_rng_next_int(native));    
}

TSO_SCRIPT_API_FN(rng_api__next_value) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazRNG_t *, TSO_OBJECT_ID__RNG);

    return topaz_script_object_from_number(script, topaz_rng_next_value(native));    
}



static topazScript_Object_t * rng_api__cleanup(
    topazScript_t * script, 
    const topazArray_t * args, 
    void * userData
) {
    free(userData);
    TSO_NO_RETURN;
}




TSO_SCRIPT_API_FN(rng_api__create) {
    topazRNG_t * ptr = topaz_rng_create();

    if (topaz_array_get_size(args) == 1) { // from string
        TSO_ARG_0;
        topaz_rng_set_seed(ptr, rng_str_to_seed(topaz_script_object_as_string(arg0)));        
    } 

    // creates new object and sets native pointer
    TSO_OBJECT_NEW_VALUE(ptr, TSO_OBJECT_ID__RNG, rng_api__cleanup, ptr);


    return object;
}





static void add_refs__rng_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_rng__create", rng_api__create);
    
    TS_MAP_NATIVE_FN("topaz_rng__set_seed", rng_api__set_seed);
    TS_MAP_NATIVE_FN("topaz_rng__next_int", rng_api__next_int);
    TS_MAP_NATIVE_FN("topaz_rng__next_value", rng_api__next_value);
}





