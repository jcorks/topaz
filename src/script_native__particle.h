/// Since particles dont have a "managed life" on their own, we wont set 
#include <topaz/particle.h>




static topazScript_Object_t * particle_api__cleanup(
    topazScript_t * script, 
    topazScript_Object_t ** args,
    void * userData
) {
    topaz_particle_destroy(userData);
    TSO_NO_RETURN;
}




TSO_SCRIPT_API_FN(particle_api__create) {
    topazScriptManager_t * scriptManager = context;
    topazParticle_t * ptr = topaz_particle_create(scriptManager->ctx);

    // creates new object and sets native pointer
    TSO_OBJECT_NEW_VALUE(ptr, TSO_OBJECT_ID__PARTICLE, particle_api__cleanup, ptr);
    
    return object;
}

TSO_SCRIPT_API_FN(particle_api__to_string) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazParticle_t *, TSO_OBJECT_ID__PARTICLE);

    return topaz_script_object_from_string(script, topaz_particle_to_string(native));    
}
TSO_SCRIPT_API_FN(particle_api__set_from_string) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazParticle_t *, TSO_OBJECT_ID__PARTICLE);

    topaz_particle_set_from_string(native, topaz_script_object_as_string(arg1));
    TSO_NO_RETURN; 
}


TSO_SCRIPT_API_FN(particle_api__set_image) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazParticle_t *, TSO_OBJECT_ID__PARTICLE);
    TSO_NATIVIZE_1(topazAsset_t *, TSO_OBJECT_TYPE__ASSET);   

    topaz_particle_set_image(native, native1);
    TSO_NO_RETURN; 
}
TSO_SCRIPT_API_FN(particle_api__get_attribute) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazParticle_t *, TSO_OBJECT_ID__PARTICLE);

    return topaz_script_object_from_number(
        script,
        topaz_renderer_attributes_get_attribute(
            topaz_particle_get_attributes(
                native
            ),
            topaz_script_object_as_number(arg1)            
        )
    );
}

TSO_SCRIPT_API_FN(particle_api__set_attribute) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazParticle_t *, TSO_OBJECT_ID__PARTICLE);

    topazRenderer_Attributes_t att = *topaz_particle_get_attributes(native);
    topaz_renderer_attributes_set_attribute(
        &att,
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    );
    topaz_particle_set_attributes(native, &att);
    TSO_NO_RETURN;
}
TSO_SCRIPT_API_FN(particle_api__set_noise_min) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazParticle_t *, TSO_OBJECT_ID__PARTICLE);

    topaz_particle_set_noise_min(
        native, 
        topaz_script_object_as_int(arg1),
        topaz_script_object_as_number(arg2)
    );
    TSO_NO_RETURN; 
}
TSO_SCRIPT_API_FN(particle_api__set_noise_max) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazParticle_t *, TSO_OBJECT_ID__PARTICLE);

    topaz_particle_set_noise_max(
        native, 
        topaz_script_object_as_int(arg1),
        topaz_script_object_as_number(arg2)
    );
    TSO_NO_RETURN; 
}
TSO_SCRIPT_API_FN(particle_api__set_function) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazParticle_t *, TSO_OBJECT_ID__PARTICLE);

    topaz_particle_set_function(
        native, 
        topaz_script_object_as_int(arg1),
        topaz_script_object_as_string(arg2)
    );
    TSO_NO_RETURN; 
}




static void add_refs__particle_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_particle__create", particle_api__create, 0);

    // member functions
    TS_MAP_NATIVE_FN("topaz_particle__to_string", particle_api__to_string, 1);
    TS_MAP_NATIVE_FN("topaz_particle__set_from_string", particle_api__set_from_string, 2);
    TS_MAP_NATIVE_FN("topaz_particle__set_image", particle_api__set_image, 2);
    TS_MAP_NATIVE_FN("topaz_particle__set_attribute", particle_api__set_attribute, 3);
    TS_MAP_NATIVE_FN("topaz_particle__get_attribute", particle_api__get_attribute, 2);
    TS_MAP_NATIVE_FN("topaz_particle__set_noise_min", particle_api__set_noise_min, 3);
    TS_MAP_NATIVE_FN("topaz_particle__set_noise_max", particle_api__set_noise_max, 3);
    TS_MAP_NATIVE_FN("topaz_particle__set_function", particle_api__set_function, 3);

    
}
