#include <topaz/compat.h>
#include <topaz/backends/input_manager.h>
#include <topaz/containers/table.h>
#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif


struct topazInputManager_t {
    topazInputManagerAPI_t api;
    topazSystem_Backend_t * backend;
};





topazInputManager_t * topaz_input_manager_create(topazSystem_Backend_t * b, topazInputManagerAPI_t api) {
    #ifdef TOPAZDC_DEBUG
        assert(b && "topazSystem_Backend_t pointer cannot be NULL.");
        assert(api.input_manager_create);
        assert(api.input_manager_destroy);
        assert(api.input_manager_handle_events);
        assert(api.input_manager_query_device);
        assert(api.input_manager_query_auxiliary_devices);
        assert(api.input_manager_max_devices);
        assert(api.input_manager_set_focus);
        assert(api.input_manager_get_focus);
        assert(api.input_manager_show_virtual_keyboard);
    #endif
    topazInputManager_t * out = calloc(1, sizeof(topazInputManager_t));
    out->api = api;
    out->backend = b;
    out->api.input_manager_create(&out->api);
    return out;
}


void topaz_input_manager_destroy(topazInputManager_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
    #endif
    t->api.input_manager_destroy(&t->api);
}





topazSystem_Backend_t * topaz_input_manager_get_backend(topazInputManager_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
    #endif

    return t->backend;
}

topazInputManagerAPI_t topaz_input_manager_get_api(topazInputManager_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazSystem_Backend_t pointer cannot be NULL.");
    #endif

    return t->api;
}


static char * builtIn[] = {
    "NotAnInput",
    "Key_0", ///< 0
    "Key_1", ///< 1
    "Key_2", ///< 2
    "Key_3", ///< 3
    "Key_4", ///< 4
    "Key_5", ///< 5
    "Key_6", ///< 6
    "Key_7", ///< 7
    "Key_8", ///< 8
    "Key_9", ///< 9
    "Key_a", ///< a
    "Key_b", ///< b
    "Key_c", ///< c
    "Key_d", ///< d
    "Key_e", ///< e
    "Key_f", ///< f
    "Key_g", ///< g
    "Key_h", ///< h
    "Key_i", ///< i
    "Key_j", ///< j
    "Key_k", ///< k
    "Key_l", ///< l
    "Key_m", ///< m
    "Key_n", ///< n
    "Key_o", ///< o
    "Key_p", ///< p
    "Key_q", ///< q
    "Key_r", ///< r
    "Key_s", ///< s
    "Key_t", ///< t
    "Key_u", ///< u
    "Key_v", ///< v
    "Key_w", ///< w
    "Key_x", ///< x
    "Key_y", ///< y
    "Key_z", ///< z
    "Key_lshift", ///< Left shift key
    "Key_rshift", ///< Right shift key
    "Key_lctrl",  ///< Left control key
    "Key_rctrl",  ///< Right control key
    "Key_lalt",   ///< Left alt key
    "Key_ralt",   ///< Right alt key
    "Key_tab",    ///< Tab
    "Key_F1",     ///< F1
    "Key_F2",     ///< F2
    "Key_F3",     ///< F3
    "Key_F4",     ///< F4
    "Key_F5",     ///< F5
    "Key_F6",     ///< F6
    "Key_F7",     ///< F7
    "Key_F8",     ///< F8
    "Key_F9",     ///< F9
    "Key_F10",    ///< F10
    "Key_F11",    ///< F11
    "Key_F12",    ///< F12
    "Key_up",     ///< Up arrow
    "Key_down",   ///< Down arrow
    "Key_left",   ///< Left arrow
    "Key_right",  ///< Right arrow
    "Key_minus",  ///< -
    "Key_equal",  ///< = 
    "Key_backspace",  ///< Backspace
    "Key_grave",  ///< `
    "Key_esc",    ///< Escape
    "Key_home",   ///< Home key
    "Key_pageUp", ///< Page up key
    "Key_pageDown",  ///< Page down key
    "Key_end",    ///< End key
    "Key_backslash", ///< '\'
    "Key_lbracket", ///< [
    "Key_rbracket", ///< ]
    "Key_semicolon", ///< ;
    "Key_apostrophe", ///< '
    "Key_frontslash", ///< /
    "Key_enter", ///< Enter
    "Key_delete", ///< Delete
    "Key_numpad0", ///< Numpad 0
    "Key_numpad1", ///< Numpad 1
    "Key_numpad2", ///< Numpad 2
    "Key_numpad3", ///< Numpad 3
    "Key_numpad4", ///< Numpad 4
    "Key_numpad5", ///< Numpad 5
    "Key_numpad6", ///< Numpad 6
    "Key_numpad7", ///< Numpad 7
    "Key_numpad8", ///< Numpad 8
    "Key_numpad9", ///< Numpad 9
    "Key_prtscr", ///< Print screen button
    "Key_lsuper", ///< Left Super key (Windows key)
    "Key_rsuper", ///< Right Super key (Windows key)
    "Key_space",  ///< Space
    "Key_insert", ///< Insert key
    "Key_comma", ///< ,
    "Key_period", ///< .

    "Pointer_0", ///< Left click
    "Pointer_1", ///< Right click
    "Pointer_2", ///< Middle click

    "Pointer_X", ///< Horizontal axis. Usually for the X axis of the pointer
    "Pointer_Y", ///< Horizontal axis. Usually for the X axis of the pointer
    "Pointer_Wheel", ///< Mouse wheel.

    "Pad_a",     ///< Button 0
    "Pad_b",     ///< Button 1
    "Pad_c",     ///< Button 2
    "Pad_x",     ///< Button 3
    "Pad_y",     ///< Button 4
    "Pad_r",     ///< Button 5
    "Pad_l",     ///< Button 6
    "Pad_r2",    ///< Button 7
    "Pad_l2",    ///< Button 8
    "Pad_r3",    ///< Button 9
    "Pad_l3",    ///< Button 10
    "Pad_start",    ///< Button 11
    "Pad_select",///< Button 12
    "Pad_b13",///< Button 13
    "Pad_b14",///< Button 14
    "Pad_b15",///< Button 15
    "Pad_b16",///< Button 16
    "Pad_b17",///< Button 17
    "Pad_b18",///< Button 18
    "Pad_b19",///< Button 19
    "Pad_b20",///< Button 20
    "Pad_b21",///< Button 21
    "Pad_b22",///< Button 22
    "Pad_b23",///< Button 23
    "Pad_b24",///< Button 24
    "Pad_b25",///< Button 25
    "Pad_b26",///< Button 26
    "Pad_b27",///< Button 27
    "Pad_b28",///< Button 28
    "Pad_b29",///< Button 29
    "Pad_b30",///< Button 30
    "Pad_b31",///< Button 31
    "Pad_b32",///< Button 32

    "Pad_axisX", ///< X button
    "Pad_axisY", ///< Y button
    "Pad_axisZ", ///< Z button
    "Pad_axisX2",///< X2 button 
    "Pad_axisY2",///< Y2 button
    "Pad_axisZ2",///< Z2 button
    "Pad_axisX3",///< X3 button
    "Pad_axisY3",///< Y3 button
    "Pad_axisZ3",///< Z3 button
    "Pad_axisX4",///< X4 button
    "Pad_axisY4",///< Y4 button
    "Pad_axisZ4",///< Z4 button
    "Pad_axisX5",///< X4 button
    "Pad_axisY5",///< Y4 button
    "Pad_axisZ5",///< Z4 button
    "Pad_axisX6",///< X4 button
    "Pad_axisY6",///< Y4 button
    "Pad_axisZ6",///< Z4 button

    "Pad_axisR",    
    "Pad_axisL",    
    "Pad_axisR2",    
    "Pad_axisL2",    
    "Pad_axisR3",    
    "Pad_axisL3",    
    "Pad_axisR4",    
    "Pad_axisL4",    

    "Pad_options"
};
    

const topazString_t * topaz_input_manager_id_to_string(int id) {
    if (id < topazInput_Count) {
        return TOPAZ_STR_CAST(builtIn[id]);
    }
    static topazTable_t * table = NULL;
    if (!table) {
        table = topaz_table_create_hash_pointer();
    }

    topazString_t * c = topaz_table_find_by_int(table, id);
    if (!c) {
        c = topaz_string_create();
        topaz_string_concat_printf(c, "Input%d", id);
        topaz_table_insert_by_int(table, id, c);
    }
    return c;
}


int topaz_input_manager_handle_events(topazInputManager_t * t) {
    return t->api.input_manager_handle_events(&t->api);
}

topazInputDevice_t * topaz_input_manager_query_device(topazInputManager_t * t, int ID) {
    return t->api.input_manager_query_device(&t->api, ID);
}

int topaz_input_manager_query_auxiliary_devices(topazInputManager_t * t, int * IDs) {
    return t->api.input_manager_query_auxiliary_devices(&t->api, IDs);
}

int topaz_input_manager_max_devices(topazInputManager_t * t) {
    return t->api.input_manager_max_devices(&t->api);
}

void topaz_input_manager_set_focus(topazInputManager_t * t, topazDisplay_t * d) {
    t->api.input_manager_set_focus(&t->api, d);
}

topazDisplay_t * topaz_input_manager_get_focus(topazInputManager_t * t) {
    return t->api.input_manager_get_focus(&t->api);
}


void topaz_input_manager_show_virtual_keyboard(topazInputManager_t * t, int doIt) {
    t->api.input_manager_show_virtual_keyboard(&t->api, doIt);
}



