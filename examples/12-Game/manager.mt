@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');
@:Piece = import(module:'piece.mt');
@:Block = import(module:'block.mt');
@:Parameters = import(module:'parameters.mt');


@:LIMIT_X0 = Number.floor(of:Parameters.BLOCK_SIZE * Parameters.FIELD_WIDTH * (-0.5));
@:LIMIT_X1 = LIMIT_X0 + Parameters.FIELD_WIDTH*Parameters.BLOCK_SIZE;
@:LIMIT_Y0 = Number.floor(of:Parameters.BLOCK_SIZE * Parameters.FIELD_HEIGHT * (-0.5));
@:LIMIT_Y1 = LIMIT_Y0 + Parameters.FIELD_WIDTH*Parameters.BLOCK_SIZE;


@:checkMoveBlock ::(piece, input) {
    if (input == Topaz.Input.KEY.LEFT)
        if (piece.position.x > LIMIT_X0)
            piece.position.x -= Parameters.BLOCK_SIZE;

    if (input == Topaz.Input.KEY.RIGHT)
        if (piece.position.x < LIMIT_X1)
            piece.position.x += Parameters.BLOCK_SIZE;
    
    if (input == Topaz.Input.KEY.UP)
        piece.rotate();
};


return class(
    inherits : [Topaz.Entity],
    define:::(this) {
        this.position.y = -Topaz.defaultDisplay.height / 2;

	
        this.onStep = ::{
            @currentPiece = Parameters.CurrentPiece;
            if (currentPiece == empty) ::<= {
                currentPiece = Piece.new();
                Parameters.CurrentPiece = currentPiece;
                currentPiece.position.y = Parameters.FIELD_HEIGHT*Parameters.BLOCK_SIZE;
                this.attach(entity:currentPiece);
            };
        };
        Topaz.Input.addKeyboardListener(onPress: ::(input) {
            when(Parameters.CurrentPiece == empty) empty;
            checkMoveBlock(piece:Parameters.CurrentPiece, input:input);
        });

        this.interface = {
            // checks if there are any full lines
            checkCompleteLines ::{
            },

            addBlock::(color, x, y) {
                @:block = Block.new(
                    color: color,
                    x: x,
                    y: y
                );

                this.attach(entity:block);

            }
        };
    }
);
