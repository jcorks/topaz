@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');
@:Parameters = import(module:'parameters.mt');
@:createBlock = import(module:'block.mt');


@:TYPES = {
    LINE : 0,
    T:1,
    L:2,
    L_REVERSE:3,
    BLOCK:4
}

@:getColor = ::<={
    @:COLORS = [
        Topaz.Color.fromString(str:'pink'),
        Topaz.Color.fromString(str:'yellow'),
        Topaz.Color.fromString(str:'chartreuse'),
        Topaz.Color.fromString(str:'cyan'),
        Topaz.Color.fromString(str:'violet')
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
    
    @:shape = Topaz.Shape2D.create();
    shape.formRectangle(
        width: Parameters.BLOCK_SIZE,
        height:Parameters.BLOCK_SIZE
    );
    shape.setPosition(
        value : {
            x: x*Parameters.BLOCK_SIZE,
            y: y*Parameters.BLOCK_SIZE
        }
    );
    position.x = shape.getPosition().x;
    position.y = shape.getPosition().y;

    return shape;
}


@:LIMIT_X0 = (Parameters.BLOCK_SIZE * Parameters.FIELD_WIDTH * (-0.5))->floor;
@:LIMIT_X1 = LIMIT_X0 + Parameters.FIELD_WIDTH*Parameters.BLOCK_SIZE;
@:LIMIT_Y0 = (Parameters.BLOCK_SIZE * Parameters.FIELD_HEIGHT * (-0.5))->floor;
@:LIMIT_Y1 = LIMIT_Y0 + Parameters.FIELD_WIDTH*Parameters.BLOCK_SIZE;


@:createPiece ::{
    @:this = Topaz.Entity.create(
        attributes : {
            onStep :: {
                fall += Parameters.FALL_RATE;
                if (fall > 100) ::<={
                    fall = 0;

                    if (!checkBlocked(
                            offsetX:this.getPosition().x,
                            offsetY:this.getPosition().y
                    )) ::<= {
                        @:p = this.getPosition();
                        p.y -= Parameters.BLOCK_SIZE;
                        this.setPosition(value:p);
                        resetShadow();
                    } else 
                        blockize();
                }
            }            
        }
    );


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
        @y = this.getPosition().y;
        @offset = 0;

        {:::} {
            forever ::{
                when(checkBlocked(offsetX:this.getPosition().x, offsetY:y)) send();
                y -= Parameters.BLOCK_SIZE;
                offset += 1;
            }
        }
        
        Parameters.Manager.setShadow(
            position: {
                x: this.getPosition().x,
                y: y
            },
            
            layout: layout
        );
    }
    
    @:containLeft :: {
        @x = this.getPosition().x;
        foreach(anchors)::(index, pos) {
            if (x+pos.x < LIMIT_X0) ::<={
                @:p = this.getPosition();
                p.x += Parameters.BLOCK_SIZE;
                this.setPosition(value:p);
                x = p.x;                        
            }
        }
    }
    
    @:containRight :: {
        @x = this.getPosition().x;
        foreach(anchors)::(index, pos) {
            if (x+pos.x >= LIMIT_X1) ::<={
                @:p = this.getPosition();
                p.x -= Parameters.BLOCK_SIZE;
                this.setPosition(value:p);
                x = p.x;                        
            }
        }
    }
    

    @:resetPiece :: {
        foreach(this.getComponents())::(k, v) {
            v.destroy();
        }
        anchors = [];   

        @colliderIter = 0;
        foreach(layout)::(index, block) {
            @:pos = {}
            @:visual = layoutToShape(block:block, position:pos);
            visual.setColor(color);
            this.addComponent(component: visual);
            anchors->push(value:pos);
        }
    }

    @:blockize :: { // replace with snapped blocks
        fall = 0; // dont trigger same-step
        @:x = Parameters.snapToBlock(pos:this.getPosition().x);
        @:y = Parameters.snapToBlock(pos:this.getPosition().y);
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
    
    

    this. = {
        rotate:: {
            foreach(layout)::(index, block) {
                @angle = Topaz.Vector.pointAt2D(
                    p0 : {x:0, y:0},
                    p1 : {x:block[0], y:block[1]}
                );
                @length = Topaz.Vector.getLength(v:{
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
                    when(checkBlocked(offsetX:this.getPosition().x, offsetY:this.getPosition().y)) ::<={
                        blockize();
                        send();
                    }
                    @:p = this.getPosition();
                    p.y -= Parameters.BLOCK_SIZE;
                    this.setPosition(value:p); 
                }
            }
            
        },
        moveLeft :: {
            @:p = this.getPosition();
            p.x -= Parameters.BLOCK_SIZE;
            this.setPosition(value:p);
            containLeft();
            resetShadow();
        },
        
        moveRight :: {
            @:p = this.getPosition();
            p.x += Parameters.BLOCK_SIZE;
            this.setPosition(value:p);
            containRight();          
            resetShadow();
        },
        
        resetShadow :: {
            resetShadow();
        }
    }
    resetPiece();
    resetShadow();            

    
    // makes the object into functions / setters|getters only.
    this->setIsInterface(enabled:true);

    return this;
}
return createPiece;
