@:class         = import(module:'Matte.Core.Class');
@:Topaz         = import(module:'Topaz');
@:createPhysicsObject = import(module:'physics_object.mt');



// A floor that, when instantiated, always 
// matches the bottom of the display
return ::{
    @:this = createPhysicsObject(static:true);

    @:display = Topaz.ViewManager.getDefault();
            
    @:updateFloor = ::{        
        @:width  = display.getParameter(param:Topaz.Display.Parameter.Width);
        @:height = display.getParameter(param:Topaz.Display.Parameter.Height);
    
        this.physics.setColliderRectangle(
            width,
            height:2
        );

        
        this.setPosition(
            value: {
                x: 0,
                y: -height/2
            }
        );
    };

    display.addParameterCallback(callback:::(
        display,
        parameter
    ) {
        if (parameter == Topaz.Display.Parameter.Width ||
            parameter == Topaz.Display.Parameter.Height)
                updateFloor();
    });
    
    updateFloor();
    return this;
}
