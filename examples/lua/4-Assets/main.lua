

local asset = Topaz.Resources.createDataAssetFromPath(
    'base.png',
    'Example_Image'
);

asset = Topaz.Resources.convertAsset(
    'png',
    asset
);


local createEffect = function()

    local visual = Topaz.Shape2D.create();

    local wiggleValue = function(rate)
        return 0.7+0.3*math.sin(Topaz.getTime()*0.001*rate);
    end


    local this = Topaz.Entity.create(
        {
            onStep = function()
                visual:setColor(
                    {
                        r = wiggleValue(0.2),
                        g = wiggleValue(0.5),
                        b = wiggleValue(0.3),
                        a =1             
                    }
                );
                visual:setScale(
                    {
                        x = wiggleValue(0.8),
                        y = wiggleValue(1),      
                        z = wiggleValue(1.3)       
                    }    
                );
            end
        }
    );

    visual:formImage(asset);
    visual:setCenter({x=asset:getWidth()/2, y=asset:getHeight()/2});

    this:addComponent(visual);
    
    return this;
end


local entity = createEffect();


local display = Topaz.ViewManager.getDefault();

local reposition = function()
    local width  = display:getParameter(Topaz.Display.Parameter.Width);
    local height = display:getParameter(Topaz.Display.Parameter.Height);

    entity:setScale({
        x=(width/asset:getWidth()),
        y=(height/asset:getHeight())
    })
end

display:addParameterCallback(
    function(display, parameter)
        if (parameter == Topaz.Display.Parameter.Width or
            parameter == Topaz.Display.Parameter.Height) then 
            reposition();
        end
    end
);

reposition();

display:setRoot(entity);
