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

int main(int argc, char ** argv) {
    char * path = "script";
    if (argc > 1)
        path = argv[1];

    // Create the context and window
    topaz_t * ctx = topaz_context_create();
    topaz_view_manager_create_main_default(topaz_context_get_view_manager(ctx), TOPAZ_STR_CAST("Scripting"));


    // Creates a script instance. The permissions can 
    // activate / deactive certain features within the script context.
    topazScript_t * script = topaz_script_manager_create_context(
        topaz_context_get_script_manager(ctx),
        topazScriptManager_Permission_All
    );

    // enable debugging!
    topaz_script_enable_debugging(script);

    // We want to enable use of the debugging console.
    //
    topazConsole_t * console = topaz_context_get_console(ctx);
    topaz_console_enable(console, TRUE);


    // In case something goes wrong, it can be helpful to attach
    // a script context to the console.
    topaz_console_attach_script(
        console,
        script
    );


    // Optional
    run_script(ctx, script, TOPAZ_STR_CAST("preload"));
    if (!run_script(ctx, script, TOPAZ_STR_CAST(path))) {
        topazString_t * message = topaz_string_create_from_c_str("Script \"%s\" could not be opened or was empty.", path);
        topaz_console_print(console, message);
        topaz_string_destroy(message);
        return 1;
    }

    topaz_context_run(ctx);
}
