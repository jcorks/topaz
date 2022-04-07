@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');
@:Parameters = import(module:'parameters.mt');
@:Block = import(module:'block.mt');


@:TYPES = {
    LINE : 0,
    T:1,
    L:2,
    L_REVERSE:3
};

@:getColor = ::<={
    @:COLORS = [
        Topaz.Color.parse(string:'red'),
        Topaz.Color.parse(string:'yellow'),
        Topaz.Color.parse(string:'green'),
        Topaz.Color.parse(string:'blue'),
        Topaz.Color.parse(string:'purple')
    ];
    
    return ::(type){
        return COLORS[type];
    };
};


@:getLayout = ::(type) {
    return match(type) {
      (TYPES.LINE): 
        // line
        [
            [0, 0],
            [0, 1],
            [0, 2],
            [0, 3]
        ],

       (TYPES.T):
        [
            [0, 0],
            [0, 1],
            [0, 2],
            [1, 1]
        ],

       (TYPES.L):
        // L
        [
            [0, 0],
            [0, 1],
            [0, 2],
            [1, 0]
        ],

       (TYPES.L_REVERSE):
        // L -reverse
        [
            [0, 0],
            [0, 1],
            [0, 2],
            [1, 2]
        ]
    };
};


// takes a shape layout and converts it into a visual
@:layoutToShape::(block) {
    @:x = block[0];
    @:y = block[1];
    
    @:shape = Topaz.Shape2D.new();
    shape.formRectangle(
        width: Parameters.BLOCK_SIZE,
        height:Parameters.BLOCK_SIZE
    );
    shape.position = {
        x: x*Parameters.BLOCK_SIZE,
        y: y*Parameters.BLOCK_SIZE
    };

    return shape;
};

return class(
    inherits: [Topaz.Entity],
    statics: {
        FALL_RATE : -3
    },
    define:::(this) {
        @:type = Number.floor(of:Number.random()*4);
        @:layout = getLayout(type:type);
        @:color = getColor(type:type);        

        @:resetPiece :: {
            foreach(in:this.components, do:::(k, v) {
                v.destroy();
            });

            @:o2d = Topaz.Object2D.new();
            o2d.group = Parameters.PIECE_GROUP;

            @:collider = [];
            @colliderIter = 0;
            foreach(in:layout, do:::(index, block) {
                @:visual = layoutToShape(block:block);
                visual.color = color;
                this.attach(component: visual);

                collider[colliderIter] = Parameters.BLOCK_SIZE*(block[0] + 0.4);colliderIter+=1;
                collider[colliderIter] = Parameters.BLOCK_SIZE*block[1];colliderIter+=1;

                collider[colliderIter] = Parameters.BLOCK_SIZE*(block[0] + 0.6);colliderIter+=1;
                collider[colliderIter] = Parameters.BLOCK_SIZE*block[1];colliderIter+=1;
            });
            o2d.collider = collider;
            o2d.velocityY = this.class.FALL_RATE;
            o2d.installHook(event:'on-collide', callback:::(source){
                blockize();
            });
            this.attach(component:o2d);
        };

        @:blockize :: { // replace with snapped blocks
            @:x = Parameters.snapToBlock(pos:this.position.x);
            @:y = Parameters.snapToBlock(pos:this.position.y);
            Parameters.CurrentPiece = empty;
            this.remove();

            foreach(in:layout, do:::(index, block) {
                Parameters.Manager.addBlock(
                    color: color,
                    x: x+Parameters.BLOCK_SIZE*block[0],
                    y: y+Parameters.BLOCK_SIZE*block[1]
                );
            });
        };

        this.onStep = ::{
            if (this.position.y < 0)
                blockize();
        };



        this.interface = {
            rotate:: {
                foreach(in:layout, do:::(index, block) {
                    @angle = Topaz.Vector.pointAt2D(
                        from : {x:0, y:0},
                        to   : {x:block[0], y:block[1]}
                    );
                    @length = Topaz.Vector.length(of:{
                        x: block[0],
                        y: block[1]
                    });
                    angle = Number.toRadians(value:angle+90);
                    block[0] = Number.cos(of:angle) * length;
                    block[1] = Number.sin(of:angle) * length;
                });
                resetPiece();
            }
        };
        resetPiece();
    }

    
);
