@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');



@:font = Topaz.Resources.createAsset(
    path:"PublicSans-Regular.ttf",
    name:"MyFont"
);
Topaz.FontManager.registerFont(asset:font);



@:StringVisual = class(
    inherits : [Topaz.Entity],
    define:::(this) {
        @visual = Topaz.Text2D.new();    
        visual.font = font;
        visual.size =  40;


        @bg = Topaz.Shape2D.new();
        bg.color = 'purple';

        this.components = [bg, visual];

        this.interface = {
            update::(text) {
                visual.text = text;
                bg.formRectangle(
                    width:visual.extentWidth,
                    height:visual.extentHeight
                );
            }
        };
    }
);





@ent = StringVisual.new();
ent.update(text:'Hello world!\nThis is text.');



Topaz.defaultDisplay.root = ent;


