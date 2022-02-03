@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');

@:StringVisual = class(
    inherits : [Topaz.Entity],
    define:::(this) {
        @visual = Topaz.Text2D.new();    
        visual.font = 'MyFont';
        visual.size =  40;


        @bg = Topaz.Shape2D.new();
        bg.color = Topaz.Color.parse(string:'purple');

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



Topaz.Resources.loadAsset(
    extension:"", // asset extension,
    path:"PublicSans-Regular.ttf",
    name:"MyFont"
);


Topaz.FontManager.registerFont(name:'MyFont');



@ent = StringVisual.new();
ent.update(text:'Hello world!\nThis is text.');



ent.position = {x:200, y:200};
Topaz.defaultDisplay.root = ent;


