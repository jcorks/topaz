// All topaz symbols are contained within the module "Topaz".
// It needs to be pulled in before accessing its features.
@:Topaz = import(module:'Topaz');


@:createRectangle :: {
    @:entity = Topaz.Entity.create(
        attributes : {
            onStep ::{
                @:r = entity.getRotation();
                r.x += 0.01;
                r.y += 0.2;
                r.z += 0.03;
                entity.setRotation(value:r);
            }
        }
    );
    
    @:visual = Topaz.Shape2D.create();
    visual.formRectangle(width:200, height:200);
    visual.setCenter(value:{x:100, y:100});
    
    entity.addComponent(component:visual);
    
    
    entity.setColor1 = ::{
        visual.setColor(color:Topaz.Color.fromString(str:'purple'));    
    }

    entity.setColor2 = ::{
        visual.setColor(color:Topaz.Color.fromString(str:'green'));    
    }

    
    return entity;
}


@:room = Topaz.Entity.create();

@:r = createRectangle();
r.setColor1();
@:r2 = createRectangle();
r2.setColor2();

r2.setPosition(value:{x:100, y:40});

room.attach(child:r);
room.attach(child:r2);


@:display = Topaz.ViewManager.getDefault();
display.setName(name:"Example 1");
display.setRoot(newRoot:room);


