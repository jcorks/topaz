
Game.createBlock = function(onCleanup)
    local this 
    
    this = Topaz.Entity.create({});

    local shape = Topaz.Shape2D.create();
    local index_x;    
    local index_y;    

    this:addComponent(shape);

    shape:formImageScaled(
        Game.Parameters.BLOCK_IMAGE,
        Game.Parameters.BLOCK_SIZE,
        Game.Parameters.BLOCK_SIZE
    );

        
    this.setup = function(x, y, color)
        this:setPosition({x=x, y=y});
        index_x = Game.Parameters.posToIndex(x);
        index_y = Game.Parameters.posToIndex(y);            
        shape:setColor(color);
    end
    
    this.indexX = function()
        return index_x;
    end

    this.indexY = function()
        return index_y;
    end
    this.moveDown = function()
        local p = this:getPosition();
        p.y = p.y - Game.Parameters.BLOCK_SIZE;
        this:setPosition(p);
        index_y=index_y-1;
    end
    
    return this;
end


