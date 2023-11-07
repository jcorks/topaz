--Topaz.import('physics.lua');


Sample.createBall = function()
    local this = Sample.createPhysicsObject(false);

    local shape2D = Topaz.Shape2D.create();

    this.physics:setColliderRadial(Sample.Constants.BALL_SIZE/2, 10);

                    
    shape2D:setScale(
        {
            x=Sample.Constants.BALL_SIZE / Sample.Constants.BALL_IMAGE:getWidth(),
            y=Sample.Constants.BALL_SIZE / Sample.Constants.BALL_IMAGE:getHeight()
        }
    )
    

    shape2D:formImage(Sample.Constants.BALL_IMAGE);
    shape2D:setCenter(
        {
            x = Sample.Constants.BALL_IMAGE:getWidth()/2,
            y = Sample.Constants.BALL_IMAGE:getHeight()/2
        }
    );
    
    shape2D:setColor(
        {
            r= (0.25 + 0.75 * math.random()),
            g= (0.25 + 0.75 * math.random()),
            b= (0.25 + 0.75 * math.random()),
            a= 255
        }
    )
    

            
            
            
    this:addComponent(shape2D);
            

            
    this.physics:setVelocityX(math.random()*4-2);
    this.physics:installHook(
        'on-collide',
        function(component, source)
            local selfPhys = this.physics;
            local otherPhys = source.physics;
            
            -- collide with another ball
            if (otherPhys:getGroup() == Sample.Constants.BALL_GROUP) then
                --when (selfPhys.speed < otherPhys.speed) empty;

                                    
                -- create a force to this and the other object
                -- tangential to the center of mass and the 
                -- collision location.
                local lastCollidedPosition = {};
                selfPhys:getLastCollided(lastCollidedPosition);
                
                local thisForce = {
                    x=lastCollidedPosition.x - this:getPosition().x,
                    y=lastCollidedPosition.y - this:getPosition().y
                }

                Topaz.Vector.normalize(thisForce);

                thisForce.x = thisForce.x * selfPhys:getSpeed();
                thisForce.y = thisForce.y * selfPhys:getSpeed();


                -- at this point, "thisForce" is the force applied to the 
                -- direction-specific collision
                -- The force here is split between the colliding bodies
                local toOther = {
                    x=thisForce.x*0.5,
                    y=thisForce.y*0.5
                }


                -- skip non-bounce entities.                
                this:setPosition(selfPhys:getLastPosition());
                selfPhys:resetMotion();

                otherPhys:setVelocityX(otherPhys:getVelocityX() + toOther.x);
                otherPhys:setVelocityY(otherPhys:getVelocityY() + toOther.y);
                
            else 
                this:setPosition({
                    x= this:getPosition().x,
                    y= selfPhys:getLastPosition().y
                });
                selfPhys:resetMotion();
                selfPhys:setVelocityY(selfPhys:getVelocityY() * -0.5);
                selfPhys:setVelocityX(selfPhys:getVelocityX() * 0.95);
                if (math.abs(selfPhys:getVelocityY()) < 2) then 
                    selfPhys:setVelocityY(0);
                end
            end
                        
        end
    );


    return this;
end


