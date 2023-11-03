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


/// The console is an object that listens and dispatches 
/// messages, usually to something viewable on the system. 
/// Consoles are especially useful for debugging purposes while 
/// within a running program.
///
/// Main features include: posting different 
/// kinds of "messages" (lines of text) to the console for viewing,
/// inserting custom debugging commands,
/// attaching to scripting instances for built-in 
/// debugging (if available), and more.
///
/// The console is not enabled by default. Enabling it will 
/// show the console using the preferred system backend for ConsoleDisplays.
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
/// (No script mapping)
///
topazConsole_t * topaz_console_create(
    /// The topaz context instance.
    topaz_t * context
);

/// Destroys and frees a topaz console instance.
///
/// (No script mapping)
///
void topaz_console_destroy(
    /// The console to display.
    topazConsole_t * console
);




/// Enables or disables the console. The default state is the console is disabled.
///
void topaz_console_enable(
    /// The console to set the state of.
    topazConsole_t * console, 

    /// Whether to enable the console.
    int enable
);

/// Prints a line to the console
/// This is equivalent to topaz_console_print_type() with the 
/// message type as topazConsole_MessageType_Normal
///
void topaz_console_print(
    /// The console to print to.
    topazConsole_t * console, 

    /// The text to print.
    const topazString_t * message
);

/// Prints a line to the console
/// The message type includes a specific color and header 
/// strings to make it more clear what type the message is 
/// once displayed.
///
void topaz_console_print_message(
    /// The console to print to.
    topazConsole_t * console, 

    /// The text to print.
    const topazString_t * message,

    /// The type of the message.
    topazConsole_MessageType_t type
);


/// Attaches a script context to the console.
/// Once attached, debugging features will be available
///
/// (No script mapping)
///
void topaz_console_attach_script(
    /// The console to attach to.
    topazConsole_t * console,

    /// The scripting instance to attach.
    topazScript_t * script
);


/// A callback in response to a unicode event.
///
typedef void (*topaz_console_listener_callback)(
    /// The console from where the message originates.
    ///
    topazConsole_t *,
    
    /// The message text.
    ///
    const topazString_t * line,
    
    /// The kind of message incoming.
    ///
    topazConsole_MessageType_t type,	
    
    /// The user data for the callback.
    ///
    void * userdata
);


/// Adds a callback to run when a new console message 
/// is printed to the console. The ID of the callback is 
/// returned. 
///
int topaz_console_add_listener(
    /// The console to attach to.
    ///
    topazConsole_t * console,

    /// The function to call once a new message is encountered.
    ///    
    topaz_console_listener_callback,
    
    /// The userdata to pass to the listener.
    ///
    void * userdata
);


/// Removes the console listener. 
/// 
void topaz_console_remove_listener(
    /// The console to remove the listener from.
    ///
    topazConsole_t * console,
    
    /// The ID of the callback
    int id
);


/// Runs a command.
/// Command syntax:
///
///     commandName[SEPARATOR]arg0[SEPARATOR]arg1[SEPARATOR]...
///
/// Where [SEPARATOR] can be any of the following symbols:
/// ()[]{},.;:
///
topazString_t * topaz_console_run(
    /// The console to run a command in.
    topazConsole_t * console, 

    /// The command to run.
    const topazString_t * command
);



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
topazConsole_CommandContext_t * topaz_console_get_default_command_context(
    /// The parent console of this context.
    topazConsole_t * console
);


/// Creates a new console command context.
///
topazConsole_CommandContext_t * topaz_console_command_context_create();


/// Destroys a console command context.
///
void topaz_console_command_context_destroy(
    /// The context to destroy.
    topazConsole_CommandContext_t * context
);


/// Pushes a command context to the top of the stack. 
/// All commands will be read from this context.
///
void topaz_console_push_command_context(
    /// The console to push the command context to.
    topazConsole_t * console, 

    /// The context to become current.
    topazConsole_CommandContext_t * context
);

/// Pops the top context.
/// If the stack is empty, the default context is used. 
///
void topaz_console_pop_command_context(
    /// The console to modify.
    topazConsole_t * console
);




/// A console command function. When a user enters a command,
/// the corresponding topaz_console_command_callback() is called.
/// In the console, the user may give arguments the the command. 
/// These are passed to the callback as read-only strings within the
/// "args" array.
///
typedef topazString_t * (*topaz_console_command_callback)(
    /// The console receiving the command.
    topazConsole_t * console, 
    /// The data bound to the callback.
    void * callbackData, 

    /// A read-only array of arguments. Each member is a topazString_t *
    const topazArray_t * args,

    /// The full, uncomidified string entered by the user.
    const topazString_t * fullCommand
);




/// Adds a custom command. If the user enters a command matching the given
/// string. Calling this replaces any existing callback associated with 
/// the command string.
///
void topaz_console_command_context_add_command(
    /// The context to add a command to.
    topazConsole_CommandContext_t * context, 

    /// The command string itself.
    const topazString_t * command,

    /// The callback to be called if a user enters this command
    topaz_console_command_callback callback,

    /// The data to bind to this callback.
    void * data
);


/// Adds a default handler. This will be called for a context 
/// if an unrecognized command is given to the context.
/// Calling this replaces the existing handler and callback 
/// data. To disable usage of a default handler, 
/// pass the handler as NULL.
///
void topaz_console_command_context_set_default_handler(
    /// The context to modify.
    topazConsole_CommandContext_t * context,

    /// The default handler to call.
    topaz_console_command_callback handler,

    /// The data to bind to the callback.
    void * callbackData

);

/// Sets the header prompt text to show within the console.
///
void topaz_console_command_context_set_prompt(
    /// The context to modify.
    topazConsole_CommandContext_t * context,

    /// The string prompt.
    const topazString_t * header
);


#endif
