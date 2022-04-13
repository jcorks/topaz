@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');
@:Piece = import(module:'piece.mt');
@:Block = import(module:'block.mt');
@:Parameters = import(module:'parameters.mt');




@:checkMoveBlock ::(piece, input) {
    if (input == Topaz.Input.KEY.LEFT)
        piece.moveLeft();

    if (input == Topaz.Input.KEY.RIGHT)
        piece.moveRight();

    
    if (input == Topaz.Input.KEY.UP)
        piece.rotate();

    if (input == Topaz.Input.KEY.SPACE)
        piece.hardDrop();


};


return class(
    inherits : [Topaz.Entity],
    define:::(this) {
        @:blocks = [];
        this.position.y = -Topaz.defaultDisplay.height / 2;
        @:shadow = Topaz.Entity.new();
        @:shadow_shapes = [
            Topaz.Shape2D.new(),
            Topaz.Shape2D.new(),
            Topaz.Shape2D.new(),
            Topaz.Shape2D.new()
        ];
        shadow.components = shadow_shapes;
        for(in:[0, 4], do:::(i) {
            shadow_shapes[i].formRectangle(
                width :Parameters.BLOCK_SIZE,
                height:Parameters.BLOCK_SIZE
            );
            shadow_shapes[i].color = {
                r: 1, g: 1, b: 1,
                a: 0.2
            };
        });
        this.attach(entity:shadow);
	
        this.onStep = ::{
            @currentPiece = Parameters.CurrentPiece;
            if (currentPiece == empty) ::<= {
                this.checkCompleteLines();
                currentPiece = Piece.new();
                Parameters.CurrentPiece = currentPiece;
                currentPiece.position.y = Parameters.FIELD_HEIGHT*Parameters.BLOCK_SIZE;
                currentPiece.resetShadow();
                this.attach(entity:currentPiece);
            };
        };
        Topaz.Input.addKeyboardListener(onPress: ::(input) {
            when(Parameters.CurrentPiece == empty) empty;
            checkMoveBlock(piece:Parameters.CurrentPiece, input:input);
        }, onActive:::(input, value) {
            if (input == Topaz.Input.KEY.DOWN)
                Parameters.CurrentPiece.fastFall();      
        });

        // erases the row index
        @:eraseRow :: (row){
            @:toRemove = [];
            foreach(in:blocks, do:::(k, block) {
                @:index = block.indexY;
                when(index != row) empty;
                
                Object.push(object:toRemove, value:block);
            });

            foreach(in:toRemove, do:::(k, block) {
                block.remove();
            });
            
            foreach(in:blocks, do:::(k, block) {
                if (block.indexY > row)
                    block.moveDown();
            });            
        };

        this.interface = {
            // checks if there are any full lines
            checkCompleteLines ::{
                @:table = {};
                foreach(in:blocks, do:::(k, block) {
                    @:index = block.indexY;
                    @:r = table[index];
                    if (r == empty)
                        table[index]=1
                    else 
                        table[index]+=1
                    ;
                    

                });
                
                // for each full line, erase
                loop(func:::{
                    return listen(to:::{
                        foreach(in:table, do:::(row, count) {
                            Topaz.Console.print(message: ''+row + ':' + count);
                            if (count >= Parameters.FIELD_WIDTH) ::<= {
                                eraseRow(row:row);
                                Object.removeKey(from:table, key:row);
                                send(message:true);
                            };
                        });
                        return false;
                    });
                });
            },

            addBlock::(color, x, y) {
                @:block = Block.new(
                    color: color,
                    x: x,
                    y: y
                );
                
                block.onRemove = ::{
                    @:index = Object.findIndex(object:blocks, value:block);
                    Object.removeKey(from:blocks, key:index);
                };
                Object.push(object:blocks, value:block);

                this.attach(entity:block);

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
                for(in:[0, 4], do:::(i) {
                    @:x = layout[i][0];
                    @:y = layout[i][1];
                    shadow_shapes[i].position = {
                        x: x*Parameters.BLOCK_SIZE,
                        y: y*Parameters.BLOCK_SIZE
                    };
                });   
                
                shadow.position = position;     
            }
        };
    }
);
