


local font = Topaz.Resources.createDataAssetFromPath(
    "PublicSans-Regular.ttf",
    "MyFont"
);
Topaz.FontManager.registerFont(font);



local createStringVisual = function()
    local this = Topaz.Entity.create({});
    
    local visual = Topaz.Text2D.create();    
    local bg = Topaz.Shape2D.create();

    visual:setFont(font, 40);

    bg:setColor(Topaz.Color.fromString('purple'));
    this:addComponent(bg);
    this:addComponent(visual);


    this.update = function(text) 
        visual:setText(text);
        bg:formRectangle(
            visual:getExtentWidth(),
            visual:getExtentHeight()
        );
    end
    
    return this;
end





local ent = createStringVisual();
ent.update('Hello world!\nThis is text.');



Topaz.ViewManager.getDefault():getViewport():attach(ent);


