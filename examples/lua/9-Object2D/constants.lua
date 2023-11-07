
Topaz.Object2D.setGroupInteraction(
    Topaz.Object2D.Group.A,
    Topaz.Object2D.Group.B,
    true
);


Topaz.Object2D.setGroupInteraction(
    Topaz.Object2D.Group.A,
    Topaz.Object2D.Group.A,
    true
);

Sample.Constants =  {
    BALL_GROUP = Topaz.Object2D.Group.A,    
    FLOOR_GROUP =  Topaz.Object2D.Group.B,
    GRAVITY = 0.4,
    BALL_SIZE = 50,
    BALL_IMAGE = Topaz.Resources.convertAsset(
        'png',
        Topaz.Resources.createDataAssetFromPath(
            'ball.png', 
            'Bouncy.ball'
        )
    )
};


