/// Since particles dont have a "managed life" on their own, we wont set 
#include <topaz/particle.h>




static topazScript_Object_t * particle_emitter_2d_api__cleanup(
    topazScript_t * script, 
    topazScript_Object_t ** args,
    void * userData
) {
    topaz_entity_remove(userData);
    TSO_NO_RETURN;
}




TSO_SCRIPT_API_FN(particle_emitter_2d_api__create) {
    topazEntity_t * ptr = topaz_particle_emitter_2d_create(
        ((topazScriptManager_t*)context)->ctx
    );

    // creates new object and sets native pointer
    TSO_OBJECT_NEW_VALUE(ptr, TSO_OBJECT_ID__PARTICLE_EMITTER_2D | TSO_OBJECT_TYPE__ENTITY, particle_emitter_2d_api__cleanup, ptr);
    
    TSO_OBJECT_KEEP_REF(ptr);
    return object;
}





TSO_SCRIPT_API_FN(particle_emitter_2d_api__set_particle) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__PARTICLE_EMITTER_2D);
    TSO_NATIVIZE_1(topazParticle_t *, TSO_OBJECT_ID__PARTICLE);

    topaz_particle_emitter_2d_set_particle(
        native, 
        native1
    );
    TSO_NO_RETURN; 
}

TSO_SCRIPT_API_FN(particle_emitter_2d_api__emit) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazEntity_t *, TSO_OBJECT_ID__PARTICLE_EMITTER_2D);

    topaz_particle_emitter_2d_emit(
        native, 
        topaz_script_object_as_int(arg1)
    );
    TSO_NO_RETURN; 
}



static void add_refs__particle_emitter_2d_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_particle_emitter_2d__create", particle_emitter_2d_api__create, 0);

    // member functions
    TS_MAP_NATIVE_FN("topaz_particle_emitter_2d__set_particle", particle_emitter_2d_api__set_particle, 2);
    TS_MAP_NATIVE_FN("topaz_particle_emitter_2d__emit", particle_emitter_2d_api__emit, 2);




    
}
