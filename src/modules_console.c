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
#include <math.h>


typedef struct {
    topaz_console_command_callback cb;
    void * data;
} TopazConsoleCommand;



struct topazConsole_CommandContext_t {
    topazTable_t * commands; // string -> TopazConsoleCommand *
    topazString_t * header;

    TopazConsoleCommand defaultHandler;
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
        if (c->defaultHandler.cb) {
            return c->defaultHandler.cb(console, c->defaultHandler.data, args, fullCommand);
        } else {
            return topaz_string_create_from_c_str("Error: command not found.");
        }
    }
    return cmd->cb(console, cmd->data, args, fullCommand);
}




typedef struct {
    int line;
    topazString_t * file;
    topazString_t * label; // NULL if never recevied fulfillment of request
    int id; // unique id. This is what the user types
} DebugBreakpoint;

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
    topazArray_t * debugBreakpoints;
    topazString_t * debugLastCommand;
};

static void print_prompt(topazConsole_t * c) {
    topaz_console_display_add_text(
        c->display,
        topaz_array_get_size(c->contextStack) ?
            topaz_array_at(c->contextStack, 
                topazConsole_CommandContext_t *,
                topaz_array_get_size(c->contextStack)-1
            )->header
        :
            c->cmd->header,
        &color_normal
    );

}

static void send_command(
    topazConsoleDisplay_t * d, 
    const topazString_t * line, 
    void * userData
) {
    topazConsole_t * c = userData;
    topazString_t * output = topaz_console_run(c, line);
    if (topaz_string_get_length(output)) {
        topaz_console_print(c, output);
        print_prompt(c);
    }

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


static const topazString_t * stackframe_to_string(
    const topazScript_DebugState_t * stack,
    int level 
) {
    static topazString_t * out = NULL;
    if (!out) out = topaz_string_create();
    topaz_string_clear(out);

    if (level < 0 || level >= topaz_array_get_size(stack->callstack)) {
        return out;        
    }

    topazScript_CallstackFrame_t * frame = &topaz_array_at(stack->callstack, topazScript_CallstackFrame_t, level);

    topaz_string_concat_printf(
        out, 
        "[%d]  %s()  %s:%d",
        level,

        topaz_string_get_length(frame->functionName) ? 
            topaz_string_get_c_str(frame->functionName)
        :   
            "<unnamed function>",

        topaz_string_get_length(frame->filename) ? 
            topaz_string_get_c_str(frame->filename) 
        : 
            "[unknown file]",
        frame->lineNumber
    );

    return out;
}


static void console_print_debug_state(
    topazConsole_t * console,
    const topazScript_DebugState_t * state
) {
    topazColor_t line = color_normal;
    line.r *= 0.7;
    line.g *= 0.7;
    line.b *= 0.7;
    topazColor_t lineBold;
    lineBold.r = 255;
    lineBold.g = 255;
    lineBold.b = 255;



    if (topaz_array_get_size(state->callstack) == 0) {
        topaz_console_print_message(console, topaz_string_create_from_c_str("[Callstack empty or unknown.]"), topazConsole_MessageType_Error);
        return;
    }


    #define LINES_FROM_CENTER 10
    // first print source.
    {
        const topazScript_CallstackFrame_t * homeFrame = &topaz_array_at(state->callstack, topazScript_CallstackFrame_t, console->debugLevel);
        int l;
        topazString_t * output = topaz_string_create();
        int digitCount = ((int)(log10(topaz_script_get_line_count(console->script, homeFrame->filename))))+1;
        if (digitCount < 2) digitCount = 2;
        topazString_t * headerNormal = topaz_string_create_from_c_str(" %%%dd | ", digitCount);
        topazString_t * headerCurrent = topaz_string_create_from_c_str(" %%%ds | ", digitCount);
        int lineIndex = homeFrame->lineNumber-1;        
        for(l = lineIndex-LINES_FROM_CENTER; l <= lineIndex+LINES_FROM_CENTER; ++l) {
            topaz_string_clear(output);
            if (l < 0 || l > topaz_script_get_line_count(console->script, homeFrame->filename)) {
                topaz_console_print_color(
                    console,
                    TOPAZ_STR_CAST("-----------------------------"),
                    &line
                );
            } else {
                if (l == lineIndex) {
                    topaz_string_concat_printf(output, topaz_string_get_c_str(headerCurrent), "->");
                    topaz_string_concat(output, topaz_script_get_line(console->script, homeFrame->filename, l));
                } else {
                    topaz_string_concat_printf(output, topaz_string_get_c_str(headerNormal), l+1);
                    topaz_string_concat(output, topaz_script_get_line(console->script, homeFrame->filename, l));
                }

                topaz_console_print_color(
                    console,
                    output,
                    (l == lineIndex ? &lineBold : &line)
                );

            }



        }
        topaz_string_destroy(output);

        topaz_console_print_color(
            console,
            TOPAZ_STR_CAST(""),
            (l == lineIndex ? &lineBold : &line)
        );

    }

    for(int i = 0; i < topaz_array_get_size(state->callstack); ++i) {
        topazString_t * str = topaz_string_create();
        const topazString_t * substate = stackframe_to_string(
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
            (i == console->debugLevel ? &lineBold : &line)
        );

        topaz_string_destroy(str);
    }
}

static topazString_t * trim_command(const topazString_t * fullCommand) {
    int indexCommandStart = 0;
    uint32_t i;
    uint32_t len = topaz_string_get_length(fullCommand);
    for(i = 0; i < len; ++i) {
        indexCommandStart++;
        if (topaz_string_get_char(fullCommand, i) == ' ') {
            break;
        }
    }
    if (indexCommandStart == len) return topaz_string_clone(fullCommand);

    return topaz_string_clone(
        topaz_string_get_substr(
            fullCommand, 
            indexCommandStart,
            len-1
        )
    ); 

}

static void command_debug_response(
    topazScript_t * script,
    topazScript_DebugCommand_t command,
    const topazString_t * result,
    void * userData
) {
    topazConsole_t * console = userData;
    const topazScript_DebugState_t * state = topaz_script_debug_get_state(console->script);
    if (!topaz_array_get_size(console->contextStack)) {
        topaz_console_run(console, TOPAZ_STR_CAST("dbg"));
    }

    switch(command) {
      case topazScript_DebugCommand_Pause:
        topaz_console_display_clear(console->display);
        console_print_debug_state(
            console,
            state
        );


        if (!topaz_string_get_length(result)) {
            topaz_console_print(console, TOPAZ_STR_CAST(""));
            topaz_console_print(console, TOPAZ_STR_CAST("Debugging context pause."));
            topaz_console_print(console, TOPAZ_STR_CAST(""));
        } else {
            topaz_console_print(console, TOPAZ_STR_CAST(""));
            topaz_console_print(console, TOPAZ_STR_CAST("Debugging context pause."));
            topaz_console_print_message(console, result, topazConsole_MessageType_Error);
            topaz_console_print(console, TOPAZ_STR_CAST(""));
        }
        break;

      case topazScript_DebugCommand_Resume:
        topaz_console_display_clear(console->display);
        topaz_console_print(console, TOPAZ_STR_CAST("Debugging context resumed."));
        return; // circumvent prompt
        break;

      case topazScript_DebugCommand_StepInto:
        topaz_console_display_clear(console->display);
        console_print_debug_state(
            console,
            state
        );
        break;

      case topazScript_DebugCommand_StepOver:
        topaz_console_display_clear(console->display);
        console_print_debug_state(
            console,
            state
        );
        break;

      case topazScript_DebugCommand_ScopedEval:
        topaz_console_print(console, result);
        break;

      case topazScript_DebugCommand_AddBreakpoint: {
        uint32_t i;
        uint32_t len = topaz_array_get_size(console->debugBreakpoints);
        for(i = 0; i < len; ++i) {
            DebugBreakpoint * bp = &topaz_array_at(console->debugBreakpoints, DebugBreakpoint, i);
            if (!bp->label) {

                if (topaz_string_get_length(result)) {
                    bp->label = topaz_string_clone(result);
                    topazString_t * message = topaz_string_create_from_c_str(
                        "Added breakpoint in file "
                    );

                    topaz_string_concat(message, bp->file);

                    topaz_string_concat_printf(
                        message,                    
                        ", line %d.", 
                        bp->line 
                    );

                    topaz_console_print(
                        console, 
                        message
                    );
                    topaz_string_destroy(message);
                    static int idPool = 1;
                    bp->id = idPool++;

                    message = topaz_string_create_from_c_str("(Breakpoint %d)", bp->id);
                    topaz_console_print(console, message);
                    topaz_string_destroy(message);                
                } else {
                    topaz_string_destroy(bp->file);
                    topaz_array_remove(console->debugBreakpoints, i);        
                    topaz_console_print(console, TOPAZ_STR_CAST("Failed to add breakpoint."));

                }
                break;

            }           
        }
        break;

      }        

      case topazScript_DebugCommand_RemoveBreakpoint: {
        if (!topaz_string_get_length(result)) {
            topaz_console_print(console, TOPAZ_STR_CAST("Could not remove breakpoint!"));
        } else {
            uint32_t i;
            uint32_t len = topaz_array_get_size(console->debugBreakpoints);
            for(i = 0; i < len; ++i) {
                DebugBreakpoint * bp = &topaz_array_at(console->debugBreakpoints, DebugBreakpoint, i);
                if (topaz_string_test_eq(bp->label, result)) {
                    topaz_string_destroy(bp->file);
                    if (bp->label)
                        topaz_string_destroy(bp->label);
                    topaz_array_remove(console->debugBreakpoints, i);

                    topaz_console_print(console, TOPAZ_STR_CAST("Removed breakpoint."));
                    break;
                }
            }
        }
      }
      default:
        break;
    }

    print_prompt(console);
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
    topaz_string_set(console->debugLastCommand, fullCommand);
    const topazScript_DebugState_t * state = topaz_script_debug_get_state(console->script);

    console_print_debug_state(
        console,
        state
    );

    return topaz_string_create_from_c_str(".");
}


TOPAZCCOMMAND(command__eval) {    
    topaz_string_set(console->debugLastCommand, fullCommand);
    topazString_t * eval = topaz_string_create();
    
    if (topaz_array_get_size(args)) {

        topazString_t * realCommand = trim_command(fullCommand);
        topaz_string_concat_printf(eval, "%d:", console->debugLevel);
        topaz_string_concat(eval, realCommand);
        topaz_string_destroy(realCommand);
        topaz_script_debug_send_command(
            console->script,
            topazScript_DebugCommand_ScopedEval,
            eval
        );

        topaz_string_destroy(eval);
    }
    return topaz_string_create();
}

TOPAZCCOMMAND(command__default_dbg) {    

    // repeat previous command
    if (!topaz_string_get_length(fullCommand)) {
        topaz_console_run(console, console->debugLastCommand);

    } else { // else eval
        topazString_t * eval = topaz_string_create();
        topazString_t * realCommand = topaz_string_clone(fullCommand);
        topaz_string_concat_printf(eval, "%d:", console->debugLevel);
        topaz_string_concat(eval, realCommand);
        topaz_string_destroy(realCommand);
        topaz_script_debug_send_command(
            console->script,
            topazScript_DebugCommand_ScopedEval,
            eval
        );
        topaz_string_destroy(eval);
    }
    return topaz_string_create();
}



TOPAZCCOMMAND(command__continue) {    
    topaz_string_set(console->debugLastCommand, fullCommand);
    int paused = topaz_script_debug_is_paused(console->script);
    if (paused) {
        topaz_script_debug_send_command(
            console->script,
            topazScript_DebugCommand_Resume,
            TOPAZ_STR_CAST("")
        );
    } else {
        return topaz_string_create_from_c_str("Debug state is not currently paused.");
    }
    return topaz_string_create();
}

TOPAZCCOMMAND(command__pause) {
    topaz_string_set(console->debugLastCommand, fullCommand);
    int paused = topaz_script_debug_is_paused(console->script);    
    if (paused) {
        return topaz_string_create_from_c_str("Debug state is already paused.");
    } else {
        topaz_script_debug_send_command(
            console->script,
            topazScript_DebugCommand_Pause,
            TOPAZ_STR_CAST("")
        );
    }
    return topaz_string_create();
}

TOPAZCCOMMAND(command__up) {    
    topaz_string_set(console->debugLastCommand, fullCommand);
    topaz_console_display_clear(console->display);
    const topazScript_DebugState_t * state = topaz_script_debug_get_state(console->script);
    console->debugLevel++;
    if (console->debugLevel >= topaz_array_get_size(state->callstack)) {
        console->debugLevel = topaz_array_get_size(state->callstack)-1;
    }

    console_print_debug_state(
        console,
        state
    );    
    print_prompt(console);

    return topaz_string_create();
}


TOPAZCCOMMAND(command__down) {   
    topaz_string_set(console->debugLastCommand, fullCommand);
    topaz_console_display_clear(console->display); 
    const topazScript_DebugState_t * state = topaz_script_debug_get_state(console->script);
    console->debugLevel--;
    if (console->debugLevel < 0) {
        console->debugLevel = 0;
    }

    console_print_debug_state(
        console,
        state
    );    
    print_prompt(console);

    return topaz_string_create();
}

TOPAZCCOMMAND(command__next) {    
    topaz_string_set(console->debugLastCommand, fullCommand);
    topaz_script_debug_send_command(
        console->script,
        topazScript_DebugCommand_StepOver,
        TOPAZ_STR_CAST("")
    );
    return topaz_string_create();
}

TOPAZCCOMMAND(command__step) {    
    topaz_string_set(console->debugLastCommand, fullCommand);
    topaz_script_debug_send_command(
        console->script,
        topazScript_DebugCommand_StepInto,
        TOPAZ_STR_CAST("")
    );
    return topaz_string_create();
}

    
TOPAZCCOMMAND(command__add_breakpoint) {
    topaz_string_set(console->debugLastCommand, fullCommand);
    topazString_t * arg = trim_command(fullCommand);
    
    if (topaz_string_test_contains(arg, TOPAZ_STR_CAST(":"))) {

        DebugBreakpoint bp;
        bp.file = topaz_string_clone(topaz_string_chain_start(arg, TOPAZ_STR_CAST(":")));
        bp.line = atoi(topaz_string_get_c_str(topaz_string_chain_proceed(arg)));
        bp.label = NULL;
        topaz_array_push(console->debugBreakpoints, bp);

        topaz_script_debug_send_command(
            console->script,
            topazScript_DebugCommand_AddBreakpoint,
            arg
        );
        topaz_string_destroy(arg);
      
    } else {
        const topazScript_DebugState_t * state = topaz_script_debug_get_state(console->script);
        const topazScript_CallstackFrame_t * frame = &topaz_array_at(state->callstack, topazScript_CallstackFrame_t, console->debugLevel);

        if (topaz_array_get_size(state->callstack)) {
            if (topaz_string_get_length(frame->filename)) {
                topazString_t * realArg = topaz_string_clone(frame->filename);
                topaz_string_concat_printf(realArg, ":");
                topaz_string_concat(realArg, arg);

                DebugBreakpoint bp;
                bp.file = topaz_string_clone(frame->filename);
                bp.line = atoi(topaz_string_get_c_str(arg));
                bp.label = NULL;
                topaz_array_push(console->debugBreakpoints, bp);

                topaz_script_debug_send_command(
                    console->script,
                    topazScript_DebugCommand_AddBreakpoint,
                    realArg
                );
                topaz_string_destroy(realArg);
                topaz_string_destroy(arg);
                return topaz_string_create();
            }
        }
        return topaz_string_create_from_c_str("Unable to add breakpoint: current file unknown.");
            
    }
    return topaz_string_create();
}



TOPAZCCOMMAND(command__delete_breakpoint__confirm_yes) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(console->debugBreakpoints);
    DebugBreakpoint * bp;
    for(i = 0; i < len; ++i) {
        bp = &topaz_array_at(console->debugBreakpoints, DebugBreakpoint, i);
        topaz_script_debug_send_command(
            console->script,
            topazScript_DebugCommand_RemoveBreakpoint,
            bp->label
        );
    }   

    topaz_console_pop_command_context(console);
    return topaz_string_create();
}

TOPAZCCOMMAND(command__delete_breakpoint__confirm_no) {

    topaz_console_pop_command_context(console);
    return topaz_string_create();
}



TOPAZCCOMMAND(command__delete_breakpoint) {
    topaz_string_set(console->debugLastCommand, fullCommand);
    if (topaz_array_get_size(args)) {
        topazString_t * arg = trim_command(fullCommand);
        int id = atoi(topaz_string_get_c_str(arg));
        uint32_t i;
        uint32_t len = topaz_array_get_size(console->debugBreakpoints);
        DebugBreakpoint * bp;
        for(i = 0; i < len; ++i) {
            bp = &topaz_array_at(console->debugBreakpoints, DebugBreakpoint, i);
            if (bp->id == id) {
                topazString_t * out = topaz_string_create_from_c_str(
                    "Removing breakpoint %d (",
                    id
                );

                topaz_string_concat(out, bp->file);
                topaz_string_concat_printf(out, ":%d)", bp->line);


                // request server-side removal
                topaz_script_debug_send_command(
                    console->script,
                    topazScript_DebugCommand_RemoveBreakpoint,
                    bp->label
                );
                                

                return out;
            }
        }
        return topaz_string_create_from_c_str("Could not remove breakpoint %d: no such breakpoint.", id);
    } else {
        topazConsole_CommandContext_t * yn = topaz_console_command_context_create(console);
        topaz_console_command_context_add_command(yn, TOPAZ_STR_CAST("Y"), command__delete_breakpoint__confirm_yes, console);
        topaz_console_command_context_add_command(yn, TOPAZ_STR_CAST("n"), command__delete_breakpoint__confirm_no,  console);
        topaz_console_push_command_context(console, yn);
        return topaz_string_create_from_c_str("Are you sure you want to remove all breakpoints? (Y/n)");

    }
}

TOPAZCCOMMAND(command__list_breakpoints) {
    topaz_string_set(console->debugLastCommand, fullCommand);
    uint32_t i;
    uint32_t len = topaz_array_get_size(console->debugBreakpoints);
    DebugBreakpoint * bp;

    if (!len) {
        return topaz_string_create_from_c_str("No breakpoints active.");
    }

    topazString_t * out = topaz_string_create_from_c_str("Breakpoints:\n");
    for(i = 0; i < len; ++i) {
        bp = &topaz_array_at(console->debugBreakpoints, DebugBreakpoint, i);
        topaz_string_concat_printf(out, "%d: ", bp->id);
        topaz_string_concat(out, bp->file);
        topaz_string_concat_printf(out, ":%d\n", bp->line);
    }
    return out;
}




topazConsole_t * topaz_console_create(topaz_t * t) {
    topazConsole_t * out = calloc(1, sizeof(topazConsole_t));
    {
        topazConsoleDisplayAPI_t api = {};
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
    out->debugBreakpoints = topaz_array_create(sizeof(DebugBreakpoint));
    out->debugLastCommand = topaz_string_create();

    topaz_console_command_context_set_prompt(out->dbg, TOPAZ_STR_CAST("(dbg) "));

    // add basic commands
    topaz_console_command_context_add_command(out->cmd, TOPAZ_STR_CAST("echo"), command__echo, NULL);
    topaz_console_command_context_add_command(out->cmd, TOPAZ_STR_CAST("quit"), command__quit, t);
    topaz_console_command_context_add_command(out->cmd, TOPAZ_STR_CAST("eval"), command__eval, t);
    topaz_console_command_context_add_command(out->cmd, TOPAZ_STR_CAST("dbg"),  command__dbg, t);


    // debug commands 
    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("backtrace"), command__backtrace, t);
    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("bt"),        command__backtrace, t);

    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("eval"),      command__eval, t);
    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("e"),         command__eval, t);

    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("continue"),  command__continue, t);
    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("c"),         command__continue, t);

    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("quit"),      command__exit_dbg, t);
    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("q"),         command__exit_dbg, t);

    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("pause"),     command__pause, t);
    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("p"),         command__pause, t);

    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("up"),        command__up, t);
    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("down"),      command__down, t);

    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("next"),      command__next, t);
    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("n"),         command__next, t);

    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("step"),      command__step, t);
    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("s"),         command__step, t);

    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("break"),     command__add_breakpoint, t);
    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("b"),         command__add_breakpoint, t);

    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("delete"),    command__delete_breakpoint, t);
    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("d"),         command__delete_breakpoint, t);

    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("list"),      command__list_breakpoints, t);
    topaz_console_command_context_add_command(out->dbg, TOPAZ_STR_CAST("l"),         command__list_breakpoints, t);

    topaz_console_command_context_set_default_handler(out->dbg, command__default_dbg, t);

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


void topaz_console_add_text_color(topazConsole_t * c, const topazString_t * str, const topazColor_t * color) {
    topaz_console_display_add_text(c->display, str, color);
}

void topaz_console_print_color(topazConsole_t * c, const topazString_t * str, const topazColor_t * color) {
    topaz_console_display_add_text(c->display, str, color);
    topaz_console_display_new_line(c->display);
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

    if (!command) command = topaz_string_create();

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
    out->header = topaz_string_create_from_c_str("$ ");
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

void topaz_console_command_context_set_prompt(
    topazConsole_CommandContext_t * c,
    const topazString_t * str
) {
    topaz_string_set(c->header, str);
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

void topaz_console_command_context_set_default_handler(
    topazConsole_CommandContext_t * c, 
    topaz_console_command_callback cb, 
    void * cbData
) {

    c->defaultHandler.cb = cb;
    c->defaultHandler.data = cbData;
}


