#include <topaz/modules/console.h>
#include <topaz/backends/console_display.h>
#include <topaz/containers/table.h>
#include <topaz/containers/array.h>
#include <topaz/containers/string.h>
#include <topaz/backends/script.h>
#include <topaz/color.h>
#include <topaz/topaz.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct {
    topaz_console_command_callback cb;
    void * data;
} TopazConsoleCommand;



struct topazConsole_CommandContext_t {
    topazTable_t * commands; // string -> TopazConsoleCommand *
};


const topazColor_t color_normal  = {255, 255, 200, 255};
const topazColor_t color_warning = {255, 128, 64,  255};
const topazColor_t color_debug   = {30,  87,  120, 255};
const topazColor_t color_error   = {255, 64,  64,  255};








static topazString_t * topaz_ccontext_run(
    topazConsole_t * console,
    topazConsole_CommandContext_t * c, 
    const topazString_t * fullCommand,
    const topazString_t * name, 
    const topazArray_t * args
) {

    TopazConsoleCommand * cmd = topaz_table_find(c->commands, name);
    if (!cmd) {         
        return topaz_string_create_from_c_str("Error: command not found.");
    }
    return cmd->cb(console, cmd->data, args, fullCommand);
}






struct topazConsole_t {
    topazConsoleDisplay_t * display;
    topazArray_t * contextStack;
    topazConsole_CommandContext_t * dbg;
    topazConsole_CommandContext_t * cmd;
    topaz_t * ctx;
    topazScript_t * script;
    uint32_t inputID;


    int debugLevel;
    uint32_t debugCBID;
};

static void send_command(
    topazConsoleDisplay_t * d, 
    const topazString_t * line, 
    void * userData
) {
    topazConsole_t * c = userData;
    topazString_t * output = topaz_console_run(c, line);
    topaz_console_print(c, output);
    topaz_string_destroy(output);
}



#define TOPAZCCOMMAND(__name__) static topazString_t * __name__(topazConsole_t * console, void * userData, const topazArray_t * args, const topazString_t * fullCommand) 



TOPAZCCOMMAND(command__echo) {
    if (topaz_array_get_size(args)) 
        return topaz_string_clone(topaz_array_at(args, topazString_t *, 0));
    else 
        return topaz_string_create();
}


TOPAZCCOMMAND(command__quit) {
    topaz_context_quit(userData);
    return topaz_string_create();
}

TOPAZCCOMMAND(command__eval) {
    if (!console->script) {
        return topaz_string_create_from_c_str("No script attached to console!\n");
    }

    if (topaz_string_get_length(fullCommand) <= 5) {
        return topaz_string_create();
    }

    
    return topaz_string_clone(topaz_script_object_as_string(
        topaz_script_expression(
            console->script,
            // TODO: assumes eval[DELIMITER], this will cause problems
            topaz_string_get_substr(fullCommand, 5, topaz_string_get_length(fullCommand)-1)
        )
    ));
}

static topazString_t * stackframe_to_string(
    const topazScript_DebugState_t * stack,
    int level 
) {
    static topazString_t * out = NULL;
    if (!out) out = topaz_string_create();
    topaz_string_clear(out);

    if (level < 0 || level >= topaz_array_get_size(stack->callstack)) {
        return out;        
    }

    topazScript_CallstackFrame_t * frame = topaz_array_at(stack->callstack, topazScript_CallstackFrame_t *, level);
    if (!topaz_string_get_length(frame->functionName)) {
        topaz_string_concat_printf(
            out, 
            "[%d]  %s()  %s:%d",
            level,
            topaz_string_get_c_str(frame->functionName),
            topaz_string_get_c_str(frame->filename),
            frame->lineNumber
        );
    } else {
        topaz_string_concat_printf(
            out, 
            "<unknown>"
        );
    }

    return out;
}


static void console_print_debug_state(
    topazConsole_t * console,
    const topazScript_DebugState_t * state
) {
    topazColor_t line = color_normal;
    topazColor_t lineBold = color_normal;
    line.r *= 0.7;
    line.g *= 0.7;
    line.b *= 0.7;

    if (topaz_array_get_size(state->callstack) == 0) {
        topaz_console_print_message(console, topaz_string_create_from_c_str("[Callstack empty or unknown.]"), topazConsole_MessageType_Error);
        return;
    }
    for(int i = 0; i < topaz_array_get_size(state->callstack); ++i) {
        topazString_t * str = topaz_string_create();
        topazString_t * substate = stackframe_to_string(
            state,
            i
        );    
        topaz_string_concat_printf(
            str, 
            "%s", 
            (i == console->debugLevel ? "-> " : "   ")
        );

        topaz_string_concat(str, substate);
        topaz_console_print_color(
            console,
            str,
            console->debugLevel ? &lineBold : &line
        );

        topaz_string_destroy(str);
        topaz_string_destroy(substate);
    }
}

static void command_debug_response(
    topazScript_t * script,
    topazScript_DebugCommand_t command,
    const topazString_t * result,
    void * userData
) {
    topazConsole_t * console = userData;
    
    switch(command) {
      case topazScript_DebugCommand_Pause:
        topaz_console_print(console, topaz_string_create_from_c_str("Debugging context pause."));
        break;

      case topazScript_DebugCommand_Resume:
        topaz_console_print(console, topaz_string_create_from_c_str("Debugging context resumed."));
        break;

      case topazScript_DebugCommand_StepInto:
        topaz_console_print(console, topaz_string_create_from_c_str("Debugging context pause."));
        break;

      case topazScript_DebugCommand_ScopedEval:
        topaz_console_print(console, result);
        break;
      default:
        topaz_console_print(console, result);
        break;
    }
}

TOPAZCCOMMAND(command__dbg) {
    if (console->script) {
        topaz_console_push_command_context(console, console->dbg);
        return topaz_string_create_from_c_str("[Entered topaz script debug context]");
    } else {
        topaz_console_print_message(
            console, 
            topaz_string_create_from_c_str("Error: No script attached for debugging (topaz_console_attach_script)."), 
            topazConsole_MessageType_Error
        );
        return topaz_string_create();

    }
}

TOPAZCCOMMAND(command__exit_dbg) {
    topaz_console_pop_command_context(console);
    return topaz_string_create_from_c_str("[Exiting topaz script debug context]");
}


TOPAZCCOMMAND(command__backtrace) {    
    const topazScript_DebugState_t * state = topaz_script_debug_get_state(console->script);

    console_print_debug_state(
        console,
        state
    );

    return topaz_string_create();
}


TOPAZCCOMMAND(command__print) {    
    topazString_t * eval = topaz_string_create();
    
    topaz_string_concat_printf(eval, "%d:", console->debugLevel);
    topaz_string_concat(eval, topaz_array_at(args, topazString_t *, 0));

    topaz_script_debug_send_command(
        console->script,
        topazScript_DebugCommand_ScopedEval,
        eval
    );

    topaz_string_destroy(eval);
    return topaz_string_create();
}

TOPAZCCOMMAND(command__continue) {    
    topaz_script_debug_send_command(
        console->script,
        topazScript_DebugCommand_Resume,
        TOPAZ_STR_CAST("")
    );
    return topaz_string_create();
}


topazConsole_t * topaz_console_create(topaz_t * t) {
    topazConsole_t * out = calloc(1, sizeof(topazConsole_t));
    {
        topazConsoleDisplayAPI_t api;
        topazSystem_Backend_t * b = topaz_system_create_backend(
            topaz_context_get_system(t),
            TOPAZ_STR_CAST("consoleDisplay"),
            &api
        );

        out->display = topaz_console_display_create(
            t,
            b,
            api
        );
    }
    out->dbg = topaz_console_command_context_create(out);
    out->cmd = topaz_console_command_context_create(out);
    out->contextStack = topaz_array_create(sizeof(topazConsole_CommandContext_t*));

    // add basic commands
    topaz_console_command_context_add_command(out->cmd, TOPAZ_STR_CAST("echo"), command__echo, NULL);
    topaz_console_command_context_add_command(out->cmd, TOPAZ_STR_CAST("quit"), command__quit, t);
    topaz_console_command_context_add_command(out->cmd, TOPAZ_STR_CAST("eval"), command__eval, t);
    topaz_console_command_context_add_command(out->cmd, TOPAZ_STR_CAST("dbg"),  command__dbg, t);


    // debug commands 
    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("backtrace"), command__backtrace, t);
    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("print"),     command__print, t);
    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("continue"),  command__continue, t);
    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("quit"),      command__exit_dbg, t);


    out->inputID = topaz_console_display_add_input_callback(
        out->display,
        send_command,
        out
    );

    return out;
}

void topaz_console_destroy(topazConsole_t * t) {
    topaz_console_display_remove_input_callback(t->display, t->inputID);
    topaz_console_command_context_destroy(t->dbg);
    topaz_console_command_context_destroy(t->cmd);
    topaz_array_destroy(t->contextStack);
    free(t);
}


void topaz_console_print(topazConsole_t * c, const topazString_t * str) {
    topaz_console_print_color(c, str, &color_normal);
}

void topaz_console_print_message(topazConsole_t * c, const topazString_t * str, topazConsole_MessageType_t t) {
    switch(t) {
      case topazConsole_MessageType_Normal:
        topaz_console_print_color(c, str, &color_normal);
        break;
      case topazConsole_MessageType_Debug: {
        char text[100];
        time_t now = time(NULL);
        struct tm * t = localtime(&now);
        strftime(text, sizeof(text)-1, "%H:%M:%S", t);
        topazString_t * base = topaz_string_create();
        topaz_string_concat_printf(base, "[Debug @ %s] ", text);
        topaz_string_concat(base, str);
        topaz_console_print_color(c, base, &color_debug);
        topaz_string_destroy(base);
        break;
      }
      case topazConsole_MessageType_Warning: {
        char text[100];
        time_t now = time(NULL);
        struct tm * t = localtime(&now);
        strftime(text, sizeof(text)-1, "%H:%M:%S", t);
        topazString_t * base = topaz_string_create();
        topaz_string_concat_printf(base, "[Warning @ %s] ", text);
        topaz_string_concat(base, str);
        topaz_console_print_color(c, base, &color_warning);
        topaz_string_destroy(base);
        break;
      }

      case topazConsole_MessageType_Error: {
        char text[100];
        time_t now = time(NULL);
        struct tm * t = localtime(&now);
        strftime(text, sizeof(text)-1, "%H:%M:%S", t);
        topazString_t * base = topaz_string_create();
        topaz_string_concat_printf(base, "[Error @ %s] ", text);
        topaz_string_concat(base, str);
        topaz_console_print_color(c, base, &color_error);
        topaz_string_destroy(base);
        break;
      }

      default:;
    }
}




void topaz_console_print_color(topazConsole_t * c, const topazString_t * str, const topazColor_t * color) {
    topaz_console_display_add_line(c->display, str, color);
}

void topaz_console_attach_script(
    topazConsole_t * c,
    topazScript_t * script
) {
    if (c->script) {
        topaz_script_debug_remove_command_callback(
            c->script,
            c->debugCBID
        );
        c->debugCBID = 0;
    }
    c->script = script;
    c->debugCBID = topaz_script_debug_add_command_callback(
        c->script,
        command_debug_response,
        c
    );
}



void topaz_console_enable(topazConsole_t * c, int enabled) {
    topaz_console_display_enable(c->display, enabled);
}

topazString_t * topaz_console_run(topazConsole_t * c, const topazString_t * strSrc) {
    topazString_t * out = NULL;
    topazString_t * str = topaz_string_clone(strSrc);
    topazString_t * command = NULL;
    topazArray_t * arr = topaz_array_create(sizeof(topazString_t *));
    const topazString_t * iter;

    // break up command
    for(
        iter = topaz_string_chain_start(str, TOPAZ_STR_CAST("()[]{}.,;: \n\t"));
        !topaz_string_chain_is_end(str);
        iter = topaz_string_chain_proceed(str)    
    ) {
        if (command) {
            topazString_t * substr = topaz_string_clone(iter);
            topaz_array_push(arr, substr);
        } else {
            command = topaz_string_clone(iter);
        }
    }



    if (command && topaz_string_get_length(command)) {
        if (topaz_array_get_size(c->contextStack)) {
            out = topaz_ccontext_run(
                c,
                topaz_array_at(c->contextStack, 
                    topazConsole_CommandContext_t *,
                    topaz_array_get_size(c->contextStack)-1
                ),
                strSrc,
                command,
                arr
            );
        } else {
            // default context
            out = topaz_ccontext_run(
                c,
                c->cmd,
                strSrc,
                command,
                arr
            );
        }

    } else {
        out = topaz_string_create_from_c_str("Error: command empty.");
    }


    // cleanup
    if (command)
        topaz_string_destroy(command);       

    uint32_t i;
    uint32_t len = topaz_array_get_size(arr);
    for(i = 0; i < len; ++i) {
        topaz_string_destroy(topaz_array_at(arr, topazString_t *, i));
    }
    topaz_array_destroy(arr);

    return out; 
}


topazConsole_CommandContext_t * topaz_console_get_default_command_context(topazConsole_t * c) {
    return c->cmd;
}


topazConsole_CommandContext_t * topaz_console_command_context_create() {
    topazConsole_CommandContext_t * out = calloc(1, sizeof(topazConsole_CommandContext_t));
    out->commands = topaz_table_create_hash_topaz_string();
    return out;
}



void topaz_console_command_context_destroy(topazConsole_CommandContext_t * c) {
    topaz_table_destroy(c->commands);
    free(c);
}


void topaz_console_push_command_context(topazConsole_t * c, topazConsole_CommandContext_t * cc) {
    topaz_array_push(c->contextStack, cc);
}

void topaz_console_pop_command_context(topazConsole_t * c) {
    topaz_array_set_size(c->contextStack, topaz_array_get_size(c->contextStack)-1);
}


void topaz_console_command_context_add_command(
    topazConsole_CommandContext_t * c, 
    const topazString_t * name, 
    topaz_console_command_callback cb, 
    void * cbData
) {

    TopazConsoleCommand * cmd = topaz_table_find(c->commands, name);
    if (!cmd) {
        cmd = malloc(sizeof(TopazConsoleCommand));
        topaz_table_insert(c->commands, name, cmd);
    }
    cmd->cb = cb;
    cmd->data = cbData;
}
