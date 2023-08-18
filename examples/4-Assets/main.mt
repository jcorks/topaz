@:class = import(module:'Matte.Core.Class');
@:Topaz = import(module:'Topaz');


@:asset = Topaz.Resources.createAsset(
    path: 'base.png'
);
@:asset = Topaz.Resources.convertAsset(
    asset: asset,
    extension: 'png'
);


@:Effect = class(
    inherits : [Topaz.Entity],
    define:::(this) {
        @:visual = Topaz.Shape2D.new();

        @:wiggleValue ::(rate) {
            return 0.7+0.3*(Topaz.time*0.001*rate)->sin;
        }

        this.constructor = ::{
            visual.formImage(image:asset);
            visual.center = {x:asset.width/2, y:asset.height/2}


            this.attach(component:visual);



            this.onStep = ::{
                visual.color = {
                    r: wiggleValue(rate:0.2),
                    g: wiggleValue(rate:0.5),
                    b: wiggleValue(rate:0.3),
                    a:1             
                }
                
                visual.scale = {
                    x: wiggleValue(rate:0.8),
                    y: wiggleValue(rate:1),      
                    z: wiggleValue(rate:1.3)       
                }
            }
        }
    }
);


@entity = Effect.new();




@:reposition :: {

    entity.scale = {
        x:(Topaz.defaultDisplay.width/asset.width),
        y:(Topaz.defaultDisplay.height/asset.height)
    }
}
Topaz.defaultDisplay.addParameterCallback(
    func:::(display, parameter) {
        when(parameter != Topaz.Display.PARAMETER.WIDTH &&
             parameter != Topaz.Display.PARAMETER.HEIGHT) empty;
        reposition();
    }
);
reposition();

Topaz.defaultDisplay.root = entity;
