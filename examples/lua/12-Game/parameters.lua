

Game.Parameters = {
    FALL_RATE = 3,

    BLOCK_SIZE   = 24,
    FIELD_WIDTH  = 10,
    FIELD_HEIGHT = 20,


    PIECE_GROUP = Topaz.Object2D.Group.A,
    BLOCK_GROUP = Topaz.Object2D.Group.B,

    CurrentPiece = nil,
    Manager = nil
};

local round = function(val)
    local frac = val - math.floor(val);
    if (frac < 0.5) then 
        return math.floor(val);
    else 
        return math.floor(val)+1;    
    end
end

-- function that snaps a position to 
-- the nearest block position
Game.Parameters.snapToBlock = function(pos)
    return round((pos / Game.Parameters.BLOCK_SIZE)) * Game.Parameters.BLOCK_SIZE;
end

Game.Parameters.posToIndex = function(pos)
    return round((pos / Game.Parameters.BLOCK_SIZE));
end

Game.Parameters.BLOCK_IMAGE = Topaz.Resources.convertAsset(
    'png',
    Topaz.Resources.createDataAssetFromPath('piece.png', 'PieceImage')
);


