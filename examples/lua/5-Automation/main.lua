
local createAnimatedRectangle = function()

    local this;

    local v = Topaz.Shape2D.create();
    local a = Topaz.Automation.create();
    
    this = Topaz.Entity.create(
        {
            onStep = function()
                local p = this:getPosition();
                p.x = a:current();
                this:setPosition(p);
            end
        }
    );

        
        
    v:formRectangle(30, 30);
    v:setCenter({x=15, y=15});
    a:installHook(
        'on-anim-end', 
        function(component, source) 
            Topaz.Console.print('Animation reset!');
        end
    );
    a:setDurationSeconds(4);
    a:setLooped(true);


    Topaz.Input.addKeyboardListener(
        {
            onPress = function(input)
                if (input == Topaz.Key.space) then
                    a:setSpeed(a:getSpeed() + 0.25);


                elseif (input == Topaz.Key.backspace) then
                    local speed = a:getSpeed();
                    if (speed > 0) then
                        a:setSpeed(speed - 0.25);
                    else
                        a:setSpeed(0);
                    end
                end
            end
        }
    );

    this:addComponent(a);
    this:addComponent(v);

    this.anim = a;
    this.visual = v;
    
    return this;
end


local  createRectangleLinear = function()
    local this = createAnimatedRectangle();
    this.visual:setColor(Topaz.Color.fromString('purple'));
    this.anim:addKeyframe(0,   Topaz.Automation.Function.None,   0);
    this.anim:addKeyframe(200, Topaz.Automation.Function.Linear, 0.5);
    this.anim:addKeyframe(0,   Topaz.Automation.Function.Linear, 1);

    this:setPosition({x=0, y=-100});
    return this;
end

local createRectangleAccel = function()
    local this = createAnimatedRectangle();
    this.visual:setColor(Topaz.Color.fromString('blue'));
    this.anim:addKeyframe(0,   Topaz.Automation.Function.None,  0);
    this.anim:addKeyframe(200, Topaz.Automation.Function.Accel, 0.5);
    this.anim:addKeyframe(0,   Topaz.Automation.Function.Accel, 1);

    this:setPosition({x=0, y=0});
    return this;
end

local createRectangleSlow = function()
    local this = createAnimatedRectangle();
    this.visual:setColor(Topaz.Color.fromString('red'));
    this.anim:addKeyframe(0,   Topaz.Automation.Function.None, 0);
    this.anim:addKeyframe(200, Topaz.Automation.Function.Slow, 0.5);
    this.anim:addKeyframe(0,   Topaz.Automation.Function.Slow, 1);


    this:setPosition({x=0, y=100});
    return this;
end



local root = Topaz.Entity.create();
root:setPosition({
    x=-200
});
root:attach(createRectangleLinear());
root:attach(createRectangleAccel());
root:attach(createRectangleSlow());

Topaz.ViewManager.getDefault():setRoot(root);

