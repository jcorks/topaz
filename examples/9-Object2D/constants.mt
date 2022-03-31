@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');



Topaz.Object2D.setGroupInteraction(
    thisGroup: Topaz.Object2D.GROUP.A,
    otherGroup:Topaz.Object2D.GROUP.B,
    interact:true
);


Topaz.Object2D.setGroupInteraction(
    thisGroup: Topaz.Object2D.GROUP.A,
    otherGroup:Topaz.Object2D.GROUP.A,
    interact:true
);

return {
    BALL_GROUP : Topaz.Object2D.GROUP.A,
    
    FLOOR_GROUP :  Topaz.Object2D.GROUP.B,
    
    GRAVITY : 0.4,
    
    BALL_SIZE : 50,
    
    BALL_IMAGE : Topaz.Resources.convertAsset(
        asset:Topaz.Resources.createAsset(
            path:'ball.png', 
            name:'Bouncy.ball'
        ), 
        extension:'png'
    )
};


