@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');



@:font = Topaz.Resources.createDataAssetFromPath(
    path:"PublicSans-Regular.ttf",
    name:"MyFont"
);
Topaz.FontManager.registerFont(asset:font);



@:createStringVisual ::{
    @:this = Topaz.Entity.create();
    
    @visual = Topaz.Text2D.create();    
    @bg = Topaz.Shape2D.create();

    visual.setFont(font, pixelSize:40);

    bg.setColor(color:Topaz.Color.fromString(str:'purple'));
    this.addComponent(component:bg);
    this.addComponent(component:visual);


    this.update = ::(text) {
        visual.setText(text);
        bg.formRectangle(
            width:visual.getExtentWidth(),
            height:visual.getExtentHeight()
        );
    }
    
    return this;
}





@ent = createStringVisual();
ent.update(text:'Hello world!\nThis is text.');



Topaz.ViewManager.getDefault().setRoot(newRoot:ent);


