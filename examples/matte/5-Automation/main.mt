@:Topaz  = import(module:'Topaz');

@:createAnimatedRectangle = ::{

    @:this = Topaz.Entity.create(
        attributes : {
            onStep :: {
                @:p = this.getPosition();
                p.x = a.current();
                this.setPosition(value:p);
            }
        }
    );

    @:v = Topaz.Shape2D.create();
    @:a = Topaz.Automation.create();
        
        
    v.formRectangle(width:30, height:30);
    v.setCenter(value: {x:15, y:15});
    a.installHook(event:'on-anim-end', hook:::(component, source) {
        Topaz.Console.print(message:'Animation reset!');
    });
    a.setDurationSeconds(seconds:4);
    a.setLooped(loop:true);


    Topaz.Input.addKeyboardListener(
        listener : {
            onPress :::(input) {
                match(input) {
                  (Topaz.Key.space):::<={
                    a.setSpeed(speed:a.getSpeed() + 0.25);
                  },

                  (Topaz.Key.backspace):::<={
                    @:speed = a.getSpeed();
                    if (speed > 0) ::<={
                        a.setSpeed(speed:speed - 0.25);
                    } else ::<={
                        a.setSpeed(speed:0);
                    }
                  }
                }
            }
        }
    );

    this.addComponent(component:a);
    this.addComponent(component:v);

    this.anim = a;
    this.visual = v;
    
    return this;
}


@: createRectangleLinear = ::{
    @:this = createAnimatedRectangle();
    this.visual.setColor(color:Topaz.Color.fromString(str:'purple'));
    this.anim.addKeyframe(value:0,   function:Topaz.Automation.Function.None,   offset:0);
    this.anim.addKeyframe(value:200, function:Topaz.Automation.Function.Linear, offset:0.5);
    this.anim.addKeyframe(value:0,   function:Topaz.Automation.Function.Linear, offset:1);

    this.setPosition(value: {x:0, y:-100});
    return this;
}

@:createRectangleAccel :: {
    @:this = createAnimatedRectangle();
    this.visual.setColor(color:Topaz.Color.fromString(str:'blue'));
    this.anim.addKeyframe(value:0,   function:Topaz.Automation.Function.None,  offset:0);
    this.anim.addKeyframe(value:200, function:Topaz.Automation.Function.Accel, offset:0.5);
    this.anim.addKeyframe(value:0,   function:Topaz.Automation.Function.Accel, offset:1);

    this.setPosition(value: {x:0, y:0});
    return this;
}

@:createRectangleSlow = ::{
    @:this = createAnimatedRectangle();
    this.visual.setColor(color:Topaz.Color.fromString(str:'red'));
    this.anim.addKeyframe(value:0,   function:Topaz.Automation.Function.None, offset:0);
    this.anim.addKeyframe(value:200, function:Topaz.Automation.Function.Slow, offset:0.5);
    this.anim.addKeyframe(value:0,   function:Topaz.Automation.Function.Slow, offset:1);


    this.setPosition(value: {x:0, y:100});
    return this;
}



@:root = Topaz.Entity.create();
root.setPosition(value: {
    x:-200
});
root.attach(child:createRectangleLinear());
root.attach(child:createRectangleAccel());
root.attach(child:createRectangleSlow());

Topaz.ViewManager.getDefault().getViewport().attach(child:root);

