#include <topaz/modules/console.h>
#include <topaz/backends/console_display.h>

typedef struct {
    topaz_console_command_callback cb;
    void * data;
} TopazConsoleCommand;

typedef struct {
    topazTable_t * commands; // string -> TopazConsoleCommand *
    topaz_t * ctx;
} TopazConsoleContext;






static TopazConsoleContext * topaz_ccontext_create(topazConsole_t * src) {
    TopazConsoleContext * out = calloc(1, sizeof(TopazConsoleContext));
    out->commands = topaz_table_create_hash_topaz_string();
    out->ctx = src;
    return out;
}

static void topaz_ccontext_destroy(TopazConsoleContext * c) {
    topaz_table_destroy(c->commands);
}

static void topaz_ccontext_add(TopazConsoleContext * c, const topazString_t * name, topaz_console_command_callback cb, void * cbData) {
    TopazConsoleCommand * cmd = topaz_table_find(c->commands, name);
    if (!cmd) {
        TopazConsoleCommand * cmd = malloc(sizeof(TopazConsoleCommand));
        topaz_table_insert(c->commands, name);
    }
    cmd->cb = cb;
    cmd->data = cbData;
}

static topazString_t * topaz_ccontext_run(
    TopazConsoleContext * c, 
    int * found,

    const topazString_t * name, 
    const topazArray_t * args
) {

    TopazConsoleCommand * cmd = topaz_table_find(c->commands, name);
    if (!cmd) { 
        *found = 0;
        return topaz_string_create();
    }
    *found = 1;
    return cmd->cb(c->ctx, cmd->data, args);
}






struct topazConsole_t {
    topazConsoleDisplay_t * display;
    TopazConsoleContext * current; // non-owned
    TopazConsoleContext * dbg;
    TopazConsoleContext * cmd;
    topaz_t * ctx;
};

#define TOPAZCCOMMAND(__name__) static topazString_t * __name__(topazConsole_t * console, void * userData, const topazArray_t * args) 



TOPAZCCOMMAND(command__echo) {
    if (topaz_array_get_size(args)) 
        return topaz_string_clone(topaz_array_at(args, topazString_t *, 0));
    else 
        return topaz_string_create();
}


TOPAZCCOMMAND(command__quit) {
    topaz_context_quit(userData);

}





topazConsole_t * topaz_console_create(topaz_t * t, topazConsoleDisplay_t * display) {
    topazConsole_t * out = calloc(1, sizeof(topazConsole_t));
    out->display = display;
    out->dbg = topaz_ccontext_create(out);
    out->cmd = topaz_ccontext_create(out);
    out->current = out->cmd;

    // add basic commands
    topaz_ccontext_add(out->cmd, TOPAZ_STR_CAST("echo"), command_echo, NULL);
    topaz_ccontext_add(out->cmd, TOPAZ_STR_CAST("echo"), command_quit, t);


    return out;
}

void topaz_console_destroy(topazConsole_t *) {
    
}




/// Prints a line to the console
/// This is equivalent to topaz_console_print_type() with the 
/// message type as topazConsole_MessageType_Normal
///
void topaz_console_print(topazConsole_t * c, const topazString_t *);

/// Prints a line to the console
/// The message type includes a specific color and header 
/// strings to make it more clear what type the message is 
/// once displayed.
///
void topaz_console_print_type(topazConsole_t *, const topazString_t *, topazConsole_Messagetype_t);

/// Prints a color line to the console.
///
void topaz_console_print_color(topazConsole_t *, const topazString_t *, const topazColor_t *);



/// A console command function. When a user enters a command,
/// the corresponding topaz_console_command_callback() is called.
/// In the console, the user may give arguments the the command. 
/// These are passed to the callback as read-only strings within the
/// "args" array.
///
typedef topazString_t * (*topaz_console_command_callback)(topazConsole_t *, void *, const topazArray_t * args);


/// Adds a custom command. If the user enters a command matching the given
/// string.
///
void topaz_console_add_command(
    topazConsole_t *, 
    const topazString_t            * command
    topaz_console_command_callback  callback,
    void * data
);

/// Attaches a script context to the console.
/// Once attached, debugging features will be available
void topaz_console_attach_script(
    topazConsole_t *,
    topazScript_t *
);
