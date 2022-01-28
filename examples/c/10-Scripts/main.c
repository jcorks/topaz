#include <topaz/topaz.h>
#include <topaz/modules/view_manager.h>
#include <topaz/all.h>


static int run_script(topaz_t * ctx, topazScript_t * script, const topazString_t * name) {
    // extract the script data
    topazFilesystem_t * fs = topaz_context_get_filesystem(ctx);
    const topazFilesystem_Path_t * path = topaz_filesystem_get_path_from_string(
        fs, 
        topaz_filesystem_get_path(fs, topazFilesystem_DefaultNode_Resources),
        name    
    );
    if (!path) {
        return 0;
    }
    topazRbuffer_t * scriptFile = topaz_filesystem_path_read(path);
    if (!scriptFile) {
        return 0;
    }
    const topazString_t * scriptText  = topaz_rbuffer_read_string(
        scriptFile,
        topaz_rbuffer_get_size(scriptFile)
    );
          
    topaz_script_run(
        script,
        name,
        scriptText
    );
    return 1;   
}

static void preload_all_scripts(topaz_t * ctx) {
    
    // load all .mt files in the first in case theyre going to be included.
    const topazFilesystem_Path_t * s = topaz_filesystem_get_path(
        topaz_context_get_filesystem(ctx),
        topazFilesystem_DefaultNode_Resources
    );
    
    const topazArray_t * sources = topaz_filesystem_path_get_children(s);
    uint32_t i;
    uint32_t len = topaz_array_get_size(sources);
    for(i = 0; i < len; ++i) {
        if (topaz_string_test_contains(
                topaz_filesystem_path_as_string(
                    topaz_array_at(
                        sources, 
                        topazFilesystem_Path_t *, 
                        i
                    )
                ), 
                
                TOPAZ_STR_CAST(".mt")
            )
        ) {
            topaz_resources_load_asset(
                topaz_context_get_resources(ctx),
                TOPAZ_STR_CAST(""),
                topaz_filesystem_path_as_string(
                    topaz_array_at(
                        sources, 
                        topazFilesystem_Path_t *, 
                        i
                    )
                ), 
                topaz_filesystem_path_get_name(
                    topaz_array_at(
                        sources, 
                        topazFilesystem_Path_t *, 
                        i
                    )
                )
            );
        }
    }
}

static void window_close_callback(
    /// The display responding to the event.
    topazDisplay_t * display,
    /// The data bound to the callback.
    void * ctx
) {
    topaz_context_quit(ctx);
}

int main(int argc, char ** argv) {
    char * path = "script";
    if (argc > 1)
        path = argv[1];

    // Create the context and window
    topaz_t * ctx = topaz_context_create();


    // Creates a script instance. The permissions can 
    // activate / deactive certain features within the script context.
    topazScript_t * script = topaz_script_manager_create_context(
        topaz_context_get_script_manager(ctx),
        topazScriptManager_Permission_All
    );

    // enable debugging!
    topaz_script_enable_debugging(script);


    
    // the script can import other scripts.
    // The scripts that can be imported are through resources
    preload_all_scripts(ctx);
    


    // Optional
    topazDisplay_t * display = topaz_view_manager_create_main_default(topaz_context_get_view_manager(ctx), TOPAZ_STR_CAST("Scripting"));

    // add behavior for system X button
    topaz_display_add_close_callback(
        display,
        window_close_callback,
        ctx
    );

    run_script(ctx, script, TOPAZ_STR_CAST("preload"));

    if (!run_script(ctx, script, TOPAZ_STR_CAST(path))) {

        // We want to enable use of the debugging console.
        //
        topazConsole_t * console = topaz_context_get_console(ctx);
        topaz_console_enable(console, TRUE);

        topazString_t * message = topaz_string_create_from_c_str("Script \"%s\" could not be opened or was empty.", path);
        topaz_console_print(console, message);
        topaz_string_destroy(message);
        return 1;
    } 
    topaz_context_run(ctx);
}
