@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');
@:Parameters = import(module:'parameters.mt');


return ::(onCleanup) {
    @:this = Topaz.Entity.create(
        attributes : {
            onRemove : onCleanup
        }
    );

    @:shape = Topaz.Shape2D.create();
    @index_x;    
    @index_y;    

    this.addComponent(component:shape);

    shape.formImageScaled(
        width :Parameters.BLOCK_SIZE,
        height:Parameters.BLOCK_SIZE,
        asset: Parameters.BLOCK_IMAGE
    );

        
    this. = {
        setup::(x, y, color) {
            this.setPosition(value:{x:x, y:y});
            index_x = Parameters.posToIndex(pos:x);
            index_y = Parameters.posToIndex(pos:y);            
            shape.setColor(color);
        },
    
        indexX : {
            get :: {
                return index_x;
            }
        },
        indexY : {            
            get:: {
                return index_y;
            }
        },
        moveDown :: {
            @:p = this.getPosition();
            p.y -= Parameters.BLOCK_SIZE;
            this.setPosition(value:p);
            index_y-=1;
        }
    }
    
    // makes the object into functions / setters|getters only.
    this->setIsInterface(enabled:true);
    return this;
}


