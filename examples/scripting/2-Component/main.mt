// In the previous example, we defined our own entity and 
// used a built-in component, Shape2D, to provide our entity 
// the ability to show 2D shapes on the screen
//
// In this example, we will explore the concept of components more
// by making our own.
//
// 
// 
@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');


// Here, we're going to make a component and an entity 
// that could serve as part of a game. In this game,
// people (a 'Person' entity) can give each other fruit, 
// so a component was made to facilitate holding the fruit
// ('FruitBasket'). 
//
// Why use a component instead of just a simple variable
// for this purpose?
//
// A few reasons:
// - On a basic level, a simple variable may work, but for 
//   further development and consistent behavior,
//   it makes sense to have a separate class wrap the 
//   conceptual behavior regarding health. Consider things 
//   like max capacity, what happens when fruit equals 0, and 
//   other things. While you could still manage these within 
//   the entity, having it be a standard separate class 
//   increases readability and maintainability, especially with 
//   functions to interact with it.
//
// - Someone familiar with object oriented programming may ask:
//   why not just use a normal class? That would most certainly be 
//   a valid approach! However, components have 2 significant 
//   uses 1) Components are essentially event systems;
//   they can be extended to support custom events which other objects 
//   can subscribe to. 2) like Entities, they can be updated every 
//   frame with onStep() and onDraw() functions. 
//   It's up to you when to choose to write a new entity, component,
//   or a simple plain object.
//
//
//
@:FruitBasket = class(
    name : 'FruitBasket',        
    inherits : [Topaz.Component],
    define:::(this) {
        // Current amount of fruit in the basket.
        // We start the basket with a decent amount of fruit.
        //
        // Since we want certain, specific behavior to happen when 
        // the mount of fruit changes, it makes sense to make 
        // "amount" a private variable. Here, only this FruitBasket instance 
        // can touch "amount".
        @amount = 0;
        
        this.constructor = ::(startingFruit => Number) {
            amount = startingFruit;
            return this;
        };

        // Here is where things get interesting:
        // Any component can extend its functionality by adding 
        // its own "events". An event is usually a response to 
        // some action: when an event is emitted, any instance 
        // subscribed to the event will receive a notification 
        // that the event was emitted.
        //
        //
        // Here we want to track 2 events: adding fruit and 
        // removing fruit. The names of the functions defined here are the 
        // names of the events to add.
        this.installEvent(
            event   :'onAddFruit', 
            callback:::(source) {
                Topaz.log(message:'Fruit was added!');
            }
        );


        this.installEvent(
            event   :'onRemoveFruit', 
            callback:::(source) {
                Topaz.log(message:'Fruit was removed!');
            }
        );
 


        this.interface = {

            // Adds a fruit to the basket from someone.
            // For member functions, "this" will always refer to the component.
            addFruit::(from) {

                amount+=1;

                // emitEvent() also supports a second argument: a source entity.
                // If theres an entity involved in the event, this argument 
                // can optionally be passed to be referred to in the notification.
                // All components have a "host" property that refers to 
                // the owning entity that holds the component if any.
                this.emit(event:'onAddFruit', source:from.host);
            },

            // Function to get the current amount of fruit left.
            getFruitLeft :: {
                return amount;
            },



            // Function called when taking a fruit from the basket.
            // It will only "work" if theres fruit in the basket, 
            // which is checked.
            removeFruit :: {
                if (amount > 0) ::<= {
                    amount-=1;

                    // emitEvent() notifies the component that the event is occurring.
                    this.emit(event:'onRemoveFruit');
                };
            },
    
            // Simple way an outside instance can ask whether the 
            // basket contains fruit.
            isEmpty :: {
                return amount == 0;
            }

          
        };
    }
);


// Now that we have defined a FruitBasket, we can define a Person 
// to hold the basket. Every person has a name and a basket of 
// their own.
@:Person = class(
    name : 'Person',        
    inherits : [Topaz.Entity],
    define : ::(this) {
        // Like always, using the class name followed by parantheses is how 
        // one instantiates a class.
        @myBasket = FruitBasket.new(startingFruit:5);
        @myName = 'No name!';

        // And since it is a component, we want to make sure to attach it 
        // so that the component gets updated.
        this.add(component:myBasket);


        this.interface = {
            // The basket owned by the person.
            basket : {
                get :: {
                    return myBasket;
                }
            },

            // The person's own name.
            firstName : {
                get :: {
                    return myName;
                },

                set ::(value) {
                    myName = value;
                }
            },

            // convenience function to remove fruit from 
            // a persons own basket to give to another.
            // The transaction is cancelled if the giver runs 
            // out of fruit.
            //
            // As a note, when a props function is called, the "this" binding 
            // is always the prefab function component/entity and can be used as an alternative 
            // to the prefab function argument.
            giveFruit ::(to, amount) {
                listen(to:::{
                    for(in:[0, amount], do:::(i) {
                        // Oh no! out of fruit, so abort
                        when(myBasket.isEmpty()) send(message:empty);

                        // Remove from this person's basket....
                        myBasket.removeFruit();

                        // ... and give to another person. The argument is 
                        // who gave the fruit                        
                        to.basket.addFruit(from:myBasket);
                    });
                });
            }
        
        };
    }
);






// Lets create our people involved
@throcky = Person.new();
@alex    = Person.new();


// And assign names to them!
throcky.firstName = 'Throckmorton';
   alex.firstName = 'Alex';

// Next, we will set up behavior to use what we've set up 
// To easily see this, we will just use text in the 
// debugging console. By default it is enabled, but 
// it can be activated with Topaz.enableConsole().
Topaz.enableConsole();


// Up until now, we have registered the onAddFruit and onRemoveFruit 
// events with the FruitBasket, but now we will actually add behavior 
// to resond to the event. installHook() is the most convenient way 
// to do this: it adds a custom closure or function to run when 
// the specified event is detected.
// The first argument is always the component that is emitting the event.
alex.basket.installHook(
    // The name of the event to respond to.
    event: 'onRemoveFruit',

    // The closure to run.
    callback:::(source) {
        // Often for debugging, its useful to use Topaz.log(), which 
        // will print a string to the system console.
        Topaz.log(message:alex.firstName + ' removed a fruit from their basket. ' + alex.basket.getFruitLeft() + ' left.');
    }
);

// Now lets add a hook for onAddFruit for Alex. Notice 
// that the closure we pass has a second argument. 
// In the case that we pass an entity to the emitEvent,
// a second argument will be populated with the entity in question.
// This can help identify the nature of the event when applicable.
alex.basket.installHook(
    event: 'onAddFruit',
    callback:::(source) {
        Topaz.log(message:alex.firstName + ' gained fruit, thanks to ' + source.firstName +'! ' + alex.basket.getFruitLeft() + ' total.');
    }
);

// Now lets exchange some fruit. Because of our hooks above,
// this will print various statements to the console.
// Notice how both our custom notification and the base notification are 
// called; also note the order. Hooks / handlers installed last are called first.
throcky.giveFruit(to:alex, amount:2);
alex.giveFruit(to:throcky, amount:5);

// Notice how there was no need to populate Topaz.root.
// Making an entity part of the main entity tree allows 
// frame-based behavior, such as onStep and onDraw, to be 
// managed by topaz. There may be times where you dont 
// need to attach entities for the lifetime of the 
// program.
