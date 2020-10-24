






var healthComponent = {
    tag   : 'health',
    props : {
        health : 1000,
        amt : 0
    },
    events : {
        'onDeath' : function(props) {
            topaz.log('Am dead!\n');

        },


        'onDamage' : function(props) {
            topaz.log('Ow! ' + props.amt + '\n');
        }
    },
    onReady : function(props) {
        // add methods
        props.component.damage = function(amt) {
            props.amt = amt;
            props.component.emitEvent('onDamage');   
            props.health-=props.amt;
            if (props.health <= 0) {
                props.component.emitEvent('onDeath');
            }
        }
    }
}


var enemyPrefab = {
    name  : 'Test',
    props : {
        size : 100
    },

    onReady : function(props) {
        var damager = new topaz.component(healthComponent);

        props.rect = new topaz.shape2d()
        props.rect.formRectangle(40, 40);
        props.rect.color = new topaz.color('#ff00ff');        
        
        props.o2d = new topaz.object2d();
        props.o2d.addVelocity(3, 45);
        props.o2d.frictionX = 0.01;
        props.o2d.frictionY = 0.01;
    


        props.entity.addComponent(damager);
        props.entity.addComponent(props.rect);
        props.entity.addComponent(props.o2d);

        damager.installHook('onDamage', function() {
            topaz.log(props.entity.name + ' says: Ow!\n');
        });

        damager.installHook('onDeath', function() {
            topaz.log(props.entity.name + ' says: xwx\n');
        });


    },


    onStep : function(props) {
        //topaz.log('i am stepping once per frame\n');

    }
}




c0 = new topaz.entity(enemyPrefab);
topaz.attachPreManager(c0);
c0.queryComponent('health').damage(23);
c0.queryComponent('health').damage(23);


c0.queryComponent('health').damage(23);
c0.queryComponent('health').damage(16);
c0.queryComponent('health').damage(143);
c0.queryComponent('health').damage(1111);







topaz.root = c0;
