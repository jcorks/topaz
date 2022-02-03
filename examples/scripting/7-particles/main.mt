@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');
@:Emitter = class(
    inherits : [Topaz.Entity],
    define:::(this) {
        @:emitter = Topaz.ParticleEmitter2D.new();    

        @a = Topaz.Automation.new();
        a.addKeyframe(value:0,   function:Topaz.Automation.FUNCTION.SQUARE_ROOT, offset:0);
        a.addKeyframe(value:0.5, function:Topaz.Automation.FUNCTION.SQUARE_ROOT, offset:0.5);
        a.addKeyframe(value:1,   function:Topaz.Automation.FUNCTION.SQUARE_ROOT, offset:1);
        @ramp01 = a.string;
        a.clear();
        a.addKeyframe(value:1, function:Topaz.Automation.FUNCTION.NONE, offset:0);
        a.addKeyframe(value:1, function:Topaz.Automation.FUNCTION.NONE, offset:1);
        @stable1 = a.string;
        a.clear();
        a.addKeyframe(value:0, function:Topaz.Automation.FUNCTION.LINEAR,   offset:0);
        a.addKeyframe(value:360, function:Topaz.Automation.FUNCTION.LINEAR, offset:1);
        @ramp360 = a.string;
        a.clear();
        a.addKeyframe(value:1,   function:Topaz.Automation.FUNCTION.SQUARE_ROOT, offset:0);
        a.addKeyframe(value:0.5, function:Topaz.Automation.FUNCTION.SQUARE_ROOT, offset:0.5);
        a.addKeyframe(value:0,   function:Topaz.Automation.FUNCTION.SQUARE_ROOT, offset:1);
        @ramp10 = a.string;
        a.clear();
        a.addKeyframe(value:10, function:Topaz.Automation.FUNCTION.SQUARE_ROOT, offset:0);
        a.addKeyframe(value:5,  function:Topaz.Automation.FUNCTION.SQUARE_ROOT, offset:0.5);
        a.addKeyframe(value:0,  function:Topaz.Automation.FUNCTION.SQUARE_ROOT, offset:1);
        @ramp100 = a.string;
        
        
        Topaz.Resources.loadAsset(extension:'png', path:'base.png', name:'example');


        @particle = Topaz.Particle.new();
        particle.image = 'example';

        particle.setAttribute(attribute:Topaz.RENDERER.ATTRIBUTE.ALPHA_RULE,  value:Topaz.RENDERER.ALPHA_RULE.ALLOW);
        particle.setAttribute(attribute:Topaz.RENDERER.ATTRIBUTE.ETCH_RULE,   value:Topaz.RENDERER.ETCH_RULE.NOETCHING);
        particle.setAttribute(attribute:Topaz.RENDERER.ATTRIBUTE.DEPTH_TEST, value: Topaz.RENDERER.DEPTH_TEST.NONE);
        particle.setAttribute(attribute:Topaz.RENDERER.ATTRIBUTE.TEXTURE_FILTER_HINT, value:Topaz.RENDERER.TEXTURE_FILTER_HINT.LINEAR);

        particle.setFunction(property:Topaz.Particle.PROPERTY.SCALE_X, value:ramp01);
        particle.setFunction(property:Topaz.Particle.PROPERTY.SCALE_Y, value:ramp01);
        particle.setFunction(property:Topaz.Particle.PROPERTY.SPEED_X, value:ramp100);
        particle.setFunction(property:Topaz.Particle.PROPERTY.SPEED_Y, value:ramp100);
        particle.setFunction(property:Topaz.Particle.PROPERTY.SCALE_MULTIPLIER, value:stable1);
        particle.setFunction(property:Topaz.Particle.PROPERTY.DIRECTION, value:ramp360);
        //particle.setFunction(Topaz.Particle.PROPERTY.Rotation, ramp01);
        particle.setFunction(property:Topaz.Particle.PROPERTY.RED,   value:stable1);
        particle.setFunction(property:Topaz.Particle.PROPERTY.GREEN, value:stable1);
        particle.setFunction(property:Topaz.Particle.PROPERTY.BLUE,  value:stable1);
        particle.setFunction(property:Topaz.Particle.PROPERTY.ALPHA, value:ramp10);

        //particle.setNoiseMin(Topaz.Particle.PROPERTY.SpeedX, 0.4);
        //particle.setNoiseMax(Topaz.Particle.PROPERTY.SpeedX, 5);
        //particle.setNoiseMin(Topaz.Particle.PROPERTY.SpeedY, 0.4);
        //particle.setNoiseMax(Topaz.Particle.PROPERTY.SpeedY, 5);

        particle.setNoiseMin(property:Topaz.Particle.PROPERTY.SCALE_MULTIPLIER, value:10);
        particle.setNoiseMax(property:Topaz.Particle.PROPERTY.SCALE_MULTIPLIER, value:14);


        particle.setNoiseMin(property:Topaz.Particle.PROPERTY.DURATION, value:1);
        particle.setNoiseMin(property:Topaz.Particle.PROPERTY.DURATION, value:100);

        particle.setNoiseMin(property:Topaz.Particle.PROPERTY.DIRECTION, value:0);
        particle.setNoiseMin(property:Topaz.Particle.PROPERTY.DIRECTION, value:356);



        particle.setNoiseMin(property:Topaz.Particle.PROPERTY.RED, value:0);
        particle.setNoiseMax(property:Topaz.Particle.PROPERTY.RED, value:1);



        // FIXME
        emitter.particle = particle;
        
        this.attach(entity:emitter);

        this.onStep = :: {
            emitter.emit(amount:1);
        };
    }
);


@v = Emitter.new();
v.position = {x:200, y:200};
Topaz.defaultDisplay.root = v;
