// Creates an entity that is a circle and... Spins! 
// It uses randomly generated colors from the rng color bundle.

@:Topaz  = import(module:'Topaz');
@:class  = import(module:'Matte.Core.Class');
@:generateColor = import(module:'RNG-Color.Generator');

@:circleImage = Topaz.Resources.fetchAsset(name:'CircleShape.Image');


// The class itself is returned, not just one instance.
return class(
    name: 'CircleShape.Entity',
    inherits : [Topaz.Entity],

    define:::(this) {
    
        // An image is used for the circle.
        @:shape = Topaz.Shape2D.new();
        shape.formImage(image:circleImage);

        // centered for rotation
        shape.center = {
            x:circleImage.width/2,
            y:circleImage.height/2
        };

        // Every step, it turns a little.        
        this.onStep = ::{
            shape.rotation.y += 0.5;
        };
        
        // RNG color. See the RNG-Color bundle.
        shape.color = generateColor();
        this.components = [shape];
    }
);





