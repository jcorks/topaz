@:Topaz = import(module:'Topaz');


@:Parameters = {
    BLOCK_SIZE   : 24,
    FIELD_WIDTH  : 10,
    FIELD_HEIGHT : 20,


    PIECE_GROUP : Topaz.Object2D.GROUP.A,
    BLOCK_GROUP : Topaz.Object2D.GROUP.B,

    CurrentPiece : empty,
    Manager : empty

};

// function that snaps a position to 
// the nearest block position
Parameters.snapToBlock = ::(pos) {
    return Number.round(value:(pos / Parameters.BLOCK_SIZE)) * Parameters.BLOCK_SIZE;
};

Parameters.posToIndex = ::(pos) {
    return Number.round(value:(pos / Parameters.BLOCK_SIZE));
};

Parameters.BLOCK_IMAGE = Topaz.Resources.convertAsset(
    asset: Topaz.Resources.createAsset(path:'piece.png'),
    extension: 'png'
);








return Parameters;
