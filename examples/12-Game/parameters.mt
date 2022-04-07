@:Topaz = import(module:'Topaz');


@:Parameters = {
    BLOCK_SIZE   : 24,
    FIELD_WIDTH  : 7,
    FIELD_HEIGHT : 17,


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


Topaz.Object2D.setGroupInteraction(
    thisGroup: Parameters.PIECE_GROUP,
    thisGroup: Parameters.PIECE_GROUP,
    interact:false
);

Topaz.Object2D.setGroupInteraction(
    thisGroup: Parameters.PIECE_GROUP,
    otherGroup: Parameters.BLOCK_GROUP,
    interact:true
);

Topaz.Object2D.setGroupInteraction(
    thisGroup: Parameters.BLOCK_GROUP,
    otherGroup: Parameters.BLOCK_GROUP,
    interact: false
);

return Parameters;
