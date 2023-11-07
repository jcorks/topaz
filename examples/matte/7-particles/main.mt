@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');

@:emitter = Topaz.ParticleEmitter2D.create();    

@a = Topaz.Automation.create();
a.addKeyframe(value:0,   function:Topaz.Automation.Function.Slow, offset:0);
a.addKeyframe(value:0.5, function:Topaz.Automation.Function.Slow, offset:0.5);
a.addKeyframe(value:1,   function:Topaz.Automation.Function.Slow, offset:1);
@ramp01 = a.toString();
a.clear();
a.addKeyframe(value:1, function:Topaz.Automation.Function.None, offset:0);
a.addKeyframe(value:1, function:Topaz.Automation.Function.None, offset:1);
@stable1 = a.toString();
a.clear();
a.addKeyframe(value:1, function:Topaz.Automation.Function.None, offset:0);
a.addKeyframe(value:0, function:Topaz.Automation.Function.None, offset:1);
@stable0 = a.toString();
a.clear();
a.addKeyframe(value:0, function:Topaz.Automation.Function.LINEAR,   offset:0);
a.addKeyframe(value:360, function:Topaz.Automation.Function.LINEAR, offset:1);
@ramp360 = a.toString();
a.clear();
a.addKeyframe(value:1,   function:Topaz.Automation.Function.Slow, offset:0);
a.addKeyframe(value:0,   function:Topaz.Automation.Function.Slow, offset:1);
@ramp10 = a.toString();
a.clear();
a.addKeyframe(value:10, function:Topaz.Automation.Function.Slow, offset:0);
a.addKeyframe(value:5,  function:Topaz.Automation.Function.Slow, offset:0.5);
a.addKeyframe(value:2,  function:Topaz.Automation.Function.Slow, offset:1);
@ramp100 = a.toString();


@:image = Topaz.Resources.convertAsset(
    fileType:'png', 
    asset:Topaz.Resources.createDataAssetFromPath(
        path:'base.png', 
        name:'example'
    )
);


@particle = Topaz.Particle.create();
particle.setImage(image);

particle.setAttribute(
    attribute:Topaz.Renderer.Attribute.AlphaRule,
    value:    Topaz.Renderer.AlphaRule.Translucent
);
Topaz.debug();

particle.setAttribute(
    attribute:Topaz.Renderer.Attribute.TextureFilterHint,
    value:    Topaz.Renderer.TextureFilterHint.Linear
);

particle.setFunction(prop:Topaz.Particle.Property.ScaleX, animString:ramp10);
particle.setFunction(prop:Topaz.Particle.Property.ScaleY, animString:ramp10);
particle.setFunction(prop:Topaz.Particle.Property.SpeedX, animString:stable1);
particle.setFunction(prop:Topaz.Particle.Property.SpeedY, animString:stable1);
particle.setFunction(prop:Topaz.Particle.Property.ScaleMultiplier, animString:ramp100);
particle.setFunction(prop:Topaz.Particle.Property.Red,   animString: ramp10);
particle.setFunction(prop:Topaz.Particle.Property.Green, animString: stable1);
particle.setFunction(prop:Topaz.Particle.Property.Blue,  animString: stable1);
particle.setFunction(prop:Topaz.Particle.Property.Alpha, animString: ramp10);


particle.setOffsetMin(prop:Topaz.Particle.Property.SpeedX, value:-10);
particle.setOffsetMin(prop:Topaz.Particle.Property.SpeedY, value:0);
particle.setOffsetMin(prop:Topaz.Particle.Property.Duration, value:1);
particle.setOffsetMin(prop:Topaz.Particle.Property.Direction, value:75);
particle.setOffsetMin(prop:Topaz.Particle.Property.Red, value:-0.1);

particle.setOffsetMax(prop:Topaz.Particle.Property.SpeedX, value:10);
particle.setOffsetMax(prop:Topaz.Particle.Property.SpeedY, value:10);
particle.setOffsetMax(prop:Topaz.Particle.Property.Duration, value:100);
particle.setOffsetMax(prop:Topaz.Particle.Property.Direction, value:115);
particle.setOffsetMax(prop:Topaz.Particle.Property.Red, value:0.1);



// FIXME
emitter.setParticle(particle);


Topaz.Input.addPointerListener(
    listener : {
        onActive:::(input, value) {
            if (input == Topaz.Pointer._0) ::<={
                emitter.setPosition(
                    value : {
                        x : Topaz.Input.mouseX(),
                        y : Topaz.Input.mouseY()
                    }
                );
                emitter.emit(count:1);
            }
        }
    }
);
Topaz.ViewManager.getDefault().setRoot(newRoot:emitter);
