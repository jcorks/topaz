@:Topaz = import(module:'Topaz');



Topaz.Object2D.setGroupInteraction(
    a: Topaz.Object2D.Group.A,
    b: Topaz.Object2D.Group.B,
    shouldInteract:true
);


Topaz.Object2D.setGroupInteraction(
    a: Topaz.Object2D.Group.A,
    b:Topaz.Object2D.Group.A,
    shouldInteract:true
);

return {
    BALL_GROUP : Topaz.Object2D.Group.A,    
    FLOOR_GROUP :  Topaz.Object2D.Group.B,
    GRAVITY : 0.4,
    BALL_SIZE : 50,
    BALL_IMAGE : Topaz.Resources.convertAsset(
        asset:Topaz.Resources.createDataAssetFromPath(
            path:'ball.png', 
            name:'Bouncy.ball'
        ), 
        fileType:'png'
    )
};


