
local emitter = Topaz.ParticleEmitter2D.create();    

local a = Topaz.Automation.create();
a:addKeyframe(0,   Topaz.Automation.Function.Slow, 0);
a:addKeyframe(0.5, Topaz.Automation.Function.Slow, 0.5);
a:addKeyframe(1,   Topaz.Automation.Function.Slow, 1);
local ramp01 = a:toString();
a:clear();
a:addKeyframe(1, Topaz.Automation.Function.None, 0);
a:addKeyframe(1, Topaz.Automation.Function.None, 1);
local stable1 = a:toString();
a:clear();
a:addKeyframe(1, Topaz.Automation.Function.None, 0);
a:addKeyframe(0, Topaz.Automation.Function.None, 1);
local stable0 = a:toString();
a:clear();
a:addKeyframe(0, Topaz.Automation.Function.LINEAR,   0);
a:addKeyframe(360, Topaz.Automation.Function.LINEAR, 1);
local ramp360 = a:toString();
a:clear();
a:addKeyframe(1,   Topaz.Automation.Function.Slow, 0);
a:addKeyframe(0,   Topaz.Automation.Function.Slow, 1);
local ramp10 = a:toString();
a:clear();
a:addKeyframe(10, Topaz.Automation.Function.Slow, 0);
a:addKeyframe(5,  Topaz.Automation.Function.Slow, 0.5);
a:addKeyframe(2,  Topaz.Automation.Function.Slow, 1);
local ramp100 = a:toString();


local image = Topaz.Resources.convertAsset(
    'png', 
    Topaz.Resources.createDataAssetFromPath(
        'base.png', 
        'example'
    )
);


local particle = Topaz.Particle.create();
particle:setImage(image);

particle:setAttribute(
    Topaz.Renderer.Attribute.AlphaRule,
    Topaz.Renderer.AlphaRule.Translucent
);

particle:setAttribute(
    Topaz.Renderer.Attribute.TextureFilterHint,
    Topaz.Renderer.TextureFilterHint.Linear
);

particle:setFunction(Topaz.Particle.Property.ScaleX, ramp10);
particle:setFunction(Topaz.Particle.Property.ScaleY, ramp10);
particle:setFunction(Topaz.Particle.Property.SpeedX, stable1);
particle:setFunction(Topaz.Particle.Property.SpeedY, stable1);
particle:setFunction(Topaz.Particle.Property.ScaleMultiplier, ramp100);
particle:setFunction(Topaz.Particle.Property.Red,    ramp10);
particle:setFunction(Topaz.Particle.Property.Green,  stable1);
particle:setFunction(Topaz.Particle.Property.Blue,   stable1);
particle:setFunction(Topaz.Particle.Property.Alpha,  ramp10);


particle:setOffsetMin(Topaz.Particle.Property.SpeedX, -10);
particle:setOffsetMin(Topaz.Particle.Property.SpeedY, 0);
particle:setOffsetMin(Topaz.Particle.Property.Duration, 1);
particle:setOffsetMin(Topaz.Particle.Property.Direction, 75);
particle:setOffsetMin(Topaz.Particle.Property.Red, -0.1);

particle:setOffsetMax(Topaz.Particle.Property.SpeedX, 10);
particle:setOffsetMax(Topaz.Particle.Property.SpeedY, 10);
particle:setOffsetMax(Topaz.Particle.Property.Duration, 100);
particle:setOffsetMax(Topaz.Particle.Property.Direction, 115);
particle:setOffsetMax(Topaz.Particle.Property.Red, 0.1);



emitter:setParticle(particle);


Topaz.Input.addPointerListener(
    {
        onActive = function(input, value)
            if (input == Topaz.Pointer._0) then
                emitter:setPosition(
                    {
                        x = Topaz.Input.mouseX(),
                        y = Topaz.Input.mouseY()
                    }
                );
                emitter:emit(1);
            end
        end
    }
);
Topaz.ViewManager.getDefault():setRoot(emitter);
