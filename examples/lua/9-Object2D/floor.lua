

-- A floor that, when instantiated, always 
-- matches the bottom of the display
Sample.createFloor = function() 
    local this = Sample.createPhysicsObject(true);

    local display = Topaz.ViewManager.getDefault();
            
    local updateFloor = function()        
        local width  = display:getParameter(Topaz.Display.Parameter.Width);
        local height = display:getParameter(Topaz.Display.Parameter.Height);
    
        this.physics:setColliderRectangle(
            width,
            2
        );

        
        this:setPosition(
            {
                x= 0,
                y= -height/2
            }
        );
    end

    display:addParameterCallback(function(
        display,
        parameter
    ) 
        if (parameter == Topaz.Display.Parameter.Width or
            parameter == Topaz.Display.Parameter.Height) then 
                updateFloor();
        end
    end);
    
    updateFloor();
    return this;
end
