

local createRectangle = function() 
    local entity;
    
    entity = Topaz.Entity.create(
        {
            onStep = function()
                local r = entity:getRotation();
                r.x = r.x + 0.01;
                r.y = r.y + 0.2;
                r.z = r.z + 0.03;
                
                entity:setRotation(r);
            end
        }
    );
    
    local visual = Topaz.Shape2D.create();
    visual:formRectangle(200, 200);
    visual:setCenter({x=100, y=100});
    
    entity:addComponent(visual);
    
    
    entity.setColor1 = function()
        visual:setColor(Topaz.Color.fromString('purple'));    
    end

    entity.setColor2 = function()
        visual:setColor(Topaz.Color.fromString('green'));    
    end

    
    return entity;
end


local room = Topaz.Entity.create();

local r = createRectangle();
r.setColor1();
local r2 = createRectangle();
r2.setColor2();

r2:setPosition({x=100, y=40});

room:attach(r);
room:attach(r2);


local display = Topaz.ViewManager.getDefault();
display:setName("Example 1");
display:getViewport():attach(room);


