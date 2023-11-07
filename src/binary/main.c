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



static void window_close_callback(
    /// The display responding to the event.
    topazDisplay_t * display,
    /// The data bound to the callback.
    void * ctx
) {
    topaz_context_quit(ctx);
}

int main(int argc, char ** argv) {
    topazString_t * path = NULL;
    if (argc > 1)
        path = topaz_string_create_from_c_str(argv[1]);
 
    // Create the context and window
    topaz_t * ctx = topaz_context_create();


    // Creates a script instance. The permissions can 
    // activate / deactive certain features within the script context.
    topazScript_t * script = topaz_script_manager_create_context(
        topaz_context_get_script_manager(ctx),
        topazScriptManager_Permission_All
    );

    if (path == NULL) {
        path = topaz_string_create_from_c_str("main.");
        topaz_string_concat(path, topaz_script_get_file_extension(script));
    }
        



    


    // Optional
    topazDisplay_t * display = topaz_view_manager_create_display(topaz_context_get_view_manager(ctx), TOPAZ_STR_CAST(""));

    // add behavior for system X button
    topaz_display_add_close_callback(
        display,
        window_close_callback,
        ctx
    );

    run_script(ctx, script, TOPAZ_STR_CAST("preload"));

    if (!run_script(ctx, script, path)) {

        // We want to enable use of the debugging console.
        //
        topazConsole_t * console = topaz_context_get_console(ctx);
        topaz_console_enable(console, TRUE);

        topazString_t * message = topaz_string_create_from_c_str("Script \"%s\" could not be opened or was empty.", topaz_string_get_c_str(path));
        topaz_console_print(console, message);
        topaz_string_destroy(message);
    } 
    topaz_context_run(ctx);
}
