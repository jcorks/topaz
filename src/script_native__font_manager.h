#include <topaz/modules/font_manager.h>


TSO_SCRIPT_API_FN(font_manager_api__preload_glyphs) {
    TSO_ASSERT_ARG_COUNT(3);
    TSO_ARG_0;
    TSO_ARG_1;
    TSO_ARG_2;

    topazFontManager_t * r = topaz_context_get_font_manager(((topazScriptManager_t*)context)->ctx);
    topaz_font_manager_preload_glyphs(
        r,
        topaz_script_object_as_string(arg0),
        topaz_script_object_as_int(arg1),
        topaz_script_object_as_string(arg2)
    );


    TSO_NO_RETURN;
}

TSO_SCRIPT_API_FN(font_manager_api__register_font) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    topazFontManager_t * r = topaz_context_get_font_manager(((topazScriptManager_t*)context)->ctx);
    topaz_font_manager_register_font(
        r,
        topaz_script_object_as_string(arg0)
    );

    TSO_NO_RETURN;
}


TSO_SCRIPT_API_FN(font_manager_api__unregister_font) {
    TSO_ASSERT_ARG_COUNT(1);
    TSO_ARG_0;

    topazFontManager_t * r = topaz_context_get_font_manager(((topazScriptManager_t*)context)->ctx);
    topaz_font_manager_register_font(
        r,
        topaz_script_object_as_string(arg0)
    );

    TSO_NO_RETURN;
}






static void add_refs__font_manager_api(topazScript_t * script, topazScriptManager_t * context) {
    TS_MAP_NATIVE_FN("topaz_font_manager__register_font", font_manager_api__register_font);
    TS_MAP_NATIVE_FN("topaz_font_manager__preload_glyphs", font_manager_api__preload_glyphs);
    TS_MAP_NATIVE_FN("topaz_font_manager__unregister_font", font_manager_api__unregister_font);
}
