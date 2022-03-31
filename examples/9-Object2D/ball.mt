@:class         = import(module:'Matte.Core.Class');
@:Topaz         = import(module:'Topaz');
@:PhysicsObject = import(module:'physics_object.mt');
@:constants     = import(module:'constants.mt');




return class(
    name: 'Ball',
    inherits: [
        PhysicsObject
    ],
    
    define:::(this) {
        this.constructor = ::{
            this.constructor[PhysicsObject](static:false);
            this.physics.setColliderRadial(radius:constants.BALL_SIZE/2, numIterations:10);
            return this;
        };
        @:shape2D = Topaz.Shape2D.new();
                
                
        shape2D.scale = {
            x:constants.BALL_SIZE / constants.BALL_IMAGE.width,
            y:constants.BALL_SIZE / constants.BALL_IMAGE.height
        };
        

        shape2D.formImage(image:constants.BALL_IMAGE);
        shape2D.center = {
            x: constants.BALL_IMAGE.width/2,
            y: constants.BALL_IMAGE.height/2
        };
        
        shape2D.color = {
            r: 0.25 + 0.75 * Number.random(),
            g: 0.25 + 0.75 * Number.random(),
            b: 0.25 + 0.75 * Number.random(),
            a: 1
        };
        

        
        
        
        this.attach(component:shape2D);
        

        
        this.physics.velocityX = Number.random()*4-2;
        this.physics.installHook(
            event: 'on-collide',
            callback :::(source) {
                @selfPhys = this.physics;
                @otherPhys = source.physics;
                
                // collide with another ball
                if (otherPhys.group == constants.BALL_GROUP) ::<={
                    //when (selfPhys.speed < otherPhys.speed) empty;

                                        
                    // create a force to this and the other object
                    // tangential to the center of mass and the 
                    // collision location.
                    @:thisForce = {
                        x:selfPhys.lastCollidedPosition.x - this.position.x,
                        y:selfPhys.lastCollidedPosition.y - this.position.y
                    };

                    Topaz.Vector.normalize(value:thisForce);

                    thisForce.x *= selfPhys.speed;
                    thisForce.y *= selfPhys.speed;


                    // at this point, "thisForce" is the force applied to the 
                    // direction-specific collision
                    // The force here is split between the colliding bodies
                    @:toOther = {
                        x:thisForce.x*0.5,
                        y:thisForce.y*0.5
                    };


                    // skip non-bounce entities.                
                    this.position = selfPhys.lastPosition;
                    selfPhys.resetMotion();

                    otherPhys.velocityX += toOther.x;
                    otherPhys.velocityY += toOther.y;
                    
                } else ::<={
                    this.position = {
                        x: this.position.x,
                        y: selfPhys.lastPosition.y
                    };
                    selfPhys.resetMotion();
                    selfPhys.velocityY *= -0.5;
                    selfPhys.velocityX *= 0.95;
                    if (Number.abs(of:selfPhys.velocityY) < 2) selfPhys.velocityY = 0;
                };
                            
            }
        );
    }
);


