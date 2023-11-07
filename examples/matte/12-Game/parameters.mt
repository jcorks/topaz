@:Topaz = import(module:'Topaz');


@:Parameters = {
    FALL_RATE : 3,

    BLOCK_SIZE   : 24,
    FIELD_WIDTH  : 10,
    FIELD_HEIGHT : 20,


    PIECE_GROUP : Topaz.Object2D.Group.A,
    BLOCK_GROUP : Topaz.Object2D.Group.B,

    CurrentPiece : empty,
    Manager : empty
};

// function that snaps a position to 
// the nearest block position
Parameters.snapToBlock = ::(pos) {
    return ((pos / Parameters.BLOCK_SIZE))->round * Parameters.BLOCK_SIZE;
};

Parameters.posToIndex = ::(pos) {
    return ((pos / Parameters.BLOCK_SIZE))->round;
};

Parameters.BLOCK_IMAGE = Topaz.Resources.convertAsset(
    asset: Topaz.Resources.createDataAssetFromPath(name:'PieceImage', path:'piece.png'),
    fileType: 'png'
);








return Parameters;
