
@:class = import(module:'Matte.Core.Class');
@:JSON  = import(module:'Matte.Core.JSON');


@Topaz;
@:Color = ::<={
    @:topaz_color__to_hex_string = getExternalFunction(name:'topaz_color__to_hex_string');
    @:topaz_color__set_from_string = getExternalFunction(name:'topaz_color__set_from_string');

    @updateColor;

    @:updateColorNative ::(a, b, c, d) {
        updateColor.r = a;
        updateColor.g = b;
        updateColor.b = c;
        updateColor.a = d;
    };

    return {
        asString : ::(color) {
            return topaz_color__to_hex_string(a:color.r, b:color.g, c:color.b, d:color.a);
        },

        parse : ::(string) {
            updateColor = {};
            topaz_color__set_from_string(a:string, b:updateColorNative);
            return updateColor;
        }
    };
};


@:Vector = ::<={
    @:topaz_vector__get_distance = getExternalFunction(name:'topaz_vector__get_distance');
    @:topaz_vector__normalize = getExternalFunction(name:'topaz_vector__normalize');
    @:topaz_vector__cross = getExternalFunction(name:'topaz_vector__cross');
    @:topaz_vector__reflect_2d = getExternalFunction(name:'topaz_vector__reflect_2d');
    @:topaz_vector__look_at_rotation = getExternalFunction(name:'topaz_vector__look_at_rotation');
    @:topaz_vector__point_at_2d = getExternalFunction(name:'topaz_vector__point_at_2d');
    @:topaz_vector__dot = getExternalFunction(name:'topaz_vector__dot');
    @:topaz_vector__floor = getExternalFunction(name:'topaz_vector__floor');
    @:topaz_vector__get_length = getExternalFunction(name:'topaz_vector__get_length');

    @updateTarget;

    @:updateVectorNative ::(a, b, c) {
        updateTarget.x = a;
        updateTarget.y = b;
        updateTarget.z = c;
    };

    // Vector.Expression << {x:0, y:1} - {}
    @VectorExpression = class(
        name : 'VectorExpression',
        define:::(this) {
            @x = 0; @y = 0; @z = 0;

            this.interface = {
                x : {
                    get :: {
                        return x;
                    },

                    set ::(value) {
                        x = value;
                    }
                },
                y : {
                    get :: {
                        return y;
                    },

                    set ::(value) {
                        y = value;
                    }
                },
                z : {
                    get :: {
                        return z;
                    },

                    set ::(value) {
                        z = value;
                    }
                }

            };



            this.attributes = {
                (Number) : ::(value) {
                    @:out = VectorExpression.new();
                    out.x = value;
                    out.y = value;
                    out.z = value;
                    return out;                    
                },

                (Object) : ::(value) {
                    @:out = VectorExpression.new();
                    out.x = value.x;
                    out.y = value.y;
                    out.z = value.z;
                    return out;                
                },

                '<<' : ::(value) {
                    @:out = VectorExpression.new();
                    out.x = value.x;
                    out.y = value.y;
                    out.z = value.z;
                    return out;
                },

                '+' : ::(value) {
                    @:out = VectorExpression.new();
                    if (value.x != empty) out.x = this.x + value.x;
                    if (value.y != empty) out.y = this.y + value.y;
                    if (value.z != empty) out.z = this.z + value.z;
                    return out;
                },

                '-' : ::(other) {
                    @:out = VectorExpression.new();
                    out.x = this.x - other.x;
                    out.y = this.y - other.y;
                    out.z = this.z - other.z;
                    return out;
                },

                '*' : ::(other) {
                    @:out = VectorExpression.new();
                    out.x = this.x * other.x;
                    out.y = this.y * other.y;
                    out.z = this.z * other.z;
                    return out;
                },

                '/' : ::(other) {
                    @:out = VectorExpression.new();
                    out.x = this.x / other.x;
                    out.y = this.y / other.y;
                    out.z = this.z / other.z;
                    return out;
                }

            };
        }
    );


    return {

        length : ::(of){
            return topaz_vector__get_length(a:of.x, b:of.y, c:of.z);
        },

        _updateRemote ::(target) {
            updateTarget = target;
            return updateVectorNative;
        },

        getDistance ::(from, to){ 
            return topaz_vector__get_distance(
                a:from.x, b:from.y, c:from.z,
                d:to.x,   e:to.y,   f:to.z 
            );
        },
        
        normalize ::(value){ 
            updateTarget = value;
            topaz_vector__normalize(a:value.x, b:value.y, c:value.z, d:updateVectorNative);   
        },
        
        cross ::(a, b){ 
            updateTarget = {};
            topaz_vector__cross(
                a:a.x, b:a.y, c:a.z,
                d:b.x, e:b.y, f:b.z,
                g:updateVectorNative
            );
            return updateTarget;
        },
        
        reflect2D ::(direction, surface) {
            updateTarget = {};
            topaz_vector__reflect_2d(
                a:direction.x, b:direction.y, c:direction.z,
                d:surface.x,   e:surface.y,   f:surface.z,
                g:updateVectorNative
            );
            return updateTarget;
        },
        lookAtRotation ::(from, to, up) {
            updateTarget = {};
            topaz_vector__look_at_rotation(
                a:from.x, b:from.y, c:from.z,
                d:to.x,   e:to.y,   f:to.z,
                g:up.x,   h:up.y,   h:up.z,
                i:updateVectorNative
            );
            return updateTarget;        
        },
        pointAt2D ::(from, to) {
            return topaz_vector__point_at_2d(
                a:from.x, b:from.y, c:from.z,
                d:to.x,   e:to.y,   f:to.z
            );
        },

        dot ::(a, b){ 
            return topaz_vector__dot(
                a:a.x, b:a.y, c:a.z,
                d:b.x, e:b.y, f:b.z
            );
        },

        floor ::(value){ 
            updateTarget = value;
            topaz_vector__floor(a:value.native, b:updateVectorNative);
            return value;
        },
        


        

        expression :  VectorExpression.new()
    };
};

@:ManagedVector = class(
    define:::(this) {
        @set_x;
        @set_y;
        @set_z;
        @get_x;
        @get_y;
        @get_z;
        @native;
        this.constructor = ::(native_, getx, gety, getz, setx, sety, setz){
            set_x = setx;
            set_y = sety;
            set_z = setz;
            get_x = getx;
            get_y = gety;
            get_z = getz;
            native = native_;
            return this;
        };

        this.interface = {
            x : {
                get :: {return get_x(a:native);},
                set ::(value) {
                    set_x(a:native, b:value);
                }               
            },

            y : {
                get :: {return get_y(a:native);},
                set ::(value) {
                    set_y(a:native, b:value);
                }               
            },

            z : {
                get :: {return get_z(a:native);},
                set ::(value) {
                    set_z(a:native, b:value);
                }               
            }
        };
    }
);



@:ManagedColor = class(
    define:::(this) {
        @set_r;
        @set_g;
        @set_b;
        @set_a;
        @get_r;
        @get_g;
        @get_b;
        @get_a;
        @native;
        this.constructor = ::(native_, getr, getg, getb, geta, setr, setg, setb, seta){
            set_r = setr;
            set_g = setg;
            set_b = setb;
            set_a = seta;
            get_r = getr;
            get_g = getg;
            get_b = getb;
            get_a = geta;
            native = native_;
            return this;
        };

        this.interface = {
            r : {
                get :: {return get_r(a:native);},
                set ::(value) {
                    set_r(a:native, b:value);
                }               
            },

            g : {
                get :: {return get_g(a:native);},
                set ::(value) {
                    set_g(a:native, b:value);
                }               
            },

            b : {
                get :: {return get_b(a:native);},
                set ::(value) {
                    set_b(a:native, b:value);
                }               
            },

            a : {
                get :: {return get_a(a:native);},
                set ::(value) {
                    set_a(a:native, b:value);
                }               
            }
            
        };
    }
);

Topaz = class(
    define :::(this){ 
        @__Topaz__ = this;

        // Declares a class which has external, native data.
        // Instances of this_ class act as "shells" or interfaces for 
        // this_ external data and usually have little to no state 
        // outside of this_.
        @instanceID = 0;
        @__Native__ = class(   
            name : 'Topaz.Native',        
            define : ::(this){ 
                @impl = empty;
                
                this.interface = {
                    // binds a native instance to this_ table. Will throw an error 
                    // if no since native instance can be bound. Binding happens through 2 
                    // mechanisms: 1) instData.nativeCreate is a native function that can { 
                    // return a new object for us to use or 2) instData.instance contains 
                    // a preexisting native reference.
                    bindNative ::(instance, nativeCreate, args) { 
                        impl = instance;
                        
                        
                        if (impl == empty && nativeCreate != empty) ::<={ 
                            if (args == empty) ::<={
                                impl = nativeCreate();
                            } else ::<={
                                match(Object.length(of:args)) {
                                  (1): ::<={
                                    impl = nativeCreate(a:args[0]);
                                  },
                                  
                                  (2): ::<={
                                    impl = nativeCreate(a:args[0], b:args[1]);
                                  },
                                  
                                  (3): ::<={
                                    impl = nativeCreate(a:args[0], b:args[1], c:args[2]);
                                  },
                                  (5): ::<={
                                    impl = nativeCreate(a:args[0], b:args[1], c:args[2], d:args[3], e:args[4]);
                                  },
                                  default:
                                    error(detail:'Too many arguments to dispatch native create call')
                                };
                            };
                        };
                        when (impl == empty) ::<={
                            error(detail:'Topaz.Native instance cannot have a empty native reference.');
                        };
                        impl.__ctx = this;
                        impl.__id = instanceID;
                        instanceID+=1;
                        return impl;
                    },
                    
                    
                    
                    
                    //Gets the external reference to this_ external class
                    native : { 
                        get ::() { 
                            return impl;
                        }
                    }          
                };
            
            }
        );

        // Must be separate since Topaz inherit from the asset ahead of time before Topaz is computed.
        @:topaz_asset__get_type = getExternalFunction(name:'topaz_asset__get_type');
        @__Asset__ = ::<={
            @:topaz_asset__get_name = getExternalFunction(name:'topaz_asset__get_name');

            return class(
                name : 'Topaz.Asset',    
                statics : {
                    TYPE : {
                        NONE : 0,
                        IMAGE : 1,
                        SOUND : 2,
                        MATERIAL : 3,
                        PARTICLE : 4,
                        DATA : 5,
                        MESH : 6,
                        BUNDLE : 7
                    }
                }, 
                inherits : [__Native__],
                define : ::(this) { 
                    @impl;
                    
                    this.constructor = ::(native) {
                        impl = this.bindNative(
                            instance : native
                        );
                        return this;
                    };

                    this.interface = {
                        type : {
                            get : ::()   {return topaz_asset__get_type(a:impl);}
                        },

                        name : {
                            get : ::()   {return topaz_asset__get_name(a:impl);}
                        }
                    };
                }
            );
        };

        @__Filesystem__ = ::<={
            @:topaz_filesystem__get_path = getExternalFunction(name:'topaz_filesystem__get_path');
            @:topaz_filesystem__get_path_from_string = getExternalFunction(name:'topaz_filesystem__get_path_from_string');
            @:topaz_filesystem_path__as_string = getExternalFunction(name:'topaz_filesystem_path__as_string');
            @:topaz_filesystem_path__get_name = getExternalFunction(name:'topaz_filesystem_path__get_name');
            @:topaz_filesystem_path__get_parent = getExternalFunction(name:'topaz_filesystem_path__get_parent');
            @:topaz_filesystem_path__get_nth_child = getExternalFunction(name:'topaz_filesystem_path__get_nth_child');
            @:topaz_filesystem_path__get_child_count = getExternalFunction(name:'topaz_filesystem_path__get_child_count');
            return {     
                DEFAULT_NODE : {
                    RESOURCES : 0,
                    TOPAZ : 1,
                    USER_DATA : 2
                },

                
                Path : class(
                    name : 'Topaz.Filesystem.Path',        
                    inherits : [__Native__],
                    define   : ::(this){ 
                        @impl;
                        
                        this.constructor = ::(fromNode, fromString, relativePath, native) {
                            when(native != empty) ::<= {
                                impl = this.bindNative(instance : native);
                                return this;
                            };

                            @:native = if (fromNode != empty) 
                                topaz_filesystem__get_path(a:fromNode) 
                            else (
                                if (relativePath == empty) 
                                    topaz_filesystem__get_path_from_string(a:fromString)
                                else 
                                    topaz_filesystem__get_path_from_string(a:relativePath.native, b:fromString)
                            );
                            impl = this.bindNative(instance : native);
                            return this;
                        };

                        this.interface = {
                            string : {
                                get ::{ 
                                    return topaz_filesystem_path__as_string(a:impl);
                                }
                            },
                            
                            name : {
                                get ::{
                                    return topaz_filesystem_path__get_name(a:impl);
                                }
                            },
                            
                            parent : {
                                get ::{ 
                                    return __Topaz__.Filesystem.Path.new(native:topaz_filesystem_path__get_parent(a:impl));
                                }
                            },
                            
                            children : {
                                get ::{ 
                                    @out = [];
                                    @len = topaz_filesystem_path__get_child_count(a:impl);
                                    for(in:[0, len], do:::(i) { 
                                        @p = __Topaz__.Filesystem.Path.new(native:topaz_filesystem_path__get_nth_child(a:impl, b:i));
                                        if (p != empty) ::<= {
                                            out[i] = p;
                                        };
                                    });
                                    return out;
                                }
                            }
                        };
                    }
                )
            };
        };

        @__RNG__ = ::<={
            @:topaz_rng__create = getExternalFunction(name:'topaz_rng__create');
            @:topaz_rng__set_seed = getExternalFunction(name:'topaz_rng__set_seed');
            @:topaz_rng__next_int = getExternalFunction(name:'topaz_rng__next_int');
            @:topaz_rng__next_value = getExternalFunction(name:'topaz_rng__next_value');

            return class(
                name : 'Topaz.RNG',        
                inherits : [__Native__],
                define : ::(this){ 
        
                    @impl;
                    
                    this.constructor = ::(native, seed) {
                        impl = this.bindNative(
                            instance : native,
                            nativeCreate : topaz_rng__create,
                            args : [seed]
                        );
                        return this;
                    };
        
                    
                    this.interface = {
                        seed : {
                            set ::(value) { 
                                topaz_rng__set_seed(a:impl, b:value);
                            }
                        },
                        
                        integer : {
                            get : ::() { 
                                return topaz_rng__next_int(a:impl);
                            }
                        },
                        
                        value : {
                            get : ::() { 
                                return topaz_rng__next_value(a:impl);
                            }
                        }
                    };
                }
            );
        };

        @__Display__ = ::<={
            @:topaz_view_manager__create_display = getExternalFunction(name:'topaz_view_manager__create_display');
            @:topaz_display__destroy = getExternalFunction(name:'topaz_display__destroy');
            @:topaz_display__set_parameter = getExternalFunction(name:'topaz_display__set_parameter');
            @:topaz_display__add_parameter_callback = getExternalFunction(name:'topaz_display__add_parameter_callback');
            @:topaz_display__add_close_callback = getExternalFunction(name:'topaz_display__add_close_callback');
            @:topaz_display__remove_callback = getExternalFunction(name:'topaz_display__remove_callback');
            @:topaz_display__get_parameter = getExternalFunction(name:'topaz_display__get_parameter');
            @:topaz_display__get_framebuffer = getExternalFunction(name:'topaz_display__get_framebuffer');
            @:topaz_display__use_framebuffer = getExternalFunction(name:'topaz_display__use_framebuffer');
            @:topaz_display__clear_main_framebuffer = getExternalFunction(name:'topaz_display__clear_main_framebuffer');
            @:topaz_display__capture_main_framebuffer = getExternalFunction(name:'topaz_display__capture_main_framebuffer');
            @:topaz_display__get_camera_2d = getExternalFunction(name:'topaz_display__get_camera_2d');
            @:topaz_display__get_camera_3d = getExternalFunction(name:'topaz_display__get_camera_3d');
            @:topaz_display__get_main_framebuffer = getExternalFunction(name:'topaz_display__get_main_framebuffer');
            @:topaz_display__set_root = getExternalFunction(name:'topaz_display__set_root');
            @:topaz_display__get_root = getExternalFunction(name:'topaz_display__get_root');
            return class(
                name : 'Topaz.Display',        
                inherits : [__Native__],
                statics : {
                    VIEW_POLICY : {
                        NONE      : 0,
                        MATCH_SIZE : 1
                    },

                    PARAMETER : {
                        X : 0,
                        Y : 1,
                        WIDTH : 2,
                        HEIGHT : 3,
                        SHOW : 4,
                        FULLSCREEN : 5,
                        LOCK_CLIENT_RESIZE : 6,
                        LOCK_CLIENT_POSITION : 7,
                        VIEW_POLICY : 8,
                        INPUT_FOCUS : 9,
                        ACTIVE : 10,
                        DECORATED:11
                    },
                    
                    FRAMEBUFFER : {
                        A : 0,
                        B : 1,
                        C : 2,
                        D : 3
                    }
                },
                define : ::(this) { 
                    @impl;
                    this.constructor = ::(native){
                        impl = this.bindNative(
                            instance : native,
                            nativeCreate : topaz_view_manager__create_display
                        );
                        return this;
                    };
                    
                    this.interface = {
                        destroy : ::(){ 
                            topaz_display__destroy(a:impl);
                        },
        
                        resize : ::(width, height){ 
                            topaz_display__set_parameter(a:impl, b:2, c:width);
                            topaz_display__set_parameter(a:impl, b:3, c:height);
                        },
        
                        addParameterCallback : ::(func){ 
                            return topaz_display__add_parameter_callback(a:impl, b:::(a, b){func(display:a, parameter:b);});
                        },
        
                        addCloseCallback : ::(func){
                            return topaz_display__add_close_callback(a:impl, b:::(a){func(display:a);});
                        },
        
                        removeCallback : ::(id){ 
                            topaz_display__remove_callback(a:impl, b:id);
                        },
        
                        getParameter : ::(parameter){ 
                            return topaz_display__get_parameter(a:impl, b:parameter);
                        },
        
                        setParameter : ::(parameter, value){ 
                            topaz_display__set_parameter(a:impl, b:parameter, c:value);
                        },
        
        
                        getFramebuffer : ::(id){ 
                            return __Topaz__.Framebuffer.new(native:topaz_display__get_framebuffer(a:impl, b:id));
                        },
        
                        useFramebuffer : ::(id){ 
                            return topaz_display__use_framebuffer(a:impl, b:id);
                        },
        
                        clearMainFramebuffer : ::(layer) { 
                            topaz_display__clear_main_framebuffer(a:impl, b:layer);
                        },
        
                        captureMainFramebuffer : ::(capture){ 
                            topaz_display__capture_main_framebuffer(a:impl, b:capture.native);
                        },
        
                        width : {
                            get : ::(){ 
                                return topaz_display__get_parameter(a:impl, b:2);
                            },
                            set : ::(value){ 
                                topaz_display__set_parameter(a:impl, b:2, c:value);
                            }
                        },
        
                        height : {
                            get : ::(){ 
                                return topaz_display__get_parameter(a:impl, b:3);
                            },
                            set : ::(value){ 
                                topaz_display__set_parameter(a:impl, b:3, c:value);
                            }
                        },
                        root : {
                            get : ::(){ 
                                return topaz_display__get_root(a:impl, b:0);
                            },
                            set : ::(value){ 
                                topaz_display__set_root(a:impl, b:value.native);
                            }
                        },
                        x : {
                            get : ::(){ 
                                return topaz_display__get_parameter(a:impl, b:0);
                            },
                            set : ::(value){ 
                                topaz_display__set_parameter(a:impl, b:0, c:value);
                            }
                        },
        
                        y : {
                            get : ::(){ 
                                return topaz_display__get_parameter(a:impl, b:1);
                            },
                            set : ::(value){ 
                                topaz_display__set_parameter(a:impl, b:1, c:value);
                            }
                        },

                        active : {
                            get :: {
                                return topaz_display__get_parameter(a:impl, b:10) > 0.5;
                            },

                            set ::(value) {
                                topaz_display__set_parameter(a:impl, b:10, c:value);
                            }
                        },

                        focused : {
                            get :: {
                                return topaz_display__get_parameter(a:impl, b:9) > 0.5;
                            },
                            set ::(value) {
                                topaz_display__set_parameter(a:impl, b:9, c:value);
                            }
                        },

                        decorated : {
                            get :: {
                                return topaz_display__get_parameter(a:impl, b:11) > 0.5;
                            },
                            set ::(value) {
                                topaz_display__set_parameter(a:impl, b:11, c:value);
                            }

                        },
        
                        camera2d : {
                            get : ::(){ 
                                return __Topaz__.Entity.new(native:topaz_display__get_camera_2d(a:impl));
                            }
                        },
        
                        camera3d : {
                            get : ::(){ 
                                return __Topaz__.Entity.new(native:topaz_display__get_camera_3d(a:impl));
                            }
                        },
        
                        framebuffer : {
                            get : ::(){ 
                                return __Topaz__.Framebuffer.new(native:topaz_display__get_main_framebuffer(a:impl));
                            }
                        }
                    };
                }
            );
        };

        @__Framebuffer__ = ::<={
            @:topaz_framebuffer__get_width = getExternalFunction(name:'topaz_framebuffer__get_width');
            @:topaz_framebuffer__get_height = getExternalFunction(name:'topaz_framebuffer__get_height');
            @:topaz_framebuffer__resize = getExternalFunction(name:'topaz_framebuffer__resize');
            @:topaz_framebuffer__get_filtered_hint = getExternalFunction(name:'topaz_framebuffer__get_filtered_hint');
            @:topaz_framebuffer__set_filtered_hint = getExternalFunction(name:'topaz_framebuffer__set_filtered_hint');
            return class(
                name : 'Topaz.Framebuffer',        
                inherits : [__Native__],
                define : ::(this) { 
                    @impl;
                    
                    this.constructor = ::(native) {
                        impl = this.bindNative(instance:native);                        
                        return this;
                    };
        
                    this.interface = {
                        width : {
                            get : ::(){ 
                                return topaz_framebuffer__get_width(a:impl);
                            }
                        },
        
                        height : {
                            get : ::(){ 
                                return topaz_framebuffer__get_height(a:impl);
                            }
                        },
        
                        filteredHint : {
                            get : ::(){ 
                                return topaz_framebuffer__get_filtered_hint(a:impl);
                            },
                            set : ::(value){ 
                                return topaz_framebuffer__set_filtered_hint(a:impl, b:value);
                            }
                        },

        
                        resize : ::(width, height){ 
                            topaz_framebuffer__resize(a:impl, b:width, c:height);
                        }
                    };
                }
            );
        };

        @__Mesh__ = ::<={
            @:topaz_mesh__get_vertex_count = getExternalFunction(name:'topaz_mesh__get_vertex_count');
            @:topaz_mesh__set_vertex_count = getExternalFunction(name:'topaz_mesh__set_vertex_count');
            @:topaz_mesh__get_vertex = getExternalFunction(name:'topaz_mesh__get_vertex');
            @:topaz_mesh__set_vertex = getExternalFunction(name:'topaz_mesh__set_vertex');
            @:topaz_mesh__add_object = getExternalFunction(name:'topaz_mesh__add_object');
            @:topaz_mesh__set_object = getExternalFunction(name:'topaz_mesh__set_object');
            @:topaz_mesh__remove_object = getExternalFunction(name:'topaz_mesh__remove_object');
            @:topaz_mesh__define_vertices = getExternalFunction(name:'topaz_mesh__define_vertices');
            @:topaz_mesh__get_object_count = getExternalFunction(name:'topaz_mesh__get_object_count');

            return class(
                name : 'Topaz.Mesh',        
                inherits : [__Asset__],
                define : ::(this){ 
                    @impl;
                    this.constructor = ::(native){
                        this.constructor[__Asset__](native:native);
                        impl = this.native;
                        return this;
                    };    
                    this.interface = {
                        vertexCount : {
                            get : ::()       {return topaz_mesh__get_vertex_count(a:impl);},
                            set : ::(value)  {return topaz_mesh__set_vertex_count(a:impl, b:value);} 
                        },
        
        
                        vertices : {
                            set : ::(value){ 
                                topaz_mesh__define_vertices(a:impl, b:value); 
                            },
                            
                            get : ::(){ 
                                @out = {};
                                @len = this.vertexCount-1;
                                @iter = 0;
                                for(in:[0, len], do:::(i){
                                    out[iter] = topaz_mesh__get_vertex(a:impl, b:0, c:i, d:0); iter+=1;
                                    out[iter] = topaz_mesh__get_vertex(a:impl, b:0, c:i, d:1); iter+=1;
                                    out[iter] = topaz_mesh__get_vertex(a:impl, b:0, c:i, d:2); iter+=1;
                
                                    out[iter] = topaz_mesh__get_vertex(a:impl, b:1, c:i, d:0); iter+=1;
                                    out[iter] = topaz_mesh__get_vertex(a:impl, b:1, c:i, d:1); iter+=1;
                                    out[iter] = topaz_mesh__get_vertex(a:impl, b:1, c:i, d:2); iter+=1;
                
                                    out[iter] = topaz_mesh__get_vertex(a:impl, b:2, c:i, d:0); iter+=1;
                                    out[iter] = topaz_mesh__get_vertex(a:impl, b:2, c:i, d:1); iter+=1;
                
                                    out[iter] = topaz_mesh__get_vertex(a:impl, b:3, c:i, d:0); iter+=1;
                                    out[iter] = topaz_mesh__get_vertex(a:impl, b:3, c:i, d:1); iter+=1;
                                    out[iter] = topaz_mesh__get_vertex(a:impl, b:3, c:i, d:2); iter+=1;
                                    out[iter] = topaz_mesh__get_vertex(a:impl, b:3, c:i, d:3); iter+=1;
                                });
                                
                                return out;
                            }
                        },
        
                        setVertex : ::(i, value){ 
                            topaz_mesh__set_vertex(a:impl, b:i, c:value);
                        },
        
                        getVertex : ::(i){ 
                            @out = {};
                            @iter = 0;
                            out[iter] = topaz_mesh__get_vertex(a:impl, b:0, c:i, d:0); iter+=1;
                            out[iter] = topaz_mesh__get_vertex(a:impl, b:0, c:i, d:1); iter+=1;
                            out[iter] = topaz_mesh__get_vertex(a:impl, b:0, c:i, d:2); iter+=1;
        
                            out[iter] = topaz_mesh__get_vertex(a:impl, b:1, c:i, d:0); iter+=1;
                            out[iter] = topaz_mesh__get_vertex(a:impl, b:1, c:i, d:1); iter+=1;
                            out[iter] = topaz_mesh__get_vertex(a:impl, b:1, c:i, d:2); iter+=1;
        
                            out[iter] = topaz_mesh__get_vertex(a:impl, b:2, c:i, d:0); iter+=1;
                            out[iter] = topaz_mesh__get_vertex(a:impl, b:2, c:i, d:1); iter+=1;
        
                            out[iter] = topaz_mesh__get_vertex(a:impl, b:3, c:i, d:0); iter+=1;
                            out[iter] = topaz_mesh__get_vertex(a:impl, b:3, c:i, d:1); iter+=1;
                            out[iter] = topaz_mesh__get_vertex(a:impl, b:3, c:i, d:2); iter+=1;
                            out[iter] = topaz_mesh__get_vertex(a:impl, b:3, c:i, d:3); iter+=1;
                            return out;            
                        },
        
                        addObject : ::(indices){ 
                            topaz_mesh__add_object(a:impl);
                            if (object != empty) ::<={
                                topaz_mesh__set_object(a:this.objectCount-1, b:object);
                            };
                        },
        
                        objectCount : {
                            get : ::() { 
                                return topaz_mesh__get_object_count(a:impl);
                            }
                        },
        
                        removeObject : ::(i){ 
                            topaz_mesh__remove_object(a:impl, b:i);
                        },
        
                        setObject : ::(i, value){ 
                            topaz_mesh__set_object(a:impl, b:i, c:value);
                        }
                    };
                }
            );
        };

        @__Input__ = ::<={
            @:topaz_input__mouse_x = getExternalFunction(name:'topaz_input__mouse_x');
            @:topaz_input__mouse_y = getExternalFunction(name:'topaz_input__mouse_y');
            @:topaz_input__mouse_delta_x = getExternalFunction(name:'topaz_input__mouse_delta_x');
            @:topaz_input__mouse_delta_y = getExternalFunction(name:'topaz_input__mouse_delta_y');
            @:topaz_input__mouse_wheel = getExternalFunction(name:'topaz_input__mouse_wheel');
            @:topaz_input__add_keyboard_listener = getExternalFunction(name:'topaz_input__add_keyboard_listener');
            @:topaz_input__add_pad_listener = getExternalFunction(name:'topaz_input__add_pad_listener');
            @:topaz_input__add_pointer_listener = getExternalFunction(name:'topaz_input__add_pointer_listener');
            @:topaz_input__add_mapped_listener = getExternalFunction(name:'topaz_input__add_mapped_listener');
            @:topaz_input__remove_listener = getExternalFunction(name:'topaz_input__remove_listener');
            @:topaz_input__get_state = getExternalFunction(name:'topaz_input__get_state');
            @:topaz_input__get_pad_state = getExternalFunction(name:'topaz_input__get_pad_state');
            @:topaz_input__get_mapped_state = getExternalFunction(name:'topaz_input__get_mapped_state');
            @:topaz_input__set_deadzone = getExternalFunction(name:'topaz_input__set_deadzone');
            @:topaz_input__query_pad_count = getExternalFunction(name:'topaz_input__query_pad_count');
            @:topaz_input__query_pad_id = getExternalFunction(name:'topaz_input__query_pad_id');
            @:topaz_input__add_unicode_listener = getExternalFunction(name:'topaz_input__add_unicode_listener');
            @:topaz_input__remove_unicode_listener = getExternalFunction(name:'topaz_input__remove_unicode_listener');

            return class(
                name : 'Topaz.Input',        
                define : ::(this) {
                    @:constants = {
                        KEY : {
                            _0: 1, ///< 0
                            _1: 2, ///< 1
                            _2: 3, ///< 2
                            _3: 4, ///< 3
                            _4: 5, ///< 4
                            _5: 6, ///< 5
                            _6: 7, ///< 6
                            _7: 8, ///< 7
                            _8: 9, ///< 8
                            _9: 10, ///< 9
                            A: 11, ///< a
                            B: 12, ///< b
                            C: 13, ///< c
                            D: 14, ///< d
                            E: 15, ///< e
                            F: 16, ///< f
                            G: 17, ///< g
                            H: 18, ///< h
                            I: 19, ///< i
                            J: 20, ///< j
                            K: 21, ///< k
                            L: 22, ///< l
                            M: 23, ///< m
                            N: 24, ///< n
                            O: 25, ///< o
                            P: 26, ///< p
                            Q: 27, ///< q
                            R: 28, ///< r
                            S: 29, ///< s
                            T: 30, ///< t
                            U: 31, ///< u
                            V: 32, ///< v
                            W: 33, ///< w
                            X: 34, ///< x
                            Y: 35, ///< y
                            Z: 36, ///< z
                            L_SHIFT: 37, ///< Left shift key
                            R_SHIFT: 38, ///< Right shift key
                            L_CTRL: 39,  ///< Left control key
                            R_CTRL: 40,  ///< Right control key
                            L_ALT: 41,   ///< Left alt key
                            R_ALT: 42,   ///< Right alt key
                            TAB: 43,    ///< Tab
                            F1: 44,     ///< F1
                            F2: 45,     ///< F2
                            F3: 46,     ///< F3
                            F4: 47,     ///< F4
                            F5: 48,     ///< F5
                            F6: 49,     ///< F6
                            F7: 50,     ///< F7
                            F8: 51,     ///< F8
                            F9: 52,     ///< F9
                            F10: 53,    ///< F10
                            F11: 54,    ///< F11
                            F12: 55,    ///< F12
                            UP: 100,     ///< Up arrow
                            DOWN: 101,   ///< Down arrow
                            LEFT: 102,   ///< Left arrow
                            RIGHT: 103,  ///< Right arrow
                            MINUS: 104,  ///< -
                            EQUAL: 105,  ///< = 
                            BACKSPACE: 106,  ///< Backspace
                            GRAVE: 107,  ///< `
                            ESC: 108,    ///< Escape
                            HOME: 109,   ///< Home key
                            PAGE_UP: 110, ///< Page up key
                            PAGE_DOWN: 111,  ///< Page down key
                            END: 112,    ///< End key
                            BACKSLASH: 113, ///< '\'
                            L_BRACKET: 114, ///< [
                            R_BRACKET: 115, ///< ]
                            SEMICOLON: 116, ///< ;
                            APOSTROPHE: 117, ///< '
                            FRONTSLASH: 118, ///< /
                            ENTER: 119, ///< Enter
                            DELETE: 120, ///< Delete
                            NUMPAD_0: 121, ///< Numpad 0
                            NUMPAD_1: 122, ///< Numpad 1
                            NUMPAD_2: 123, ///< Numpad 2
                            NUMPAD_3: 124, ///< Numpad 3
                            NUMPAD_4: 125, ///< Numpad 4
                            NUMPAD_5: 126, ///< Numpad 5
                            NUMPAD_6: 127, ///< Numpad 6
                            NUMPAD_7: 128, ///< Numpad 7
                            NUMPAD_8: 129, ///< Numpad 8
                            NUMPAD_9: 130, ///< Numpad 9
                            PRINT_SCREEN: 131, ///< Print screen button
                            L_SUPER: 132, ///< Left Super key (Windows key)
                            R_SUPER: 133, ///< Right Super key (Windows key)
                            SPACE: 134,  ///< Space
                            INSERT: 135, ///< Insert key
                            COMMA: 136, ///< ,
                            PERIOD: 137 , ///< .
                            WORLD_1: 138, /// I8n key0
                            WORLD_2: 139, /// I8n key1
                            WORLD_3: 140, /// I8n key2
                            WORLD_4: 141, /// I8n key3
                            WORLD_5: 142, /// I8n key4
                            WORLD_6: 143, /// I8n key5
                            WORLD_7: 144, /// I8n key6
                            WORLD_8: 145, /// I8n key7
                            WORLD_9: 146 /// I8n key8
                        },



                        POINTER : {
                            _0: 256, ///< Left click
                            _1: 257, ///< Right click
                            _2: 258, ///< Middle click
                        
                            X: 259, ///< Horizontal axis. Usually for the X axis of the pointer
                            Y: 260, ///< Horizontal axis. Usually for the X axis of the pointer
                            WHEEL: 261 ///< Mouse wheel.
                        },

                        PAD : {
                        
                            A: 300,     ///< Button 0
                            B: 301,     ///< Button 1
                            C: 302,     ///< Button 2
                            X: 303,     ///< Button 3
                            Y: 304,     ///< Button 4
                            R: 305,     ///< Button 5
                            L: 306,     ///< Button 6
                            R2: 307,    ///< Button 7
                            L2: 308,    ///< Button 8
                            R3: 309,    ///< Button 9
                            L3: 310,    ///< Button 10
                            START: 311,    ///< Button 11
                            SELECT: 312,///< Button 12
                            B13: 313,///< Button 13
                            B14: 314,///< Button 14
                            B15: 315,///< Button 15
                            B16: 316,///< Button 16
                            B17: 317,///< Button 17
                            B18: 318,///< Button 18
                            B19: 319,///< Button 19
                            B20: 320,///< Button 20
                            B21: 321,///< Button 21
                            B22: 322,///< Button 22
                            B23: 323,///< Button 23
                            B24: 324,///< Button 24
                            B25: 325,///< Button 25
                            B26: 326,///< Button 26
                            B27: 327,///< Button 27
                            B28: 328,///< Button 28
                            B29: 329,///< Button 29
                            B30: 330,///< Button 30
                            B31: 331,///< Button 31
                            B32: 332,///< Button 32
                        
                            AXIS_X: 400, ///< X button
                            AXIS_Y: 401, ///< Y button
                            AXIS_Z: 402, ///< Z button
                            AXIS_X2: 403,///< X2 button 
                            AXIS_Y2: 404,///< Y2 button
                            AXIS_Z2: 405,///< Z2 button
                            AXIS_X3: 406,///< X3 button
                            AXIS_Y3: 407,///< Y3 button
                            AXIS_Z3: 408,///< Z3 button
                            AXIS_X4: 409,///< X4 button
                            AXIS_Y4: 410,///< Y4 button
                            AXIS_Z4: 411,///< Z4 button
                            AXIS_X5: 412,///< X4 button
                            AXIS_Y5: 413,///< Y4 button
                            AXIS_Z5: 414,///< Z4 button
                            AXIS_X6: 415,///< X4 button
                            AXIS_Y6: 416,///< Y4 button
                            AXIS_Z6: 417,///< Z4 button
                        
                            AXIS_R: 450,   
                            AXIS_L: 451,    
                            AXIS_R2: 452,    
                            AXIS_L2: 453,    
                            AXIS_R3: 454,    
                            AXIS_L3: 455,    
                            AXIS_R4: 456,    
                            AXIS_L4: 457,    
                        
                            OPTIONS: 511
                        }
                    };

                    
                


                    

                    this.interface = {
                        KEY : {
                            get :: {
                                return constants.KEY;
                            }
                        },

                        PAD : {
                            get :: {
                                return constants.PAD;
                            }
                        
                        },

                        POINTER : {
                            get :: {
                                return constants.POINTER;
                            }
                        },

                        DEFAULT_INPUT_COUNT: {                            
                            get ::(){return 512;}
                        },


                        mouse : {   
                            get : ::(){ 
                                return {x:topaz_input__mouse_x(), y:topaz_input__mouse_y()};
                            }
                        },
        
                        mouseDelta : {
                            get : ::(){ 
                                return {x:topaz_input__mouse_delta_x(), y:topaz_input__mouse_delta_y()};
                            } 
                        },
        
                        mouseWheel : {
                            get : ::() {return topaz_input__mouse_wheel(); } 
                        },
        
        
                        addKeyboardListener : ::(onPress, onActive, onRelease, onUpdate) { 
                            @onPressA   = if (onPress)   ::(a){onPress(input:a);} else empty;
                            @onActiveA  = if (onActive)  ::(a, b){onActive(input:a, value:b);} else empty;
                            @onReleaseA = if (onRelease) ::(a){onRelease(input:a);} else empty;
                            @onUpdateA  = if (onUpdate)  ::(a, b){onUpdate(input:a, value:b);} else empty; 
                                                     
                            return topaz_input__add_keyboard_listener(a:{onPress:onPressA, onActive:onActiveA, onRelease:onReleaseA, onUpdate:onUpdateA});
                        },
                        addPadListener : ::(onPress, onActive, onRelease, onUpdate, pad) { 
                            @onPressA   = if (onPress)   ::(a){onPress(input:a);} else empty;
                            @onActiveA  = if (onActive)  ::(a, b){onActive(input:a, value:b);} else empty;
                            @onReleaseA = if (onRelease) ::(a){onRelease(input:a);} else empty;
                            @onUpdateA  = if (onUpdate)  ::(a, b){onUpdate(input:a, value:b);} else empty; 

                            return topaz_input__add_pad_listener(a:{onPress:onPressA, onActive:onActiveA, onRelease:onReleaseA, onUpdate:onUpdateA}, b:pad);
                        },
                        addPointerListener : ::(onPress, onActive, onRelease, onUpdate) { 
                            @onPressA   = if (onPress)   ::(a){onPress(input:a);} else empty;
                            @onActiveA  = if (onActive)  ::(a, b){onActive(input:a, value:b);} else empty;
                            @onReleaseA = if (onRelease) ::(a){onRelease(input:a);} else empty;
                            @onUpdateA  = if (onUpdate)  ::(a, b){onUpdate(input:a, value:b);} else empty; 

                            return topaz_input__add_pointer_listener(a:{onPress:onPressA, onActive:onActiveA, onRelease:onReleaseA, onUpdate:onUpdateA});
                        },
                        addMappedListener : ::(onPress, onActive, onRelease, onUpdate, name) { 
                            @onPressA   = if (onPress)   ::(a){onPress(input:a);} else empty;
                            @onActiveA  = if (onActive)  ::(a, b){onActive(input:a, value:b);} else empty;
                            @onReleaseA = if (onRelease) ::(a){onRelease(input:a);} else empty;
                            @onUpdateA  = if (onUpdate)  ::(a, b){onUpdate(input:a, value:b);} else empty; 

                            return topaz_input__add_mapped_listener(a:{onPress:onPressA, onActive:onActiveA, onRelease:onReleaseA, onUpdate:onUpdateA}, b:name);
                        },
                        removeListener : ::(id) { 
                            topaz_input__remove_listener(a:id);
                        },
                
                
                        getState : ::(input) { 
                            return topaz_input__get_state(a:input);
                        },
                
                        getPadState : ::(pad, input) { 
                            return topaz_input__get_pad_state(a:input, b:pad);
                        },
                
                        getMappedState : ::(input) { 
                            return topaz_input__get_mapped_state(a:input);
                        },
                
                        setDeadzone : ::(pad, input, deadzone) { 
                            topaz_input__set_deadzone(a:pad, b:input, c:deadzone);
                        },
                
                        queryPads : ::() { 
                            @len = topaz_input__query_pad_count();
                            @out = [];
                            for(in:[0, len], do:::(i) {
                                out[i] = topaz_input__query_pad_id(a:i);
                            });
                            return out;
                        },
                
                        addUnicodeListener : ::(onNewUnicode, onRepeatUnicode) { 
                            if (onNewUnicode == empty)    onNewUnicode = ::(unicode){};
                            if (onRepeatUnicode == empty) onRepeatUnicode = ::(unicode){};
                            return topaz_input__add_unicode_listener(a:{
                                onNewUnicode::(a){onNewUnicode(unicode:a);},
                                onRepeatUnicode::(a){onRepeatUnicode(unicode:a);}
                            });
                        },
                
                        removeUnicodeListener : ::(id) { 
                            topaz_input__remove_unicode_listener(a:id);
                        }
                
                    };
                }
            ).new();
        };

        @__Audio__ = ::<={
            @:topaz_audio__playback_set_volume = getExternalFunction(name:'topaz_audio__playback_set_volume');
            @:topaz_audio__playback_set_panning = getExternalFunction(name:'topaz_audio__playback_set_panning');
            @:topaz_audio__playback_set_repeat = getExternalFunction(name:'topaz_audio__playback_set_repeat');
            @:topaz_audio__playback_pause = getExternalFunction(name:'topaz_audio__playback_pause');
            @:topaz_audio__playback_resume = getExternalFunction(name:'topaz_audio__playback_resume');
            @:topaz_audio__playback_seek = getExternalFunction(name:'topaz_audio__playback_seek');
            @:topaz_audio__playback_stop = getExternalFunction(name:'topaz_audio__playback_stop');

            @:topaz_audio__play_sound = getExternalFunction(name:'topaz_audio__play_sound');
            @:topaz_audio__channel_halt = getExternalFunction(name:'topaz_audio__channel_halt');
            @:topaz_audio__channel_set_volume = getExternalFunction(name:'topaz_audio__channel_set_volume');
            @:topaz_audio__channel_set_panning = getExternalFunction(name:'topaz_audio__channel_set_panning');



            return class(
                name : 'Topaz.Audio',        
                define : ::(this){ 
                    @ps = class(
                        name : 'Topaz.Audio.PlaybackSound',        
                        inherits : [__Native__],
                        define : ::(this) { 
                            @impl;
                            this.constructor = ::(native) {
                                @impl = this.bindNative(
                                    instance : native
                                );
                                return this;
                            };
        
        
                            this.interface = {
                                volume : {
                                    set : ::(value)  {topaz_audio__playback_set_volume(a:impl, b:value);} 
                                },
        
                                panning : {
                                    set : ::(value)  {topaz_audio__playback_set_panning(a:impl, b:value);} 
                                },
        
                                repeatSound : {
                                    set : ::(value)  {topaz_audio__playback_set_repeat(a:impl, b:value);} 
                                },
        
                                paused : {
                                    set : ::(value) { 
                                        if (Boolean(from:value)) ::<={ 
                                            topaz_audio__playback_pause(a:impl);
                                        } else ::<= {
                                            topaz_audio__playback_resume(a:impl);
                                        };
                                    }
                                },
        
                                seek : {
                                    set : ::(value) { 
                                        topaz_audio__playback_seek(a:impl, b:value);
                                    }
                                },
        
                                stop : ::(){ 
                                    topaz_audio__playback_stop(a:impl);
                                }
                            };
                        }
                    );
                    this.interface = {
                        // get class ref
                        PlaybackSound : {
                            get : ::(){ 
                                return ps;
                            }
                        },
                    
                        playSound : ::(asset, channel) { 
                            if (channel == empty) ::<={ 
                                return ps.new(native:topaz_audio__play_sound(a:asset.native, b:0));
                            } else ::<= {
                                return ps.new(native:topaz_audio__play_sound(a:asset.native, b:channel));
                            };
                        },
                        
                        channelHalt : ::(channel) { 
                            topaz_audio__channel_halt(a:channel);
                        },
                        
                        channelSetVolume : ::(channel, volume) { 
                            topaz_audio__channel_set_volume(a:channel, b:volume);
                        },
                
                        channelSetPanning : ::(channel, panning) { 
                            topaz_audio__channel_set_panning(a:channel, b:panning);
                        }
                    };
                }
            ).new();
        };

        @__Material__ = ::<={
            @:topaz_material__set_program_data = getExternalFunction(name:'topaz_material__set_program_data');
            @:topaz_material__get_program_data = getExternalFunction(name:'topaz_material__get_program_data');
            @:topaz_material__clear_sources = getExternalFunction(name:'topaz_material__clear_sources');
            @:topaz_material__set_program_source = getExternalFunction(name:'topaz_material__set_program_source');
            return class(
                name : 'Topaz.Material',        
                inherits : [__Asset__],
                define : ::(this){ 
                    @impl;
                    this.constructor = ::(native) {
                        this.constructor[__Asset__](native:native);
                        impl = this.native;
                        return this;
                    };
                    this.interface = {
                        setProgramData : ::(i, value){ 
                            topaz_material__set_program_data(a:impl, b:i, c:value);
                        },
        
                        getProgramData : ::(i){ 
                            return topaz_material__get_program_data(a:impl, b:i);
                        },
        
                        clearSources : ::(){ 
                            topaz_material__clear_sources(a:impl);
                        },
        
                        setProgramSource : ::(language, vertexShader, fragmentShader){ 
                            topaz_material__set_program_source(a:impl, b:language, c:vertexShader, d:fragmentShader); 
                        }
        
        
                    };
                }
            );
        };
        
        @__Image__ = ::<={
            @:topaz_image__get_width = getExternalFunction(name:'topaz_image__get_width');
            @:topaz_image__get_height = getExternalFunction(name:'topaz_image__get_height');
            @:topaz_image__get_frame_count = getExternalFunction(name:'topaz_image__get_frame_count');
            @:topaz_image__resize = getExternalFunction(name:'topaz_image__resize');
            @:topaz_image__add_frame = getExternalFunction(name:'topaz_image__add_frame');
            @:topaz_image__remove_frame = getExternalFunction(name:'topaz_image__remove_frame');
            @:topaz_image__frame_set_rgba = getExternalFunction(name:'topaz_image__frame_set_rgba');
            return class(
                name : 'Topaz.Image',        
                inherits : [__Asset__],
                define : ::(this){ 
                    @impl;
                    this.constructor = ::(native) {
                        this.constructor[__Asset__](native:native);
                        impl = this.native;
                        return this;
                    };
                    this.interface = {
                        width : {
                            get : ::()   {return topaz_image__get_width(a:impl);} 
                        },
                        
                        height : {
                            get : ::()   {return topaz_image__get_height(a:impl);}
                        },
                        
                        frameCount : {
                            get : ::()   {return topaz_image__get_frame_count(a:impl);} 
                        },
                        
                        resize : ::(width, height) { 
                            topaz_image__resize(a:impl, b:width, c:height);
                        },
                        
                        addFrame : ::() { 
                            topaz_image__add_frame(a:impl);
                        },
                        
                        removeFrame : ::(index) { 
                            topaz_image__remove_frame(a:impl, b:index);
                        },
                        
                        setRGBA : ::(index, rgbaData) { 
                            topaz_image__frame_set_rgba(a:impl, b:index, c:rgbaData);
                        }
                    };
                }
            );
        };
        @__Data__ = ::<={
            @:topaz_data__get_byte_count = getExternalFunction(name:'topaz_data__get_byte_count');
            @:topaz_data__get_as_string = getExternalFunction(name:'topaz_data__get_as_string');
            @:topaz_data__get_as_base64 = getExternalFunction(name:'topaz_data__get_as_base64');
            @:topaz_data__get_nth_byte = getExternalFunction(name:'topaz_data__get_nth_byte');
            @:topaz_data__set_from_bytes = getExternalFunction(name:'topaz_data__set_from_bytes');
            @:topaz_data__set_from_string = getExternalFunction(name:'topaz_data__set_from_string');
            @:topaz_data__set_from_base64 = getExternalFunction(name:'topaz_data__set_from_base64');
            return class(
                name : 'Topaz.Data',        
                inherits : [__Asset__],
                define : ::(this) { 
                    @impl;
                    this.constructor = ::(native){
                        this.constructor[__Asset__](native:native);
                        impl = this.native;
                        return this;
                    };
                    this.interface = {
                        byteCount : {
                            get : ::()   {return topaz_data__get_byte_count(a:impl);}
                        },
                        
                        string : {
                            get : ::()   {return topaz_data__get_as_string(a:impl);},
                            set ::(value) {
                                topaz_data__set_from_string(a:impl, b:value);
                            }
                        },

                        base64 : {
                            get : ::()   {return topaz_data__get_as_base64(a:impl);},
                            set ::(value) {
                                topaz_data__set_from_base64(a:impl, b:value);
                            }
                        },


                        bytes : {
                            get : ::() { 
                                @bytes = [];
                                @len = topaz_data__get_byte_count(a:impl);
                                for(in:[0, len], do:::(i) {
                                    bytes[i] = topaz_data__get_nth_byte(a:impl, b:i);
                                });
                                return bytes;
                            },

                            set : ::(value){ 
                                topaz_data__set_from_bytes(a:impl, b:value);
                            }
                        }
                    }; 
                }
            );
        };
        

        
        @__Sound__ = ::<={
            @:topaz_sound__get_sample_count = getExternalFunction(name:'topaz_sound__get_sample_count');
            @:topaz_sound__get_nth_sample_left = getExternalFunction(name:'topaz_sound__get_nth_sample_left');
            @:topaz_sound__get_nth_sample_right = getExternalFunction(name:'topaz_sound__get_nth_sample_right');
            @:topaz_sound__set_samples = getExternalFunction(name:'topaz_sound__set_samples');
            return class(
                name : 'Topaz.Sound',        
                inherits : [__Asset__],
                define : ::(this){ 
                    @impl = this.native;            
                    
                    this.interface = {
                        sampleCount : {
                            get : ::() {return topaz_sound__get_sample_count(a:impl);} 
                        },
                        
                        samples : {
                            get : ::()  { 
                                @out = [];
                                @iter = 0;
                                @len = this.sampleCount;
                                for(in:[0, len], do:::(i) {
                                    out[iter] = topaz_sound__get_nth_sample_left(a:impl,  b:i); iter+=1;
                                    out[iter] = topaz_sound__get_nth_sample_right(b:impl, b:i); iter+=1;
                                });
                                return out;
                            },
                            
                            
                            set : ::(v) { 
                                topaz_sound__set_samples(a:impl, b:v);
                            }
                        }
                    };
                }
            );
        };
        
        @:__Console__ = ::<= {
            @:topaz_console__enable = getExternalFunction(name:'topaz_console__enable');
            @:topaz_console__print_message = getExternalFunction(name:'topaz_console__print_message');
            @:topaz_console__add_listener = getExternalFunction(name:'topaz_console__add_listener');
            @:topaz_console__remove_listener = getExternalFunction(name:'topaz_console__remove_listener');
            @:topaz_console__run = getExternalFunction(name:'topaz_console__run');
            @:topaz_console__command_context_create = getExternalFunction(name:'topaz_console__command_context_create');
            @:topaz_console__command_context_destroy = getExternalFunction(name:'topaz_console__command_context_destroy');
            @:topaz_console__command_context_add_command = getExternalFunction(name:'topaz_console__command_context_add_command');
            @:topaz_console__command_context_set_default_handler = getExternalFunction(name:'topaz_console__command_context_set_default_handler');
            @:topaz_console__command_context_set_prompt = getExternalFunction(name:'topaz_console__command_context_set_prompt');
            @:topaz_console__push_command_context = getExternalFunction(name:'topaz_console__push_command_context');
            @:topaz_console__pop_command_context = getExternalFunction(name:'topaz_console__pop_command_context');
            
            
            @:CommandContext = class(
                name : 'Topaz.Console.CommandContext',
                define:::(this) {
                    @native;
                    this.constructor = ::(commands, prompt, defaultHandler) {
                        native = topaz_console__command_context_create();
                        if (commands != empty) ::<= {
                            foreach(in:commands, do:::(command, cb) {
                                this.addCommand(name:command, callback:cb);
                            });
                        };
                        
                        if (prompt != empty) this.prompt = prompt;
                        if (defaultHandler != empty) this.defaultHandler = defaultHandler;
                        return this;
                    };              
                    
                    this.interface = {
                        addCommand ::(name, callback) {
                            @ args = [];
                            @ argcount = 0;
                            
                            @:addArg::(a) {
                                args[argcount] = a;  
                                argcount+=1;
                            };
    
                            @:doAction::(a) {
                                @:res = callback(
                                    args : args,
                                    fullCommand : a
                                );
                                args = [];
                                argcount = 0;
                                when(res == empty) '';
                                return res;
                            };                   
                            
                            topaz_console__command_context_add_command(
                                a:native,
                                b:name,
                                c:addArg,
                                d:doAction
                            );
                        },
                        
                        native : {
                            get ::{
                                return native;
                            }
                        },
                        
                        defaultHandler : {
                            set ::(value) {
                                if (value == empty) 
                                    topaz_console__command_context_set_default_handler()                             
                                else ::<= {
                                    @ args = [];
                                    @ argcount = 0;
                                    
                                    @:addArg::(a) {
                                        args[argcount] = a;  
                                        argcount+=1;
                                    };
            
                                    @:doAction::(a) {
                                        @:res = value(
                                            args : args,
                                            fullCommand : a
                                        );
                                        args = [];
                                        argcount = 0;
                                        when(res == empty) '';
                                        return res;
                                    };                   

                                    topaz_console__command_context_set_default_handler(
                                        a:native,
                                        b:addArg,
                                        c:doAction
                                    );
                                }
                                ;
                            }
                        },
                        
                        destroy :: {
                            topaz_console__command_context_destroy(a:native);
                            native = empty;
                        },
                        
                        prompt : {
                            set ::(value) {
                                topaz_console__command_context_set_prompt(a:native, b:value);
                            }
                        }
                    };
                }
            );
            
            
            return class(
                name : 'Topaz.Console',
                define:::(this) {
                    @:messageType = {
                        NORMAL : 0,
                        DEBUG : 1,
                        WARNING : 2,
                        ERROR : 3
                    };
                
                    this.interface = {
                        MESSAGE_TYPE : {
                            get ::{
                                return messageType;
                            }
                        },
                        CommandContext : {
                            get ::{
                                return CommandContext;
                            }
                        },
                        
                        enabled : {
                            set ::(value) {
                                topaz_console__enable(a:value);
                            }
                        },
                        
                        'print' ::(message, type) {
                            if (type == empty)
                                type = 0;
                            topaz_console__print_message(a:message, b:type);
                        },
                        
                        addListener ::(callback) {
                            return topaz_console__add_listener(
                                a:::(a, b) {
                                    callback(text:a, type:b);
                                }
                            );
                        },
                        
                        removeListener ::(id) {
                            return topaz_console__remove_listener(a:id);
                        },
                        
                        run ::(command) {
                            topaz_console__run(a:command);
                        },
                        
                        pushContext ::(commands => CommandContext.type) {
                            topaz_console__push_command_context(a:commands.native);
                        },
                        
                        popContext :: {
                            topaz_console__pop_command_context();
                        }
                    };
                }            
            ).new();
        };
        
        @:__Resources__ = ::<={
            @:topaz_resources__fetch_asset = getExternalFunction(name:'topaz_resources__fetch_asset');
            @:topaz_resources__create_asset = getExternalFunction(name:'topaz_resources__create_asset');
            @:topaz_resources__create_data_asset_from_path = getExternalFunction(name:'topaz_resources__create_data_asset_from_path');
            @:topaz_resources__convert_asset = getExternalFunction(name:'topaz_resources__convert_asset');
            @:topaz_resources__write_asset = getExternalFunction(name:'topaz_resources__write_asset');
            @:topaz_resources__remove_asset = getExternalFunction(name:'topaz_resources__remove_asset');
            @:topaz_resources__is_extension_supported = getExternalFunction(name:'topaz_resources__is_extension_supported');
            @:topaz_resources__get_path = getExternalFunction(name:'topaz_resources__get_path');
            @:topaz_resources__set_path = getExternalFunction(name:'topaz_resources__set_path');
            @:topaz_resources__pack_bundle = getExternalFunction(name:'topaz_resources__pack_bundle');
            @:topaz_resources__unpack_bundle = getExternalFunction(name:'topaz_resources__unpack_bundle');
            @:topaz_resources__query_bundle = getExternalFunction(name:'topaz_resources__query_bundle');
            return class(
                name : 'Topaz.Resources',        
                define : ::(this){ 
                    @swtch = {};
                    swtch[__Asset__.TYPE.IMAGE] = ::(i){return __Topaz__.Image.new(native:i);}; 
                    swtch[__Asset__.TYPE.DATA] = ::(i){return __Topaz__.Data.new(native:i);};
                    swtch[__Asset__.TYPE.SOUND] = ::(i){return __Topaz__.Sound.new(native:i);}; 
                    swtch[__Asset__.TYPE.MATERIAL] = ::(i){return __Topaz__.Material.new(native:i);}; 
                    swtch[__Asset__.TYPE.MESH] = ::(i){return __Topaz__.Mesh.new(native:i);};

                    @_rawAssetToInstance = ::(impl) { 
                        when(impl == empty) empty;
                        @fn = swtch[topaz_asset__get_type(a:impl)];
                        when(fn == empty) __Topaz__.Asset.new(native:impl);
                        return fn(i:impl);
                    };
                
                
                    this.interface = {
                        fetchAsset : ::(name){ 
                            return _rawAssetToInstance(impl:topaz_resources__fetch_asset(a:name));
                        },

                        createAsset : ::(name, path, type) {
                            when(type != empty && path != empty) ::<= {
                                error(detail:'Cannot create asset with both a type and a data source! (bytes, base64, or a filesystem path)');                                
                            };


                            when (path != empty) ::<= {
                                return _rawAssetToInstance(impl:topaz_resources__create_data_asset_from_path(a:path, b:name));                            
                            };
                            if (type == empty) type = Topaz.Asset.TYPE.DATA;


                            return _rawAssetToInstance(impl:topaz_resources__create_asset(a:name, b:type));
                        },

                        convertAsset : ::(extension, asset) { 
                            return _rawAssetToInstance(impl:topaz_resources__convert_asset(a:extension, b:asset.native));
                        },



                        writeAsset : ::(asset, extension, path) { 
                            return topaz_resources__write_asset(a:asset.native, b:extension, c:path);
                        },

                        removeAsset : ::(asset) { 
                            topaz_resources__remove_asset(a:asset.native);
                        },

                        

                        isExtensionSupported : ::(extension) { 
                            return topaz_resources__is_extension_supported(a:extension);
                        },
                        
                        path : {
                            get : ::() { 
                                return topaz_resources__get_path();                        
                            },
                            
                            set : ::(value) { 
                                topaz_resources__set_path(a:value);             
                            }
                        },
                        
            
                        /*
                        {
                            name: 
                            version : {
                                major:
                                minor:
                                micro:
                            },                            
                            description:
                            author:
                            depends : [
                                {
                                    name:
                                    version: {
                                        major:
                                        minor:
                                    }
                                }                                  
                            ],
                            items : [
                                {
                                    name:
                                    extension:
                                    path: 
                                }                                
                            ]                            
                        }
                        */
                        createBundle ::(sourcePath, packageOut) {
                            @:pathIn = sourcePath;
                            @:pathOut = packageOut;
                            @:CREATE_PACKAGE_NAME = "__TOPAZ_PACKAGE_IN";




                            @:clearPackageAsset ::(path) {
                                @oldP = this.fetchAsset(name:CREATE_PACKAGE_NAME);
                                if (oldP != empty)
                                    this.removeAsset(asset:oldP);
                            };


                            @:pathInJSON = pathIn+'package.json'; 
                            clearPackageAsset();
                            @:json = this.createAsset(path:pathInJSON, name:CREATE_PACKAGE_NAME);
                            if (json == empty) error(detail:'Could not access ' + pathInJSON);

                            // will throw an error itself if cant decode.
                            @:opts = JSON.decode(string:json.string);


                            @:arg0 = [
                                CREATE_PACKAGE_NAME,
                                opts.name,
                                opts.version.major,
                                opts.version.minor,
                                opts.version.micro,
                                opts.description,
                                opts.author
                            ];
                            
                            @:arg1 = [];
                            if (opts.depends != empty) ::<= {
                                foreach(in:opts.depends, do:::(k, v) {
                                    // assume any (need to keep track of VERSION_ANY?)
                                    arg1[k] = if (v.version == empty) 
                                        [v.name, -1, -1]                              
                                    else 
                                        [
                                            v.name,
                                            v.version.major,
                                            v.version.minor
                                        ]
                                    ;
                                });
                            };
                            
                            // change the resource path to the package location.
                            @:oldPath = this.path;
                                                   
                            this.path = pathIn;
                            
                            @:arg2 = [];
                            foreach(in:opts.items, do:::(k, v) {
                                if (this.createAsset(path:v.path, name:v.name) == empty) ::<={
                                    this.path = oldPath; 
                                    error(detail:'Could not read bytes for sub-asset for package! Is it missing?');
                                };
                                arg2[k] = [
                                
                                    v.name,
                                    (if (v.extension == empty) "" else v.extension)
                                ];
                            });
                            this.path = oldPath;   
                            clearPackageAsset();
                            @:bundle = topaz_resources__pack_bundle(a:arg0, b:arg1, c:arg2);
                            
                            this.writeAsset(
                                asset:{native:bundle},
                                path:pathOut,
                                extension:""
                            );
                   
                        },
                        
                        require::(bundle, major, minor, onLoadItem) {
                            topaz_resources__unpack_bundle(
                                a:bundle,
                                b:major,
                                c:minor,
                                d: if(onLoadItem == empty) empty else ::(a) {
                                    onLoadItem(asset:_rawAssetToInstance(impl:a));
                                }
                            );                        
                        }
                        
                    };
                }
            ).new();
        };
        @__FontManager__ = ::<={
            @:topaz_font_manager__register_font = getExternalFunction(name:'topaz_font_manager__register_font');
            @:topaz_font_manager__preload_glyphs = getExternalFunction(name:'topaz_font_manager__preload_glyphs');
            @:topaz_font_manager__unregister_font = getExternalFunction(name:'topaz_font_manager__unregister_font');
            return class(
                name : 'Topaz.FontManager',        
                define : ::(this) { 
                    this.interface = {
                        registerFont : ::(asset){ 
                            topaz_font_manager__register_font(a:asset.native);
                        },

                        preloadGlyphs : ::(asset, sizeRequest, characters){ 
                            topaz_font_manager__preload_glyphs(a:asset.native, b:sizeRequest, c:characters);
                        },

                        unregisterFont : ::(asset){ 
                            topaz_font_manager__unregister_font(a:asset.native);
                        }        
                    };
                }
            ).new();
        };


                
        @:topaz_entity__remove_component = getExternalFunction(name:'topaz_entity__remove_component');
        @:topaz_entity__add_component = getExternalFunction(name:'topaz_entity__add_component');

        @__Entity__ = ::<={
            @:topaz_entity__is_valid = getExternalFunction(name:'topaz_entity__is_valid');
            @:topaz_entity__remove = getExternalFunction(name:'topaz_entity__remove');
            @:topaz_entity__get_child_count = getExternalFunction(name:'topaz_entity__get_child_count');
            @:topaz_entity__get_nth_child = getExternalFunction(name:'topaz_entity__get_nth_child');
            @:topaz_entity__detach = getExternalFunction(name:'topaz_entity__detach');
            @:topaz_entity__attach = getExternalFunction(name:'topaz_entity__attach');
            @:topaz_entity__step = getExternalFunction(name:'topaz_entity__step');
            @:topaz_entity__draw = getExternalFunction(name:'topaz_entity__draw');
            @:topaz_entity__get_parent = getExternalFunction(name:'topaz_entity__get_parent');
            @:topaz_entity__query = getExternalFunction(name:'topaz_entity__query');
            @:topaz_entity__search = getExternalFunction(name:'topaz_entity__search');
            @:topaz_entity__get_priority = getExternalFunction(name:'topaz_entity__get_priority');
            @:topaz_entity__set_priority = getExternalFunction(name:'topaz_entity__set_priority');
            @:topaz_entity__set_priority_last = getExternalFunction(name:'topaz_entity__set_priority_last');
            @:topaz_entity__set_priority_first = getExternalFunction(name:'topaz_entity__set_priority_first');
            @:topaz_entity__get_rotation_x = getExternalFunction(name:'topaz_entity__get_rotation_x');
            @:topaz_entity__get_rotation_y = getExternalFunction(name:'topaz_entity__get_rotation_y');
            @:topaz_entity__get_rotation_z = getExternalFunction(name:'topaz_entity__get_rotation_z');
            @:topaz_entity__get_position_x = getExternalFunction(name:'topaz_entity__get_position_x');
            @:topaz_entity__get_position_y = getExternalFunction(name:'topaz_entity__get_position_y');
            @:topaz_entity__get_position_z = getExternalFunction(name:'topaz_entity__get_position_z');
            @:topaz_entity__get_scale_x = getExternalFunction(name:'topaz_entity__get_scale_x');
            @:topaz_entity__get_scale_y = getExternalFunction(name:'topaz_entity__get_scale_y');
            @:topaz_entity__get_scale_z = getExternalFunction(name:'topaz_entity__get_scale_z');
            @:topaz_entity__set_rotation_x = getExternalFunction(name:'topaz_entity__set_rotation_x');
            @:topaz_entity__set_rotation_y = getExternalFunction(name:'topaz_entity__set_rotation_y');
            @:topaz_entity__set_rotation_z = getExternalFunction(name:'topaz_entity__set_rotation_z');
            @:topaz_entity__set_position_x = getExternalFunction(name:'topaz_entity__set_position_x');
            @:topaz_entity__set_position_y = getExternalFunction(name:'topaz_entity__set_position_y');
            @:topaz_entity__set_position_z = getExternalFunction(name:'topaz_entity__set_position_z');
            @:topaz_entity__set_scale_x = getExternalFunction(name:'topaz_entity__set_scale_x');
            @:topaz_entity__set_scale_y = getExternalFunction(name:'topaz_entity__set_scale_y');
            @:topaz_entity__set_scale_z = getExternalFunction(name:'topaz_entity__set_scale_z');
            @:topaz_entity__get_global_position_x = getExternalFunction(name:'topaz_entity__get_global_position_x');
            @:topaz_entity__get_global_position_y = getExternalFunction(name:'topaz_entity__get_global_position_y');
            @:topaz_entity__get_global_position_z = getExternalFunction(name:'topaz_entity__get_global_position_z');
            @:topaz_entity__is_stepping = getExternalFunction(name:'topaz_entity__is_stepping');
            @:topaz_entity__is_drawing = getExternalFunction(name:'topaz_entity__is_drawing');
            @:topaz_entity__get_drawing = getExternalFunction(name:'topaz_entity__get_drawing');
            @:topaz_entity__set_drawing = getExternalFunction(name:'topaz_entity__set_drawing');
            @:topaz_entity__get_stepping = getExternalFunction(name:'topaz_entity__get_stepping');
            @:topaz_entity__set_stepping = getExternalFunction(name:'topaz_entity__set_stepping');
            @:topaz_entity__get_name = getExternalFunction(name:'topaz_entity__get_name');
            @:topaz_entity__set_name = getExternalFunction(name:'topaz_entity__set_name');
            @:topaz_entity__add_component_after = getExternalFunction(name:'topaz_entity__add_component_after');
            @:topaz_entity__get_component_count = getExternalFunction(name:'topaz_entity__get_component_count');
            @:topaz_entity__get_nth_component = getExternalFunction(name:'topaz_entity__get_nth_component');
            @:topaz_entity__query_component = getExternalFunction(name:'topaz_entity__query_component');
            @:topaz_entity__set_on_step = getExternalFunction(name:'topaz_entity__set_on_step');
            @:topaz_entity__set_on_draw = getExternalFunction(name:'topaz_entity__set_on_draw');
            @:topaz_entity__set_on_pre_step = getExternalFunction(name:'topaz_entity__set_on_pre_step');
            @:topaz_entity__set_on_pre_draw = getExternalFunction(name:'topaz_entity__set_on_pre_draw');
            @:topaz_entity__set_on_attach = getExternalFunction(name:'topaz_entity__set_on_attach');
            @:topaz_entity__set_on_detach = getExternalFunction(name:'topaz_entity__set_on_detach');
            @:topaz_entity__set_on_remove = getExternalFunction(name:'topaz_entity__set_on_remove');
            @:topaz_entity__create = getExternalFunction(name:'topaz_entity__create');


            return class(
                name : 'Topaz.Entity',        
                inherits : [__Native__],
                define : ::(this)    { 
                    this.bindNative(
                        nativeCreate : topaz_entity__create
                    );
                    this.constructor = ::(native){
                        if (native != empty) ::<={
                            this.bindNative(
                                instance : native
                            );                        
                        };
                        return this;
                    };

                    @position;
                    @rotation;
                    @scale;
                    @globalPosition;
                    
                    this.interface = {
                        isValid : {
                            get : ::(){ 
                                return topaz_entity__is_valid(a:this.native) == 1;
                            }
                        },
                        
                        remove : ::(){ 
                            topaz_entity__remove(a:this.native);
                        },
                        
                        childCount : {
                            get : ::(){ 
                                return topaz_entity__get_child_count(a:this.native);
                            }
                        },
                        
                        nthChild : ::(n){ 
                            @f = topaz_entity__get_nth_child(a:this.native, b:n);
                            when (f == empty) empty;
                            when (f.__ctx != empty) f.__ctx;
                            return this.class.new(native:f);                
                        },
                        
                        
                        children : {
                            get : ::(){ 
                                @children = [];
                                @len = topaz_entity__get_child_count(a:this.native);
                                for(in:[0, len], do:::(i) { 
                                    children[i] = topaz_entity__get_nth_child(a:this.native, b:i).__ctx;
                                });
                                return children;
                            },
                            
                            set : ::(value){ 
                                loop(func:::{
                                    when(topaz_entity__get_child_count(a:this.native) == 0) false;
                                    topaz_entity__detach(a:topaz_entity__get_nth_child(a:this.native, b:0));
                                    return true;
                                });
                                    
                                for(in:[0, Object.length(of:value)], do:::(i) {
                                    topaz_entity__attach(a:this.native, b:value[i].native);
                                });
                            }
                        },
                        
                        
                        //TODO: the rest, also onStep, onDraw, etc <3
                        step : ::() { 
                            topaz_entity__step(a:this.native);
                        },
                        
                        draw : ::() { 
                            topaz_entity__draw(a:this.native);
                        },
        
                        attach ::(entity, component, addAfter){ 
                            when (entity != empty)
                               topaz_entity__attach(a:this.native, b:entity.native);
                               
                            when (component != empty)
                                if (addAfter != empty && addAfter == true) 
                                    topaz_entity__add_component_after(a:this.native, b:component.native);
                                else 
                                    topaz_entity__add_component(a:this.native, b:component.native);
                        },
        
                        detach : ::() { 
                            topaz_entity__detach(a:this.native);
                        },
                        
                        parent : {
                            get : ::() { 
                                @f = topaz_entity__get_parent(a:this.native);
                                when (f == empty) empty;
                                when (f.__ctx != empty) f.__ctx;
                                return this.class.new(native:f);
                            },
                            
                            set : ::(value){ 
                                topaz_entity__attach(a:value.native, b:this.native);
                            }
                        },
                        
                        query : ::(name) { 
                            @f = topaz_entity__query(a:this.native, b:name);
                            when (f == empty) empty;
                            when (f.__ctx != empty) f.__ctx;
                            return this.class.new(native:f);
                        },
                        
                        
                        search : ::(name){ 
                            @f = topaz_entity__search(a:this.native, b:name);
                            when (f == empty) empty;
                            when (f.__ctx != empty) f.__ctx;
                            return this.class.new(native:f);
                        },
                        
                        priority : {
                            get : ::() {return topaz_entity__get_priority(a:this.native);}, 
                            set : ::(value){topaz_entity__set_priority(a:this.native, b:value);}
                        },
                        
            
                        setPriorityLast : ::(){ 
                            topaz_entity__set_priority_last(a:this.native);
                        },
            
                        setPriorityFirst : ::(){ 
                            topaz_entity__set_priority_first(a:this.native);
                        },
        
                        rotation : {
                            get : ::()  {
                                if (rotation == empty)
                                    rotation = ManagedVector.new(
                                        native_:this.native,
                                        getx: topaz_entity__get_rotation_x,
                                        gety: topaz_entity__get_rotation_y,
                                        getz: topaz_entity__get_rotation_z,

                                        setx: topaz_entity__set_rotation_x,
                                        sety: topaz_entity__set_rotation_y,
                                        setz: topaz_entity__set_rotation_z
                                    );


                                return rotation;
                            },
                            set : ::(value) {
                                if (value.x != empty) topaz_entity__set_rotation_x(a:this.native, b:value.x);
                                if (value.y != empty) topaz_entity__set_rotation_y(a:this.native, b:value.y);
                                if (value.z != empty) topaz_entity__set_rotation_z(a:this.native, b:value.z);
                            } 
                        },

        
                        position : {
                            get : ::()  {
                                if (position == empty)
                                    position = ManagedVector.new(
                                        native_:this.native,
                                        getx: topaz_entity__get_position_x,
                                        gety: topaz_entity__get_position_y,
                                        getz: topaz_entity__get_position_z,

                                        setx: topaz_entity__set_position_x,
                                        sety: topaz_entity__set_position_y,
                                        setz: topaz_entity__set_position_z
                                    );


                                return position;
                            },
                            set : ::(value) {
                                if (value.x != empty) topaz_entity__set_position_x(a:this.native, b:value.x);
                                if (value.y != empty) topaz_entity__set_position_y(a:this.native, b:value.y);
                                if (value.z != empty) topaz_entity__set_position_z(a:this.native, b:value.z);
                            } 
                        },
                        
                        scale : {
                            get : ::()  {
                                if (scale == empty)
                                    scale = ManagedVector.new(
                                        native_:this.native,
                                        getx: topaz_entity__get_scale_x,
                                        gety: topaz_entity__get_scale_y,
                                        getz: topaz_entity__get_scale_z,

                                        setx: topaz_entity__set_scale_x,
                                        sety: topaz_entity__set_scale_y,
                                        setz: topaz_entity__set_scale_z
                                    );


                                return scale;
                            },
                            set : ::(value) {
                                if (value.x != empty) topaz_entity__set_scale_x(a:this.native, b:value.x);
                                if (value.y != empty) topaz_entity__set_scale_y(a:this.native, b:value.y);
                                if (value.z != empty) topaz_entity__set_scale_z(a:this.native, b:value.z);
                            }                         
                        },
                        
                        globalPosition : {
                            get :: {
                                if (globalPosition == empty)
                                    globalPosition = ManagedVector.new(
                                        native_:this.native,
                                        getx: topaz_entity__get_global_position_x,
                                        gety: topaz_entity__get_global_position_y,
                                        getz: topaz_entity__get_global_position_z
                                    );


                                return globalPosition;
                            }
                        },

                        isStepping : {
                            get : ::() {return topaz_entity__is_stepping(a:this.native);}
                        },
                        
                        isDrawing : {
                            get : ::() {return topaz_entity__is_drawing(a:this.native); } 
                        },
                        
                        stepping : {
                            get : ::()  {return topaz_entity__get_stepping(a:this.native);},
                            set : ::(value) {return topaz_entity__set_stepping(a:this.native, b:value);} 
                        },
                        
                        drawing : {
                            get : ::()  {return topaz_entity__get_drawing(a:this.native);}, 
                            set : ::(value) {return topaz_entity__set_drawing(a:this.native, b:value);} 
                        },
                        
                        name : {
                            get : ::()  {return topaz_entity__get_name(a:this.native);},
                            set : ::(value) {return topaz_entity__set_name(a:this.native, b:value);} 
                        },
        
                    
                            
                        components : {
                            get : ::(){ 
                                @len = topaz_entity__get_component_count(a:this.native);
                                @out = [];
                                for(in:[0, len], do:::(i) {
                                    @f = topaz_entity__get_nth_component(a:this.native, b:i);
                                    when(f == empty) out[i] = empty;
                                    out[i] = if (f.__ctx != empty) f.__ctx else __Topaz__.Component.new(native:f);
                                });
                                return out;
                            },
                            set : ::(value){ 
                                loop(func:::{
                                    when(topaz_entity__get_component_count(a:this.native) == 0) false;
                                    topaz_entity__remove_component(a:this.native, b:topaz_entity__get_nth_component(a:this.native, b:0));
                                    return true;
                                });
        
                                for(in:[0, Object.length(of:value)], do:::(i) {
                                    topaz_entity__add_component(a:this.native, b:value[i].native);
                                });
                            }
                        },
        
        
                        queryComponent : ::(tag) { 
                            @f = topaz_entity__query_component(a:this.native, b:tag);
                            when (f == empty) empty;
                            when (f.__ctx != empty) f.__ctx;
                            return __Topaz__.Component.new(native:f);
                        },
                    
                        remove : ::(component){ 
                            return topaz_entity__remove_component(a:this.native, b:component.native);
                        },
                        
                        onStep : {set : ::(value){ 
                                topaz_entity__set_on_step(a:this.native, b:value);
        
                        }},
        
                        onDraw : {set : ::(value){ 
                                topaz_entity__set_on_draw(a:this.native, b:value);
                        }},
        
                        onPreStep : {set : ::(value){ 
                                topaz_entity__set_on_pre_step(a:this.native, b:value);
                        }},
        
                        onPreDraw : {set : ::(value){ 
                                topaz_entity__set_on_pre_draw(a:this.native, b:value);
                        }},
                        
                        onAttach : {set : ::(value){ 
                                topaz_entity__set_on_attach(a:this.native, b:value);
                        }},
        
                        onDetach : {set : ::(value){ 
                                topaz_entity__set_on_detach(a:this.native, b:value);
                        }},
        
                        onRemove : {set : ::(value){ 
                                topaz_entity__set_on_remove(a:this.native, b:value);
                        }}
        
                        
        
        
                    
                    };        
                }    
            );
        };

        @__Component__ = ::<= {
            @:topaz_component__destroy = getExternalFunction(name:'topaz_component__destroy');
            @:topaz_component__step = getExternalFunction(name:'topaz_component__step');
            @:topaz_component__draw = getExternalFunction(name:'topaz_component__draw');
            @:topaz_component__get_stepping = getExternalFunction(name:'topaz_component__get_stepping');
            @:topaz_component__set_stepping = getExternalFunction(name:'topaz_component__set_stepping');
            @:topaz_component__get_drawing = getExternalFunction(name:'topaz_component__get_drawing');
            @:topaz_component__set_drawing = getExternalFunction(name:'topaz_component__set_drawing');
            @:topaz_component__get_tag = getExternalFunction(name:'topaz_component__get_tag');
            @:topaz_component__set_tag = getExternalFunction(name:'topaz_component__set_tag');
            @:topaz_component__get_host = getExternalFunction(name:'topaz_component__get_host');
            @:topaz_component__emit_event_anonymous = getExternalFunction(name:'topaz_component__emit_event_anonymous');
            @:topaz_component__emit_event = getExternalFunction(name:'topaz_component__emit_event');
            @:topaz_component__can_handle_event = getExternalFunction(name:'topaz_component__can_handle_event');
            @:topaz_component__install_event = getExternalFunction(name:'topaz_component__install_event');
            @:topaz_component__uninstall_event = getExternalFunction(name:'topaz_component__uninstall_event');
            @:topaz_component__install_hook = getExternalFunction(name:'topaz_component__install_hook');
            @:topaz_component__uninstall_hook = getExternalFunction(name:'topaz_component__uninstall_hook');
            @:topaz_component__install_handler = getExternalFunction(name:'topaz_component__install_handler');
            @:topaz_component__uninstall_handler = getExternalFunction(name:'topaz_component__uninstall_handler');
            @:topaz_component__set_on_step = getExternalFunction(name:'topaz_component__set_on_step');
            @:topaz_component__set_on_draw = getExternalFunction(name:'topaz_component__set_on_draw');
            @:topaz_component__set_on_attach = getExternalFunction(name:'topaz_component__set_on_attach');
            @:topaz_component__set_on_detach = getExternalFunction(name:'topaz_component__set_on_detach');
            @:topaz_component__set_on_destroy = getExternalFunction(name:'topaz_component__set_on_destroy');
            @:topaz_component__create = getExternalFunction(name:'topaz_component__create');
            @:nativeToE = ::(native) {
                when (native == empty) empty;
                when (native.__ctx != empty) native.__ctx;
                return __Entity__.new(native:native);
            };
            return class(
                name : 'Topaz.Component',        
                inherits : [__Native__],
                define : ::(this){ 
                    this.bindNative(
                        nativeCreate : topaz_component__create
                    );

                    // some classes will inherit and overwrite the native instance, so a 
                    // @impl doesnt make sense.
                    this.constructor = ::(native, tag) {
                        if (native != empty) ::<= {
                            this.bindNative(
                                instance : native,
                                args: [tag]
                            );
                        };
                        return this;
                    };
                    
        
                    this.interface = {
                        destroy : ::(){ 
                            topaz_component__destroy(a:this.native);
                        },
                        
                        step : ::(){ 
                            topaz_component__step(a:this.native);
                        },
                        
                        draw : ::(){ 
                            topaz_component__draw(a:this.native);
                        },
                        
                        stepping : {
                            get : ::()  {return topaz_component__get_stepping(a:this.native);},
                            set : ::(value) {topaz_component__set_stepping(a:this.native, b:value);} 
                        },
                        
                        drawing : {
                            get : ::()  {return topaz_component__get_drawing(a:this.native);},
                            set : ::(value) {topaz_component__set_drawing(a:this.native, b:value);} 
                        },
                        
                        tag : {
                            get : ::()  {return topaz_component__get_tag(a:this.native);}, 
                            set : ::(value) {topaz_component__set_tag(a:this.native, b:value);} 
                        },
                        
                        host : {
                            get : ::(){ 
                                return nativeToE(native:topaz_component__get_host(a:this.native));
                            },

                            set : ::(v) {
                                @:p = topaz_component__get_host(a:this.native);
                                if (p != empty) ::<={
                                    topaz_entity__remove_component(a:p, b:this.native);
                                };
                                if (v != empty) ::<={
                                    topaz_entity__add_component(a:v.native, b:this.native);
                                };
                            }
                        },
                        
                        emit : ::(event, source){ 
                            if (source == empty) ::<={
                                return topaz_component__emit_event_anonymous(a:this.native, b:event);
                            } else ::<={
                                return topaz_component__emit_event(a:this.native, b:event, c:source.native);
                            };
                        },
                        
                        canHandleEvent : ::(name){ 
                            return topaz_component__can_handle_event(a:this.native, b:name);
                        },
                        
                        installEvent : ::(event, callback){ 
                            @cb = if (callback != empty) ::(a){callback(source:nativeToE(native:a));} else ::(a){};                                 
                            topaz_component__install_event(a:this.native, b:event, c:cb);
                        },
                        
                        uninstallEvent : ::(event){ 
                            topaz_component__uninstall_event(a:this.native, b:event);
                        },
                        
                        installHook : ::(event, callback){ 
                            @cb = if (callback != empty) ::(a){callback(source:nativeToE(native:a));} else ::(a){};                                 
                            return topaz_component__install_hook(a:this.native, b:event, c:cb);
                        },
                        
                        uninstallHook : ::(event, id){ 
                            topaz_component__uninstall_hook(a:this.native, b:event, c:id);
                        },
                        
                        installHandler : ::(event, callback){ 
                            @cb = if (callback != empty) ::(a){callback(source:nativeToE(native:a));} else ::(a){};                                 
                            return topaz_component__install_handler(a:this.native, b:event, c:cb);
                        },
                        
                        uninstallHandler : ::(event, id){ 
                            topaz_component__uninstall_handler(a:this.native, b:event, c:id);
                        },
                        
                        onStep : {set : ::(value){ 
                            topaz_component__set_on_step(a:this.native, b:value);
        
                        }},
        
                        onDraw : {set : ::(value){ 
                                topaz_component__set_on_draw(a:this.native, b:value);
                        }},
                            
                        onAttach : {set : ::(value){ 
                                topaz_component__set_on_attach(a:this.native, b:value);
                        }},
        
                        onDetach : {set : ::(value){ 
                                topaz_component__set_on_detach(a:this.native, b:value);
                        }},
        
                        onDestroy : {set : ::(value){ 
                                topaz_component__set_on_destroy(a:this.native, b:value);
                        }}

                    };
                }
            );
        };

        
        
        @__Text2D__ = ::<={
            @:topaz_text2d__create = getExternalFunction(name:'topaz_text2d__create');
            @:topaz_text2d__get_text = getExternalFunction(name:'topaz_text2d__get_text');
            @:topaz_text2d__set_text = getExternalFunction(name:'topaz_text2d__set_text');
            @:topaz_text2d__set_font = getExternalFunction(name:'topaz_text2d__set_font');
            @:topaz_text2d__set_color = getExternalFunction(name:'topaz_text2d__set_color');
            @:topaz_text2d__set_color_section = getExternalFunction(name:'topaz_text2d__set_color_section');
            @:topaz_text2d__get_extent_width = getExternalFunction(name:'topaz_text2d__get_extent_width');
            @:topaz_text2d__get_extent_height = getExternalFunction(name:'topaz_text2d__get_extent_height');
            @:topaz_text2d__get_char_x = getExternalFunction(name:'topaz_text2d__get_char_x');
            @:topaz_text2d__get_char_y = getExternalFunction(name:'topaz_text2d__get_char_y');
            @:topaz_text2d__get_position_x = getExternalFunction(name:'topaz_text2d__get_position_x');
            @:topaz_text2d__get_position_y = getExternalFunction(name:'topaz_text2d__get_position_y');
            @:topaz_text2d__get_position_z = getExternalFunction(name:'topaz_text2d__get_position_z');
            @:topaz_text2d__set_position_x = getExternalFunction(name:'topaz_text2d__set_position_x');
            @:topaz_text2d__set_position_y = getExternalFunction(name:'topaz_text2d__set_position_y');
            @:topaz_text2d__set_position_z = getExternalFunction(name:'topaz_text2d__set_position_z');
            @:topaz_text2d__set_scale_x = getExternalFunction(name:'topaz_text2d__set_scale_x');
            @:topaz_text2d__set_scale_y = getExternalFunction(name:'topaz_text2d__set_scale_y');
            @:topaz_text2d__set_scale_z = getExternalFunction(name:'topaz_text2d__set_scale_z');
            @:topaz_text2d__get_scale_x = getExternalFunction(name:'topaz_text2d__get_scale_x');
            @:topaz_text2d__get_scale_y = getExternalFunction(name:'topaz_text2d__get_scale_y');
            @:topaz_text2d__get_scale_z = getExternalFunction(name:'topaz_text2d__get_scale_z');
            @:topaz_text2d__set_rotation_x = getExternalFunction(name:'topaz_text2d__set_rotation_x');
            @:topaz_text2d__set_rotation_y = getExternalFunction(name:'topaz_text2d__set_rotation_y');
            @:topaz_text2d__set_rotation_z = getExternalFunction(name:'topaz_text2d__set_rotation_z');
            @:topaz_text2d__get_rotation_x = getExternalFunction(name:'topaz_text2d__get_rotation_x');
            @:topaz_text2d__get_rotation_y = getExternalFunction(name:'topaz_text2d__get_rotation_y');
            @:topaz_text2d__get_rotation_z = getExternalFunction(name:'topaz_text2d__get_rotation_z');
            @:topaz_text2d__set_attribute = getExternalFunction(name:'topaz_text2d__set_attribute');
            @:topaz_text2d__get_attribute = getExternalFunction(name:'topaz_text2d__get_attribute');

            return class(
                name : 'Topaz.Text2D',        
                inherits :[__Component__],
                define : ::(this){ 
                    @impl;
                    this.constructor = ::(native) {
                        // whoops, the component constructor already made a generic component native.
                        // destroy it and make a real one
                        this.native.__ctx.destroy();

                        impl = this.bindNative(
                            instance : native,
                            nativeCreate : topaz_text2d__create
                        );
                        return this;
                    };
                    
                    @fontState = {};
                    @sizeState;
                    
                    @position;
                    @scale;
                    @rotation;



                    this.interface = {
                        text : {
                            get : ::() {return topaz_text2d__get_text(a:impl);},
                            set : ::(value){ topaz_text2d__set_text(a:impl, b:value);} 
                        },
                        
                        font : {
                            get : ::()  {return fontState;},
                            set : ::(value) { 
                                fontState = value;
                                topaz_text2d__set_font(a:impl, b:fontState.native, c:sizeState);
                            }
                        },
                        
                        size :  {
                            get : ::()  {return sizeState;},
                            set : ::(value) { 
                                sizeState = value;
                                topaz_text2d__set_font(a:impl, b:fontState.native, c:sizeState);
                            }
                        },
                        
                        color :{
                            set : ::(value) {
                                if (getType(of:value) == String) ::<={
                                    value = Color.parse(string:value);
                                };

                                topaz_text2d__set_color(a:this.native, b:value.r, c:value.g, d:value.b, e:value.a);
                            } 
                        },


                        setColorSection : ::(from, to, color) { 
                            if (getType(of:color) == String) ::<={
                                color = Color.parse(string:color);
                            };

                            
                            topaz_text2d__set_color_section(
                                a:impl, b:from, c:to, 
                                d:color.r, e:color.g, f:color.b, g:color.a
                            );
                        },
                        
                        extentWidth : {
                            get : ::() {return topaz_text2d__get_extent_width(a:impl);} 
                        },
                        
                        extentHeight : {
                            get : ::() {return topaz_text2d__get_extent_height(a:impl);} 
                        },
                        
                        getCharX : ::(index) { 
                            return topaz_text2d__get_char_x(a:impl, b:index);
                        },
                        
                        getCharY : ::(index) { 
                            return topaz_text2d__get_char_y(a:impl, b:index);
                        },
                        
                        rotation : {
                            get : ::()  {
                                if (rotation == empty)
                                    rotation = ManagedVector.new(
                                        native_:this.native,
                                        getx: topaz_text2d__get_rotation_x,
                                        gety: topaz_text2d__get_rotation_y,
                                        getz: topaz_text2d__get_rotation_z,

                                        setx: topaz_text2d__set_rotation_x,
                                        sety: topaz_text2d__set_rotation_y,
                                        setz: topaz_text2d__set_rotation_z
                                    );


                                return rotation;
                            },
                            set : ::(value) {
                                if (value.x != empty) topaz_text2d__set_rotation_x(a:this.native, b:value.x);
                                if (value.y != empty) topaz_text2d__set_rotation_y(a:this.native, b:value.y);
                                if (value.z != empty) topaz_text2d__set_rotation_z(a:this.native, b:value.z);
                            } 
                        },

        
                        position : {
                            get : ::()  {
                                if (position == empty)
                                    position = ManagedVector.new(
                                        native_:this.native,
                                        getx: topaz_text2d__get_position_x,
                                        gety: topaz_text2d__get_position_y,
                                        getz: topaz_text2d__get_position_z,

                                        setx: topaz_text2d__set_position_x,
                                        sety: topaz_text2d__set_position_y,
                                        setz: topaz_text2d__set_position_z
                                    );


                                return position;
                            },
                            set : ::(value) {
                                if (value.x != empty) topaz_text2d__set_position_x(a:this.native, b:value.x);
                                if (value.y != empty) topaz_text2d__set_position_y(a:this.native, b:value.y);
                                if (value.z != empty) topaz_text2d__set_position_z(a:this.native, b:value.z);
                            } 
                        },
                        
                        scale : {
                            get : ::()  {
                                if (scale == empty)
                                    scale = ManagedVector.new(
                                        native_:this.native,
                                        getx: topaz_text2d__get_scale_x,
                                        gety: topaz_text2d__get_scale_y,
                                        getz: topaz_text2d__get_scale_z,

                                        setx: topaz_text2d__set_scale_x,
                                        sety: topaz_text2d__set_scale_y,
                                        setz: topaz_text2d__set_scale_z
                                    );


                                return scale;
                            },
                            set : ::(value) {
                                if (value.x != empty) topaz_text2d__set_scale_x(a:this.native, b:value.x);
                                if (value.y != empty) topaz_text2d__set_scale_y(a:this.native, b:value.y);
                                if (value.z != empty) topaz_text2d__set_scale_z(a:this.native, b:value.z);
                            }                         
                        },
                        
                        setAttributes : ::(primitive, alphaRule, depthTest, etchRule, textureFilterHint){
                            if (primitive != empty) topaz_text2d__set_attribute(a:impl, b:0, c:primitive);
                            if (alphaRule != empty) topaz_text2d__set_attribute(a:impl, b:1, c:alphaRule);
                            if (depthTest != empty) topaz_text2d__set_attribute(a:impl, b:2, c:depthTest);
                            if (etchRule  != empty) topaz_text2d__set_attribute(a:impl, b:3, c:etchRule);
                            if (textureFilterHint != empty) topaz_text2d__set_attribute(a:impl, b:4, c:textureFilterHint);

                            /*PRIMITIVE : 0,
                            ALPHA_RULE : 1,
                            DEPTH_TEST : 2,
                            ETCH_RULE : 3,
                            TEXTURE_FILTER_HINT : 4*/

                        },
                        
                        getAttribute : ::(attribute){ 
                            return topaz_text2d__get_attribute(a:impl, b:attribute);
                        }
                    };

                }
            );
        };

        @__Scheduler__ = ::<={
            @:topaz_scheduler__create = getExternalFunction(name:'topaz_scheduler__create');
            @:topaz_scheduler__pause = getExternalFunction(name:'topaz_scheduler__pause');
            @:topaz_scheduler__resume = getExternalFunction(name:'topaz_scheduler__resume');
            @:topaz_scheduler__reset = getExternalFunction(name:'topaz_scheduler__reset');
            @:topaz_scheduler__get_task_interval_remaining = getExternalFunction(name:'topaz_scheduler__get_task_interval_remaining');


            return class(
                name : 'Topaz.Scheduler',        
                inherits :[__Component__],
                statics : {
                    MODE : {
                        TIME : 0,
                        FRAME : 1
                    }
                },
                define : ::(this){ 
                    @impl;
                    this.constructor = ::(mode, justOnce, interval, intervalDelay, callback, native) {
                        // whoops, the component constructor already made a generic component native.
                        // destroy it and make a real one
                        this.native.__ctx.destroy();

                        impl = this.bindNative(
                            instance : native,
                            nativeCreate : topaz_scheduler__create,
                            args : [mode, if (justOnce == empty) false else justOnce , interval, intervalDelay, callback]
                        );
                        return this;
                    };
                    
                    
                    this.interface = {
                    
                        pause : ::(){ 
                            topaz_scheduler__pause(a:impl);
                        },
                        
                        resume : ::() { 
                            topaz_scheduler__resume(a:impl);
                        },

                        reset ::() {
                            topaz_scheduler__reset(a:impl);

                        },
                        
                        getTaskIntervalRemaining : ::(name){ 
                            return topaz_scheduler__get_task_interval_remaining(a:impl);
                        }
                    };
                } 
            );
        };
        @__StateControl__ = ::<={
            @:topaz_state_control__create = getExternalFunction(name:'topaz_state_control__create');
            @:topaz_state_control__add = getExternalFunction(name:'topaz_state_control__add');
            @:topaz_state_control__remove = getExternalFunction(name:'topaz_state_control__remove');
            @:topaz_state_control__execute = getExternalFunction(name:'topaz_state_control__execute');
            @:topaz_state_control__halt = getExternalFunction(name:'topaz_state_control__halt');
            @:topaz_state_control__is_halted = getExternalFunction(name:'topaz_state_control__is_halted');
            @:topaz_state_control__get_current = getExternalFunction(name:'topaz_state_control__get_current');
            return class(
                name : 'Topaz.StateControl',        
                inherits :[__Component__],
                define : ::(this){ 
                    @impl;
                    this.constructor = ::(native) {
                        // whoops, the component constructor already made a generic component native.
                        // destroy it and make a real one
                        this.native.__ctx.destroy();

                        impl = this.bindNative(
                            instance : native,
                            nativeCreate : topaz_state_control__create
                        );
                        return this;
                    };
                    
                    this.interface = {
                        add : ::(name, onStep, onDraw, onInit){ 
                            topaz_state_control__add(a:impl, b:name, c:onStep, d:onDraw, e:onInit);
                        },
                        
                        remove : ::(name){ 
                            topaz_state_control__remove(a:impl, b:name);
                        },
                        
                        execute : ::(name){ 
                            topaz_state_control__execute(a:impl, b:name);
                        },
                        
                        halt : ::(){ 
                            topaz_state_control__halt(a:impl);
                        },
                        
                        isHalted : {
                            get : ::(){ 
                                return topaz_state_control__is_halted(a:impl);
                            }
                        },
                        
                        state : {
                            get : ::(){ 
                                return topaz_state_control__get_current(a:impl);
                            }
                        }                    
                        
                               
                    };
                }
            );
        };
        @__Object2D__ = ::<={
            @:topaz_object2d__create = getExternalFunction(name:'topaz_object2d__create');
            @:topaz_object2d__add_velocity = getExternalFunction(name:'topaz_object2d__add_velocity');
            @:topaz_object2d__add_velocity_towards = getExternalFunction(name:'topaz_object2d__add_velocity_towards');
            @:topaz_object2d__set_velocity = getExternalFunction(name:'topaz_object2d__set_velocity');
            @:topaz_object2d__set_velocity_towards = getExternalFunction(name:'topaz_object2d__set_velocity_towards');
            @:topaz_object2d__get_friction_x = getExternalFunction(name:'topaz_object2d__get_friction_x');
            @:topaz_object2d__set_friction_x = getExternalFunction(name:'topaz_object2d__set_friction_x');
            @:topaz_object2d__get_friction_y = getExternalFunction(name:'topaz_object2d__get_friction_y');
            @:topaz_object2d__set_friction_y = getExternalFunction(name:'topaz_object2d__set_friction_y');
            @:topaz_object2d__get_direction = getExternalFunction(name:'topaz_object2d__get_direction');
            @:topaz_object2d__halt = getExternalFunction(name:'topaz_object2d__halt');
            @:topaz_object2d__reset_motion = getExternalFunction(name:'topaz_object2d__reset_motion');
            @:topaz_object2d__get_velocity_x = getExternalFunction(name:'topaz_object2d__get_velocity_x');
            @:topaz_object2d__get_velocity_y = getExternalFunction(name:'topaz_object2d__get_velocity_y');
            @:topaz_object2d__set_velocity_x = getExternalFunction(name:'topaz_object2d__set_velocity_x');
            @:topaz_object2d__set_velocity_y = getExternalFunction(name:'topaz_object2d__set_velocity_y');
            @:topaz_object2d__get_speed = getExternalFunction(name:'topaz_object2d__get_speed');
            @:topaz_object2d__set_speed = getExternalFunction(name:'topaz_object2d__set_speed');
            @:topaz_object2d__get_next_position = getExternalFunction(name:'topaz_object2d__get_next_position');
            @:topaz_object2d__get_last_position = getExternalFunction(name:'topaz_object2d__get_last_position');
            @:topaz_object2d__get_group = getExternalFunction(name:'topaz_object2d__get_group');
            @:topaz_object2d__set_group = getExternalFunction(name:'topaz_object2d__set_group');
            @:topaz_object2d__set_unit_lock = getExternalFunction(name:'topaz_object2d__set_unit_lock');
            @:topaz_object2d__set_collider = getExternalFunction(name:'topaz_object2d__set_collider');
            @:topaz_object2d__set_collider_radial = getExternalFunction(name:'topaz_object2d__set_collider_radial');
            @:topaz_object2d__set_collider_rectangle = getExternalFunction(name:'topaz_object2d__set_collider_rectangle');
            @:topaz_object2d__get_collider_len = getExternalFunction(name:'topaz_object2d__get_collider_len');
            @:topaz_object2d__get_last_collided = getExternalFunction(name:'topaz_object2d__get_last_collided');
            @:topaz_object2d__get_last_collided_position = getExternalFunction(name:'topaz_object2d__get_last_collided_position');
            @:topaz_object2d__get_collider_point_x = getExternalFunction(name:'topaz_object2d__get_collider_point_x');
            @:topaz_object2d__get_collider_point_y = getExternalFunction(name:'topaz_object2d__get_collider_point_y');
            return class(
                name : 'Topaz.Object2D',        
                inherits :[__Component__],
                statics: {
                    GROUP : {
                        A : 0,
                        B : 1,
                        C : 2,
                        D : 3,
                        E : 4,
                        F : 5,
                        G : 6,
                        H : 7,
                        I : 8,
                        J : 9,
                        K : 10,
                        L : 11,
                        M : 12,
                        N : 13,
                        O : 14,
                        P : 15,
                        Q : 16,
                        R : 17,
                        S : 18,
                        T : 19,
                        U : 20,
                        V : 21,
                        W : 22,
                        X : 23,
                        Y : 24,
                        Z : 25
                    },

                    setGroupInteraction ::(thisGroup, otherGroup, interact){ 
                        topaz_object2d__set_group_interaction(a:thisGroup, b:otherGroup, c:interact);
                    }
                }, 
                define : ::(this){ 
                    @impl;
                    this.constructor = ::(native) {
                        // whoops, the component constructor already made a generic component native.
                        // destroy it and make a real one
                        this.native.__ctx.destroy();

                        impl = this.bindNative(
                            instance : native,
                            nativeCreate : topaz_object2d__create
                        );
                        return this;
                    };
                    
                    
                    @_collider;
                    
                    this.interface = {
                        addVelocity : ::(amount, direction) { 
                            topaz_object2d__add_velocity(a:impl, b:amount, c:direction);
                        },

                        addVelocityTowards : ::(amount, target, offset) { 
                            topaz_object2d__add_velocity_towards(a:impl, b:amount, c:target.x, d:target.y, e:offset);
                        },

                        setVelocity : ::(amount, direction) { 
                            topaz_object2d__set_velocity(a:impl, b:amount, c:direction);
                        },

                        setVelocityTowards : ::(amount, target, offset) { 
                            topaz_object2d__set_velocity_towards(a:impl, b:amount, c:target.x, d:target.y, e:offset);
                        },




                        frictionX : {
                            get : ::()  {return topaz_object2d__get_friction_x(a:impl);}, 
                            set : ::(value) {topaz_object2d__set_friction_x(a:impl, b:value);} 
                        },

                        frictionY : {
                            get : ::()  {return topaz_object2d__get_friction_y(a:impl);},
                            set : ::(value) {topaz_object2d__set_friction_y(a:impl, b:value);} 
                        },

                        direction : {
                            get : ::()  {return topaz_object2d__get_direction(a:impl);},
                            set : ::(value){ 
                                topaz_object2d__set_velocity(
                                    a:impl,
                                    b:topaz_object2d__get_speed(a:impl),
                                    c:value
                                );
                            }
                        },

                        halt : ::() {topaz_object2d__halt(a:impl);},
                        resetMotion : ::() {topaz_object2d__reset_motion(a:impl);},

                        velocityX : {
                            get : ::()  {return topaz_object2d__get_velocity_x(a:impl);},
                            set : ::(value) {topaz_object2d__set_velocity_x(a:impl, b:value);} 
                        },

                        velocityY : {
                            get : ::()  {return topaz_object2d__get_velocity_y(a:impl);},
                            set : ::(value) {topaz_object2d__set_velocity_y(a:impl, b:value);} 
                        },

                        speed : {
                            get : ::()  {return topaz_object2d__get_speed(a:impl);},
                            set : ::(value) {topaz_object2d__set_speed(a:impl, b:value);} 
                        },

                        nextPosition : {
                            get : ::()  {
                                @:out = {};
                                topaz_object2d__get_next_position(a:impl, b:Vector._updateRemote(target:out));
                                return out;
                            } 
                        },
                        lastPosition : {
                            get : ::()  {
                                @:out = {};
                                topaz_object2d__get_last_position(a:impl, b:Vector._updateRemote(target:out));
                                return out;
                            } 
                        },
                        group : {
                            get : ::()  {return topaz_object2d__get_group(a:impl);}, 
                            set : ::(value) {topaz_object2d__set_group(a:impl, b:value);} 
                        },

                        unitLock : {
                            set ::(value) {topaz_object2d__set_unit_lock(a:impl, b:value);}
                        },

                        collider : {
                            get : ::()  {return _collider; },
                            set : ::(value) {_collider=value; topaz_object2d__set_collider(a:impl, b:value);} 
                        },

                        setColliderRectangle ::(width, height) {
                            topaz_object2d__set_collider_rectangle(a:impl, b:width, c:height);
                        },
                        setColliderRadial : ::(radius, numIterations) { 
                            topaz_object2d__set_collider_radial(a:impl, b:radius, c:numIterations);
                            _collider = [];
                            @iter = 0;
                            @len = topaz_object2d__get_collider_len(a:impl);
                            for(in:[0, len], do:::(i) {
                                _collider[iter] = topaz_object2d__get_collider_point_x(a:impl, b:i); iter += 1;
                                _collider[iter] = topaz_object2d__get_collider_point_y(a:impl, b:i); iter += 1;
                            });
                        },

                        lastCollided : {
                            get : ::()  {return __Topaz__.Entity.new(native:topaz_object2d__get_last_collided(a:impl));} 
                        },
                        lastCollidedPosition : {
                            get : ::()  {
                                @:out = {};
                                topaz_object2d__get_last_collided_position(a:impl, b:Vector._updateRemote(target:out));
                                return out;
                            } 
                        }

                    };                
                }
            );
        };
        @:topaz_object2d__set_group_interaction = getExternalFunction(name:'topaz_object2d__set_group_interaction');

        @__Shape2D__ = ::<={
            @:topaz_shape2d__create = getExternalFunction(name:'topaz_shape2d__create');
            @:topaz_shape2d__get_color_r = getExternalFunction(name:'topaz_shape2d__get_color_r');
            @:topaz_shape2d__get_color_g = getExternalFunction(name:'topaz_shape2d__get_color_g');
            @:topaz_shape2d__get_color_b = getExternalFunction(name:'topaz_shape2d__get_color_b');
            @:topaz_shape2d__get_color_a = getExternalFunction(name:'topaz_shape2d__get_color_a');
            @:topaz_shape2d__set_color_r = getExternalFunction(name:'topaz_shape2d__set_color_r');
            @:topaz_shape2d__set_color_g = getExternalFunction(name:'topaz_shape2d__set_color_g');
            @:topaz_shape2d__set_color_b = getExternalFunction(name:'topaz_shape2d__set_color_b');
            @:topaz_shape2d__set_color_a = getExternalFunction(name:'topaz_shape2d__set_color_a');
            @:topaz_shape2d__get_anim_speed = getExternalFunction(name:'topaz_shape2d__get_anim_speed');
            @:topaz_shape2d__set_anim_speed = getExternalFunction(name:'topaz_shape2d__set_anim_speed');
            @:topaz_shape2d__get_center_x = getExternalFunction(name:'topaz_shape2d__get_center_x');
            @:topaz_shape2d__get_center_y = getExternalFunction(name:'topaz_shape2d__get_center_y');
            @:topaz_shape2d__get_center_z = getExternalFunction(name:'topaz_shape2d__get_center_z');
            @:topaz_shape2d__set_center_x = getExternalFunction(name:'topaz_shape2d__set_center_x');
            @:topaz_shape2d__set_center_y = getExternalFunction(name:'topaz_shape2d__set_center_y');
            @:topaz_shape2d__set_center_z = getExternalFunction(name:'topaz_shape2d__set_center_z');
            @:topaz_shape2d__set_position_x = getExternalFunction(name:'topaz_shape2d__set_position_x');
            @:topaz_shape2d__set_position_y = getExternalFunction(name:'topaz_shape2d__set_position_y');
            @:topaz_shape2d__set_position_z = getExternalFunction(name:'topaz_shape2d__set_position_z');
            @:topaz_shape2d__get_position_x = getExternalFunction(name:'topaz_shape2d__get_position_x');
            @:topaz_shape2d__get_position_y = getExternalFunction(name:'topaz_shape2d__get_position_y');
            @:topaz_shape2d__get_position_z = getExternalFunction(name:'topaz_shape2d__get_position_z');
            @:topaz_shape2d__set_rotation_x = getExternalFunction(name:'topaz_shape2d__set_rotation_x');
            @:topaz_shape2d__set_rotation_y = getExternalFunction(name:'topaz_shape2d__set_rotation_y');
            @:topaz_shape2d__set_rotation_z = getExternalFunction(name:'topaz_shape2d__set_rotation_z');
            @:topaz_shape2d__get_rotation_x = getExternalFunction(name:'topaz_shape2d__get_rotation_x');
            @:topaz_shape2d__get_rotation_y = getExternalFunction(name:'topaz_shape2d__get_rotation_y');
            @:topaz_shape2d__get_rotation_z = getExternalFunction(name:'topaz_shape2d__get_rotation_z');
            @:topaz_shape2d__set_scale_x = getExternalFunction(name:'topaz_shape2d__set_scale_x');
            @:topaz_shape2d__set_scale_y = getExternalFunction(name:'topaz_shape2d__set_scale_y');
            @:topaz_shape2d__set_scale_z = getExternalFunction(name:'topaz_shape2d__set_scale_z');
            @:topaz_shape2d__get_scale_x = getExternalFunction(name:'topaz_shape2d__get_scale_x');
            @:topaz_shape2d__get_scale_y = getExternalFunction(name:'topaz_shape2d__get_scale_y');
            @:topaz_shape2d__get_scale_z = getExternalFunction(name:'topaz_shape2d__get_scale_z');
            @:topaz_shape2d__form_lines = getExternalFunction(name:'topaz_shape2d__form_lines');
            @:topaz_shape2d__form_triangles = getExternalFunction(name:'topaz_shape2d__form_triangles');
            @:topaz_shape2d__set_attribute = getExternalFunction(name:'topaz_shape2d__set_attribute');
            @:topaz_shape2d__get_attribute = getExternalFunction(name:'topaz_shape2d__get_attribute');
            @:topaz_shape2d__form_rectangle = getExternalFunction(name:'topaz_shape2d__form_rectangle');
            @:topaz_shape2d__form_image = getExternalFunction(name:'topaz_shape2d__form_image');
            @:topaz_shape2d__form_image_frame = getExternalFunction(name:'topaz_shape2d__form_image_frame');
            @:topaz_shape2d__form_radial = getExternalFunction(name:'topaz_shape2d__form_radial');

            return class(
                name : 'Topaz.Shape2D',        
                inherits :[__Component__],
                define : ::(this){ 
                    @impl;
                    this.constructor = ::(native) {
                        this.constructor[__Component__](native:native);
                        // whoops, the component constructor already made a generic component native.
                        // destroy it and make a real one
                        this.native.__ctx.destroy();

                        impl = this.bindNative(
                            instance : native,
                            nativeCreate : topaz_shape2d__create
                        );
                        return this;
                    };
                    
                    @_lines;
                    @_tris;
                    
                    @rotation;
                    @position;
                    @scale;
                    @center;
                    
                    @color;
                    this.interface = {
                        color : {
                            get : ::() {
                                if (color == empty)
                                    color = ManagedColor.new(
                                        native_:this.native,
                                        getr: topaz_shape2d__get_color_r,
                                        getg: topaz_shape2d__get_color_g,
                                        getb: topaz_shape2d__get_color_b,
                                        geta: topaz_shape2d__get_color_a,

                                        setr: topaz_shape2d__set_color_r,
                                        setg: topaz_shape2d__set_color_g,
                                        setb: topaz_shape2d__set_color_b,
                                        seta: topaz_shape2d__set_color_a
                                    );


                                return color;
                            }, 
                            set : ::(value){
                                if (getType(of:value) == String) ::<={
                                    value = Color.parse(string:value);
                                };

                                if (value.r != empty) topaz_shape2d__set_color_r(a:this.native, b:value.r);
                                if (value.g != empty) topaz_shape2d__set_color_g(a:this.native, b:value.g);
                                if (value.b != empty) topaz_shape2d__set_color_b(a:this.native, b:value.b);
                                if (value.a != empty) topaz_shape2d__set_color_a(a:this.native, b:value.a);

                            } 
                        },


                        animSpeed : {
                            get : ::() {return topaz_shape2d__get_anim_speed(a:impl);},
                            set : ::(value){topaz_shape2d__set_anim_speed(a:impl, b:value);} 
                        },

                        center : {
                            get : ::()  {
                                if (center == empty)
                                    center = ManagedVector.new(
                                        native_:this.native,
                                        getx: topaz_shape2d__get_center_x,
                                        gety: topaz_shape2d__get_center_y,
                                        getz: topaz_shape2d__get_center_z,

                                        setx: topaz_shape2d__set_center_x,
                                        sety: topaz_shape2d__set_center_y,
                                        setz: topaz_shape2d__set_center_z
                                    );


                                return center;
                            },
                            set : ::(value) {
                                if (value.x != empty) topaz_shape2d__set_center_x(a:this.native, b:value.x);
                                if (value.y != empty) topaz_shape2d__set_center_y(a:this.native, b:value.y);
                                if (value.z != empty) topaz_shape2d__set_center_z(a:this.native, b:value.z);
                            } 
                        },

                        rotation : {
                            get : ::()  {
                                if (rotation == empty)
                                    rotation = ManagedVector.new(
                                        native_:this.native,
                                        getx: topaz_shape2d__get_rotation_x,
                                        gety: topaz_shape2d__get_rotation_y,
                                        getz: topaz_shape2d__get_rotation_z,

                                        setx: topaz_shape2d__set_rotation_x,
                                        sety: topaz_shape2d__set_rotation_y,
                                        setz: topaz_shape2d__set_rotation_z
                                    );


                                return rotation;
                            },
                            set : ::(value) {
                                if (value.x != empty) topaz_shape2d__set_rotation_x(a:this.native, b:value.x);
                                if (value.y != empty) topaz_shape2d__set_rotation_y(a:this.native, b:value.y);
                                if (value.z != empty) topaz_shape2d__set_rotation_z(a:this.native, b:value.z);
                            } 
                        },

        
                        position : {
                            get : ::()  {
                                if (position == empty)
                                    position = ManagedVector.new(
                                        native_:this.native,
                                        getx: topaz_shape2d__get_position_x,
                                        gety: topaz_shape2d__get_position_y,
                                        getz: topaz_shape2d__get_position_z,

                                        setx: topaz_shape2d__set_position_x,
                                        sety: topaz_shape2d__set_position_y,
                                        setz: topaz_shape2d__set_position_z
                                    );


                                return position;
                            },
                            set : ::(value) {
                                if (value.x != empty) topaz_shape2d__set_position_x(a:this.native, b:value.x);
                                if (value.y != empty) topaz_shape2d__set_position_y(a:this.native, b:value.y);
                                if (value.z != empty) topaz_shape2d__set_position_z(a:this.native, b:value.z);
                            } 
                        },
                        
                        scale : {
                            get : ::()  {
                                if (scale == empty)
                                    scale = ManagedVector.new(
                                        native_:this.native,
                                        getx: topaz_shape2d__get_scale_x,
                                        gety: topaz_shape2d__get_scale_y,
                                        getz: topaz_shape2d__get_scale_z,

                                        setx: topaz_shape2d__set_scale_x,
                                        sety: topaz_shape2d__set_scale_y,
                                        setz: topaz_shape2d__set_scale_z
                                    );


                                return scale;
                            },
                            set : ::(value) {
                                if (value.x != empty) topaz_shape2d__set_scale_x(a:this.native, b:value.x);
                                if (value.y != empty) topaz_shape2d__set_scale_y(a:this.native, b:value.y);
                                if (value.z != empty) topaz_shape2d__set_scale_z(a:this.native, b:value.z);
                            }                         
                        },

                        lines : {
                            get : ::() {return _lines;},
                            set : ::(value){topaz_shape2d__form_lines(a:impl, b:value); _lines = value;} 
                        },

                        triangles : {
                            get : ::() {return _tris;}, 
                            set : ::(value){topaz_shape2d__form_triangles(a:impl, b:value); _tris = value;} 
                        },

                        setAttributes : ::(primitive, alphaRule, depthTest, etchRule, textureFilterHint){
                            if (primitive != empty) topaz_shape2d__set_attribute(a:impl, b:0, c:primitive);
                            if (alphaRule != empty) topaz_shape2d__set_attribute(a:impl, b:1, c:alphaRule);
                            if (depthTest != empty) topaz_shape2d__set_attribute(a:impl, b:2, c:depthTest);
                            if (etchRule  != empty) topaz_shape2d__set_attribute(a:impl, b:3, c:etchRule);
                            if (textureFilterHint != empty) topaz_shape2d__set_attribute(a:impl, b:4, c:textureFilterHint);
                        },

                        getAttribute : ::(attribute) { 
                            return topaz_shape2d__get_attribute(a:impl, b:attribute);
                        },

                        formRectangle : ::(width, height){ 
                            topaz_shape2d__form_rectangle(a:impl, b:width, c:height);
                        },

                        formImage : ::(image) { 
                            topaz_shape2d__form_image(a:impl, b:image.native);
                        },

                        formImageFrame : ::(image, frameIndex) { 
                            topaz_shape2d__form_image_frame(a:impl, b:image.native, c:frameIndex);
                        },

                        formImageScaled : ::(image, width, height) { 
                            topaz_shape2d__form_image(a:impl, b:image.native, c:width, d:height);
                        },

                        formRadial : ::(radius, numSides) { 
                            topaz_shape2d__form_radial(a:impl, b:radius, c:numSides);
                        }








                    };
                }
            );
        };
        
        @__Shape3D__ = ::<={
            @:topaz_shape3d__create = getExternalFunction(name:'topaz_shape3d__create');
            @:topaz_shape3d__set_position_x = getExternalFunction(name:'topaz_shape3d__set_position_x');
            @:topaz_shape3d__set_position_y = getExternalFunction(name:'topaz_shape3d__set_position_y');
            @:topaz_shape3d__set_position_z = getExternalFunction(name:'topaz_shape3d__set_position_z');
            @:topaz_shape3d__get_position_x = getExternalFunction(name:'topaz_shape3d__get_position_x');
            @:topaz_shape3d__get_position_y = getExternalFunction(name:'topaz_shape3d__get_position_y');
            @:topaz_shape3d__get_position_z = getExternalFunction(name:'topaz_shape3d__get_position_z');
            @:topaz_shape3d__set_rotation_x = getExternalFunction(name:'topaz_shape3d__set_rotation_x');
            @:topaz_shape3d__set_rotation_y = getExternalFunction(name:'topaz_shape3d__set_rotation_y');
            @:topaz_shape3d__set_rotation_z = getExternalFunction(name:'topaz_shape3d__set_rotation_z');
            @:topaz_shape3d__get_rotation_x = getExternalFunction(name:'topaz_shape3d__get_rotation_x');
            @:topaz_shape3d__get_rotation_y = getExternalFunction(name:'topaz_shape3d__get_rotation_y');
            @:topaz_shape3d__get_rotation_z = getExternalFunction(name:'topaz_shape3d__get_rotation_z');
            @:topaz_shape3d__set_scale_x = getExternalFunction(name:'topaz_shape3d__set_scale_x');
            @:topaz_shape3d__set_scale_y = getExternalFunction(name:'topaz_shape3d__set_scale_y');
            @:topaz_shape3d__set_scale_z = getExternalFunction(name:'topaz_shape3d__set_scale_z');
            @:topaz_shape3d__get_scale_x = getExternalFunction(name:'topaz_shape3d__get_scale_x');
            @:topaz_shape3d__get_scale_y = getExternalFunction(name:'topaz_shape3d__get_scale_y');
            @:topaz_shape3d__get_scale_z = getExternalFunction(name:'topaz_shape3d__get_scale_z');
            @:topaz_shape3d__set_attribute = getExternalFunction(name:'topaz_shape3d__set_attribute');
            @:topaz_shape3d__get_attribute = getExternalFunction(name:'topaz_shape3d__get_attribute');
            @:topaz_shape3d__set_texture = getExternalFunction(name:'topaz_shape3d__set_texture');
            @:topaz_shape3d__set_mesh = getExternalFunction(name:'topaz_shape3d__set_mesh');
            @:topaz_shape3d__set_sample_framebuffer = getExternalFunction(name:'topaz_shape3d__set_sample_framebuffer');
            @:topaz_shape3d__set_material = getExternalFunction(name:'topaz_shape3d__set_material');

            return class(
                name : 'Topaz.Shape3D',        
                inherits :[__Component__],
                statics: {
                    TEXTURE : {
                        SLOT_0 : 0,
                        SLOT_1 : 1,
                        SLOT_2 : 2
                    }
                },
                define : ::(this){ 
                    @impl;
                    this.constructor = ::(native) {
                        // whoops, the component constructor already made a generic component native.
                        // destroy it and make a real one
                        this.native.__ctx.destroy();

                        impl = this.bindNative(
                            instance : native,
                            nativeCreate : topaz_shape3d__create
                        );
                        return this;
                    };
                    
                    @position;
                    @rotation;
                    @scale;
                    
                    this.interface = {
                        rotation : {
                            get : ::()  {
                                if (rotation == empty)
                                    rotation = ManagedVector.new(
                                        native_:this.native,
                                        getx: topaz_shape3d__get_rotation_x,
                                        gety: topaz_shape3d__get_rotation_y,
                                        getz: topaz_shape3d__get_rotation_z,

                                        setx: topaz_shape3d__set_rotation_x,
                                        sety: topaz_shape3d__set_rotation_y,
                                        setz: topaz_shape3d__set_rotation_z
                                    );


                                return rotation;
                            },
                            set : ::(value) {
                                if (value.x != empty) topaz_shape3d__set_rotation_x(a:this.native, b:value.x);
                                if (value.y != empty) topaz_shape3d__set_rotation_y(a:this.native, b:value.y);
                                if (value.z != empty) topaz_shape3d__set_rotation_z(a:this.native, b:value.z);
                            } 
                        },

        
                        position : {
                            get : ::()  {
                                if (position == empty)
                                    position = ManagedVector.new(
                                        native_:this.native,
                                        getx: topaz_shape3d__get_position_x,
                                        gety: topaz_shape3d__get_position_y,
                                        getz: topaz_shape3d__get_position_z,

                                        setx: topaz_shape3d__set_position_x,
                                        sety: topaz_shape3d__set_position_y,
                                        setz: topaz_shape3d__set_position_z
                                    );


                                return position;
                            },
                            set : ::(value) {
                                if (value.x != empty) topaz_shape3d__set_position_x(a:this.native, b:value.x);
                                if (value.y != empty) topaz_shape3d__set_position_y(a:this.native, b:value.y);
                                if (value.z != empty) topaz_shape3d__set_position_z(a:this.native, b:value.z);
                            } 
                        },
                        
                        scale : {
                            get : ::()  {
                                if (scale == empty)
                                    scale = ManagedVector.new(
                                        native_:this.native,
                                        getx: topaz_shape3d__get_scale_x,
                                        gety: topaz_shape3d__get_scale_y,
                                        getz: topaz_shape3d__get_scale_z,

                                        setx: topaz_shape3d__set_scale_x,
                                        sety: topaz_shape3d__set_scale_y,
                                        setz: topaz_shape3d__set_scale_z
                                    );


                                return scale;
                            },
                            set : ::(value) {
                                if (value.x != empty) topaz_shape3d__set_scale_x(a:this.native, b:value.x);
                                if (value.y != empty) topaz_shape3d__set_scale_y(a:this.native, b:value.y);
                                if (value.z != empty) topaz_shape3d__set_scale_z(a:this.native, b:value.z);
                            }                         
                        },

                        setAttributes : ::(primitive, alphaRule, depthTest, etchRule, textureFilterHint){
                            if (primitive != empty) topaz_shape3d__set_attribute(a:impl, b:0, c:primitive);
                            if (alphaRule != empty) topaz_shape3d__set_attribute(a:impl, b:1, c:alphaRule);
                            if (depthTest != empty) topaz_shape3d__set_attribute(a:impl, b:2, c:depthTest);
                            if (etchRule  != empty) topaz_shape3d__set_attribute(a:impl, b:3, c:etchRule);
                            if (textureFilterHint != empty) topaz_shape3d__set_attribute(a:impl, b:4, c:textureFilterHint);
                        },

                        getAttribute : ::(attribute){ 
                            return topaz_shape3d__set_attribute(a:impl, b:attribute);
                        },

                        setTexture : ::(slot, texture){ 
                            topaz_shape3d__set_texture(a:impl, b:slot, c:texture.native);
                        },

                        mesh : {
                            set : ::(value){ 
                                topaz_shape3d__set_mesh(a:impl, b:value.native);            
                            }
                        },

                        sampleFramebuffer : {
                            set : ::(value) { 
                                topaz_shape3d__set_sample_framebuffer(a:impl, b:value.native);
                            }
                        },

                        material : {
                            set : ::(value){ 
                                topaz_shape3d__set_material(a:impl, b:value.native);
                            }
                        }
                    };  
                }
            );
        };
        @__Automation__ = ::<={
            @:topaz_automation__create = getExternalFunction(name:'topaz_automation__create');
            @:topaz_automation__add_keyframe = getExternalFunction(name:'topaz_automation__add_keyframe');
            @:topaz_automation__add_vector_keyframe = getExternalFunction(name:'topaz_automation__add_vector_keyframe');
            @:topaz_automation__clear = getExternalFunction(name:'topaz_automation__clear');
            @:topaz_automation__blend = getExternalFunction(name:'topaz_automation__blend');
            @:topaz_automation__smooth = getExternalFunction(name:'topaz_automation__smooth');
            @:topaz_automation__add_from_string = getExternalFunction(name:'topaz_automation__add_from_string');
            @:topaz_automation__skip_to = getExternalFunction(name:'topaz_automation__skip_to');
            @:topaz_automation__resume = getExternalFunction(name:'topaz_automation__resume');
            @:topaz_automation__pause = getExternalFunction(name:'topaz_automation__pause');
            @:topaz_automation__vector_at = getExternalFunction(name:'topaz_automation__vector_at');
            @:topaz_automation__at = getExternalFunction(name:'topaz_automation__at');
            @:topaz_automation__current_vector = getExternalFunction(name:'topaz_automation__current_vector');
            @:topaz_automation__current = getExternalFunction(name:'topaz_automation__current');
            @:topaz_automation__to_string = getExternalFunction(name:'topaz_automation__to_string');
            @:topaz_automation__set_from_string = getExternalFunction(name:'topaz_automation__set_from_string');
            @:topaz_automation__get_length = getExternalFunction(name:'topaz_automation__get_length');
            @:topaz_automation__set_duration_frames = getExternalFunction(name:'topaz_automation__set_duration_frames');
            @:topaz_automation__set_duration_seconds = getExternalFunction(name:'topaz_automation__set_duration_seconds');
            @:topaz_automation__get_duration = getExternalFunction(name:'topaz_automation__get_duration');
            @:topaz_automation__get_looped = getExternalFunction(name:'topaz_automation__get_looped');
            @:topaz_automation__set_looped = getExternalFunction(name:'topaz_automation__set_looped');
            @:topaz_automation__set_speed = getExternalFunction(name:'topaz_automation__set_speed');
            @:topaz_automation__get_speed = getExternalFunction(name:'topaz_automation__get_speed');

            return class(
                name : 'Topaz.Automation',        
                inherits :[__Component__],
                statics : {
                    FUNCTION : {
                        NONE : 0,
                        LINEAR : 1,
                        ACCEL : 2,
                        SLOW : 3,
                        SOFT_ACCEL : 4,
                        SOFT_SLOW : 5,
                        RANDOM : 6    
                    }      
                },
                define : ::(this){ 
                    @impl;
                    this.constructor = ::(native) {
                        // whoops, the component constructor already made a generic component native.
                        // destroy it and make a real one
                        this.native.__ctx.destroy();

                        impl = this.bindNative(
                            instance : native,
                            nativeCreate : topaz_automation__create
                        );
                        return this;
                    };
                    
                    
                    this.interface = {
                        addKeyframe : ::(value, function, offset){ 
                            topaz_automation__add_keyframe(a:impl, b:value, c:function, d:offset);
                        },

                        addVectorKeyframe : ::(value, function, offset){ 
                            topaz_automation__add_vector_keyframe(a:impl, b:value.native, c:function, d:offset);
                        },

                        clear : ::(){ 
                            topaz_automation__clear(a:impl);
                        },

                        blend : ::(other){ 
                            topaz_automation__blend(a:impl, b:other.native);
                        },

                        smooth : ::(){ 
                            topaz_automation__smooth(a:impl);
                        },

                        addFromString : ::(string){ 
                            topaz_automation__add_from_string(a:impl, b:string);
                        },

                        skipTo : ::(location){ 
                            topaz_automation__skip_to(a:impl, b:location);
                        },

                        resume : ::() { 
                            topaz_automation__resume(a:impl);
                        },

                        pause : ::() { 
                            topaz_automation__pause(a:impl);
                        },

                        vectorAt : ::(progress) { 
                            @:out = {};
                            topaz_automation__vector_at(a:impl, b:progress, c:Vector._updateRemote(target:out));
                            return out;
                        },

                        at : ::(progress) { 
                            return topaz_automation__at(a:impl, b:progress);
                        },

                        vector : {
                            get : ::() {
                                @:out = {};
                                topaz_automation__current_vector(a:impl, b:Vector._updateRemote(target:out));
                                return out;
                            }
                        },

                        value : {
                            get : ::() {return topaz_automation__current(a:impl);}
                        },

                        string : {
                            get : ::() {return topaz_automation__to_string(a:impl);},
                            set : ::(value) {return topaz_automation__set_from_string(a:impl, b:value);}
                        },

                        length : {
                            get : ::() {return topaz_automation__get_length(a:impl);}
                        },

                        durationFrames : {
                            set : ::(value){ topaz_automation__set_duration_frames(a:impl, b:value);} 
                        },

                        durationSeconds : {
                            set : ::(value){ topaz_automation__set_duration_seconds(a:impl, b:value);} 
                        },

                        duration : {
                            get : ::() {return topaz_automation__get_duration(a:impl);}
                        },

                        looped : {
                            get : ::() {return topaz_automation__get_looped(a:impl);},
                            set : ::(value) {return topaz_automation__set_looped(a:impl, b:value);}
                        },

                        speed : {
                            get : ::() {return topaz_automation__get_speed(a:impl);},
                            set : ::(value) {return topaz_automation__set_speed(a:impl, b:value);}
                        }
                    };
                }
            );
        };
        @__Particle__ = ::<={
            @:topaz_particle__set_attribute = getExternalFunction(name:'topaz_particle__set_attribute');
            @:topaz_particle__set_noise_min = getExternalFunction(name:'topaz_particle__set_noise_min');
            @:topaz_particle__set_noise_max = getExternalFunction(name:'topaz_particle__set_noise_max');
            @:topaz_particle__set_offset_min = getExternalFunction(name:'topaz_particle__set_offset_min');
            @:topaz_particle__set_offset_max = getExternalFunction(name:'topaz_particle__set_offset_max');
            @:topaz_particle__set_function = getExternalFunction(name:'topaz_particle__set_function');
            @:topaz_particle__to_string = getExternalFunction(name:'topaz_particle__to_string');
            @:topaz_particle__set_from_string = getExternalFunction(name:'topaz_particle__set_from_string');
            @:topaz_particle__set_image = getExternalFunction(name:'topaz_particle__set_image');
            @:topaz_particle__create = getExternalFunction(name:'topaz_particle__create');
            return class(
                name : 'Topaz.Particle',      
                inherits : [__Native__],
                define : ::(this){ 

                    @impl;
                    this.constructor = :: { 
                        impl = this.bindNative(
                            nativeCreate : topaz_particle__create
                        );
                        return this;
                    };
                    
                    this.interface = {
                        setAttributes : ::(primitive, alphaRule, depthTest, etchRule, textureFilterHint){
                            if (primitive != empty) topaz_particle__set_attribute(a:impl, b:0, c:primitive);
                            if (alphaRule != empty) topaz_particle__set_attribute(a:impl, b:1, c:alphaRule);
                            if (depthTest != empty) topaz_particle__set_attribute(a:impl, b:2, c:depthTest);
                            if (etchRule  != empty) topaz_particle__set_attribute(a:impl, b:3, c:etchRule);
                            if (textureFilterHint != empty) topaz_particle__set_attribute(a:impl, b:4, c:textureFilterHint);
                        },

                        setNoiseMin : ::(duration, scaleX, scaleY, scaleMultiplier, rotation, direction, speedX, speedY, red, green, blue, alpha) { 
                            if (duration != empty) topaz_particle__set_noise_min(a:impl, b:0, c:duration);
                            if (scaleX   != empty) topaz_particle__set_noise_min(a:impl, b:1, c:scaleX);
                            if (scaleY   != empty) topaz_particle__set_noise_min(a:impl, b:2, c:scaleY);
                            if (scaleMultiplier   != empty) topaz_particle__set_noise_min(a:impl, b:3, c:scaleMultiplier);
                            if (rotation != empty) topaz_particle__set_noise_min(a:impl, b:4, c:rotation);
                            if (direction != empty) topaz_particle__set_noise_min(a:impl, b:5, c:direction);
                            if (speedX   != empty) topaz_particle__set_noise_min(a:impl, b:6, c:speedX);
                            if (speedY   != empty) topaz_particle__set_noise_min(a:impl, b:7, c:speedY);
                            if (red   != empty) topaz_particle__set_noise_min(a:impl, b:8, c:red);
                            if (green   != empty) topaz_particle__set_noise_min(a:impl, b:9, c:green);
                            if (blue   != empty) topaz_particle__set_noise_min(a:impl, b:10, c:blue);
                            if (alpha != empty) topaz_particle__set_noise_min(a:impl, b:11, c:alpha);

                        },

                        setNoiseMax : ::(duration, scaleX, scaleY, scaleMultiplier, rotation, direction, speedX, speedY, red, green, blue, alpha) { 
                            if (duration != empty) topaz_particle__set_noise_max(a:impl, b:0, c:duration);
                            if (scaleX   != empty) topaz_particle__set_noise_max(a:impl, b:1, c:scaleX);
                            if (scaleY   != empty) topaz_particle__set_noise_max(a:impl, b:2, c:scaleY);
                            if (scaleMultiplier   != empty) topaz_particle__set_noise_max(a:impl, b:3, c:scaleMultiplier);
                            if (rotation != empty) topaz_particle__set_noise_max(a:impl, b:4, c:rotation);
                            if (direction != empty) topaz_particle__set_noise_max(a:impl, b:5, c:direction);
                            if (speedX   != empty) topaz_particle__set_noise_max(a:impl, b:6, c:speedX);
                            if (speedY   != empty) topaz_particle__set_noise_max(a:impl, b:7, c:speedY);
                            if (red   != empty) topaz_particle__set_noise_max(a:impl, b:8, c:red);
                            if (green   != empty) topaz_particle__set_noise_max(a:impl, b:9, c:green);
                            if (blue   != empty) topaz_particle__set_noise_max(a:impl, b:10, c:blue);
                            if (alpha != empty) topaz_particle__set_noise_max(a:impl, b:11, c:alpha);
                        },


                        setOffsetMin : ::(duration, scaleX, scaleY, scaleMultiplier, rotation, direction, speedX, speedY, red, green, blue, alpha) { 
                            if (duration != empty) topaz_particle__set_offset_min(a:impl, b:0, c:duration);
                            if (scaleX   != empty) topaz_particle__set_offset_min(a:impl, b:1, c:scaleX);
                            if (scaleY   != empty) topaz_particle__set_offset_min(a:impl, b:2, c:scaleY);
                            if (scaleMultiplier   != empty) topaz_particle__set_offset_min(a:impl, b:3, c:scaleMultiplier);
                            if (rotation != empty) topaz_particle__set_offset_min(a:impl, b:4, c:rotation);
                            if (direction != empty) topaz_particle__set_offset_min(a:impl, b:5, c:direction);
                            if (speedX   != empty) topaz_particle__set_offset_min(a:impl, b:6, c:speedX);
                            if (speedY   != empty) topaz_particle__set_offset_min(a:impl, b:7, c:speedY);
                            if (red   != empty) topaz_particle__set_offset_min(a:impl, b:8, c:red);
                            if (green   != empty) topaz_particle__set_offset_min(a:impl, b:9, c:green);
                            if (blue   != empty) topaz_particle__set_offset_min(a:impl, b:10, c:blue);
                            if (alpha != empty) topaz_particle__set_offset_min(a:impl, b:11, c:alpha);

                        },

                        setOffsetMax : ::(duration, scaleX, scaleY, scaleMultiplier, rotation, direction, speedX, speedY, red, green, blue, alpha) { 
                            if (duration != empty) topaz_particle__set_offset_max(a:impl, b:0, c:duration);
                            if (scaleX   != empty) topaz_particle__set_offset_max(a:impl, b:1, c:scaleX);
                            if (scaleY   != empty) topaz_particle__set_offset_max(a:impl, b:2, c:scaleY);
                            if (scaleMultiplier   != empty) topaz_particle__set_offset_max(a:impl, b:3, c:scaleMultiplier);
                            if (rotation != empty) topaz_particle__set_offset_max(a:impl, b:4, c:rotation);
                            if (direction != empty) topaz_particle__set_offset_max(a:impl, b:5, c:direction);
                            if (speedX   != empty) topaz_particle__set_offset_max(a:impl, b:6, c:speedX);
                            if (speedY   != empty) topaz_particle__set_offset_max(a:impl, b:7, c:speedY);
                            if (red   != empty) topaz_particle__set_offset_max(a:impl, b:8, c:red);
                            if (green   != empty) topaz_particle__set_offset_max(a:impl, b:9, c:green);
                            if (blue   != empty) topaz_particle__set_offset_max(a:impl, b:10, c:blue);
                            if (alpha != empty) topaz_particle__set_offset_max(a:impl, b:11, c:alpha);
                        },

                        setFunctions : ::(duration, scaleX, scaleY, scaleMultiplier, rotation, direction, speedX, speedY, red, green, blue, alpha) { 
                            if (duration != empty) topaz_particle__set_function(a:impl, b:0, c:duration);
                            if (scaleX   != empty) topaz_particle__set_function(a:impl, b:1, c:scaleX);
                            if (scaleY   != empty) topaz_particle__set_function(a:impl, b:2, c:scaleY);
                            if (scaleMultiplier   != empty) topaz_particle__set_function(a:impl, b:3, c:scaleMultiplier);
                            if (rotation != empty) topaz_particle__set_function(a:impl, b:4, c:rotation);
                            if (direction != empty) topaz_particle__set_function(a:impl, b:5, c:direction);
                            if (speedX   != empty) topaz_particle__set_function(a:impl, b:6, c:speedX);
                            if (speedY   != empty) topaz_particle__set_function(a:impl, b:7, c:speedY);
                            if (red   != empty) topaz_particle__set_function(a:impl, b:8, c:red);
                            if (green   != empty) topaz_particle__set_function(a:impl, b:9, c:green);
                            if (blue   != empty) topaz_particle__set_function(a:impl, b:10, c:blue);
                            if (alpha != empty) topaz_particle__set_function(a:impl, b:11, c:alpha);
                        },

                        string : {get : ::() {return topaz_particle__to_string(a:impl);}, set : ::(value){topaz_particle__set_from_string(a:impl, b:value);}}, 
                        image :  {set : ::(value) {return topaz_particle__set_image(a:impl, b:value.native);}}
                    };
                }        
            );
        };
        @__ParticleEmitter2D__  = ::<={ 
            @:topaz_particle_emitter_2d__create = getExternalFunction(name:'topaz_particle_emitter_2d__create');
            @:topaz_particle_emitter_2d__set_particle = getExternalFunction(name:'topaz_particle_emitter_2d__set_particle');
            @:topaz_particle_emitter_2d__set_independent = getExternalFunction(name:'topaz_particle_emitter_2d__set_independent');
            @:topaz_particle_emitter_2d__emit = getExternalFunction(name:'topaz_particle_emitter_2d__emit');
            return class(
                name : 'Topaz.ParticleEmitter2D',        
                inherits :[__Entity__],
                define : ::(this) { 
                    @impl;
                    this.constructor = ::(native) {
                        // whoops, the component constructor already made a generic component native.
                        // destroy it and make a real one
                        //this.native.__ctx.destroy();

                        impl = this.bindNative(
                            instance : native,
                            nativeCreate : topaz_particle_emitter_2d__create
                        );
                        return this;
                    };
                    this.interface = {

                        particle : {set : ::(value){topaz_particle_emitter_2d__set_particle(a:impl, b:value.native);}}, 
                        independent : {set : ::(value){topaz_particle_emitter_2d__set_independent(a:impl, b:value);}}, 

                        emit : ::(amount){ 
                            topaz_particle_emitter_2d__emit(a:impl, b:amount);
                        }
                    };
                }
            );
        };

        @:topaz__attach_pre_manager = getExternalFunction(name:'topaz__attach_pre_manager');
        @:topaz__attach_pre_manager_unpausable = getExternalFunction(name:'topaz__attach_pre_manager_unpausable');
        @:topaz__attach_post_manager = getExternalFunction(name:'topaz__attach_post_manager');
        @:topaz__attach_post_manager_unpausable = getExternalFunction(name:'topaz__attach_post_manager_unpausable');
        @:topaz__frame_start = getExternalFunction(name:'topaz__frame_start');
        @:topaz__frame_end = getExternalFunction(name:'topaz__frame_end');
        @:topaz__to_base64 = getExternalFunction(name:'topaz__to_base64');
        @:topaz__from_base64 = getExternalFunction(name:'topaz__from_base64');
        @:topaz__is_paused = getExternalFunction(name:'topaz__is_paused');
        @:topaz__get_time = getExternalFunction(name:'topaz__get_time');
        @:topaz__get_version_minor = getExternalFunction(name:'topaz__get_version_minor');
        @:topaz__get_version_micro = getExternalFunction(name:'topaz__get_version_micro');
        @:topaz__get_version_major = getExternalFunction(name:'topaz__get_version_major');
        @:topaz__debug = getExternalFunction(name:'topaz__debug');
        @: topaz_view_manager__get_default = getExternalFunction(name:'topaz_view_manager__get_default');
        @: topaz_view_manager__get_display_count = getExternalFunction(name:'topaz_view_manager__get_display_count');
        @: topaz_view_manager__get_display = getExternalFunction(name:'topaz_view_manager__get_display');
        @:topaz_view_manager__get_clipboard_as_string = getExternalFunction(name:'topaz_view_manager__get_clipboard_as_string');
        @:topaz_view_manager__set_clipboard_from_string = getExternalFunction(name:'topaz_view_manager__set_clipboard_from_string');

        @:RENDERER =  {
            ATTRIBUTE : {
                PRIMITIVE : 0,
                ALPHA_RULE : 1,
                DEPTH_TEST : 2,
                ETCH_RULE : 3,
                TEXTURE_FILTER_HINT : 4
            },
            
            PRIMITIVE : {
                TRIANGLE : 0,
                LINE : 1
            },

            ETCH_RULE : {
                NO_ETCHING : 0,
                DEFINE : 1,
                UNDEFINE : 2,
                IN : 3,
                OUT : 4
            },
            
            DEPTH_TEST : {
                LESS : 0,
                LEQ : 1,
                GREATER : 2,
                GEQ : 3,
                EQUAL : 4,
                NONE : 5
            },

            ALPHA_RULE : {
                ALLOW : 0,
                OPAQUE : 1,
                TRANSLUCENT : 2,
                INVISIBLE : 3
            },
            
            TEXTURE_FILTER_HINT : {
                LINEAR : 0,
                NONE : 1
            },
            
            LAYER : {
                COLOR : 1,
                DEPTH : 2,
                ETCH : 4,
                ALL  : 7
            }

        };
 

        this.interface = {
            run   : getExternalFunction(name:'topaz__run'),
            pause   : getExternalFunction(name:'topaz__pause'),
            pauseNow : getExternalFunction(name:'topaz__pause_now'),
            resume : getExternalFunction(name:'topaz__resume'),
            iterate : getExternalFunction(name:'topaz__iterate'),
            attachPreManager : ::(manager){topaz__attach_pre_manager(a:manager.native);},        
            attachPreManagerUnpausable : ::(manager){topaz__attach_pre_manager_unpausable(a:manager.native);},        
            attachPostManager : ::(manager){topaz__attach_post_manager(a:manager.native);},        
            attachPostManagerUnpausable : ::(manager){topaz__attach_post_manager_unpausable(a:manager.native);},        
            quit : getExternalFunction(name:'topaz__quit'),
            frameStart ::(FPS) {topaz__frame_start(a:FPS);},
            frameEnd ::(FPS) {topaz__frame_end();},
            toBase64 ::(bytes) {topaz__to_base64(a:bytes);},
            fromBase64 : ::(string) { 
                return __Topaz__.Data.new(native:topaz__from_base64(a:string));
            },
            

            debug ::{
                topaz__debug();
            },

            isPaused : {
                get : ::(){ 
                    return topaz__is_paused();
                }            
            },
            
            time : {
                get : ::(){ 
                    return topaz__get_time();
                }
            },
            
            versionMicro : {
                get : ::() { 
                    return topaz__get_version_micro();                
                }
            },
            versionMajor : {
                get : ::() { 
                    return topaz__get_version_major();                
                }
            },
            versionMinor : {
                get : ::() { 
                    return topaz__get_version_minor();                
                }
            },

            defaultDisplay : {
                get ::{ 
                    @f = topaz_view_manager__get_default();
                    when (f == empty) empty;
                    when (f.__ctx != empty) f.__ctx;
                    return __Display__.new(native:f);      
                }
            },


            displays : {
                get ::{
                    @:out = [];
                    for(in:[0, topaz_view_manager__get_display_count()], do:::(i){
                        @f = topaz_view_manager__get_display(a:i);
                        when (f == empty) empty;
                        when (f.__ctx != empty) f.__ctx;
                        return __Display__.new(native:f);    
                        out[i] = f;
                    });
                    return out;
                }
            },

            clipboard : {
                set ::(value){ 
                    topaz_view_manager__set_clipboard_from_string(a:value);
                },
                get ::{ 
                    return topaz_view_manager__get_clipboard_as_string();
                }
            },
            
    
            Filesystem  : {get : ::(){return __Filesystem__; }},
            RNG         : {get : ::(){return __RNG__; }},
            Display     : {get : ::(){return __Display__; }},
            Framebuffer : {get : ::(){return __Framebuffer__; }},
            Asset       : {get : ::(){return __Asset__; }},
            Mesh        : {get : ::(){return __Mesh__; }},
            Input       : {get : ::(){return __Input__; }},
            Audio       : {get : ::(){return __Audio__; }},
            Material    : {get : ::(){return __Material__; }},
            Entity      : {get : ::(){return __Entity__; }},
            Component   : {get : ::(){return __Component__; }},
            Resources   : {get : ::(){return __Resources__; }},
            Console     : {get : ::(){return __Console__; }},
            FontManager : {get : ::(){return __FontManager__; }},
            Image       : {get : ::(){return __Image__; }},
            Data        : {get : ::(){return __Data__; }},
            Sound       : {get : ::(){return __Sound__; }},
            Text2D      : {get : ::(){return __Text2D__; }},
            Scheduler   : {get : ::(){return __Scheduler__; }},
            StateControl: {get : ::(){return __StateControl__; }},
            Object2D    : {get : ::(){return __Object2D__; }},
            Shape2D     : {get : ::(){return __Shape2D__; }},
            Shape3D     : {get : ::(){return __Shape3D__; }},
            Automation  : {get : ::(){return __Automation__; }},
            Particle    : {get : ::(){return __Particle__; }},
            ParticleEmitter2D : {get : ::(){return __ParticleEmitter2D__; }},
            Vector : {get ::{return Vector;}},
            Color  : {get ::{return Color;}},
            RENDERER : {get ::{return RENDERER;}}


       };
    }
).new();
return Topaz;


