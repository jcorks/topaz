Sample = {}
Topaz.Lua.import('constants.lua');
Topaz.Lua.import('physics_object.lua');
Topaz.Lua.import('floor.lua');
Topaz.Lua.import('ball.lua');



local room = Topaz.Entity.create();
local floor = Sample.createFloor();

    

Topaz.Input.addPointerListener(
    {
        onPress = function(input) 
            if (input ~= Topaz.Pointer._0) then return end;
            local ball = Sample.createBall();
            ball:setPosition(
                {
                    x= Topaz.Input.mouseX(),
                    y= Topaz.Input.mouseY()
                }
            );
              
            room:attach(ball);
        end
    }        
);

room:attach(floor);
Topaz.ViewManager.getDefault():getViewport():attach(room);


