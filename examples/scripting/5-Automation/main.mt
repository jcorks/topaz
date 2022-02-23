@:Topaz  = import(module:'Topaz');
@:class  = import(module:'Matte.Core.Class');

@: Rectangle = class(
    name : 'Rectangle',
    inherits : [Topaz.Entity],
    define:::(this){
        @:v = Topaz.Shape2D.new();
        v.formRectangle(width:30, height:30);
        v.center = {x:15, y:15};


        @:a = Topaz.Automation.new();
        a.installHook(event:'on-anim-end', callback:::(source) {
            Topaz.log(message:'Animation reset!');
        });
        a.durationSeconds = 4;
        a.looped = true;


        Topaz.Input.addKeyboardListener(
            onPress :::(input) {
                match(input) {
                  (Topaz.Input.KEY.SPACE):::<={
                    a.speed += 0.25;
                  },

                  (Topaz.Input.KEY.BACKSPACE):::<={
                    if (a.speed > 0) ::<={
                        a.speed -= 0.25;
                    } else ::<={
                        a.speed = 0;
                    };
                  }
                };
            }
        );


        this.components = [a, v];

        this.onStep = ::{
            @:r = this.position;
            this.position = {x:100+a.value, y:r.y};
        };
        this.interface = {
            anim : {
                get ::{return a;}
            },

            visual : {
                get ::{return v;}
            }
        };
    }
);


@: RectangleLinear = class(
    inherits : [Rectangle],
    define:::(this) {
        this.visual.color = Topaz.Color.parse(string:'purple');
        this.anim.addKeyframe(value:0,   function:Topaz.Automation.FUNCTION.NONE,   offset:0);
        this.anim.addKeyframe(value:200, function:Topaz.Automation.FUNCTION.LINEAR, offset:0.5);
        this.anim.addKeyframe(value:0,   function:Topaz.Automation.FUNCTION.LINEAR, offset:1);

        this.position = {x:0, y:-100};
    }
);

@:RectangleAccel = class(
    inherits : [Rectangle],
    define:::(this) {
        this.visual.color = Topaz.Color.parse(string:'blue');
        this.anim.addKeyframe(value:0,   function:Topaz.Automation.FUNCTION.NONE,  offset:0);
        this.anim.addKeyframe(value:200, function:Topaz.Automation.FUNCTION.ACCEL, offset:0.5);
        this.anim.addKeyframe(value:0,   function:Topaz.Automation.FUNCTION.ACCEL, offset:1);

        this.position = {x:0, y:0};

    }
);

@:RectangleSlow = class(
    inherits : [Rectangle],
    define:::(this) {
        this.visual.color = Topaz.Color.parse(string:'red');
        this.anim.addKeyframe(value:0,   function:Topaz.Automation.FUNCTION.NONE, offset:0);
        this.anim.addKeyframe(value:200, function:Topaz.Automation.FUNCTION.SLOW, offset:0.5);
        this.anim.addKeyframe(value:0,   function:Topaz.Automation.FUNCTION.SLOW, offset:1);


        this.position = {x:0, y:100};
    }
);



::<= {
    @:root = Topaz.Entity.new();
    root.position = {
        x:-200
    };
    root.children = [
        RectangleLinear.new(),
        RectangleAccel.new(),
        RectangleSlow.new()
    ];

    Topaz.defaultDisplay.root = root;
};
