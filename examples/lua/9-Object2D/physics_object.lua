
Sample.createPhysicsObject = function(static)
    local attributes = {};
    local o2d = Topaz.Object2D.create();
    local this;
    if (static) then
        o2d:setGroup(Sample.Constants.FLOOR_GROUP);
        
    else 
        o2d:setGroup(Sample.Constants.FLOOR_FLOOR);

        -- be affected by gravity
        attributes.onStep = function()
            o2d:setVelocityY(
                o2d:getVelocityY() - Sample.Constants.GRAVITY
            );
            
            -- remove if out of bounds
            local display = Topaz.ViewManager.getDefault();
            local height = display:getParameter(Topaz.Display.Parameter.Height);
            
            if (this:getPosition().y > height+Sample.Constants.BALL_SIZE) then
                this:remove();
            end
        end
    end            



    this = Topaz.Entity.create(attributes);    
    this:addComponent(o2d);    
    this.physics = o2d;
    return this;
end

