@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');

@:createFloor = import(module:'floor.mt');
@:createBall  = import(module:'ball.mt');


@:room = Topaz.Entity.create();
@:floor = createFloor();

    

Topaz.Input.addPointerListener(
    listener : {
        onPress:::(input) {
            when (input != Topaz.Pointer._0) empty;
            @:ball = createBall();
            ball.setPosition(
                value : {
                    x: Topaz.Input.mouseX(),
                    y: Topaz.Input.mouseY()
                }
            );
              
            room.attach(child:ball);
        }
    }        
);

room.attach(child:floor);
Topaz.ViewManager.getDefault().setRoot(newRoot:room);


