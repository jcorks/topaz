// We'll also need the language class module as well
@:class = import(module:'Matte.Core.Class');


// All topaz symbols are contained within the module "Topaz".
// It needs to be pulled in before accessing its features.
@:Topaz = import(module:'Topaz');



// In Matte, topaz implements its Entities as normal classes.
// This means that inheritance can be used as well as vanilla 
// entities.
//
// As a review, entities are the basic building blocks within 
// a topaz program. They are updated every frame once the engine is started 
// and can contain their own properties and components.
//
// Typically, entities will be extended in 2 ways:
//  - Populating an update function such as onStep or onDraw, which will 
//    be called every frame.
//  - Attaching components to the entity to extend its behavior in reusable ways.
//
//
// The entity we want to create will:
//  - Show a colored rectangle on the screen
//  - Change the rotation of the rectangle 
//    every frame.
//
// Every custom entity will be its own tclass that inherits from either Topaz.Entity
// directly, or another Topaz.Entity-based class.
//
@: Rectangle = class(
    // Specifies our class to inherit the properties 
    // from a normal Topaz Entity.
    inherits : [Topaz.Entity],
    
    // The define property is a special function to the class uses 
    // to construct new instances. 'this' is a reference to the output 
    // object when instantiating from this class.
    define:::(this) {

        // It is typical to place private variables within 
        // the scope of "define". This makes sure that 
        // the variable is only accessible from within 
        // the implementation of the class.
        @visual;


        // Once all the members of the object are setup, the 
        // constructor is called. This means that we can 
        // now use all of the behaviors of the object.
        this.constructor = ::{
        
            // NOTE: EVERY inherited object from Entity or Component
            // MUST call the activate function before Entity behaviors can 
            // be used.
            this.activate();
            
            // Our entity will have a visual associated with it, so 
            // Shape2D is the basic component for drawing 2D shapes.
            // It can be formed into any shape, but we will stick with 
            // a basic rectangle.
            visual = Topaz.Shape2D.new();

            // This changes the shape into a 200 by 200 rectangle.
            visual.formRectangle(width:200, height:200);

            // By default, the center of a Shape2D is the top-left 
            // of the shape. Since rotation is about the center,
            // the set the center to one half of the rectangles span.
            visual.center = {x:100, y:100};



            // Even though the component has all the information 
            // we want stored within it, the component cannot update itself.
            // We still have to associate it with our entity.
            // This can be done by using the addComponent() function.
            //
            this.attach(component:visual);

            // Along with components, every entity can have a custom set of 
            // update functions. The most common one is onStep() which is 
            // run every frame. 
            this.onStep = ::{
                // Every entity has built-in properties, such as 
                // position, rotation, and scale.
                // Here, we add 0.01, 0.2, and 0.03 degrees to the x, y, and z 
                // rotation respectively every frame. 
                //
                // The position, rotation, and scale properties are vector objects,
                // so to update them, you actually replace them with new vectors.
                @:r = this.rotation;
                this.rotation = {
                    x:r.x + 0.01,
                    y:r.y + 0.2,
                    z:r.z + 0.03
                };
            };
        }

        // To extend a class with additional members, the interface object
        // is the main way that you as a user will refer to public members.
        // when constructed, the interface properties will be copied to 
        // the object instance.
        this.interface = {
            // Here we'll set up a public function to modify the color 
            // of the object.
            setColor ::(value) {
                // It will take a color object and set the color property 
                // of the Shape2D
                visual.color = value;
            }  
        };
    }
);


// Now that the entity has been defined, we need to actually use it.
//

// This instantiates the entity we've defined as a class.
@entity = Rectangle.new();

// The color of any Shape2D can be assigned directly. 
// The string can be a common color name, or a #RRGGBB/AA 
// formatted hex string.
entity.setColor(value:'purple');        



// The default position for an entity is 0,0,0,
// Which is the center of the display. Note how we set the center 
// of the Shape2D component already: topaz is inherently 
// hierarchical: objects will use the position / rotation / scale 
// of things that they are attached to. 



// Even though we've created the our rectangle entity,
// creating the entity on its own doesnt do much.
// Much like when we created the component, 
// topaz still needs to be aware of what to do with it.
//
// Topaz manages entities in the form of a tree, so     
// entities naturally can have children entities 
// and a parent entity. The root is the very base of the tree.
// On top of that, each display (or window) can have its 
// own hierarchy.
//  
// Since we are only making one entity, we will just 
// set the entity has the root itself. And, because the scripting 
// environment creates a window for us (the "defaultDisplay"),
// we can just use this display directly.
//
// Normally, the root will be more symbolic than physical,
// think of a "room" in a game.
//
// The tree can be manipulated while running to control 
// runtime efficiency for the program.
//
Topaz.defaultDisplay.root = entity;
