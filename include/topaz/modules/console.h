/*
Copyright (c) 2020, Johnathan Corkery. (jcorkery@umich.edu)
All rights reserved.

This file is part of the topaz project (https://github.com/jcorks/topaz)
topaz was released under the MIT License, as detailed below.



Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is furnished 
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall
be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.


*/


#ifndef H_TOPAZDC__CONSOLE__INCLUDED
#define H_TOPAZDC__CONSOLE__INCLUDED

typedef struct topaz_t topaz_t;
typedef struct topazColor_t topazColor_t;
typedef struct topazString_t topazString_t;
typedef struct topazArray_t topazArray_t;
typedef struct topazScript_t topazScript_t;
typedef struct topazSystem_Backend_t topazSystem_Backend_t;
typedef struct topazConsoleDisplay_t topazConsoleDisplay_t;

///
///    Console
///    -----
///
typedef struct topazConsole_t topazConsole_t;



/// Defines different types of messages.
///
typedef enum topazConsole_MessageType_t topazConsole_MessageType_t;
enum topazConsole_MessageType_t {
    /// Message type for normal messages.
    /// These do not include any additional information
    ///
    topazConsole_MessageType_Normal,

    /// Message type for debugging messages.
    /// Includes "Debug" as well as the time when posted.
    ///
    topazConsole_MessageType_Debug,

    /// Message type for warning messages.
    /// Includes "Warning" as well as the time when posted.
    ///
    topazConsole_MessageType_Warning,

    /// Message type for error messages.
    /// Includes "Error" as well as the time when posted.
    ///
    topazConsole_MessageType_Error
};




/// Creates a console instance. This is normally not necessary to call, as 
/// topaz_t has a default instance that it generates for you. 
/// See topaz_context_get_console()
/// Note that all the topazConsole_t instances use the same console display.
///
topazConsole_t * topaz_console_create(topaz_t *);

/// Destroys and frees a topaz console instance.
///
void topaz_console_destroy(topazConsole_t *);




/// Enables or disables the console. The default state is the console is disabled.
///
void topaz_console_enable(topazConsole_t *, int enable);

/// Prints a line to the console
/// This is equivalent to topaz_console_print_type() with the 
/// message type as topazConsole_MessageType_Normal
///
void topaz_console_print(topazConsole_t *, const topazString_t *);

/// Prints a line to the console
/// The message type includes a specific color and header 
/// strings to make it more clear what type the message is 
/// once displayed.
///
void topaz_console_print_message(topazConsole_t *, const topazString_t *, topazConsole_MessageType_t);

/// Prints a color line to the console.
///
void topaz_console_print_color(topazConsole_t *, const topazString_t *, const topazColor_t *);

/// Attaches a script context to the console.
/// Once attached, debugging features will be available
void topaz_console_attach_script(
    topazConsole_t *,
    topazScript_t *
);


/// Runs a command
/// Command syntax:
/*
    commandName[SEPARATOR]arg0[SEPARATOR]arg1[SEPARATOR]...
*/
/// Where [SEPARATOR] can be any of the following symbols:
/// ()[]{},.;:
///
topazString_t * topaz_console_run(topazConsole_t *, const topazString_t *);



///
///    ConsoleCommandContext
///    -----
///
///    A command context is the basic means by which 
///    the console can understand commands. The command context 
///    holds all the keywords for valid commands at a given time.
///
///    By pushing and popping command contexts, it is possible to create 
///    advanced commands with choices built in.
///
///    By default, the default command context is pushed
///
typedef struct topazConsole_CommandContext_t topazConsole_CommandContext_t;

/// Retrieves the default command context.
/// This can be used to add commands to or aid in controlling the 
/// context stack.
topazConsole_CommandContext_t * topaz_console_get_default_command_context(topazConsole_t *);


/// Creates a new console command context.
///
topazConsole_CommandContext_t * topaz_console_command_context_create();


/// Destroys a console command context.
///
void topaz_console_command_context_destroy(topazConsole_CommandContext_t *);


/// Pushes a command context to the top of the stack. 
/// All commands will be read from this context.
///
void topaz_console_push_command_context(topazConsole_t *, topazConsole_CommandContext_t *);

/// Pops the top context.
/// If the stack is empty, the default context is used. 
///
void topaz_console_pop_command_context(topazConsole_t *);




/// A console command function. When a user enters a command,
/// the corresponding topaz_console_command_callback() is called.
/// In the console, the user may give arguments the the command. 
/// These are passed to the callback as read-only strings within the
/// "args" array.
///
typedef topazString_t * (*topaz_console_command_callback)(
    topazConsole_t *, 
    void *, 
    const topazArray_t * args,
    const topazString_t * fullCommand
);




/// Adds a custom command. If the user enters a command matching the given
/// string.
///
void topaz_console_command_context_add_command(
    topazConsole_CommandContext_t *, 
    const topazString_t            * command,
    topaz_console_command_callback  callback,
    void * data
);


/// Adds a default handler. This will be called for a context 
/// if an unrecognized command is given to the context.
///
void topaz_console_command_context_set_default_handler(
    topazConsole_CommandContext_t *,
    topaz_console_command_callback handler;

);

/// Sets the header prompt text to show within the console.
///
void topaz_console_command_context_set_prompt(
    topazConsole_CommandContext_t *,
    const topazString_t * header
);


#endif
