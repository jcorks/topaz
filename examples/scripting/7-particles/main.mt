@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');

@:emitter = Topaz.ParticleEmitter2D.new();    

@a = Topaz.Automation.new();
a.addKeyframe(value:0,   function:Topaz.Automation.FUNCTION.SLOW, offset:0);
a.addKeyframe(value:0.5, function:Topaz.Automation.FUNCTION.SLOW, offset:0.5);
a.addKeyframe(value:1,   function:Topaz.Automation.FUNCTION.SLOW, offset:1);
@ramp01 = a.string;
a.clear();
a.addKeyframe(value:1, function:Topaz.Automation.FUNCTION.NONE, offset:0);
a.addKeyframe(value:1, function:Topaz.Automation.FUNCTION.NONE, offset:1);
@stable1 = a.string;
a.clear();
a.addKeyframe(value:1, function:Topaz.Automation.FUNCTION.NONE, offset:0);
a.addKeyframe(value:0, function:Topaz.Automation.FUNCTION.NONE, offset:1);
@stable0 = a.string;
a.clear();
a.addKeyframe(value:0, function:Topaz.Automation.FUNCTION.LINEAR,   offset:0);
a.addKeyframe(value:360, function:Topaz.Automation.FUNCTION.LINEAR, offset:1);
@ramp360 = a.string;
a.clear();
a.addKeyframe(value:1,   function:Topaz.Automation.FUNCTION.SLOW, offset:0);
a.addKeyframe(value:0,   function:Topaz.Automation.FUNCTION.SLOW, offset:1);
@ramp10 = a.string;
a.clear();
a.addKeyframe(value:10, function:Topaz.Automation.FUNCTION.SLOW, offset:0);
a.addKeyframe(value:5,  function:Topaz.Automation.FUNCTION.SLOW, offset:0.5);
a.addKeyframe(value:2,  function:Topaz.Automation.FUNCTION.SLOW, offset:1);
@ramp100 = a.string;


@:image = Topaz.Resources.loadAsset(
    extension:'png', 
    asset:Topaz.Resources.createAsset(
        path:'base.png', 
        name:'example'
    )
);


@particle = Topaz.Particle.new();
particle.image = image;

particle.setAttributes(
    alphaRule: Topaz.RENDERER.ALPHA_RULE.TRANSLUCENT,
    etchRule : Topaz.RENDERER.ETCH_RULE.NOETCHING,
    depthTest: Topaz.RENDERER.DEPTH_TEST.NONE,
    textureFilterHint:Topaz.RENDERER.TEXTURE_FILTER_HINT.LINEAR
);

particle.setFunctions(
    scaleX:ramp10,
    scaleY:ramp10,
    speedX:stable1,
    speedY:stable1,
    scaleMultiplier:ramp100,
    red:ramp10,
    green:stable1,
    blue:stable1,
    alpha:ramp10
);


particle.setOffsetMin(
    speedX:-10,
    speedY:-10,

    duration :1,
    direction :75,
    red :-0.1
);


particle.setOffsetMax(
    speedX:10,
    speedY:0,
    duration:100,
    direction:115,
    red:0.1
);



// FIXME
emitter.particle = particle;


Topaz.Input.addPointerListener(onActive:::(input, value) {
    if (input == Topaz.Input.POINTER._0) ::<={
        emitter.position = Topaz.Input.mouse;
        emitter.emit(amount:1);
    };
});
Topaz.defaultDisplay.root = emitter;
