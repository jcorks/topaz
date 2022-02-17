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
                @:length = Topaz.defaultDisplay.width;
                this.physics.collider = [
                    -length/2, 0,
                    length/2, 0,
                    length/2, 1,
                    -length/2, 1
                ];
                
                this.position = {
                    x: Topaz.defaultDisplay.width/2,
                    y: Topaz.defaultDisplay.height
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
