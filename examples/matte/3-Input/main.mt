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
@:createInputPollExample = ::{
    @position = {
        x : 0,
        y : 0,
        z : 0
    }
    @:this = Topaz.Entity.create(
        attributes: {
            onStep ::{

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

                if (Topaz.Input.getState(input:Topaz.Key.up)) ::<={
                    position.y += 1;      
                    this.setPosition(value:position);      
                }

                if (Topaz.Input.getState(input:Topaz.Key.down)) ::<={
                    position.y -= 1;           
                    this.setPosition(value:position);      
                }

                if (Topaz.Input.getState(input:Topaz.Key.left)) ::<={
                    position.x -= 1;            
                    this.setPosition(value:position);      
                }

                if (Topaz.Input.getState(input:Topaz.Key.right)) ::<={
                    position.x += 1;         
                    this.setPosition(value:position);      
                }

                // The left/right/middle button can be polled using 
                // the Topaz.Key.topazPointer_* inputs.
                // All inputs for the mouse+keyboard can use the getState() function.
                if (Topaz.Input.getState(input:Topaz.Pointer._1)) ::<={

                    // To poll position position, getState can be used. However,
                    // due to the commonality of this operation, the position 
                    // can be polled by just accessing Topaz.Input.mouseX / Y.
                    position = {
                        x:Topaz.Input.mouseX(),
                        y:Topaz.Input.mouseY()
                    }
                    this.setPosition(value:position);      
                }
            }        
        }
    );

    // Since we are making the shape once and not modifying it,
    // theres no need to keep the component around.
    @:visual = Topaz.Shape2D.create();
    
    

    // Set it to blue and make it 20x20
    visual.setColor(color:Topaz.Color.fromString(str:'blue'));
    visual.formRectangle(width:20, height:20);


    // As shorthand for .addComponent(), you 
    // can set an entity's components by setting 
    // the "components" property equal to an array
    // of the component references. It tends to be 
    // visually more compact while still very readable.
    this.addComponent(component:visual);
    
    return this;
}

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
@createInputListExample :: {
    @:this = Topaz.Entity.create(
        attributes:{}
    );

    @:position = {
        x: 0,
        y: 0,
        z: 0
    };
    
    // This will hold the state of whether the event 
    // listener detected a clicking state. When its pressed,
    // its updated to 1, but when released its updated to 0.
    @clicking = false;

    // Same story as the first prefab!
    @:visual = Topaz.Shape2D.create();
    
    
    visual.setColor(color:Topaz.Color.fromString(str:'#ff0000'));
    visual.formRectangle(width:20, height:20);
    this.addComponent(component:visual);



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
        listener : {
            // Here is the onActive property.
            // The funcion mapped takes 2 arguments: the map itself 
            // we passed in, and the input id that is active.
            onActive :::(input, value) {
                match(input) {
                  (Topaz.Key.w):::<={
                    position.y += 1;          
                    this.setPosition(value:position);
                  },

                  (Topaz.Key.s):::<={
                    position.y -= 1;           
                    this.setPosition(value:position);
                  },

                  (Topaz.Key.a):::<={
                    position.x -= 1;          
                    this.setPosition(value:position);
                  },
                    

                  (Topaz.Key.d):::<={
                    position.x += 1;       
                    this.setPosition(value:position);
                  }
                }
            }
        }
    );





    // Similar to adding a keyboard listener, pointer 
    // listeners will respond only to mouse events.
    Topaz.Input.addPointerListener(
        listener : {
            // The onUpdate event respondes to any time the 
            // device sends a new event to topaz.
            onUpdate :::(input, value) {

                // The clicking state will by set here to detect when to apply the 
                // x / y position change
                if (input == Topaz.Pointer._0) ::<={
                    clicking = value > 0;
                }

                // If the clicking state is active, then we check to 
                // see if the event we're receiving is an X / Y update 
                // event for the pointer. If it is, replace the component 
                // in question with the raw value.
                if (clicking) ::<={
                    match(input) {
                      (Topaz.Pointer.X):::<={
                        position.x = value;
                        this.setPosition(value:position);
                      },

                      (Topaz.Pointer.Y):::<={
                        position.y = value;
                        this.setPosition(value:position);
                      }
                    }
                }
            }
        }
    );



    // Finally this adds an event listener for the first connected gamepad.
    // Gamepad listeners respond to axes and button events.
    // This listener is mapped to the onActive event, which 
    // is every frame that an input is non-zero. This is 
    // convenient for when buttons are held.
    Topaz.Input.addPadListener(
        padIndex : 0, // pad 0 is always the first one connected.

        listener : {
            // Here is the onActive property.
            // The funcion mapped takes 2 arguments: the map itself 
            // we passed in, and the input id that is active.
            onActive :::(input, value) {
                Topaz.Console.print(message:'input ' + input + ', val ' + value);
                match(input) {
                  (Topaz.Pad.d_up):::<={
                    position.y += 1;          
                    this.setPosition(value:position);
                  },

                  (Topaz.Pad.d_down):::<={
                    position.y -= 1;           
                    this.setPosition(value:position);
                  },

                  (Topaz.Pad.d_left):::<={
                    position.x -= 1;          
                    this.setPosition(value:position);
                  },
                    

                  (Topaz.Pad.d_right):::<={
                    position.x += 1;       
                    this.setPosition(value:position);
                  }
                }
            }
        }
    );   
    return this;     
}


// Since our example is going to use multiple entities,
// it is helpful to establish a basic "entity tree".
// Here, our root is an empty object whose job is purely
// to manage the input example entities we defined above.
@manager = Topaz.Entity.create();


// Next, we'll instantiate those entities 
// we defined, wrap them in a List, and modify
// the "children" property, populating the tree.
//
// Alternatively, the following could be rewritten as:
//    manager.attach(Input_Poll.new());
//    manager.attach(Input_List.new());
manager.attach(child:createInputPollExample());
manager.attach(child:createInputListExample());

// Finally, insert the base of the tree into 
// the default display itself.
Topaz.ViewManager.getDefault().getViewport().attach(child: manager);
