@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');
@:JSON = import(module:'Matte.Core.JSON');

@:constants = import(module:'constants.mt');
@:Floor = import(module:'floor.mt');
@:Ball  = import(module:'ball.mt');


@:room = Topaz.Entity.new();
@:floor = Floor.new();

    

Topaz.Input.addPointerListener(onPress:::(input) {
    when (input != Topaz.Input.POINTER._0) empty;
    @:ball = Ball.new();
    ball.position = Topaz.Input.mouse;
      
    room.attach(entity:ball);
});

room.children = [floor];
Topaz.defaultDisplay.root = room;


