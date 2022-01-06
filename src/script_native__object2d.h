#include <topaz/components/object2d.h>
#include <topaz/transform.h>

TSO_SCRIPT_API_FN(object2d_api__create) {
    topazComponent_t * component = topaz_object2d_create(((topazScriptManager_t*)context)->ctx);
    // creates new object and sets native pointer
    TSO_OBJECT_NEW_VALUE(component, TSO_OBJECT_TYPE__COMPONENT | TSO_OBJECT_ID__OBJECT2D, NULL, NULL);
    TSO_OBJECT_KEEP_REF(component);
    return object;
}

TSO_SCRIPT_API_FN(object2d_api__add_velocity) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);

    topaz_object2d_add_velocity(
        native, 
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    );
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(object2d_api__add_velocity_towards) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   
    TSO_NATIVIZE_2(topazVector_t *, TSO_OBJECT_ID__VECTOR);   

    topaz_object2d_add_velocity_towards(
        native,
        topaz_script_object_as_number(arg1),
        native2,
        topaz_script_object_as_number(arg3)
    );
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(object2d_api__set_velocity) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);

    topaz_object2d_set_velocity(
        native, 
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_number(arg2)
    );
    TSO_NO_RETURN;

}

TSO_SCRIPT_API_FN(object2d_api__set_velocity_towards) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_ARG_3;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   
    TSO_NATIVIZE_2(topazVector_t *, TSO_OBJECT_ID__VECTOR);   

    topaz_object2d_set_velocity_towards(
        native,
        topaz_script_object_as_number(arg1),
        native2,
        topaz_script_object_as_number(arg3)
    );
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(object2d_api__set_friction_x) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   

    topaz_object2d_set_friction_x(native, topaz_script_object_as_number(arg1));    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(object2d_api__set_friction_y) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   

    topaz_object2d_set_friction_y(native, topaz_script_object_as_number(arg1));    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(object2d_api__get_friction_x) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   

    return topaz_script_object_from_number(script, topaz_object2d_get_friction_x(native));
}

TSO_SCRIPT_API_FN(object2d_api__get_friction_y) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   

    return topaz_script_object_from_number(script, topaz_object2d_get_friction_y(native));
}

TSO_SCRIPT_API_FN(object2d_api__get_direction) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   

    return topaz_script_object_from_number(script, topaz_object2d_get_direction(native));
}

TSO_SCRIPT_API_FN(object2d_api__halt) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   

    topaz_object2d_halt(native);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(object2d_api__reset_motion) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   

    topaz_object2d_reset_motion(native);
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(object2d_api__set_velocity_x) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   

    topaz_object2d_set_velocity_x(native, topaz_script_object_as_number(arg1));    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(object2d_api__set_velocity_y) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   

    topaz_object2d_set_velocity_y(native, topaz_script_object_as_number(arg1));    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(object2d_api__get_velocity_x) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   

    return topaz_script_object_from_number(script, topaz_object2d_get_velocity_x(native));
}

TSO_SCRIPT_API_FN(object2d_api__get_velocity_y) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   

    return topaz_script_object_from_number(script, topaz_object2d_get_velocity_y(native));
}

TSO_SCRIPT_API_FN(object2d_api__set_speed) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   

    topaz_object2d_set_speed(native, topaz_script_object_as_number(arg1));    
    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(object2d_api__get_speed) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   

    return topaz_script_object_from_number(script, topaz_object2d_get_speed(native));
}


TSO_SCRIPT_API_FN(object2d_api__get_next_position) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   
    topazVector_t * v;
    topazScript_Object_t * out = TSO_OBJECT_INSTANTIATE(vector_api__create, v);
    *v = topaz_object2d_get_next_position(native);
    return out;
}





TSO_SCRIPT_API_FN(object2d_api__set_group) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   

    topaz_object2d_set_group(native, topaz_script_object_as_int(arg1));    
    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(object2d_api__get_group) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   

    return topaz_script_object_from_int(script, topaz_object2d_get_group(native));    
}


TSO_SCRIPT_API_FN(object2d_api__set_group_interaction) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;

    topaz_object2d_set_group_interaction(
        ((topazScriptManager_t*)context)->ctx,
        topaz_script_object_as_int(arg0),
        topaz_script_object_as_int(arg1),    
        topaz_script_object_as_int(arg2)
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(object2d_api__set_collider) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   


    if (topaz_script_object_reference_get_feature_mask(arg1) & topazScript_Object_Feature_Array) {
        uint32_t len = topaz_script_object_reference_array_get_count(arg1);
        uint32_t i;
        topazVector_t * v = calloc(len, sizeof(topazVector_t));
        for(i = 0; i < len; i+=2) {
            v[i/2].x = topaz_script_object_as_number(topaz_script_object_reference_array_get_nth(arg1, i));
            v[i/2].y = topaz_script_object_as_number(topaz_script_object_reference_array_get_nth(arg1, i+1));
        }
        topaz_object2d_set_collider(
            native,
            TOPAZ_ARRAY_CAST(v, topazVector_t, len/2)
        );
        free(v);
    }
    TSO_NO_RETURN;    
}


TSO_SCRIPT_API_FN(object2d_api__set_collider_radial) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   

    topaz_object2d_set_collider_radial(
        native,
        topaz_script_object_as_number(arg1),
        topaz_script_object_as_int(arg2)    
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(object2d_api__get_collider_len) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   
    return topaz_script_object_from_int(script, topaz_array_get_size(topaz_object2d_get_collider(native)));
}

TSO_SCRIPT_API_FN(object2d_api__get_collider_point) {
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   

    uint32_t len = topaz_array_get_size(topaz_object2d_get_collider(native));
    uint32_t index = topaz_script_object_as_int(arg1);
    if (index >= len) {
        script_error(script, "Bad index for collider point!");
        TSO_NO_RETURN;
    }

    topazVector_t * v;
    topazScript_Object_t * out = TSO_OBJECT_INSTANTIATE(vector_api__create, v);
    *v = topaz_array_at(topaz_object2d_get_collider(native), topazVector_t, index) ;
    return out;
}

TSO_SCRIPT_API_FN(object2d_api__get_last_collided) {
    TSO_ARG_0;
    TSO_NATIVIZE(topazComponent_t *, TSO_OBJECT_ID__OBJECT2D);   

    topazEntity_t * out = topaz_object2d_get_last_collided(native);
    if (out) {
        topazScript_Object_t * a = TSO_OBJECT_FETCH_KEPT_NATIVE(out);
        if (a) return topaz_script_object_from_object(script, a);

        TSO_OBJECT_NEW_VALUE(out, TSO_OBJECT_TYPE__ENTITY, NULL, NULL);
        return object;
    }
    TSO_NO_RETURN;
}

static void add_refs__object2d_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_object2d__create", object2d_api__create, 0);
    TS_MAP_NATIVE_FN("topaz_object2d__add_velocity", object2d_api__add_velocity, 3);
    TS_MAP_NATIVE_FN("topaz_object2d__add_velocity_towards", object2d_api__add_velocity_towards, 4);
    TS_MAP_NATIVE_FN("topaz_object2d__set_velocity", object2d_api__set_velocity, 3);
    TS_MAP_NATIVE_FN("topaz_object2d__set_velocity_towards", object2d_api__set_velocity_towards, 4);


    TS_MAP_NATIVE_FN("topaz_object2d__set_friction_x", object2d_api__set_friction_x, 2);
    TS_MAP_NATIVE_FN("topaz_object2d__set_friction_y", object2d_api__set_friction_y, 2);
    TS_MAP_NATIVE_FN("topaz_object2d__get_friction_x", object2d_api__get_friction_x, 1);
    TS_MAP_NATIVE_FN("topaz_object2d__get_friction_y", object2d_api__get_friction_y, 1);
    TS_MAP_NATIVE_FN("topaz_object2d__get_direction", object2d_api__get_direction, 1);

    TS_MAP_NATIVE_FN("topaz_object2d__halt", object2d_api__halt, 1);
    TS_MAP_NATIVE_FN("topaz_object2d__reset_motion", object2d_api__reset_motion, 1);
    TS_MAP_NATIVE_FN("topaz_object2d__set_velocity_x", object2d_api__set_velocity_x, 2);
    TS_MAP_NATIVE_FN("topaz_object2d__set_velocity_y", object2d_api__set_velocity_y, 2);
    TS_MAP_NATIVE_FN("topaz_object2d__get_velocity_x", object2d_api__get_velocity_x, 1);
    TS_MAP_NATIVE_FN("topaz_object2d__get_velocity_y", object2d_api__get_velocity_y, 1);
    TS_MAP_NATIVE_FN("topaz_object2d__set_speed", object2d_api__set_speed, 2);
    TS_MAP_NATIVE_FN("topaz_object2d__get_speed", object2d_api__get_speed, 1);
    TS_MAP_NATIVE_FN("topaz_object2d__get_next_position", object2d_api__get_next_position, 1);

    TS_MAP_NATIVE_FN("topaz_object2d__get_group", object2d_api__get_group, 1);
    TS_MAP_NATIVE_FN("topaz_object2d__set_group", object2d_api__set_group, 2);
    TS_MAP_NATIVE_FN("topaz_object2d__set_group_interaction", object2d_api__set_group_interaction, 3);
    TS_MAP_NATIVE_FN("topaz_object2d__set_collider", object2d_api__set_collider, 2);
    TS_MAP_NATIVE_FN("topaz_object2d__set_collider_radial", object2d_api__set_collider_radial, 3);
    TS_MAP_NATIVE_FN("topaz_object2d__get_collider_len",   object2d_api__get_collider_len, 1);
    TS_MAP_NATIVE_FN("topaz_object2d__get_collider_point", object2d_api__get_collider_point, 2);

    TS_MAP_NATIVE_FN("topaz_object2d__get_last_collided", object2d_api__get_last_collided, 1);

}
