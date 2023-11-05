@:constants = import(module:'constants.mt');
@:class     = import(module:'Matte.Core.Class');
@:Topaz     = import(module:'Topaz');

return ::(static) {

    
    @:attributes = {};
    @:o2d = Topaz.Object2D.create();
    if (static) ::<={
        o2d.setGroup(group: constants.FLOOR_GROUP);
        
    } else ::<= {
        o2d.setGroup(group: constants.FLOOR_FLOOR);

        // be affected by gravity
        attributes.onStep = ::{
            o2d.setVelocityY(
                velocity: o2d.getVelocityY() - constants.GRAVITY
            );
            
            // remove if out of bounds
            @:display = Topaz.ViewManager.getDefault();
            @:height = display.getParameter(param:Topaz.Display.Parameter.Height);
            
            if (this.getPosition().y > height+constants.BALL_SIZE) 
                this.remove();
        }  
    }            



    @:this = Topaz.Entity.create(attributes);    
    this.addComponent(component:o2d);    
    this.physics = o2d;
    return this;
}

