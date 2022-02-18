@:constants = import(module:'constants.mt');
@:class     = import(module:'Matte.Core.Class');
@:Topaz     = import(module:'Topaz');

return class(
    name: 'PhysicsObject',
    inherits : [
        Topaz.Entity
    ],
    
    define:::(this) {
        @:o2d = Topaz.Object2D.new();
        this.components = [o2d];
        
        this.constructor = ::(static) {
            if (static) ::<={
                o2d.group = constants.FLOOR_GROUP;
                
            } else ::<= {
                o2d.group = constants.BALL_GROUP;
                
                // be affected by gravity
                this.onStep = ::{
                    o2d.velocityY += constants.GRAVITY;
                    
                    // remove if out of bounds
                    if (this.position.y > Topaz.defaultDisplay.height+constants.BALL_SIZE) 
                        this.remove();
                };  
                
            };
            
            return this;
        };
        
        this.interface = {
            physics : {
                get :: {
                    return o2d;
                }
            }
        };   
    }
);

