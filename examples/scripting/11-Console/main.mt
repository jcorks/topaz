@:Topaz = import(module:'Topaz');




// The console provides a way to log and track information
// that the program posts. 
// Before use, the console needs to be enabled:
Topaz.Console.enabled = true;
// 
// The most basic way to interact with the console is to 
// print messages to it:
Topaz.Console.print(message:'Hello, world!');


// Different message types are available as well.
// The default message type is "normal", but for 
// more important, urgent info, 
Topaz.Console.print(message:'Debugging! for debugging purposes.', type:Topaz.Console.MESSAGE_TYPE.DEBUG);
Topaz.Console.print(message:'Warnings, for when the developers should keep an eye out for something.', type:Topaz.Console.MESSAGE_TYPE.WARNING);
Topaz.Console.print(message:'Errors, for when something severe / fatal has occurred.', type:Topaz.Console.MESSAGE_TYPE.ERROR);

// But thats not all! Throughout a complex program, there 
// can be many, many console messages. So, you can also add 
// a function to process and store printed messages.
::<= {
    @condensedLog = 'Listener heard:\n';

    // A listener runs a user function for each new log message
    // Adding a new listener returns an id that can be used to remove 
    // the listener later.
    @:listenerID = Topaz.Console.addListener(
        callback:::(text, type) {
            condensedLog = String.combine(strings:[condensedLog, 'New log: ', text, '\n']);
        }
    );
    
    Topaz.Console.print(message:'This log is tracked by the listener.');
    Topaz.Console.print(message:'All logs pass through all listeners that are active.');
    Topaz.Console.print(message:'This is the last listened message. It will be removed next!');

    // after the listener is removed, the function is no longer called.
    Topaz.Console.removeListener(id:listenerID);        
    
    Topaz.Console.print(message:condensedLog);
};



// In addition to logging, the console can be used for 
// debugging interaction. On most systems, enabling the console 
// not only allows for viewing of the console, but also 
// input as well.
//
// Debugging interaction is done through CommandContexts,
// which define commands that the console can recognize 
// and act upon.
@:myContext = Topaz.Console.CommandContext.new(
    // Commands are strings in the format:
    // 
    //  commandName arg0 arg1 arg2 etc.
    //
    // Each argument can be separated by ()[]{},.;: for convenience 
    // and readability.
    //
    // Once a command is run, a string can be returned as a result of the 
    // behavior.
    //
    commands: {
        "sayHi" : ::(args, fullCommand) {
            return 'Hi!';
        },
        
        // an invoke with arguments separated as stated above. 
        // i.e.
        //
        // randomNumber(0, 100);
        "randomNumber" : ::(args, fullCommand) {
            when(Object.keycount(of:args) != 2) 
                'Syntax: randomNumber(min, max)';
                
                
            @:min = Number.parse(string:args[0]);
            @:max = Number.parse(string:args[1]);
            return min + Number.random() * (max-min);
        },
        
        // Invokes the debugger! Did you know you could do this!?
        // The debugger is its own console command context.
        "debug" : ::(args, fullCommand) {
            Topaz.debug();
        }
        

    },
    
    // Each context can also have a prompt to display when 
    // a command is waiting to be entered.
    prompt: 'test> ',
    
    
    // In the case that a user provides a command that 
    // isn't recognized, this function can be run, providing 
    // for additional complex behavior. The most useful though 
    // is just to tell the user that we didnt get what 
    // the heck they were doing
    defaultHandler :::(args, fullCommand) {
        return 'Sorry! I didnt understand you.';
    }
);

// Then to use the context, 
// the context is pushed to the console.
// Once pushed, the console ONLY accepts  
// these commands until a new context is pushed 
// or the context is popped.
//
Topaz.Console.pushContext(commands:myContext);










