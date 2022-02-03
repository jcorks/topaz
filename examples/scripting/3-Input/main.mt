// So far, we've examined the nature of entities and 
// components. There is one more major source of functionality
// within topaz: modules. Modules are collections of 
// tools that allow for interactions with system-oriented 
// behaviors. This example focuses on the Input module 
// within topaz. 
//
// This example demonstrates the 2 main methods to get
// input information: Polling and Event Listening.
//
@:Topaz  = import(module:'Topaz');
@:class  = import(module:'Matte.Core.Class');

//
//
// This prefab demonstrates how to use polling. Polling 
// is the most basic and straightforward way to use input,
// functions such as getState() return the currently recorded 
// input state for a particular input. Every frame that topaz 
// updates, the Input module will check for any incoming 
// events for input devices and save their info. As such,
// polling does not request from the device directly, but rather 
// from what topaz last recorded. As such, polling is pretty
// quick.
//
// To demonstrate the effect of input, this example entity 
// will use a blue rectangle to show itself onscreen and will 
// respond to the arrow keys and right-click + dragging.
@:Input_Poll = class(
    name : 'Input_Poll',
    inherits : [Topaz.Entity],
    define:::(this) {
        // Since we are making the shape once and not modifying it,
        // theres no need to keep the component around.
        @:visual = Topaz.Shape2D.new();

        // Set it to blue and make it 20x20
        visual.color = Topaz.Color.parse(string:'blue');
        visual.formRectangle(width:20, height:20);

        // As shorthand for .addComponent(), you 
        // can set an entity's components by setting 
        // the "components" property equal to an array
        // of the component references. It tends to be 
        // visually more compact while still very readable.
        this.components = [visual];

        this.onStep = ::{

            // For the polling method, the accuracy is dependent 
            // on how often input is re-polled. For both convenience
            // and accuracy, it is common to just re-poll for input 
            // every frame.
            //
            // Topaz.Input.getState() returns a number representing the 
            // state of the input. Usually 0 means inactive, and 
            // above 0 means its active. For buttons, this is 
            // just 0.0 vs 1.0. 
            //
            // Here we check the individual key values for the arrows.
            // If the key state is non-zero, we modify the 
            // position vector by adding "1" to it in the direction 
            // that corresponds to the arrow.

            if (Topaz.Input.getState(input:Topaz.Input.KEY.UP)) ::<={
                // The add() function for vectors allows the creation 
                // of a new vector by summing the x, y, and z components
                // of the source vector and argument vector.
                this.position = Topaz.Vector.add(a:this.position, b:{x:0, y:-1});            
            };

            if (Topaz.Input.getState(input:Topaz.Input.KEY.DOWN)) ::<={
                this.position = Topaz.Vector.add(a:this.position, b:{x:0, y:1});            
            };

            if (Topaz.Input.getState(input:Topaz.Input.KEY.LEFT)) ::<={
                this.position = Topaz.Vector.add(a:this.position, b:{x:-1, y:0});            
            };


            if (Topaz.Input.getState(input:Topaz.Input.KEY.RIGHT)) ::<={
                this.position = Topaz.Vector.add(a:this.position, b:{x:1, y:0});            
            };

            // The left/right/middle button can be polled using 
            // the Topaz.Key.topazPointer_* inputs.
            // All inputs for the mouse+keyboard can use the getState() function.
            if (Topaz.Input.getState(input:Topaz.Input.POINTER._1)) ::<={

                // To poll position position, getState can be used. However,
                // due to the commonality of this operation, the position 
                // can be polled by just accessing Topaz.Input.mouseX / Y.
                this.position = {
                    x:Topaz.Input.mouse.x,
                    y:Topaz.Input.mouse.y
                };
            };
        };

        // We won't be using any persistent, public props, so there will 
        // be no interface call.
    }
);


// This prefab demonstrates the usage of event listening.
// While polling is a pretty convenient way to respond to 
// input, it has a few issues:
//  - For best use, it requires re-polling as often as possible. 
//    However, in reality, inputs do not happen every frame;
//    in general its inefficient to poll every frame since only 
//    a handful of those frames will have input state changes.
//
//  - It is easy from a design perspective to write hard-to-read 
//    code when doing complex logic involving multiple polling states.
//
// Event listening for inputs provides a solution for these problems
// The idea is to tell the Input module to only run certain 
// functions when an event is detected, that way our code
// doesnt have to poll unnecessarily.
// 
// One of the initial differences you'll likely notice is that 
// in general event listening requires a bit more code to get 
// set up properly.
//
// To contrast it with the previous entity, this one 
// is a red square controlled with W, A, S, D + left click.
@Input_List = class(
    name : 'Input_List',
    inherits : [Topaz.Entity],
    define:::(this) {
        // This will hold the state of whether the event 
        // listener detected a clicking state. When its pressed,
        // its updated to 1, but when released its updated to 0.
        @clicking = false;

        // Same story as the first prefab!
        @:visual = Topaz.Shape2D.new();

        // Here, we're using RGB hex notation to define our color.
        visual.color = Topaz.Color.parse(string:'#ff0000');
        visual.formRectangle(width:20, height:20);
        this.components = [visual];





        // This adds an event listener for the keyboard.
        // All add listener functions expect an object;
        // the object is searched for specific keys:
        // 'onActive', 'onPress', 'onRelease', and 'onUpdate'.
        // If any are populated, a function is meant 
        // to be mapped as the value; this function will be 
        // called in response to input events when they occur.
        //
        // This listener is mapped to the onActive event, which 
        // is every frame that an input is non-zero. This is 
        // convenient for when buttons are held.
        Topaz.Input.addKeyboardListener(

            // Here is the onActive property.
            // The funcion mapped takes 2 arguments: the map itself 
            // we passed in, and the input id that is active.
            onActive :::(input, value) {
                match(input) {
                  (Topaz.Input.KEY.W):::<={
                    this.position = Topaz.Vector.add(a:this.position, b:{x:0, y:-1});            
                  },

                  (Topaz.Input.KEY.S):::<={
                    this.position = Topaz.Vector.add(a:this.position, b:{x:0, y:1});            
                  },

                  (Topaz.Input.KEY.A):::<={
                    this.position = Topaz.Vector.add(a:this.position, b:{x:-1, y:0});            
                  },
                    

                  (Topaz.Input.KEY.D):::<={
                    this.position = Topaz.Vector.add(a:this.position, b:{x:1, y:0});            
                  }
                };
            }
        );

        // Similar to adding a keyboard listener, pointer 
        // listeners will respond only to mouse events.
        Topaz.Input.addPointerListener(

            // The onUpdate event respondes to any time the 
            // device sends a new event to topaz.
            onUpdate :::(input, value) {

                // The clicking state will by set here to detect when to apply the 
                // x / y position change
                if (input == Topaz.Input.POINTER._0) ::<={
                    clicking = value > 0;
                };

                // If the clicking state is active, then we check to 
                // see if the event we're receiving is an X / Y update 
                // event for the pointer. If it is, replace the component 
                // in question with the raw value.
                if (clicking) ::<={
                    match(input) {
                      (Topaz.Input.POINTER.X):::<={
                        this.position = {x:value, y:this.position.y};
                      },

                      (Topaz.Input.POINTER.Y):::<={
                        this.position = {x:this.position.x, y:value};
                      }
                    };
                };
            }
        );

        // if no public special properties are needed, interface does not need to be called
    }
);


// Since our example is going to use multiple entities,
// it is helpful to establish a basic "entity tree".
// Here, our root is an empty object whose job is purely
// to manage the input example entities we defined above.
@manager = Topaz.Entity.new();


// Next, we'll instantiate those entities 
// we defined, wrap them in a List, and modify
// the "children" property, populating the tree.
//
// Alternatively, the following could be rewritten as:
//    manager.attach(new topaz.entity(Input_Poll));
//    manager.attach(new topaz.entity(Input_List));
manager.children = [
    Input_Poll.new(),
    Input_List.new()
];

// Finally, insert the base of the tree into 
// the default display itself.
Topaz.defaultDisplay.root = manager;
