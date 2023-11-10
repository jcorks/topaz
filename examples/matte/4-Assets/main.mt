@:class = import(module:'Matte.Core.Class');
@:Topaz = import(module:'Topaz');


@:asset = Topaz.Resources.createDataAssetFromPath(
    name: 'Example_Image',
    path: 'base.png'
);
@:asset = Topaz.Resources.convertAsset(
    asset: asset,
    fileType: 'png'
);


@:createEffect ::{

    @:visual = Topaz.Shape2D.create();

    @:wiggleValue ::(rate) {
        return 0.7+0.3*(Topaz.getTime()*0.001*rate)->sin;
    }


    @:this = Topaz.Entity.create(
        attributes : {
            onStep :: {
                visual.setColor(
                    color : {
                        r: wiggleValue(rate:0.2),
                        g: wiggleValue(rate:0.5),
                        b: wiggleValue(rate:0.3),
                        a:1             
                    }
                );
                visual.setScale(
                    value: {
                        x: wiggleValue(rate:0.8),
                        y: wiggleValue(rate:1),      
                        z: wiggleValue(rate:1.3)       
                    }    
                );
            }  
        }
    );

    visual.formImage(asset);
    visual.setCenter(value:{x:asset.getWidth()/2, y:asset.getHeight()/2});

    this.addComponent(component:visual);
    
    return this;
};


@entity = createEffect();


@:display = Topaz.ViewManager.getDefault();

@:reposition :: {
    @:width  = display.getParameter(param:Topaz.Display.Parameter.Width);
    @:height = display.getParameter(param:Topaz.Display.Parameter.Height);

    entity.setScale(value:{
        x:(width/asset.getWidth()),
        y:(height/asset.getHeight())
    })
}

display.addParameterCallback(
    callback:::(display, parameter) {
        when(parameter != Topaz.Display.Parameter.Width &&
             parameter != Topaz.Display.Parameter.Height) empty;
        reposition();
    }
);

reposition();

display.getViewport().attach(child:entity);
