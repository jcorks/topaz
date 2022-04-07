@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');
@:Parameters = import(module:'parameters.mt');


@:Block = class(
    inherits: [Topaz.Entity],
    name: 'block',
    define:::(this) {
        @:shape = Topaz.Shape2D.new();
        @:o2d = Topaz.Object2D.new();
    
        @:collider = [];
        collider[0] = 0.4*Parameters.BLOCK_SIZE;
        collider[1] = Parameters.BLOCK_SIZE;
        collider[2] = 0.6*Parameters.BLOCK_SIZE;
        collider[3] = Parameters.BLOCK_SIZE;

        o2d.collider = collider;        
        o2d.group = Parameters.BLOCK_GROUP;
        this.components = [shape, o2d];
        this.constructor = ::(color, x, y) {
            this.position.x = x;
            this.position.y = y;
            shape.formRectangle(
                width :Parameters.BLOCK_SIZE,
                height:Parameters.BLOCK_SIZE
            );

            /*            
            @:test = Topaz.Shape2D.new();
            test.lines = collider;
            test.color = Topaz.Color.parse(string:'magenta');
            this.attach(component:test);
            */

            shape.color = color;
            return this;
        };
    }

);


return Block;
