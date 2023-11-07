-- So far, we've examined the nature of entities and 
-- components. There is one more major source of functionality
-- within topaz: modules. Modules are collections of 
-- tools that allow for interactions with system-oriented 
-- behaviors. This example focuses on the Input module 
-- within topaz. 
--
-- This example demonstrates the 2 main methods to get
-- input information: Polling and Event Listening.
--

--
--
-- This prefab demonstrates how to use polling. Polling 
-- is the most basic and straightforward way to use input,
-- functions such as getState() return the currently recorded 
-- input state for a particular input. Every frame that topaz 
-- updates, the Input module will check for any incoming 
-- events for input devices and save their info. As such,
-- polling does not request from the device directly, but rather 
-- from what topaz last recorded. As such, polling is pretty
-- quick.
--
-- To demonstrate the effect of input, this example entity 
-- will use a blue rectangle to show itself onscreen and will 
-- respond to the arrow keys and right-click + dragging.
local createInputPollExample = function()
    local position = {
        x = 0,
        y = 0,
        z = 0
    }
    local this;
    
    this = Topaz.Entity.create(
        {
            onStep = function()

                -- For the polling method, the accuracy is dependent 
                -- on how often input is re-polled. For both convenience
                -- and accuracy, it is common to just re-poll for input 
                -- every frame.
                --
                -- Topaz.Input.getState() returns a number representing the 
                -- state of the input. Usually 0 means inactive, and 
                -- above 0 means its active. For buttons, this is 
                -- just 0.0 vs 1.0. 
                --
                -- Here we check the individual key values for the arrows.
                -- If the key state is non-zero, we modify the 
                -- position vector by adding "1" to it in the direction 
                -- that corresponds to the arrow.

                if (Topaz.Input.getState(Topaz.Key.up) ~= 0) then
                    position.y = position.y + 1;      
                    this:setPosition(position);      
                end

                if (Topaz.Input.getState(Topaz.Key.down) ~= 0) then
                    position.y = position.y - 1;           
                    this:setPosition(position);      
                end

                if (Topaz.Input.getState(Topaz.Key.left) ~= 0) then
                    position.x = position.x - 1;            
                    this:setPosition(position);      
                end

                if (Topaz.Input.getState(Topaz.Key.right) ~= 0) then
                    position.x = position.x + 1;         
                    this:setPosition(position);      
                end

                -- The left/right/middle button can be polled using 
                -- the Topaz.Key.topazPointer_* inputs.
                -- All inputs for the mouse+keyboard can use the getState() function.
                if (Topaz.Input.getState(Topaz.Pointer._1) ~= 0) then

                    -- To poll position position, getState can be used. However,
                    -- due to the commonality of this operation, the position 
                    -- can be polled by just accessing Topaz.Input.mouseX / Y.
                    position = {
                        x = Topaz.Input.mouseX(),
                        y = Topaz.Input.mouseY()
                    }
                    this:setPosition(position);      
                end
            end       
        }
    );

    -- Since we are making the shape once and not modifying it,
    -- theres no need to keep the component around.
    local visual = Topaz.Shape2D.create();
    
    

    -- Set it to blue and make it 20x20
    visual:setColor(Topaz.Color.fromString('blue'));
    visual:formRectangle(20, 20);


    -- As shorthand for .addComponent(), you 
    -- can set an entity's components by setting 
    -- the "components" property equal to an array
    -- of the component references. It tends to be 
    -- visually more compact while still very readable.
    this:addComponent(visual);
    
    return this;
end

-- This prefab demonstrates the usage of event listening.
-- While polling is a pretty convenient way to respond to 
-- input, it has a few issues:
--  - For best use, it requires re-polling as often as possible. 
--    However, in reality, inputs do not happen every frame;
--    in general its inefficient to poll every frame since only 
--    a handful of those frames will have input state changes.
--
--  - It is easy from a design perspective to write hard-to-read 
--    code when doing complex logic involving multiple polling states.
--
-- Event listening for inputs provides a solution for these problems
-- The idea is to tell the Input module to only run certain 
-- functions when an event is detected, that way our code
-- doesnt have to poll unnecessarily.
-- 
-- One of the initial differences you'll likely notice is that 
-- in general event listening requires a bit more code to get 
-- set up properly.
--
-- To contrast it with the previous entity, this one 
-- is a red square controlled with W, A, S, D + left click.
local createInputListExample = function()
    local this = Topaz.Entity.create(
        {}
    );

    local position = {
        x = 0,
        y = 0,
        z = 0
    };
    
    -- This will hold the state of whether the event 
    -- listener detected a clicking state. When its pressed,
    -- its updated to 1, but when released its updated to 0.
    local clicking = false;

    -- Same story as the first prefab!
    local visual = Topaz.Shape2D.create();
    
    
    visual:setColor(Topaz.Color.fromString('#ff0000'));
    visual:formRectangle(20, 20);
    this:addComponent(visual);



    -- This adds an event listener for the keyboard.
    -- All add listener functions expect an object;
    -- the object is searched for specific keys:
    -- 'onActive', 'onPress', 'onRelease', and 'onUpdate'.
    -- If any are populated, a function is meant 
    -- to be mapped as the value; this function will be 
    -- called in response to input events when they occur.
    --
    -- This listener is mapped to the onActive event, which 
    -- is every frame that an input is non-zero. This is 
    -- convenient for when buttons are held.
    Topaz.Input.addKeyboardListener(
        {
            -- Here is the onActive property.
            -- The funcion mapped takes 2 arguments: the map itself 
            -- we passed in, and the input id that is active.
            onActive = function(input, value) 
                if (input == Topaz.Key.w) then 
                    position.y = position.y + 1;          
                    this:setPosition(position);
                elseif (input == Topaz.Key.s) then
                    position.y = position.y - 1;           
                    this:setPosition(position);
                elseif (input == Topaz.Key.a) then
                    position.x = position.x - 1;          
                    this:setPosition(position);
                elseif (input == Topaz.Key.d) then                     
                    position.x = position.x + 1;       
                    this:setPosition(position);
                end
            end
        }
    );





    -- Similar to adding a keyboard listener, pointer 
    -- listeners will respond only to mouse events.
    Topaz.Input.addPointerListener(
        {
            -- The onUpdate event respondes to any time the 
            -- device sends a new event to topaz.
            onUpdate = function(input, value)

                -- The clicking state will by set here to detect when to apply the 
                -- x / y position change
                if (input == Topaz.Pointer._0) then
                    clicking = value > 0;
                end

                -- If the clicking state is active, then we check to 
                -- see if the event we're receiving is an X / Y update 
                -- event for the pointer. If it is, replace the component 
                -- in question with the raw value.
                if (clicking == true) then
                    if (input == Topaz.Pointer.X) then 
                        position.x = value;
                        this:setPosition(position);
                    elseif (input == Topaz.Pointer.Y) then
                        position.y = value;
                        this:setPosition(position);
                    end
                end
            end
        }
    );



    -- Finally this adds an event listener for the first connected gamepad.
    -- Gamepad listeners respond to axes and button events.
    -- This listener is mapped to the onActive event, which 
    -- is every frame that an input is non-zero. This is 
    -- convenient for when buttons are held.
    Topaz.Input.addPadListener(
        {
            -- Here is the onActive property.
            -- The funcion mapped takes 2 arguments: the map itself 
            -- we passed in, and the input id that is active.
            onActive = function(input, value)
                Topaz.Console.print('input '.. input .. ', val ' .. value);
                
                if (input == Topaz.Pad.d_up)then
                    position.y = position.y + 1;          
                    this:setPosition(position);
                elseif (input == Topaz.Pad.d_down) then
                    position.y = position.y - 1;           
                    this:setPosition(position);
                elseif (input == Topaz.Pad.d_left) then
                    position.x = position.x - 1;          
                    this:setPosition(position);
                elseif (input == Topaz.Pad.d_right) then
                    position.x = position.x + 1;       
                    this:setPosition(position);
                  
                end
            end
        },
        
        0 -- pad 0 is always the first one connected.        
    );   
    return this;     
end


-- Since our example is going to use multiple entities,
-- it is helpful to establish a basic "entity tree".
-- Here, our root is an empty object whose job is purely
-- to manage the input example entities we defined above.
local manager = Topaz.Entity.create();


-- Next, we'll instantiate those entities 
-- we defined, wrap them in a List, and modify
-- the "children" property, populating the tree.
--
-- Alternatively, the following could be rewritten as:
--    manager.attach(Input_Poll.new());
--    manager.attach(Input_List.new());
manager:attach(createInputPollExample());
manager:attach(createInputListExample());

-- Finally, insert the base of the tree into 
-- the default display itself.
Topaz.ViewManager.getDefault():setRoot(manager);
