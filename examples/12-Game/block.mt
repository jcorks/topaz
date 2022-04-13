@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');
@:Parameters = import(module:'parameters.mt');


@:Block = class(
    inherits: [Topaz.Entity],
    name: 'block',
    define:::(this) {
        @:shape = Topaz.Shape2D.new();
        @index_x;    
        @index_y;    

        this.components = [shape];
        this.constructor = ::(color, x, y) {
            this.position.x = x;
            this.position.y = y;
            index_x = Parameters.posToIndex(pos:x);
            index_y = Parameters.posToIndex(pos:y);
            shape.formImageScaled(
                width :Parameters.BLOCK_SIZE,
                height:Parameters.BLOCK_SIZE,
                image: Parameters.BLOCK_IMAGE
            );


            shape.color = color;
            return this;
        };
        
        this.interface = {
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
                this.position.y -= Parameters.BLOCK_SIZE;
                index_y-=1;
            }
        };
    }

);


return Block;
