@:Topaz = import(module:"Topaz");


@:createScene = ::{
    @:font = Topaz.Resources.createDataAssetFromPath(
        path:"PublicSans-Regular.ttf",
        name:"MyFont"
    );
    Topaz.FontManager.registerFont(asset:font);



    @:bg    = Topaz.Shape2D.create();
    @:shape = Topaz.Shape2D.create();
    @:text  = Topaz.Text2D.create();
    
    @scene 
    @delta = 0;
    scene = Topaz.Entity.create(
        attributes: {
            onStep :: { 
                delta = delta + Topaz.getDeltaTime() / 5000;
                shape.setRotation(
                    value : {
                        x: delta,
                        y: delta,
                        z: delta,
                    }
                )
            }
        }
    );

    bg.formRectangle(width:512, height:512);
    bg.setCenter(value:{x:256, y:256});
    bg.setColor(color:Topaz.Color.fromString(str:'#202020'));
    
    shape.formRectangle(width:50, height:50);
    shape.setCenter(value:{x:25, y:25});
    shape.setColor(color:Topaz.Color.fromString(str:'purple'));
    
    text.setPosition(value:{x:-20, y:-20});
    text.setText(text:"Hello!");
    text.setFont(font, pixelSize:40);
    
    scene.addComponent(component:bg);
    scene.addComponent(component:shape);    
    scene.addComponent(component:text);
    return scene;
}



@:createViewport = ::{
    @:viewport = Topaz.Viewport.create();
    viewport.resize(width:256, height:256);
    
    return viewport;
}


@:createOutput = ::{
    @:v = Topaz.Shape2D.create();

    @obj;
    @delta = 0;
    obj = Topaz.Entity.create(
        attributes: {
            onStep ::{
                delta = delta + 0.2;
                obj.setRotation(value : {
                    x : delta * 0.7,
                    y : delta,
                    z : delta * 1.2
                })
            }
        }
       
    )
    
    v.setCenter(value:{x : 150, y : 100});
    
    obj.addComponent(component:v);
    obj.setViewport = ::(viewport) { 
        v.formImageScaled(
            asset:viewport.getImage(), 
            width:300, 
            height:200
        ); 
    }
    return obj;
}


@:scene = createScene();
@:viewport = createViewport();

viewport.attach(child:scene);


@:output = createOutput();
output.setViewport(viewport);

@:displayViewport = Topaz.ViewManager.getDefault().getViewport()

displayViewport.attach(child:viewport);
displayViewport.attach(child:output);


