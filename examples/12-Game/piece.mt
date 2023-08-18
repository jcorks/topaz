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
}

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
    }
}


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
    }
}


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
    }
    position.x = shape.position.x;
    position.y = shape.position.y;

    return shape;
}


@:LIMIT_X0 = (Parameters.BLOCK_SIZE * Parameters.FIELD_WIDTH * (-0.5))->floor;
@:LIMIT_X1 = LIMIT_X0 + Parameters.FIELD_WIDTH*Parameters.BLOCK_SIZE;
@:LIMIT_Y0 = (Parameters.BLOCK_SIZE * Parameters.FIELD_HEIGHT * (-0.5))->floor;
@:LIMIT_Y1 = LIMIT_Y0 + Parameters.FIELD_WIDTH*Parameters.BLOCK_SIZE;


@:Piece = class(
    inherits: [Topaz.Entity],
    statics: {
        FALL_RATE : {get::<-3}
    },
    define:::(this) {
        @:type = (Number.random()*5)->floor;
        @:layout = getLayout(type:type);
        @:color = getColor(type:type);  
        @anchors = [];  
        
        @:checkBlocked ::(offsetX, offsetY){
            return {:::}{
                foreach(anchors)::(k, pos) {
                    @:x = Parameters.posToIndex(pos:pos.x + offsetX);
                    @:y = Parameters.posToIndex(pos:pos.y + offsetY);

                    foreach(Parameters.Manager.blocks)::(k, block) {
                        when(block.indexX != x) empty;
                        if (block.indexY == y - 1) ::<={
                            send(message:true);
                        }
                        
                    }
                    if (y == 0) ::<={
                        send(message:true);
                    }

                }
                return false;
            }        	
        }
        
        @:resetShadow ::{
            @y = this.position.y;
            @offset = 0;

            {:::} {
                forever ::{
                    when(checkBlocked(offsetX:this.position.x, offsetY:y)) send();
                    y -= Parameters.BLOCK_SIZE;
                    offset += 1;
                }
            }
            
            Parameters.Manager.setShadow(
                position: {
                    x: this.position.x,
                    y: y
                },
                
                layout: layout
            );
        }
        
        @:containLeft :: {
            @x = this.position.x;
            foreach(anchors)::(index, pos) {
                if (x+pos.x < LIMIT_X0) ::<={
                    this.position.x += Parameters.BLOCK_SIZE;
                    x = this.position.x;                        
                }
            }
        }
        
        @:containRight :: {
            @x = this.position.x;
            foreach(anchors)::(index, pos) {
                if (x+pos.x >= LIMIT_X1) ::<={
                    this.position.x -= Parameters.BLOCK_SIZE;
                    x = this.position.x;                        
                }
            }
        }
        

        @:resetPiece :: {
            foreach(this.components)::(k, v) {
                v.destroy();
            }
            anchors = [];   

            @colliderIter = 0;
            foreach(layout)::(index, block) {
                @:pos = {}
                @:visual = layoutToShape(block:block, position:pos);
                visual.color = color;
                this.attach(component: visual);
                anchors->push(value:pos);
                
            }
        }

        @:blockize :: { // replace with snapped blocks
            fall = 0; // dont trigger same-step
            @:x = Parameters.snapToBlock(pos:this.position.x);
            @:y = Parameters.snapToBlock(pos:this.position.y);
            Parameters.CurrentPiece = empty;
            this.remove();

            foreach(layout)::(index, block) {
                Parameters.Manager.addBlock(
                    color: color,
                    x: x+Parameters.BLOCK_SIZE*block[0],
                    y: y+Parameters.BLOCK_SIZE*block[1]
                );
            }
        }
        @fall = 0;
        
        this.constructor = ::{
            this.onStep = ::{
                fall += Piece.FALL_RATE;
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
                }
            }
            resetPiece();
            resetShadow();            
        }
        

        this.interface = {
            rotate:: {
                foreach(layout)::(index, block) {
                    @angle = Topaz.Vector.pointAt2D(
                        from : {x:0, y:0},
                        to   : {x:block[0], y:block[1]}
                    );
                    @length = Topaz.Vector.length(of:{
                        x: block[0],
                        y: block[1]
                    });
                    angle = (angle+90)->asRadians;
                    block[0] = (angle)->cos * length;
                    block[1] = (angle)->sin * length;
                }
                resetPiece();
                containLeft();
                containRight();
                resetShadow();
            },
            fastFall :: {
                fall += 40;
            },
            hardDrop :: {
                {:::} {
                    forever ::{
                        when(checkBlocked(offsetX:this.position.x, offsetY:this.position.y)) ::<={
                            blockize();
                            send();
                        }
                        this.position.y -= Parameters.BLOCK_SIZE;
                    }
                }
                
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
        }
    }

    
);
return Piece;
