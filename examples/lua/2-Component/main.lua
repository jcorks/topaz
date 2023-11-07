-- In the previous example, we defined our own entity and 
-- used a built-in component, Shape2D, to provide our entity 
-- the ability to show 2D shapes on the screen
--
-- In this example, we will explore the concept of components more
-- by making our own.
--
-- 
-- 

-- Here, we're going to make a component and an entity 
-- that could serve as part of a game. In this game,
-- people (a 'Person' entity) can give each other fruit, 
-- so a component was made to facilitate holding the fruit
-- ('FruitBasket'). 
--
-- Why use a component instead of just a simple variable
-- for this purpose?
--
-- A few reasons:
-- - On a basic level, a simple variable may work, but for 
--   further development and consistent behavior,
--   it makes sense to have a separate class wrap the 
--   conceptual behavior regarding fruit. Consider things 
--   like max capacity, what happens when fruit equals 0, and 
--   other things. While you could still manage these within 
--   the entity, having it be a standard separate class 
--   increases readability and maintainability, especially with 
--   functions to interact with it.
--
--
--
--
local createFruitBasket = function() 
    local this = Topaz.Component.create('FruitBasket', {});

    -- Current amount of fruit in the basket.
    -- We start the basket with a decent amount of fruit.
    --
    -- Since we want certain, specific behavior to happen when 
    -- the mount of fruit changes, it makes sense to make 
    -- "amount" a private variable. Here, only this FruitBasket instance 
    -- can touch "amount".
    local amount = 0;
    

    -- Here is where things get interesting:
    -- Any component can extend its functionality by adding 
    -- its own "events". An event is usually a response to 
    -- some action: when an event is emitted, any instance 
    -- subscribed to the event will receive a notification 
    -- that the event was emitted.
    --
    --
    -- Here we want to track 2 events: adding fruit and 
    -- removing fruit. The names of the functions defined here are the 
    -- names of the events to add.
    this:installEvent(
        'onAddFruit', 
        function(component, source) 
            Topaz.Console.print('Fruit was added!');
        end
    );


    this:installEvent(
        'onRemoveFruit', 
        function (component, source) 
            Topaz.Console.print('Fruit was removed!');
        end
    );
    
    -- Sets the current number of fruit.
    this.reset =  function (startingFruit)
        amount = startingFruit;            
    end

    -- Adds a fruit to the basket from someone.
    -- For member functions, "this" will always refer to the component.
    this.addFruit = function(from)

        amount=amount+1;

        -- emitEvent() also supports a second argument: a source entity.
        -- If theres an entity involved in the event, this argument 
        -- can optionally be passed to be referred to in the notification.
        -- All components have a "host" property that refers to 
        -- the owning entity that holds the component if any.
        this:emitEvent('onAddFruit', from:getHost());
    end

    -- Function to get the current amount of fruit left.
    this.getFruitLeft = function()
        return amount;
    end



    -- Function called when taking a fruit from the basket.
    -- It will only "work" if theres fruit in the basket, 
    -- which is checked.
    this.removeFruit = function()
        if (amount > 0) then
            amount = amount - 1;

            -- emitEvent() notifies the component that the event is occurring.
            this:emitEventAnonymous('onRemoveFruit');
        end
    end
    
    -- Simple way an outside instance can ask whether the 
    -- basket contains fruit.
    this.isEmpty = function()
        return amount == 0;
    end

    return this;
end


-- Now that we have defined a FruitBasket, we can define a Person 
-- to hold the basket. Every person has a name and a basket of 
-- their own.
local createPerson = function(name)
    local this = Topaz.Entity.create({});
    -- Like always, using the class name followed by parantheses is how 
    -- one instantiates a class.
    local myBasket = createFruitBasket();
    local myName = name;

            
    myBasket.reset(5);            
    -- And since it is a component, we want to make sure to attach it 
    -- so that the component gets updated.
    this:addComponent(myBasket);


    -- The basket owned by the person.
    this.getBasket = function()
        return myBasket;
    end

    -- The person's own name.
    this.getName = function() return myName end

    -- convenience function to remove fruit from 
    -- a persons own basket to give to another.
    -- The transaction is cancelled if the giver runs 
    -- out of fruit.
    --
    -- As a note, when a props function is called, the "this" binding 
    -- is always the prefab function component/entity and can be used as an alternative 
    -- to the prefab function argument.
    this.giveFruit = function(to, amount)
        for i=0, amount do
            -- Oh no! out of fruit, so abort
            if (myBasket.isEmpty()) then return end;

            -- Remove from this person's basket....
            myBasket.removeFruit();

            -- ... and give to another person. The argument is 
            -- who gave the fruit                        
            to.getBasket().addFruit(myBasket);
        end
    end
    
    return this;
end






-- Lets create our people involved
local throcky = createPerson('Throckmorton');
local alex    = createPerson('Alex');


-- Next, we will set up behavior to use what we've set up 
-- To easily see this, we will just use text in the 
-- debugging console. By default it is enabled, but 
-- it can be activated with Topaz.enableConsole().
Topaz.Console.enable(true);


-- Up until now, we have registered the onAddFruit and onRemoveFruit 
-- events with the FruitBasket, but now we will actually add behavior 
-- to resond to the event. installHook() is the most convenient way 
-- to do this: it adds a custom closure or function to run when 
-- the specified event is detected.
-- The first argument is always the component that is emitting the event.
alex.getBasket():installHook(
    -- The name of the event to respond to.
    'onRemoveFruit',

    -- The closure to run.
    function(component, source) 
        -- Often for debugging, its useful to use Topaz.log(), which 
        -- will print a string to the system console.
        Topaz.Console.print(alex.getName() .. ' removed a fruit from their basket. ' .. alex.getBasket().getFruitLeft().. ' left.');
    end
);

-- Now lets add a hook for onAddFruit for Alex. Notice 
-- that the closure we pass has a second argument. 
-- In the case that we pass an entity to the emitEvent,
-- a second argument will be populated with the entity in question.
-- This can help identify the nature of the event when applicable.
alex.getBasket():installHook(
    'onAddFruit',
    function(component, source) 
        Topaz.Console.print(alex.getName() .. ' gained fruit, thanks to ' .. source.getName() .. '! ' .. alex.getBasket().getFruitLeft() .. ' total.');
    end
);

-- Now lets exchange some fruit. Because of our hooks above,
-- this will print various statements to the console.
-- Notice how both our custom notification and the base notification are 
-- called; also note the order. Hooks / handlers installed last are called first.
throcky.giveFruit(alex, 2);
alex.giveFruit(throcky, 5);

-- Notice how there was no need to populate Topaz.root.
-- Making an entity part of the main entity tree allows 
-- frame-based behavior, such as onStep and onDraw, to be 
-- managed by topaz. There may be times where you dont 
-- need to attach entities for the lifetime of the 
-- program.
