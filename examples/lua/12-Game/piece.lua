

local TYPES = {
    LINE = 1,
    T=2,
    L=3,
    L_REVERSE=4,
    BLOCK=5
}

local getColor = (function()
    local COLORS = {
        Topaz.Color.fromString('pink'),
        Topaz.Color.fromString('yellow'),
        Topaz.Color.fromString('chartreuse'),
        Topaz.Color.fromString('cyan'),
        Topaz.Color.fromString('violet')
    };
    
    return function(type)
        return COLORS[type];
    end
end)()


local getLayout = function (type)
    if (type == TYPES.LINE) then
        -- line
        return {
            {0, 0},
            {0, 1},
            {0, 2},
            {0, 3}
        }
    elseif (type == TYPES.T) then 
        return {
            {0, 0},
            {0, 1},
            {0, 2},
            {1, 1}
        }

    elseif (type == TYPES.L) then 
        -- L
        return {
            {0, 0},
            {0, 1},
            {0, 2},
            {1, 0}
        }
    elseif (type == TYPES.L_REVERSE) then 
        -- L -reverse
        return {
            {0, 0},
            {0, 1},
            {0, 2},
            {1, 2}
        }
    elseif (type == TYPES.BLOCK) then 
        -- BLOCK
        return {
            {0, 0},
            {0, 1},
            {1, 0},
            {1, 1}
        }
    end
end


-- takes a shape layout and converts it into a visual
local layoutToShape = function(block, position)
    local x = block[1];
    local y = block[2];
    
    local shape = Topaz.Shape2D.create();
    shape:formRectangle(
        Game.Parameters.BLOCK_SIZE,
        Game.Parameters.BLOCK_SIZE
    );
    shape:setPosition(
        {
            x= x*Game.Parameters.BLOCK_SIZE,
            y= y*Game.Parameters.BLOCK_SIZE
        }
    );
    position.x = shape:getPosition().x;
    position.y = shape:getPosition().y;

    return shape;
end


local LIMIT_X0 = math.floor(Game.Parameters.BLOCK_SIZE * Game.Parameters.FIELD_WIDTH * (-0.5));
local LIMIT_X1 = LIMIT_X0 + Game.Parameters.FIELD_WIDTH*Game.Parameters.BLOCK_SIZE;
local LIMIT_Y0 = math.floor(Game.Parameters.BLOCK_SIZE * Game.Parameters.FIELD_HEIGHT * (-0.5));
local LIMIT_Y1 = LIMIT_Y0 + Game.Parameters.FIELD_WIDTH*Game.Parameters.BLOCK_SIZE;


Game.createPiece = function()
    local fall = 0;
    local this


    local type = math.floor(math.random()*5)+1;
    local layout = getLayout(type);
    local color = getColor(type);  
    local anchors = {};  
    
    
    local checkBlocked = function(offsetX, offsetY)

        for pos, k in pairs(anchors) do
            local x = Game.Parameters.posToIndex(pos.x + offsetX);
            local y = Game.Parameters.posToIndex(pos.y + offsetY);

            for block, k in pairs(Game.Parameters.Manager.getBlocks()) do

                if (block.indexX() == x) then 
                    if (block.indexY() == y - 1) then
                        return true;
                    end
                end
                
            end
            if (y <= 0) then
                return true;
            end
            
        end
        
        
        if (Game.Parameters.posToIndex(offsetY) <= 0) then 
            return true;
        end
        return false;
    end
    
    local resetShadow = function()
        local y = this:getPosition().y;
        local offset = 0;

        while(true) do
        
            if(checkBlocked(this:getPosition().x, y) == true) then 
                break;
            end
            

            
            y = y - Game.Parameters.BLOCK_SIZE;
            offset = offset + 1;
        end
        
        Game.Parameters.Manager.setShadow(
            {
                x= this:getPosition().x,
                y= y
            },
            
            layout
        );
    end
    
    local containLeft = function()
        local x = this:getPosition().x;
        for pos, k in pairs(anchors) do
            if (x+pos.x < LIMIT_X0) then
                local p = this:getPosition();
                p.x = p.x + Game.Parameters.BLOCK_SIZE;
                this:setPosition(p);
                x = p.x;                        
            end
        end
    end
    
    local containRight = function()
        local x = this:getPosition().x;
        for pos, k in pairs(anchors) do
            if (x+pos.x >= LIMIT_X1) then
                local p = this:getPosition();
                p.x = p.x - Game.Parameters.BLOCK_SIZE;
                this:setPosition(p);
                x = p.x;                        
            end
        end
    end
    

    local resetPiece = function()
        for i, v in ipairs(this:getComponents()) do
            v:destroy();
        end
        anchors = {};   

        local colliderIter = 0;
        for index, block in ipairs(layout) do
            local pos = {}
            local visual = layoutToShape(block, pos);
            visual:setColor(color);
            this:addComponent(visual);
            anchors[pos] = true;
        end
    end

    local blockize = function() -- replace with snapped blocks
        fall = 0; -- dont trigger same-step
        local x = Game.Parameters.snapToBlock(this:getPosition().x);
        local y = Game.Parameters.snapToBlock(this:getPosition().y);
        Game.Parameters.CurrentPiece = nil;
        this:remove();

        for index, block in ipairs(layout) do
            Game.Parameters.Manager.addBlock(
                color,
                x+Game.Parameters.BLOCK_SIZE*block[1],
                y+Game.Parameters.BLOCK_SIZE*block[2]
            );
        end
    end
    
        
    
    this = Topaz.Entity.create(
        {
            onStep = function()
                fall = fall + Game.Parameters.FALL_RATE;
                if (fall > 100) then
                    fall = 0;

                    if (checkBlocked(
                            this:getPosition().x,
                            this:getPosition().y
                    ) == false) then
                        local p = this:getPosition();
                        p.y = p.y - Game.Parameters.BLOCK_SIZE;
                        this:setPosition(p);
                        resetShadow();
                    else 
                        blockize();
                    end
                end
            end
        }
    );


    this.rotate = function()
        for index, block in ipairs(layout) do
            local angle = Topaz.Vector.pointAt2D(
                {x=0, y=0},
                {x=block[1], y=block[2]}
            );
            local length = Topaz.Vector.getLength({
                x= block[1],
                y= block[2]
            });
            angle = (angle+90) * (math.pi / 180);
            block[1] = math.cos(angle) * length;
            block[2] = math.sin(angle) * length;
        end
        resetPiece();
        containLeft();
        containRight();
        resetShadow();
    end
    
    this.fastFall = function()
        fall = fall + 40;
    end
    
    this.hardDrop = function()
        while(true) do
        
            if (checkBlocked(
                    this:getPosition().x, 
                    this:getPosition().y
            ) == true) then
                blockize();
                break;
            end
            local p = this:getPosition();
            p.y = p.y - Game.Parameters.BLOCK_SIZE;
            this:setPosition(p); 
        end
    end
    
    this.moveLeft = function()
        local p = this:getPosition();
        p.x = p.x - Game.Parameters.BLOCK_SIZE;
        this:setPosition(p);
        containLeft();
        resetShadow();
    end
        
    this.moveRight = function() 
        local p = this:getPosition();
        p.x = p.x + Game.Parameters.BLOCK_SIZE;
        this:setPosition(p);
        containRight();          
        resetShadow();
    end
        
    this.resetShadow = function()
        resetShadow();
    end
    resetPiece();
    resetShadow();            

    return this;
end

