#include <topaz/topaz.h>
#include <topaz/modules/view_manager.h>
#include <topaz/all.h>

int main() {
    // Create the context and window
    topaz_t * ctx = topaz_context_create();
    topaz_view_manager_create_main_default(topaz_context_get_view_manager(ctx), TOPAZ_STR_CAST("Scripting"));

    // make the center 0, 0
    topazDisplay_t * disp = topaz_view_manager_get_main(topaz_context_get_view_manager(ctx));
    topazEntity_t * cam = topaz_display_get_render_camera(disp);
    topaz_camera_set_render_resolution(cam, 320, 240);
    topazVector_t p = {160, -120, 0};
    *topaz_entity_position(topaz_display_get_2d_camera(disp)) = p;


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



    // extract the script data
    topazFilesys_t * filesystem = topaz_context_filesys_create(ctx);
    topazRbuffer_t * scriptFile = topaz_filesys_read(filesystem, TOPAZ_STR_CAST("script.js"));
    const topazString_t * scriptText  = topaz_rbuffer_read_string(
        scriptFile,
        topaz_rbuffer_get_size(scriptFile)
    );
          
    topaz_script_run(
        script,
        TOPAZ_STR_CAST("script.js"),
        scriptText
    );   

    topaz_context_run(ctx);
}
