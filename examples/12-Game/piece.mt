@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');
@:Parameters = import(module:'parameters.mt');
@:Block = import(module:'block.mt');


@:TYPES = {
    LINE : 0,
    T:1,
    L:2,
    L_REVERSE:3,
    BLOCK:4
};

@:getColor = ::<={
    @:COLORS = [
        Topaz.Color.parse(string:'pink'),
        Topaz.Color.parse(string:'yellow'),
        Topaz.Color.parse(string:'chartreuse'),
        Topaz.Color.parse(string:'cyan'),
        Topaz.Color.parse(string:'violet')
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
        ],
       (TYPES.BLOCK):
        // BLOCK
        [
            [0, 0],
            [0, 1],
            [1, 0],
            [1, 1]
        ]
    };
};


// takes a shape layout and converts it into a visual
@:layoutToShape::(block, position) {
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
    position.x = shape.position.x;
    position.y = shape.position.y;

    return shape;
};


@:LIMIT_X0 = Number.floor(of:Parameters.BLOCK_SIZE * Parameters.FIELD_WIDTH * (-0.5));
@:LIMIT_X1 = LIMIT_X0 + Parameters.FIELD_WIDTH*Parameters.BLOCK_SIZE;
@:LIMIT_Y0 = Number.floor(of:Parameters.BLOCK_SIZE * Parameters.FIELD_HEIGHT * (-0.5));
@:LIMIT_Y1 = LIMIT_Y0 + Parameters.FIELD_WIDTH*Parameters.BLOCK_SIZE;


return class(
    inherits: [Topaz.Entity],
    statics: {
        FALL_RATE : 3
    },
    define:::(this) {
        @:type = Number.floor(of:Number.random()*5);
        @:layout = getLayout(type:type);
        @:color = getColor(type:type);  
        @anchors = [];  
        
        @:checkBlocked ::(offsetX, offsetY){
            return listen(to:::{
                foreach(in:anchors, do:::(k, pos) {
                    @:x = Parameters.posToIndex(pos:pos.x + offsetX);
                    @:y = Parameters.posToIndex(pos:pos.y + offsetY);

                    foreach(in:Parameters.Manager.blocks, do:::(k, block) {
                        when(block.indexX != x) empty;
                        if (block.indexY == y - 1) ::<={
                            send(message:true);
                        };
                        
                    });
                    if (y == 0) ::<={
                        send(message:true);
                    };

                });
                return false;
            });        	
        };
        
        @:resetShadow ::{
            @y = this.position.y;
            @offset = 0;

            loop(func:::{
                when(checkBlocked(offsetX:this.position.x, offsetY:y)) false;
                y -= Parameters.BLOCK_SIZE;
                offset += 1;
                return true;
            });
            
            Parameters.Manager.setShadow(
                position: {
                    x: this.position.x,
                    y: y
                },
                
                layout: layout
            );
        };
        
        @:containLeft :: {
            @x = this.position.x;
            foreach(in:anchors, do:::(index, pos) {
                if (x+pos.x < LIMIT_X0) ::<={
                    this.position.x += Parameters.BLOCK_SIZE;
                    x = this.position.x;                        
                };
            });
        };
        
        @:containRight :: {
            @x = this.position.x;
            foreach(in:anchors, do:::(index, pos) {
                if (x+pos.x >= LIMIT_X1) ::<={
                    this.position.x -= Parameters.BLOCK_SIZE;
                    x = this.position.x;                        
                };
            });        
        };
        

        @:resetPiece :: {
            foreach(in:this.components, do:::(k, v) {
                v.destroy();
            });
            anchors = [];   

            @colliderIter = 0;
            foreach(in:layout, do:::(index, block) {
                @:pos = {};
                @:visual = layoutToShape(block:block, position:pos);
                visual.color = color;
                this.attach(component: visual);
                Object.push(object:anchors, value:pos);
                
            });
        };

        @:blockize :: { // replace with snapped blocks
            fall = 0; // dont trigger same-step
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
        @fall = 0;
        this.onStep = ::{
            fall += this.class.FALL_RATE;
            if (fall > 100) ::<={
                fall = 0;

                if (!checkBlocked(
                        offsetX:this.position.x,
                        offsetY:this.position.y
                )) ::<= {
                    this.position.y -= Parameters.BLOCK_SIZE;
                    resetShadow();
                } else 
                    blockize();
            };
 
            

            

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
                containLeft();
                containRight();
                resetShadow();
            },
            fastFall :: {
                fall += 40;
            },
            hardDrop :: {
                loop(func:::{
                    when(checkBlocked(offsetX:this.position.x, offsetY:this.position.y)) ::<={
                        blockize();
                        return false;
                    };
                    this.position.y -= Parameters.BLOCK_SIZE;
                    return true;
                });
                
            },
            moveLeft :: {
                this.position.x -= Parameters.BLOCK_SIZE;
                containLeft();
                resetShadow();
            },
            
            moveRight :: {
                this.position.x += Parameters.BLOCK_SIZE;
                containRight();          
                resetShadow();
            },
            
            resetShadow :: {
                resetShadow();
            }
        };
        resetPiece();
        resetShadow();
    }

    
);
