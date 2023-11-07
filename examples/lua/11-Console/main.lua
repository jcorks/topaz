



-- The console provides a way to log and track information
-- that the program posts. 
-- Before use, the console needs to be enabled:
--Topaz.Console.enabled = true;
-- 
-- The most basic way to interact with the console is to 
-- print messages to it:
Topaz.Console.print('Hello, world!');


-- Different message types are available as well.
-- The default message type is "normal", but for 
-- more important, urgent info, 
Topaz.Console.printMessage('Debugging! for debugging purposes.', Topaz.Console.MessageType.Debug);
Topaz.Console.printMessage('Warnings, for when the developers should keep an eye out for something.', Topaz.Console.MessageType.Warning);
Topaz.Console.printMessage('Errors, for when something severe / fatal has occurred.', Topaz.Console.MessageType.Error);

-- But thats not all! Throughout a complex program, there 
-- can be many, many console messages. So, you can also add 
-- a function to process and store printed messages.
(function()
    local condensedLog = 'Listener heard:\n';

    -- A listener runs a user function for each new log message
    -- Adding a new listener returns an id that can be used to remove 
    -- the listener later.
    local listenerID = Topaz.Console.addListener(
        function(line, type)
            condensedLog = condensedLog..'New log: '..line..'\n';
        end
    );
    
    Topaz.Console.print('This log is tracked by the listener.');
    Topaz.Console.print('All logs pass through all listeners that are active.');
    Topaz.Console.print('This is the last listened message. It will be removed next!');

    -- after the listener is removed, the function is no longer called.
    Topaz.Console.removeListener(listenerID);        
    
    Topaz.Console.print(condensedLog);
end)()



-- In addition to logging, the console can be used for 
-- debugging interaction. On most systems, enabling the console 
-- not only allows for viewing of the console, but also 
-- input as well.
--
-- Debugging interaction is done through CommandContexts,
-- which define commands that the console can recognize 
-- and act upon.
local myContext = Topaz.Console.CommandContext.create();

-- Commands are strings in the format:
-- 
--  commandName arg0 arg1 arg2 etc.
--
-- Each argument can be separated by ()[]{},.;: for convenience 
-- and readability.
--
-- Once a command is run, a string can be returned as a result of the 
-- behavior.
--
myContext:addCommand(
    'sayHi',
    function(args, fullCommand)
        return 'Hi!';
    end
);
        
-- an invoke with arguments separated as stated above. 
-- i.e.
--  
-- randomNumber(0, 100);
myContext:addCommand(
    "randomNumber",
    function(args, fullCommand)
        if (#args+1 ~= 2) then
            return 'Syntax: randomNumber(min, max)';
        end
            
        local min = tonumber(args[0]);
        local max = tonumber(args[1]);
        return min + math.random() * (max-min);
    end
);
        
-- Invokes the debugger! Did you know you could do this!?
-- The debugger is its own console command context.
myContext:addCommand(
    "debug",
    function(args, fullCommand) 
        Topaz.debug();
    end
);
    
-- Each context can also have a prompt to display when 
-- a command is waiting to be entered.
myContext:setPrompt('test> ');
    
    
-- In the case that a user provides a command that 
-- isn't recognized, this function can be run, providing 
-- for additional complex behavior. The most useful though 
-- is just to tell the user that we didnt get what 
-- the heck they were doing
myContext:setDefaultHandler(
    function(args, fullCommand)
        return 'Sorry! I didnt understand you.';
    end
);



-- Then to use the context, 
-- the context is pushed to the console.
-- Once pushed, the console ONLY accepts  
-- these commands until a new context is pushed 
-- or the context is popped.
--
Topaz.Console.pushCommandContext(myContext);










