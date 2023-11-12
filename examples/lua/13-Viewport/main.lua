

local createScene = function()
    local font = Topaz.Resources.createDataAssetFromPath(
        "PublicSans-Regular.ttf",
        "MyFont"
    );
    Topaz.FontManager.registerFont(font);



    local bg    = Topaz.Shape2D.create();
    local shape = Topaz.Shape2D.create();
    local text  = Topaz.Text2D.create();
    
    local scene 
    local delta = 0;
    scene = Topaz.Entity.create(
        {
            onStep = function() 
                delta = delta + Topaz.getDeltaTime() / 5000;
                shape:setRotation(
                    {
                        x= delta,
                        y= delta,
                        z= delta,
                    }
                )
            end
        }
    );

    bg:formRectangle(512, 512);
    bg:setCenter({x=256, y=256});
    bg:setColor(Topaz.Color.fromString('#202020'));
    
    shape:formRectangle(50, 50);
    shape:setCenter({x=25, y=25});
    shape:setColor(Topaz.Color.fromString('purple'));
    
    text:setPosition({x=-20, y=-20});
    text:setText("Hello!");
    text:setFont(font, 40);
    
    scene:addComponent(bg);
    scene:addComponent(shape);    
    scene:addComponent(text);
    return scene
end



local createViewport = function()
    local viewport = Topaz.Viewport.create();
    viewport:resize(256, 256);
    
    return viewport;
end


local createOutput = function() 
    
    local v = Topaz.Shape2D.create();

    local obj;
    local delta = 0;
    obj = Topaz.Entity.create(
        {
            onStep = function()
                delta = delta + 0.2;
                obj:setRotation({
                    x = delta * 0.7,
                    y = delta,
                    z = delta * 1.2
                })
            end
        }
       
    )
    
    v:setCenter({x = 150, y = 100});
    
    obj:addComponent(v);
    obj.setViewport = function(viewport) 
        v:formImageScaled(viewport:getImage(), 300, 200); 
    end
    return obj;
end


local scene = createScene();
local viewport = createViewport();

viewport:attach(scene);


local output = createOutput();
output.setViewport(viewport);
--output:setPosition({x=0, y=0, z=-4})

local displayViewport = Topaz.ViewManager.getDefault():getViewport()

displayViewport:attach(viewport);
displayViewport:attach(output);


