
local checkMoveBlock = function(piece, input)
    if (input == Topaz.Key.left) then
        piece.moveLeft();
    elseif (input == Topaz.Key.right) then
        piece.moveRight();
    elseif (input == Topaz.Key.up) then
        piece.rotate();
    elseif (input == Topaz.Key.space) then
        piece.hardDrop();
    end
end


local getDisplayHeight = function()
    local display = Topaz.ViewManager.getDefault();
    return display:getParameter(Topaz.Display.Parameter.Height);
end

Game.createManager = function()
    local this;
    this = Topaz.Entity.create(
        {
            onStep = function()
                local currentPiece = Game.Parameters.CurrentPiece;
                if (currentPiece == empty) then
                    this.checkCompleteLines();
                    currentPiece = Game.createPiece();
                    Game.Parameters.CurrentPiece = currentPiece;
                    local p = currentPiece:getPosition();
                    p.y = Game.Parameters.FIELD_HEIGHT*Game.Parameters.BLOCK_SIZE;
                    currentPiece:setPosition(p); 
                    currentPiece.resetShadow();
                    this:attach(currentPiece);
                end        
            end
        }
    );

    local blocks = {};
    local shadow = Topaz.Entity.create();
    local shadow_shapes = {
        Topaz.Shape2D.create(),
        Topaz.Shape2D.create(),
        Topaz.Shape2D.create(),
        Topaz.Shape2D.create()
    };

    -- erases the row index
    local eraseRow = function(row)
        local toRemove = {};
        for block, v in pairs(blocks) do
            local index = block.indexY();
            if (index == row) then            
                toRemove[block] = true;
            end
        end


        for block, v in pairs(toRemove) do
            block:remove(); --remove
            blocks[block] = nil;
        end
        
        for block, v in pairs(blocks) do
            if (block.indexY() > row) then
                block.moveDown();
            end
        end
    end


    for i, shape in ipairs(shadow_shapes) do
        shadow:addComponent(shape);
        shape:formRectangle(
            Game.Parameters.BLOCK_SIZE,
            Game.Parameters.BLOCK_SIZE
        );
        shape:setColor(
            {
                r= 255,
                g= 255,
                b= 255,
                a= 50
            }
        )


    end

    local p = this:getPosition();
    p.y = -getDisplayHeight() / 2;
    this:setPosition(p);

    
    
    this:attach(shadow);
	    
    Topaz.Input.addKeyboardListener(
        {
            onPress = function(input)
                if (Game.Parameters.CurrentPiece == nil) then return end
                checkMoveBlock(Game.Parameters.CurrentPiece, input);
            end, 
            
            onActive = function(input, value)
                if (input == Topaz.Key.down) then
                    Game.Parameters.CurrentPiece.fastFall();      
                end
            end
        }
    );



    -- checks if there are any full lines
    this.checkCompleteLines = function()
        -- for each full line, erase
        while(true) do

            local table_ = {}
            for block, v in pairs(blocks) do 
                local index = block.indexY();
                local r = table_[index];
                if (r == empty) then
                    table_[index]=1
                else 
                    table_[index]=table_[index] + 1
                end
            end        
        
            local removed = false;
            for row, count in pairs(table_) do 
                if (count >= Game.Parameters.FIELD_WIDTH) then 
                    eraseRow(row);
                    table_[row] = nil;
                    removed = true
                    break;
                end
            
            end
            if (removed == false) then 
                break 
            end
        end
    end

    this.addBlock = function(color, x, y)
        local block;
        block = Game.createBlock();
        block.setup(
            x,
            y,
            color
        );
        
        blocks[block] = true;

        this:attach(block);
    end
        
    this.getBlocks = function()
        return blocks;
    end
        
        
    this.setShadow = function(
        position,
        layout
    )
        for i, layoutItem in ipairs(layout) do 
            local x = layoutItem[1];
            local y = layoutItem[2];
            shadow_shapes[i]:setPosition({
                x= x*Game.Parameters.BLOCK_SIZE,
                y= y*Game.Parameters.BLOCK_SIZE
            });
        end
        
        shadow:setPosition(position);     
    end
    
    return this;
end
