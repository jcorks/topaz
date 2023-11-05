@:Topaz = import(module:'Topaz');
@:createPiece = import(module:'piece.mt');
@:createBlock = import(module:'block.mt');
@:Parameters = import(module:'parameters.mt');




@:checkMoveBlock ::(piece, input) {
    if (input == Topaz.Key.left)
        piece.moveLeft();

    if (input == Topaz.Key.right)
        piece.moveRight();

    
    if (input == Topaz.Key.up)
        piece.rotate();

    if (input == Topaz.Key.space)
        piece.hardDrop();


}


@:getDisplayHeight :: {
    @:display = Topaz.ViewManager.getDefault();
    return display.getParameter(param:Topaz.Display.Parameter.Height);
}

return ::{
    @:this = Topaz.Entity.create(
        attributes : {
            onStep :: {
                @currentPiece = Parameters.CurrentPiece;
                if (currentPiece == empty) ::<= {
                    this.checkCompleteLines();
                    currentPiece = createPiece();
                    Parameters.CurrentPiece = currentPiece;
                    @:p = currentPiece.getPosition();
                    p.y = Parameters.FIELD_HEIGHT*Parameters.BLOCK_SIZE;
                    currentPiece.setPosition(value:p); 
                    currentPiece.resetShadow();
                    this.attach(child:currentPiece);
                }        
            }
        }
    );

    @:blocks = [];
    @:shadow = Topaz.Entity.create();
    @:shadow_shapes = [
        Topaz.Shape2D.create(),
        Topaz.Shape2D.create(),
        Topaz.Shape2D.create(),
        Topaz.Shape2D.create()
    ];

    // erases the row index
    @:eraseRow :: (row){
        @:toRemove = [];
        foreach(blocks)::(k, block) {
            @:index = block.indexY;
            when(index != row) empty;
            
            toRemove->push(value:block);
        }

        foreach(toRemove)::(k, block) {
            block.remove();
        }
        
        foreach(blocks)::(k, block) {
            if (block.indexY > row)
                block.moveDown();
        }
    }


    foreach(shadow_shapes) ::(k, shape) {
        shadow.addComponent(component:shape);
    }

    @:p = this.getPosition();
    p.y = -getDisplayHeight() / 2;
    this.setPosition(value:p);

    for(0, 4)::(i) {
        shadow_shapes[i].formRectangle(
            width :Parameters.BLOCK_SIZE,
            height:Parameters.BLOCK_SIZE
        );
        shadow_shapes[i].setColor(
            color : {
                r: 255,
                g: 255,
                b: 255,
                a: 50
            }
        )
    }
    
    
    this.attach(child:shadow);
	    
    Topaz.Input.addKeyboardListener(
        listener : {
            onPress: ::(input) {
                when(Parameters.CurrentPiece == empty) empty;
                checkMoveBlock(piece:Parameters.CurrentPiece, input:input);
            }, 
            
            onActive:::(input, value) {
                if (input == Topaz.Key.down)
                    Parameters.CurrentPiece.fastFall();      
            }
        }
    );



    this. = {
        // checks if there are any full lines
        checkCompleteLines ::{
            @:table = {}
            foreach(blocks)::(k, block) {
                @:index = block.indexY;
                @:r = table[index];
                if (r == empty)
                    table[index]=1
                else 
                    table[index]+=1
                ;
            }
           
            // for each full line, erase
            {:::} {
                forever ::{
                    when(!{:::}{
                        foreach(table)::(row, count) {
                            Topaz.Console.print(message: ''+row + ':' + count);
                            if (count >= Parameters.FIELD_WIDTH) ::<= {
                                eraseRow(row:row);
                                table->remove(key:row);
                                send(message:true);
                            }
                        }
                        return false;
                    })
                        send();
                }
            }
        },

        addBlock::(color, x, y) {
            @:block = createBlock(
                onCleanup ::{
                    @:index = blocks->findIndex(value:block);
                    blocks->remove(key:index);
                }
            );
            block.setup(
                color: color,
                x: x,
                y: y
            );
            
            blocks->push(value:block);

            this.attach(child:block);

        },
        
        blocks : {
            get::{
                return blocks;                
            }
        },
        
        setShadow::(
            position,
            layout
        ) {
            for(0, 4)::(i) {
                @:x = layout[i][0];
                @:y = layout[i][1];
                shadow_shapes[i].setPosition(value: {
                    x: x*Parameters.BLOCK_SIZE,
                    y: y*Parameters.BLOCK_SIZE
                });
            }
            
            shadow.setPosition(value:position);     
        }
    }
    
    this->setIsInterface(enabled: true);
    return this;
}
