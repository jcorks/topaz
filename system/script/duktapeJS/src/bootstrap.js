topaz = {
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


    entity : function(implPre) {
        var impl;

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
            return topaz_entity__get_nth_child(impl, n);
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
            return new topaz.entity(topaz_entity__get_parent(impl));
        }


        this.query = function(name) {
            return new topaz.entity(topaz_entity__query(impl, name));
        }


        this.search = function(name) {
            return new topaz.entity(topaz_entity__search(impl, name));
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


    


        this.impl = impl;
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
    get : function(){return topaz__get_root();},
    set : function(v){return topaz__set_root(v.impl);}
});
Object.defineProperty(topaz, 'time', {get : function(){return topaz__get_time();}});
Object.defineProperty(topaz, 'versionMicro', {get : function(){return topaz__get_version_micro();}});
Object.defineProperty(topaz, 'versionMajor', {get : function(){return topaz__get_version_major();}});
Object.defineProperty(topaz, 'versionMinor', {get : function(){return topaz__get_version_minor();}});



c = new topaz.entity();
c.name = 'test';

