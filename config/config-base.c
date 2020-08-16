// Here, all the includes requires for the symbol 
// calls are listed. Example:
//
//     #include "../system/renderer_noRenderer/src/backend.h"
//
@config_handler_includes@

void topaz_system_configure_base() {

// Here all topaz_system_config_add_handle() calls are listed to 
// link the valid backend API implementations that are 
// compatible with the program. Example:
//
//     topaz_system_config_add_handler(TOPAZ_STR_CAST("renderer"), TOPAZ_STR_CAST("srgs"), topaz_system_renderer_srgs__backend, (void (*)(void *)) topaz_system_renderer_srgs__api);
//
@config_handler_calls@                        

}

