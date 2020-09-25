topaz = {
    uniqueObjectPool : 0,
    run : topaz__run,
    pause : topaz__pause,
    'break' : topaz__break,
    resume : topaz__resume,
    iterate : topaz__iterate,
    step : topaz__step,
    draw : topaz__draw,
    attachManager : topaz__attach_manager,        
    attachManagerUnpausable : topaz__attach_manager_unpausable,        
    quit : topaz__quit,
    wait : topaz__wait,
    log : topaz__log,
    objectToString : function(obj, levelSrc) {
        var checked = [];
        var levelG = levelSrc ? levelSrc : 10;
        var helper = function(obj, level) {
            if (!(typeof obj === 'object')) return ''+obj;
            if (checked.indexOf(obj) != -1) return '[Already Printed]'
            checked.push(obj);
            var strOut = '{\n';
            var keys = Object.keys(obj);
            var levelInv = levelG - level;
            for(var i = 0; i < keys.length; ++i) {
                var subStr = levelInv ? helper(obj[keys[i]], level+1) : obj[keys[i]];
                for(var n = 0; n < level; ++n) strOut += '  ';
                strOut += '  \'' + keys[i] + '\' : \'' + subStr + '\',\n'; 
            }
            for(var n = 0; n < level; ++n) strOut += '  ';
            strOut += '}';
            return strOut;
        }
        
        return helper(obj, 0) + '\n';
    },

    entityNull : function() {return topaz_entity__null();},
    entity : function(defineProps, implPre) {
        this.uniqueID = topaz.uniqueObjectPool++;
        var impl;
        var ctx = this;

        if (implPre)
            impl = implPre;
        else 
            impl = topaz_entity__create();


        Object.defineProperty(this, 'isValid', {get : function(){return topaz_entity__is_valid(impl);}}); 
        
        this.remove = function() {
            topaz_entity__remove(impl);
        }


        Object.defineProperty(this, 'childCount', {get : function(){return topaz_entity__get_child_count(impl);}});

        this.nthChild = function(n) {
            var f = topaz_entity__get_nth_child(impl, n);
            if (f.__ctx) return f.ctx;
            return new topaz.entity(undefined, f);
        }

        this.step = function() {topaz_entity__step(n);}
        this.draw = function() {topaz_entity__draw(n);}
        
        this.attach = function(other) {
            topaz_entity__attach(impl, other.impl);
        }

        this.detach = function() {
            topaz_entity__detach(impl);
        }

        this.getParent = function() {
            var f = topaz_entity__get_parent(impl);
            if (f.__ctx) return f.ctx;
            return new topaz.entity(undefined, f);
        }


        this.query = function(name) {
            var f = topaz_entity__query(impl, name);
            if (f.__ctx) return f.ctx;
            return new topaz.entity(undefined, f);
        }


        this.search = function(name) {
            var f = topaz_entity__search(impl, name);
            if (f.__ctx) return f.ctx;
            return new topaz.entity(undefined, f);
        }



        Object.defineProperty(this, 'priority', {
            get : function() {return topaz_entity__get_priority(impl);}, 
            set : function(v){topaz_entity__set_priority(impl, v);}
        });


        this.setPriorityLast = function() {
            topaz_entity__set_priority_last(impl);
        }

        this.setPriorityFirst = function() {
            topaz_entity__set_priority_first(impl);
        }

        this.define = function(props) {
            ctx.props = {};
            // todo: shallow copy
            var keys = Object.keys(props.props);
            for(var i = 0; i < keys.length; ++i) {
                ctx.props[keys[i]] = props.props[keys[i]];
            }

            ctx.props.entity = this;
            impl.name = props.name;
            impl.onStep = props.onStep ? function(){props.onStep(ctx.props);} : undefined;
            impl.onDraw = props.onDraw ? function(){props.onDraw(ctx.props);} : undefined;
            impl.onPreStep = props.onPreStep ? function(){props.onPreStep(ctx.props);} : undefined;
            impl.onPreDraw = props.onPreDraw ? function(){props.onPreDraw(ctx.props);} : undefined;
            impl.onAttach = props.onAttach ? function(){props.onAttach(ctx.props);} : undefined;
            impl.onDetach = props.onDetach ? function(){props.onDetach(ctx.props);} : undefined;
            impl.onRemove = props.onRemove ? function(){props.onRemove(ctx.props);} : undefined;
            if (props.onReady) {
                props.onReady(ctx.props);
            }
        }



        Object.defineProperty(
            this,
            'rotation', {
                get : function()  {return new topaz.vector(0, 0, 0, topaz_entity__get_rotation(impl));},
                set : function(v) {topaz_entity__set_rotation(impl, v.impl);}
            }
        );

        Object.defineProperty(
            this,
            'position', {
                get : function()  {return new topaz.vector(0, 0, 0, topaz_entity__get_position(impl));},
                set : function(v) {topaz_entity__set_position(impl, v.impl);}
            }
        );

        Object.defineProperty(
            this,
            'scale', {
                get : function()  {return new topaz.vector(0, 0, 0, topaz_entity__get_scale(impl));},
                set : function(v) {topaz_entity__set_scale(impl, v.impl);}
            }
        );

        Object.defineProperty(
            this,
            'globalPosition', {
                get : function()  {return new topaz.vector(0, 0, 0, topaz_entity__get_global_position(impl));}
            }
        );


        Object.defineProperty(
            this,
            'isStepping', {
                get : function() {return topaz_entity__is_stepping(impl);}
            }
        );

        Object.defineProperty(
            this,
            'isDrawing', {
                get : function() {return topaz_entity__is_drawing(impl);}
            }
        );


        Object.defineProperty(
            this,
            'stepping', {
                get : function()  {return topaz_entity__get_stepping(impl);},
                set : function(v) {return topaz_entity__set_stepping(impl, v);}
            }
        );



        Object.defineProperty(
            this,
            'drawing', {
                get : function()  {return topaz_entity__get_drawing(impl);},
                set : function(v) {return topaz_entity__set_drawing(impl, v);}
            }
        );

        Object.defineProperty(
            this,
            'name', {
                get : function()  {return topaz_entity__get_name(impl);},
                set : function(v) {return topaz_entity__set_name(impl, v);}
            }
        );

        this.addComponent = function(c) {
            topaz_entity__add_component(impl, c.impl);
        }

        this.addComponentAfter = function(c) {
            topaz_entity__add_component_after(impl, c.impl);
        }

        this.getComponentCount = function() {
            return topaz_entity__get_component_count(impl);
        }

        this.getComponent = function(index) {
            var f = topaz_entity__get_nth_component(impl, index);
            if (f.__ctx) return f.__ctx;
            return new topaz.component(undefined, f);
        }

        this.queryComponent = function(tag) {
            var f = topaz_entity__query_component(impl, tag);
            if (f.__ctx) return f.__ctx;
            return new topaz.component(undefined, f);
        }

        this.removeComponent = function(tag) {
            return topaz_entity__remove_component(impl, tag);
        }
    


        this.impl = impl;
        if (defineProps) {
            ctx.define(defineProps);
        }
        topaz.log('REFERENCE CREATED:\n' + topaz.objectToString(this));

    },
    componentNull : function() {return topaz_component__null();},
    component : function(defineProps, implPre) {
        this.uniqueID = topaz.uniqueObjectPool++;
        var ctx = this;
        var impl;

        if (implPre) 
            impl = implPre;
        else {
            impl = topaz_component__create('');
        }
        impl.__ctx = this;

        
        this.destroy = function() {
            topaz_component__destroy(impl);
        }

        this.step = function() {
            topaz_component__run(impl);
        }

        this.draw = function() {
            topaz_component__draw(impl);
        }

        // sets onStep, onDraw, etc.
        this.define = function(props) {
            ctx.props = {};
            var keys = Object.keys(props.props);
            for(var i = 0; i < keys.length; ++i) {
                ctx.props[keys[i]] = props.props[keys[i]];
            }



            ctx.props.component = this;
            ctx.tag = props.tag;
            impl.onStep = props.onStep ? function(){props.onStep(ctx.props);} : undefined;
            impl.onDraw = props.onDraw ? function(){props.onDraw(ctx.props);} : undefined;
            impl.onAttach = props.onAttach ? function(){props.onAttach(ctx.props);} : undefined;
            impl.onDetach = props.onDetach ? function(){props.onDetach(ctx.props);} : undefined;
            impl.onDestroy = props.onDestroy ? function(){props.onDestroy(ctx.props);} : undefined;

            for(var i = 0; i < props.events.length; ++i) {
                var cb = props.events[i];
                topaz_component__install_event(
                    impl, 
                    cb.name, 
                    function() {
                        topaz.log(topaz.objectToString(cb));
                        cb.fn(ctx.props);

                    }
                );
            }

            if (props.onReady) {
                props.onReady(ctx.props);
            }
        }


        Object.defineProperty(
            this,
            'stepping', {
                get : function() {return topaz_component__get_stepping(impl);},
                set : function(v){topaz_component__set_stepping(impl, v);}
            }
        );


        Object.defineProperty(
            this,
            'tag', {
                get : function() { return topaz_component__get_tag(impl);},
                set : function(v){ topaz_component__set_tag(impl, v);}
            }
        );

        Object.defineProperty(
            this,
            'host', {
                get : function() {
                    var f = topaz_component__get_host(impl);
                    if (f.__ctx) return f.__ctx;
                    return new topaz.entity(undefined, f);
                }
            }
        );

        this.emitEvent = function(eventName, entity) {
            topaz_component__emit_event(impl, eventName, entity ? entity.impl : topaz_entity__null());
        }

        this.emitEventAnonymous = function(eventName) {
            topaz_component__emit_event_anonymous(impl, eventName);
        }

        this.canHandleEvent = function(name) {
            return topaz_component__can_handle_event(impl, name);
        }

        this.installEvent = function(event, callback) {
            topaz_component__install_event(impl, event, callback);
        }

        this.uninstallEvent = function(event) {
            topaz_component__uninstall_event(impl, event);
        }

        this.installHook = function(event, callback) {
            return topaz_component__install_hook(impl, event, callback);
        }
        this.uninstallHook = function(event, id) {
            topaz_component__uninstall_hook(impl, event, id);
        }

        this.installHandler = function(event, callback) {
            return topaz_component__install_handler(impl, event, callback);
        }
        this.uninstallHandler = function(event, id) {
            topaz_component__uninstall_handler(impl, event, id);
        }



        this.impl = impl;
        if (defineProps) {
            ctx.define(defineProps);
        }
        topaz.log('REFERENCE CREATED:\n' + topaz.objectToString(this));

    },

    color : function(name, implPre) {
        var impl;

        if (implPre)
            impl = implPre;
        else 
            impl = topaz_color__create();

        this.setFromString = function(str) {
            topaz_color__set_from_string(impl, str);
        }

        this.getHex = function() {
            return topaz_color__to_hex_string(impl);
        }

        this.remove = function() {
            topaz_color__destroy(impl);
            impl = {};
        }

        Object.defineProperty(this, 'r', {get : function(){return impl.r;}, set : function(v){impl.r = v;}});
        Object.defineProperty(this, 'g', {get : function(){return impl.g;}, set : function(v){impl.g = v;}});
        Object.defineProperty(this, 'b', {get : function(){return impl.b;}, set : function(v){impl.b = v;}});
        Object.defineProperty(this, 'a', {get : function(){return impl.a;}, set : function(v){impl.a = v;}});

        if (name)
            this.setFromString(name);

        this.impl = impl;
    },


    vector : function(x, y, z, implPre) {
        var impl; 
        if (implPre)
            impl = implPre;
        else 
            impl = topaz_vector__create(x, y, z);


        Object.defineProperty(this, 'length', {get : function(){return topaz_vector__get_length(impl)}});
        Object.defineProperty(this, 'x', {get : function(){return impl.x;}, set : function(v){impl.x = v;}});
        Object.defineProperty(this, 'y', {get : function(){return impl.y;}, set : function(v){impl.y = v;}});
        Object.defineProperty(this, 'z', {get : function(){return impl.z;}, set : function(v){impl.z = v;}});

        this.getDistance = function(other) {
            return topaz_vector__get_distance(impl, other.impl);
        }        

        this.normalize = function() {
            topaz_vector__normalize(impl);   
        }

        this.cross = function(other) {
            var out = topaz_vector__cross(impl, other.impl);
            return new topaz.vector(0, 0, 0, other);
        }

        this.floor = function() {
            topaz_vector__floor(impl);
        }

        this.rotationXDiff = function(other) {
            return topaz_vector__rotation_x_diff(impl, other.impl);
        }

        this.rotationXDiffRelative = function(other) {
            return topaz_vector__rotation_x_diff_relative(impl, other.impl);
        }

        this.rotationX = function() {
            return topaz_vector__rotation_x(impl);
        }

        this.rotationYDiff = function(other) {
            return topaz_vector__rotation_y_diff(impl, other.impl);
        }

        this.rotationYDiffRelative = function(other) {
            return topaz_vector__rotation_y_diff_relative(impl, other.impl);
        }

        this.rotationY = function() {
            return topaz_vector__rotation_y(impl);
        }


        this.rotationZDiff = function(other) {
            return topaz_vector__rotation_z_diff(impl, other.impl);
        }

        this.rotationZDiffRelative = function(other) {
            return topaz_vector__rotation_z_diff_relative(impl, other.impl);
        }

        this.rotationZ = function() {
            return topaz_vector__rotation_z(impl);
        }

        this.rotateX = function(val) {
            topaz_vector__rotate_x(impl, val);
        }

        this.rotateY = function(val) {
            topaz_vector__rotate_y(impl, val);
        }

        this.rotateZ = function(val) {
            topaz_vector__rotate_z(impl, val);
        }

        this.remove = function() {
            topaz_vector__destroy(impl);
            impl = {};
        }

        this.impl = impl;
    }
}


Object.defineProperty(topaz, 'isPaused', {get : function(){return topaz__is_paused();}});
Object.defineProperty(topaz, 'root', {
    get : function(){return new topaz.entity(undefined, topaz__get_root());},
    set : function(v){return topaz__set_root(v.impl);}
});
Object.defineProperty(topaz, 'time', {get : function(){return topaz__get_time();}});
Object.defineProperty(topaz, 'versionMicro', {get : function(){return topaz__get_version_micro();}});
Object.defineProperty(topaz, 'versionMajor', {get : function(){return topaz__get_version_major();}});
Object.defineProperty(topaz, 'versionMinor', {get : function(){return topaz__get_version_minor();}});






var healthComponent = {
    tag : 'health',

    props : {
        health : 1000
    },


    events : [
        {name : 'onDeath', 
            fn : function(props) {
                topaz.log('Am dead!\n');

            }
        },
        {name : 'onDamage', 
            fn   : function(props) {
                topaz.log('Ow! '+props.amt+'\n');
            }
        }


    ],

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
        props.entity.addComponent(damager);
    }
}

c0 = new topaz.entity(enemyPrefab);
c0.queryComponent('health').damage(23);
c0.queryComponent('health').damage(23);


c0.queryComponent('health').damage(23);
c0.queryComponent('health').damage(16);
c0.queryComponent('health').damage(143);
c0.queryComponent('health').damage(1111);






topaz.root = c0;

