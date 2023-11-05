@:class         = import(module:'Matte.Core.Class');
@:Topaz         = import(module:'Topaz');
@:createPhysicsObject = import(module:'physics_object.mt');
@:constants     = import(module:'constants.mt');




return ::{
    @:this = createPhysicsObject(static:false);

    @:shape2D = Topaz.Shape2D.create();

    this.physics.setColliderRadial(radius:constants.BALL_SIZE/2, numSides:10);

                    
    shape2D.setScale(
        value : {
            x:constants.BALL_SIZE / constants.BALL_IMAGE.getWidth(),
            y:constants.BALL_SIZE / constants.BALL_IMAGE.getHeight()
        }
    )
    

    shape2D.formImage(asset:constants.BALL_IMAGE);
    shape2D.setCenter(
        value :{
            x: constants.BALL_IMAGE.getWidth()/2,
            y: constants.BALL_IMAGE.getHeight()/2
        }
    );
    
    shape2D.setColor(
        color : {
            r: 0.25 + 0.75 * Number.random(),
            g: 0.25 + 0.75 * Number.random(),
            b: 0.25 + 0.75 * Number.random(),
            a: 1
        }
    )
    

            
            
            
    this.addComponent(component:shape2D);
            

            
    this.physics.setVelocityX(velocity:Number.random()*4-2);
    this.physics.installHook(
        event: 'on-collide',
        hook :::(component, source) {
            @selfPhys = this.physics;
            @otherPhys = source.physics;
            
            // collide with another ball
            if (otherPhys.getGroup() == constants.BALL_GROUP) ::<={
                //when (selfPhys.speed < otherPhys.speed) empty;

                                    
                // create a force to this and the other object
                // tangential to the center of mass and the 
                // collision location.
                @:lastCollidedPosition = {};
                selfPhys.getLastCollided(where : lastCollidedPosition);
                
                @:thisForce = {
                    x:lastCollidedPosition.x - this.getPosition().x,
                    y:lastCollidedPosition.y - this.getPosition().y
                }

                Topaz.Vector.normalize(v:thisForce);

                thisForce.x *= selfPhys.getSpeed();
                thisForce.y *= selfPhys.getSpeed();


                // at this point, "thisForce" is the force applied to the 
                // direction-specific collision
                // The force here is split between the colliding bodies
                @:toOther = {
                    x:thisForce.x*0.5,
                    y:thisForce.y*0.5
                }


                // skip non-bounce entities.                
                this.setPosition(value:selfPhys.getLastPosition());
                selfPhys.resetMotion();

                otherPhys.setVelocityX(velocity: otherPhys.getVelocityX() + toOther.x);
                otherPhys.setVelocityY(velocity: otherPhys.getVelocityY() + toOther.y);
                
            } else ::<={
                this.setPosition(value : {
                    x: this.getPosition().x,
                    y: selfPhys.getLastPosition().y
                });
                selfPhys.resetMotion();
                selfPhys.setVelocityY(velocity: selfPhys.getVelocityY() * -0.5);
                selfPhys.setVelocityX(velocity: selfPhys.getVelocityX() * 0.95);
                if ((selfPhys.getVelocityY())->abs < 2) selfPhys.setVelocityY(velocity:0);
            }
                        
        }
    );


    return this;
}


