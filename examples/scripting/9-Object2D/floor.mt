@:class         = import(module:'Matte.Core.Class');
@:Topaz         = import(module:'Topaz');
@:PhysicsObject = import(module:'physics_object.mt');



// A floor that, when instantiated, always 
// matches the bottom of the display
return class(
    name: 'Floor', 
    inherits: [
        PhysicsObject
    ],
    
    define:::(this) {
        this.constructor = ::(length){
            
            this.constructor[PhysicsObject](static: true);
            
            @:updateFloor = ::{        
                this.physics.setColliderRectangle(
                    width: Topaz.defaultDisplay.width,
                    height:2
                );

                
                this.position = {
                    x: 0,
                    y: -Topaz.defaultDisplay.height/2
                };
            };
            Topaz.defaultDisplay.addParameterCallback(func:::(
                display,
                parameter
            ) {
                if (parameter == Topaz.Display.PARAMETER.WIDTH ||
                    parameter == Topaz.Display.PARAMETER.HEIGHT)
                        updateFloor();
            });
            
            updateFloor();
            
            return this;
        };        
    }
);
