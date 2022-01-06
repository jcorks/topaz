@:TOPAZ = import(module:'Topaz.Constants');
@:Vector = import(module:'Topaz.Vector');
@:Color = import(module:'Topaz.Color');

@:topaz__debug = getExternalFunction(name:'topaz__debug');


@class = import(module:'Matte.Core.Class');

@Topaz = class(
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
                                match(introspect.keycount(of:args)) {
                                  (1): ::<={
                                    impl = nativeCreate(a:args[0]);
                                  },
                                  
                                  (2): ::<={
                                    impl = nativeCreate(a:args[0], b:args[1]);
                                  },
                                  
                                  (3): ::<={
                                    impl = nativeCreate(a:args[0], b:args[1], c:args[2]);
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
            @:topaz_filesystem_path__get_parent = getExternalFunction(name:'topaz_filesystem_path__get_parent');
            @:topaz_filesystem_path__get_nth_child = getExternalFunction(name:'topaz_filesystem_path__get_nth_child');
            @:topaz_filesystem_path__get_child_count = getExternalFunction(name:'topaz_filesystem_path__get_child_count');
            return {        
                getPath : ::(node) { 
                    return __Topaz__.Filesystem.Path.new(native:topaz_filesystem__get_path(a:node));
                },
                
                getPathFromString : ::(pth, str){ 
                    when (str == empty) __Topaz__.Filesystem.Path.new(native:topaz_filesystem__get_path_from_string(a:pth));            
                    return              __Topaz__.Filesystem.Path.new(native:topaz_filesystem__get_path_from_string(a:pth.native, b:str));                        
                },
                
                Path : class(
                    name : 'Topaz.Filesystem.Path',        
                    inherits : [__Native__],
                    define   : ::(this){ 
                        @impl;
                        
                        this.constructor = ::(native) {
                            impl = this.bindNative(instance : native);
                            return this;
                        };

                        this.interface = {
                            string : {
                                get ::{ 
                                    return topaz_filesystem_path__as_string(a:impl);
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
        @__ViewManager__ = ::<={
            @: topaz_view_manager__set_main = getExternalFunction(name:'topaz_view_manager__set_main');
            @: topaz_view_manager__get_main = getExternalFunction(name:'topaz_view_manager__get_main');
            @: topaz_view_manager__set_clipboard_from_string = getExternalFunction(name:'topaz_view_manager__set_clipboard_from_string');
            @: topaz_view_manager__get_clipboard_as_string = getExternalFunction(name:'topaz_view_manager__get_clipboard_as_string');
            return class(
                name : 'Topaz.ViewManager',        
                define :::(this) { 

                    this.interface = {
                        mainDisplay : {
                            set ::(value){ 
                                topaz_view_manager__set_main(a:value.native);
                            },
                            get ::{ 
                                return __Topaz__.Display.new(native:topaz_view_manager__get_main());
                            }
                        },
        
                        clipboard : {
                            set ::(value){ 
                                topaz_view_manager__set_clipboard_from_string(a:value);
                            },
                            get ::{ 
                                return topaz_view_manager__get_clipboard_as_string();
                            }
                        }
                    };
                }
            ).new();
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
            return class(
                name : 'Topaz.Display',        
                inherits : [__Native__],
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
                            topaz_display__set_parameter(a:impl, b:3, c:width);
                        },
        
                        addParameterCallback : ::(func){ 
                            return topaz_display__add_parameter_callback(a:impl, b:func);
                        },
        
                        addCloseCallback : ::(func){
                            return topaz_display__add_close_callback(a:impl, b:func);
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
        
                        resize : ::(width, height){ 
                            topaz_framebuffer__resize(a:impl, b:width, c:height);
                        },
        
                        filteredHint : {
                            get : ::(){ 
                                return topaz_framebuffer__get_filtered_hint(a:impl);
                            },
                            set : ::(value){ 
                                return topaz_framebuffer__set_filtered_hint(a:impl, b:value);
                            }
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
        
                        addObject : ::(object){ 
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
        
                        removeObject : ::(index){ 
                            topaz_mesh__remove_object(a:impl, b:index);
                        },
        
                        setObject : ::(index, value){ 
                            topaz_mesh__set_object(a:impl, b:index, c:value);
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
                    this.interface = {
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
                
                        getPadState : ::(input, pad) { 
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
                
                        addUnicodeListener : ::(listener) { 
                            return topaz_input__add_unicode_listener(a:listener);
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
                        setProgramData : ::(index, value){ 
                            topaz_material__set_program_data(a:impl, b:index, c:value);
                        },
        
                        getProgramData : ::(index){ 
                            return topaz_material__get_program_data(a:impl, b:index);
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
            @:topaz_data__get_nth_byte = getExternalFunction(name:'topaz_data__get_nth_byte');
            @:topaz_data__set = getExternalFunction(name:'topaz_data__set');
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
                            get : ::()   {return topaz_data__get_as_string(a:impl);} 
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
                                topaz_data__set(a:impl, b:value);
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
        
        
        @__Resources__ = ::<={
            @:topaz_resources__fetch_asset = getExternalFunction(name:'topaz_resources__fetch_asset');
            @:topaz_resources__create_asset = getExternalFunction(name:'topaz_resources__create_asset');
            @:topaz_resources__load_asset = getExternalFunction(name:'topaz_resources__load_asset');
            @:topaz_resources__load_asset_data = getExternalFunction(name:'topaz_resources__load_asset_data');
            @:topaz_resources__load_asset_base64 = getExternalFunction(name:'topaz_resources__load_asset_base64');
            @:topaz_resources__write_asset = getExternalFunction(name:'topaz_resources__write_asset');
            @:topaz_resources__remove_asset = getExternalFunction(name:'topaz_resources__remove_asset');
            @:topaz_resources__is_extension_supported = getExternalFunction(name:'topaz_resources__is_extension_supported');
            @:topaz_resources__get_path = getExternalFunction(name:'topaz_resources__get_path');
            @:topaz_resources__set_path = getExternalFunction(name:'topaz_resources__set_path');
            return class(
                name : 'Topaz.Resources',        
                define : ::(this){ 
                    @swtch = {};
                    swtch[TOPAZ.ASSET.TYPE.IMAGE] = ::(i){return __Topaz__.Image.new(native:i);}; 
                    swtch[TOPAZ.ASSET.TYPE.DATA] = ::(i){return __Topaz__.Data.new(native:i);};
                    swtch[TOPAZ.ASSET.TYPE.SOUND] = ::(i){return __Topaz__.Sound.new(native:i);}; 
                    swtch[TOPAZ.ASSET.TYPE.MATERIAL] = ::(i){return __Topaz__.Material.new(native:i);}; 
                    swtch[TOPAZ.ASSET.TYPE.MESH] = ::(i){return __Topaz__.Mesh.new(native:i);};

                    @_rawAssetToInstance = ::(impl) { 
                        when(impl == empty) empty;
                        @fn = swtch[topaz_asset__get_type(a:impl)];
                        when(fn == empty) __Topaz__.Asset.new(native:impl);
                        return fn(i:impl);
                    };
                
                
                    this.interface = {
                        fetchAsset : ::(type, name){ 
                            return _rawAssetToInstance(impl:topaz_resources__fetch_asset(a:type, b:name));
                        },

                        createAsset : ::(type) { 
                            return _rawAssetToInstance(impl:topaz_resources__create_asset(a:type));
                        },

                        loadAsset : ::(extension, path, name) { 
                            return _rawAssetToInstance(impl:topaz_resources__load_asset(a:extension, b:path, c:name));
                        },

                        loadAssetData : ::(extension, data, name) { 
                            return _rawAssetToInstance(impl:topaz_resources__load_asset_data(a:extension, b:data, c:name));
                        },
                        loadAssetBase64 : ::(extension, base64, name) { 
                            return _rawAssetToInstance(impl:topaz_resources__load_asset_base64(a:extension, b:base64, c:name));
                        },


                        writeAsset : ::(asset, extension, path) { 
                            return topaz_resources__write_asset(a:asset.native, b:extension, c:path);
                        },

                        removeAsset : ::(name) { 
                            topaz_resources__remove_asset(a:name);
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
                        registerFont : ::(name){ 
                            topaz_font_manager__register_font(a:name);
                        },

                        preloadGlyphs : ::(fontName, sizeRequest, characters){ 
                            topaz_font_manager__preload_glyphs(a:fontName, b:sizeRequest, c:characters);
                        },

                        unregisterFont : ::(fontName){ 
                            topaz_font_manager__unregister_font(a:fontName);
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
            @:topaz_entity__get_rotation = getExternalFunction(name:'topaz_entity__get_rotation');
            @:topaz_entity__get_position = getExternalFunction(name:'topaz_entity__get_position');
            @:topaz_entity__get_scale = getExternalFunction(name:'topaz_entity__get_scale');
            @:topaz_entity__set_rotation = getExternalFunction(name:'topaz_entity__set_rotation');
            @:topaz_entity__set_position = getExternalFunction(name:'topaz_entity__set_position');
            @:topaz_entity__set_scale = getExternalFunction(name:'topaz_entity__set_scale');
            @:topaz_entity__get_global_position = getExternalFunction(name:'topaz_entity__get_global_position');
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
                    
                    this.interface = {
                        isValid : {
                            get : ::(){ 
                                return topaz_entity__is_valid(a:this.native);
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
                                    
                                for(in:[0, introspect.keycount(of:value)], do:::(i) {
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
        
                        attach : ::(entity) { 
                            topaz_entity__attach(a:this.native, b:entity.native);
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
                            get : ::()  {return Vector.fromnative(native:topaz_entity__get_rotation(a:this.native));}, 
                            set : ::(value) {Vector.statepush(v:value);topaz_entity__set_rotation(a:this.native, b:value.native);} 
                        },

        
                        position : {
                            get : ::()   {return Vector.fromnative(native:topaz_entity__get_position(a:this.native)); }, 
                            set : ::(value)  {Vector.statepush(v:value);topaz_entity__set_position(a:this.native, b:value.native);} 
                        },
                        
                        scale : {
                            get : ::()  {return Vector.fromnative(native:topaz_entity__get_scale(a:this.native));}, 
                            set : ::(value) {Vector.statepush(v:value);topaz_entity__set_scale(a:this.native, b:value.native);}
                        },
                        
                        globalPosition : {
                            get : ::() {return Vector.fromnative(native:topaz_entity__get_global_position(a:this.native));} 
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
        
                        add : ::(component){ 
                            topaz_entity__add_component(a:this.native, b:component.native);
                        },
                    
                        addAfter : ::(component){ 
                            topaz_entity__add_component_after(a:this.native, b:component.native);
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
        
                                for(in:[0, introspect.keycount(of:value)], do:::(i) {
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
            @:topaz_text2d__get_position = getExternalFunction(name:'topaz_text2d__get_position');
            @:topaz_text2d__set_position = getExternalFunction(name:'topaz_text2d__set_position');
            @:topaz_text2d__set_scale = getExternalFunction(name:'topaz_text2d__set_scale');
            @:topaz_text2d__get_scale = getExternalFunction(name:'topaz_text2d__get_scale');
            @:topaz_text2d__set_rotation = getExternalFunction(name:'topaz_text2d__set_rotation');
            @:topaz_text2d__get_rotation = getExternalFunction(name:'topaz_text2d__get_rotation');
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
                    
                    @fontState;
                    @sizeState;
                    
                    this.interface = {
                        text : {
                            get : ::() {return topaz_text2d__get_text(a:impl);},
                            set : ::(value){ topaz_text2d__set_text(a:impl, b:value);} 
                        },
                        
                        font : {
                            get : ::()  {return fontState;},
                            set : ::(value) { 
                                fontState = value;
                                topaz_text2d__set_font(a:impl, b:fontState, c:sizeState);
                            }
                        },
                        
                        size :  {
                            get : ::()  {return sizeState;},
                            set : ::(value) { 
                                sizeState = value;
                                topaz_text2d__set_font(a:impl, b:fontState, c:sizeState);
                            }
                        },
                        
                        setColor : ::(color) { 
                            Color.statepush(v:color);
                            topaz_text2d__set_color(a:impl, b:color.native);
                        },
                        
                        setColorSection : ::(from, to, color) { 
                            Color.statepush(v:color);
                            topaz_text2d__set_color_section(a:impl, b:from, c:to, d:color.native);
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
                        
                        position : {
                            get : ::() {return Vector.fromnative(native:topaz_text2d__get_position(a:impl));}, 
                            set : ::(value){Vector.statepush(v:value);topaz_text2d__set_position(a:impl, b:value.native);} 
                        },
                        
                        rotation : {
                            get : ::() {return Vector.fromnative(native:topaz_text2d__get_rotation(a:impl));},
                            set : ::(value){Vector.statepush(v:value);topaz_text2d__set_rotation(a:impl, b:value.native);} 
                        },
                        
                        scale : {
                            get : ::() {return Vector.fromnative(native:topaz_text2d__get_scale(a:impl));}, 
                            set : ::(value){Vector.statepush(v:value);topaz_text2d__set_scale(a:impl, b:value.native);} 
                        },
                        
                        setAttribute : ::(attribute, value){ 
                            topaz_text2d__set_attribute(a:impl, b:attribute, c:value);
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
            @:topaz_scheduler__start_task = getExternalFunction(name:'topaz_scheduler__start_task');
            @:topaz_scheduler__start_task_simple = getExternalFunction(name:'topaz_scheduler__start_task_simple');
            @:topaz_scheduler__end_task = getExternalFunction(name:'topaz_scheduler__end_task');
            @:topaz_scheduler__pause = getExternalFunction(name:'topaz_scheduler__pause');
            @:topaz_scheduler__resume = getExternalFunction(name:'topaz_scheduler__resume');
            @:topaz_scheduler__get_task_interval_remaining = getExternalFunction(name:'topaz_scheduler__get_task_interval_remaining');
            @:topaz_scheduler__get_task_count = getExternalFunction(name:'topaz_scheduler__get_task_count');
            @:topaz_scheduler__get_task = getExternalFunction(name:'topaz_scheduler__get_task');


            return class(
                name : 'Topaz.Scheduler',        
                inherits :[__Component__],
                define : ::(this){ 
                    @impl;
                    this.constructor = ::(native) {
                        // whoops, the component constructor already made a generic component native.
                        // destroy it and make a real one
                        this.native.__ctx.destroy();

                        impl = this.bindNative(
                            instance : native,
                            nativeCreate : topaz_scheduler__create
                        );
                        return this;
                    };
                    
                    
                    this.interface = {
                        startTask : ::(taskName, interval, intervalDelay, callback){ 
                            if (taskName != empty) ::<={
                                topaz_scheduler__start_task(
                                    a:impl,
                                    b:taskName,
                                    c:interval,
                                    d:intervalDelay,
                                    e:callback
                                );
                                return taskName;
                            } else ::<={
                                return topaz_scheduler__start_task_simple(
                                    a:impl,
                                    b:interval,
                                    c:callback
                                );
                            };
                        },
                        
                        endTask : ::(name){ 
                            topaz_scheduler__end_task(a:impl, b:name);
                        },
                    
                        pause : ::(name){ 
                            topaz_scheduler__pause(a:impl, b:name);
                        },
                        
                        resume : ::(name) { 
                            topaz_scheduler__resume(a:impl, b:name);
                        },
                        
                        getTaskIntervalRemaining : ::(name){ 
                            return topaz_scheduler__get_task_interval_remaining(a:impl, b:name);
                        },
                        
                        tasks : {
                            get : ::(){ 
                                @out = [];
                                @len = topaz_scheduler__get_task_count(a:impl);
                                for(in:[0, len], do:::(i) {
                                    out[i] = topaz_scheduler__get_task(a:impl, b:i);
                                });
                                return out;
                            }
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
            @:topaz_object2d__get_group = getExternalFunction(name:'topaz_object2d__get_group');
            @:topaz_object2d__set_group = getExternalFunction(name:'topaz_object2d__set_group');
            @:topaz_object2d__set_collider = getExternalFunction(name:'topaz_object2d__set_collider');
            @:topaz_object2d__set_collider_radial = getExternalFunction(name:'topaz_object2d__set_collider_radial');
            @:topaz_object2d__get_collider_len = getExternalFunction(name:'topaz_object2d__get_collider_len');
            @:topaz_object2d__get_last_collided = getExternalFunction(name:'topaz_object2d__get_last_collided');
            @:topaz_object2d__get_collider_point = getExternalFunction(name:'topaz_object2d__get_collider_point');
            return class(
                name : 'Topaz.Object2D',        
                inherits :[__Component__],
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
                            topaz_object2d__add_velocity_towards(a:impl, b:amount, c:target, d:offset);
                        },

                        setVelocity : ::(amount, direction) { 
                            topaz_object2d__set_velocity(a:impl, b:amount, c:direction);
                        },

                        setVelocityTowards : ::(amount, target, offset) { 
                            topaz_object2d__set_velocity_towards(a:impl, b:amount, c:target, d:offset);
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
                            get : ::()  {return topaz_object2d__get_next_position(a:impl);} 
                        },

                        group : {
                            get : ::()  {return topaz_object2d__get_group(a:impl);}, 
                            set : ::(value) {topaz_object2d__set_group(a:impl, b:value);} 
                        },

                        collider : {
                            get : ::()  {return _collider; },
                            set : ::(value) {_collider=value; topaz_object2d__set_collider(a:impl, b:value);} 
                        },

                        setColliderRadial : ::(radius, numIterations) { 
                            topaz_object2d__set_collider_radial(a:impl, b:radius, c:numIterations);
                            _collider = [];
                            @iter = 0;
                            @len = topaz_object2d__get_collider_len(a:impl);
                            for(in:[0, len], do:::(i) {
                                @pt = topaz_object2d__get_collider_point(a:impl, b:i);
                                _collider[iter] = pt.x; iter += 1;
                                _collider[iter] = pt.y; iter += 1;
                            });
                        },

                        lastCollided : {
                            get : ::()  {return __Topaz__.Entity.new(native:topaz_object2d__get_last_collided(a:impl));} 
                        }
                    };                
                }
            );
        };
        @:topaz_object2d__set_group_interaction = getExternalFunction(name:'topaz_object2d__set_group_interaction');
        __Object2D__.setGroupInteraction = ::(groupA, groupB, interact){ 
            topaz_object2d__set_group_interaction(a:groupA, b:groupB, c:interact);
        };

        @__Shape2D__ = ::<={
            @:topaz_shape2d__create = getExternalFunction(name:'topaz_shape2d__create');
            @:topaz_shape2d__get_color = getExternalFunction(name:'topaz_shape2d__get_color');
            @:topaz_shape2d__set_color = getExternalFunction(name:'topaz_shape2d__set_color');
            @:topaz_shape2d__get_anim_speed = getExternalFunction(name:'topaz_shape2d__get_anim_speed');
            @:topaz_shape2d__set_anim_speed = getExternalFunction(name:'topaz_shape2d__set_anim_speed');
            @:topaz_shape2d__get_center = getExternalFunction(name:'topaz_shape2d__get_center');
            @:topaz_shape2d__set_center = getExternalFunction(name:'topaz_shape2d__set_center');
            @:topaz_shape2d__set_position = getExternalFunction(name:'topaz_shape2d__set_position');
            @:topaz_shape2d__get_position = getExternalFunction(name:'topaz_shape2d__get_position');
            @:topaz_shape2d__set_rotation = getExternalFunction(name:'topaz_shape2d__set_rotation');
            @:topaz_shape2d__get_rotation = getExternalFunction(name:'topaz_shape2d__get_rotation');
            @:topaz_shape2d__set_scale = getExternalFunction(name:'topaz_shape2d__set_scale');
            @:topaz_shape2d__get_scale = getExternalFunction(name:'topaz_shape2d__get_scale');
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
                    
                    this.interface = {
                        color : {
                            get : ::() {return Color.fromnative(native:topaz_shape2d__get_color(a:impl));}, 
                            set : ::(value){Color.statepush(v:value);topaz_shape2d__set_color(a:impl, b:value.native);} 
                        },


                        animSpeed : {
                            get : ::() {return topaz_shape2d__get_anim_speed(a:impl);},
                            set : ::(value){topaz_shape2d__set_anim_speed(a:impl, b:value);} 
                        },

                        center : {
                            get : ::() {return Vector.fromnative(native:topaz_shape2d__get_center(a:impl));},
                            set : ::(value){Vector.statepush(v:value);topaz_shape2d__set_center(a:impl, b:value.native);} 
                        },

                        position : {
                            get : ::() {return Vector.fromnative(native:topaz_shape2d__get_position(a:impl));},
                            set : ::(value){Vector.statepush(v:value);topaz_shape2d__set_position(a:impl, b:value.native);} 
                        },

                        rotation : {
                            get : ::() {return Vector.fromnative(native:topaz_shape2d__get_rotation(a:impl));},
                            set : ::(value){Vector.statepush(v:value);topaz_shape2d__set_rotation(a:impl, b:value.native);} 
                        },

                        scale : {
                            get : ::() {return Vector.fromnative(native:topaz_shape2d__get_scale(a:impl));}, 
                            set : ::(value){Vector.statepush(v:value);topaz_shape2d__set_scale(a:impl, b:value.native);} 
                        },

                        lines : {
                            get : ::() {return _lines;},
                            set : ::(value){topaz_shape2d__form_lines(a:impl, b:value); _lines = value;} 
                        },

                        triangles : {
                            get : ::() {return _tris;}, 
                            set : ::(value){topaz_shape2d__form_triangles(a:impl, b:value); _tris = value;} 
                        },

                        setAttribute : ::(attribute, value) { 
                            topaz_shape2d__set_attribute(a:impl, b:attribute, c:value);
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
            @:topaz_shape3d__get_position = getExternalFunction(name:'topaz_shape3d__get_position');
            @:topaz_shape3d__set_position = getExternalFunction(name:'topaz_shape3d__set_position');
            @:topaz_shape3d__set_rotation = getExternalFunction(name:'topaz_shape3d__set_rotation');
            @:topaz_shape3d__get_rotation = getExternalFunction(name:'topaz_shape3d__get_rotation');
            @:topaz_shape3d__get_scale = getExternalFunction(name:'topaz_shape3d__get_scale');
            @:topaz_shape3d__set_scale = getExternalFunction(name:'topaz_shape3d__set_scale');
            @:topaz_shape3d__set_attribute = getExternalFunction(name:'topaz_shape3d__set_attribute');
            @:topaz_shape3d__get_attribute = getExternalFunction(name:'topaz_shape3d__get_attribute');
            @:topaz_shape3d__set_texture = getExternalFunction(name:'topaz_shape3d__set_texture');
            @:topaz_shape3d__set_mesh = getExternalFunction(name:'topaz_shape3d__set_mesh');
            @:topaz_shape3d__set_sample_framebuffer = getExternalFunction(name:'topaz_shape3d__set_sample_framebuffer');
            @:topaz_shape3d__set_material = getExternalFunction(name:'topaz_shape3d__set_material');

            return class(
                name : 'Topaz.Shape3D',        
                inherits :[__Component__],
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
                    
                    
                    this.interface = {
                        position : {
                            get : ::() {return Vector.fromnative(native:topaz_shape3d__get_position(a:impl));},
                            set : ::(value){Vector.statepush(v:value);topaz_shape3d__set_position(a:impl, b:value.native);} 
                        },

                        rotation : {
                            get : ::() {return Vector.fromnative(native:topaz_shape3d__get_rotation(a:impl));}, 
                            set : ::(value){Vector.statepush(v:value);topaz_shape3d__set_rotation(a:impl, b:value.native);} 
                        },

                        scale : {
                            get : ::() {return Vector.fromnative(native:topaz_shape3d__get_scale(a:impl));},
                            set : ::(value){Vector.statepush(v:value);topaz_shape3d__set_scale(a:impl, b:value.native);} 
                        },

                        setAttribute : ::(attribute, value){ 
                            topaz_shape3d__set_attribute(a:impl, b:attribute, c:value);
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
                            return Vector.fromnative(native:topaz_automation__vector_at(a:impl, b:progress));
                        },

                        at : ::(progress) { 
                            return topaz_automation__at(a:impl, b:progress);
                        },

                        vector : {
                            get : ::() {return Vector.fromnative(native:topaz_automation__current_vector(a:impl));}
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
                        setAttribute : ::(attribute, value) { 
                            topaz_particle__set_attribute(a:impl, b:attribute, c:value);
                        },

                        setNoiseMin : ::(property, value) { 
                            topaz_particle__set_noise_min(a:impl, b:property, c:value);
                        },

                        setNoiseMax : ::(property, value) { 
                            topaz_particle__set_noise_max(a:impl, b:property, c:value);
                        },

                        setFunction : ::(property, value) { 
                            topaz_particle__set_function(a:impl, b:property, c:value);
                        },

                        string : {get : ::() {return topaz_particle__to_string(a:impl);}, set : ::(value){topaz_particle__set_from_string(a:impl, b:value);}}, 
                        image :  {set : ::(value) {return topaz_particle__set_image(a:impl, b:value);}}
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
        @:topaz__wait = getExternalFunction(name:'topaz__wait');
        @:topaz__log = getExternalFunction(name:'topaz__log');
        @:topaz__to_base64 = getExternalFunction(name:'topaz__to_base64');
        @:topaz__from_base64 = getExternalFunction(name:'topaz__from_base64');
        @:topaz__set_root = getExternalFunction(name:'topaz__set_root');
        @:topaz__get_root = getExternalFunction(name:'topaz__get_root');
        @:topaz__is_paused = getExternalFunction(name:'topaz__is_paused');
        @:topaz__get_time = getExternalFunction(name:'topaz__get_time');
        @:topaz__get_version_minor = getExternalFunction(name:'topaz__get_version_minor');
        @:topaz__get_version_micro = getExternalFunction(name:'topaz__get_version_micro');
        @:topaz__get_version_major = getExternalFunction(name:'topaz__get_version_major');
        this.interface = {
            run   : getExternalFunction(name:'topaz__run'),
            pause   : getExternalFunction(name:'topaz__pause'),
            pauseNow : getExternalFunction(name:'topaz__pause_now'),
            resume : getExternalFunction(name:'topaz__resume'),
            iterate : getExternalFunction(name:'topaz__iterate'),
            step : getExternalFunction(name:'topaz__step'),
            draw : getExternalFunction(name:'topaz__draw'),
            attachPreManager : ::(value){topaz__attach_pre_manager(a:value.native);},        
            attachPreManagerUnpausable : ::(value){topaz__attach_pre_manager_unpausable(a:value.native);},        
            attachPostManager : ::(value){topaz__attach_post_manager(a:value.native);},        
            attachPostManagerUnpausable : ::(value){topaz__attach_post_manager_unpausable(a:value.native);},        
            quit : getExternalFunction(name:'topaz__quit'),
            wait ::(FPS) {topaz__wait(a:FPS);},
            log ::(message, newline) {topaz__log(a:message, b:if(newline == empty) true else newline);},
            toBase64 ::(bytes) {topaz__to_base64(a:bytes);},
            fromBase64 : ::(string) { 
                return __Topaz__.Data.new(native:topaz__from_base64(a:string));
            },
            debug ::{
                topaz__debug();
            },
            
            root : {
                set : ::(value){ 
                    return topaz__set_root(a:value.native);
                },

                get : ::(){ 
                    @o = topaz__get_root();
                    when(o != empty) o.__ctx;
                }
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
            

            
            
            
            
            
            objectToString : ::(obj, levelSrc) {
                @checked = [];
                @levelG = if (levelSrc != 0) levelSrc else 10;
                @helper = ::(obj, level) {
                    when (obj == empty) 'empty';
                    when (!(introspect.type(of:obj) == Object)) '\''+obj + '\'';
                    when (checked[obj] != empty) '[Already Printed]';
                    checked[obj] = true;
                    @strOut = '{\n';
                    @keys = introspect.keys(of:obj);
                    @levelInv = levelG - level;
                    for(in:[0, introspect.length(of:keys)], do:::(i) {
                        @subStr = if (levelInv != 0) helper(obj:obj[keys[i]], level:level+1) else obj[keys[i]];
                        for(in:[0, level], do:::(n) {strOut += '  ';});
                        strOut += '  \'' + keys[i] + '\' : ' + subStr + ',\n'; 
                    });
                    for(in:[0, level], do:::(n){strOut += '  ';});
                    strOut += '}';
                    return strOut;
                };
                
                return helper(obj:obj, level:0) + '\n';
            },
    
            Filesystem  : {get : ::(){return __Filesystem__; }},
            RNG         : {get : ::(){return __RNG__; }},
            ViewManager : {get : ::(){return __ViewManager__; }},
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
            ParticleEmitter2D : {get : ::(){return __ParticleEmitter2D__; }}
       };
    }
).new();
return Topaz;

/////////////////////////////////
///// EXTENSION: packages
// NOTE: THIS IS A ROUGH DRAFT IMPLEMENTATION. It is an extension 
// made purely in scripting so it is subject to change! if a format
// or API does change, its symbols will be preserved in legacy.
//
//
// Grants the ability to read, check, and resolve packages and dependency trees 
// following the topaz package format.
//
// Topaz package format (Version 1)
//
// A topaz package is a JSON file written in the following format:
//
// 
/*
{
    "name"          : "Package Name",
    "version"      : {
        "major" : "2",
        "minor" : "0",
        "micro" : "0",
        "build" : "alpha"
    },    
    "formatVersion" : "1",
    "depends"       : [
        {
            "name"     : "Other Package",
            "minVersion" : {
                "major" : "1",
                "minor" : "3"
            }
        },
        {
            "name"     : "another Package",
            "minVersion" : {
                "major" : "0",
                "minor" : "1"
            }
        }
    ],
    "assets" : [
        {
            "assetName" : "topazAssetName",
            "assetType" : "script",
            "assetBase64" : "AAAAA79a9d0ef0x+.",
            "autorun" : "true"
        },
        {
            "assetName" : "topazAssetName2",
            "assetType" : "png",
            "assetBase64" : "AFBBA79a9d0ef0x+.",
        },
        {
            "assetName" : "topazAssetName4",
            "assetType" : "font",
            "assetPath" : "./mydata.ttf",
        }
    ]
}
*/
/*
// Some additional notes:
// - Version requires major and minor. Micro and build are for humans,
//   they are ignored by the reader.
// - the "depends" property may be empty or missing.
// - If true, autorun will run the script through the interpreter
//   when resolve()ing packages. Note that the dependency tree is 
//   taken into account for this: dependencies will always 
//   have their autorun scripts run first.
// - autorun scripts within the same package are run in order from 
//   first marked to last marked.
@Package = (::(){
    @packages = [];
    @packagesActive = [];
    @packageDB = {}; // indexed by string.

    @:loadAsset = ::(jsonAsset, ext) {
        if (jsonAsset.assetBase64 != empty) {
            @success = Topaz.Resources.loadAssetBase64(
                ext,
                jsonAsset.assetBase64,
                jsonAsset.assetName
            );
            if (success == empty) {
                throw new Error('Error loading ' + assetSrc.assetName + '!');
            }

        }
        if (jsonAsset.assetPath != empty) {
            @success = Topaz.Resources.loadAsset(
                ext,
                jsonAsset.assetPath,
                jsonAsset.assetName
            );    
            if (success == empty) {
                throw new Error('Error loading ' + jsonAsset.assetPath + '!');
            }
        }
    }

    // can throw, watch out
    @:makePackageFromJSON = ::(json) {

        if (json.formatVersion != 1) {
            throw new Error('Package version unrecognized (' + json.formatVersion + ')');
        }


        @pkg = {};
        pkg.name = json.name;
        pkg.version = json.version;
        pkg.assets = []; // strings of asset names
        pkg.depends = json.depends;
        if (!pkg.depends) {
            pkg.depends = [];
        }
        // not a valid array, tolerate it.
        if (pkg.depends.length == empty) {
            pkg.depends = [];
        }
        pkg.autorun = []; // strings of asset names
        pkg.resolved = false;

        for(@i = 0; i < json.assets.length; ++i) {
            @:assetSrc = json.assets[i];
            switch(assetSrc.assetType) {
              case 'script':
                if (assetSrc.autorun == "true") {
                    pkg.autorun.push(assetSrc.assetName);
                }
                loadAsset(assetSrc, '')
                break;


              case 'font':
                loadAsset(assetSrc, '')
                Topaz.FontManager.registerFont(
                    assetSrc.assetName
                );

              default:
                loadAsset(assetSrc, assetSrc.assetType)
                break;
            }


        }
        return pkg;
    };


    return {
        // Adds all assets for the package from a package file.
        read : ::(path) {
            @asset = Topaz.Resources.loadAsset('', path, path);
            if (!asset) {
                Topaz.log('Could not read package ' + path + '!');
                return;
            }
            try {
                @:pkg = makePackageFromJSON(JSON.parse(asset.string));
                packageDB[pkg.name] = pkg;
                packages.push(pkg);
            } catch(e) {
                Topaz.log(e);
            }
            Topaz.Resources.removeAsset(path);
        },


        // Adds all assets for the package from a package JSON string.
        readData : ::(jsonStr) {
            try {
                @:pkg = makePackageFromJSON(JSON.parse(jsonStr));
                packageDB[pkg.name] = pkg;
                packages.push(pkg);
            } catch(e) {
                Topaz.log(e);
            }
        },

        // Verifies that package exists and has been resolved.
        //
        // In the case that only a packageName is given, the 
        // ANY version of that package will be accepted.
        // If a versionObject is given, it will either look for 
        // the property "major" as in a major version number, or BOTH 
        // "minor" and "major". If a resolved package matches the name 
        // and version at or greater, this_ function will return true.
        // If not, an error is thrown.
        require : ::(packageName, versionObject) {
            @:pkg = packageDB[packageName];
            if (!pkg) {
                throw new Error('Unknown package ' + packageName);                
            }

            if (!pkg.resolved) {
                throw new Error('Package '+packageName+' has been pre-loaded but not resolved.');                
            }


            if (versionObject != empty) {

                if (versionObject.major != empty) {
                    if (versionObject.major > pkg.version.major) {
                        throw new Error('Package '+packageName+' has major version ' + pkg.version.major + ', but version ' + versionObject.major + ' is required.');                
                    }
                }

                if (versionObject.minor != empty &&
                    versionObject.major != empty) {
                     if (versionObject.major == pkg.version.major && 
                         versionObject.minor >  pkg.version.minor) {
                        throw new Error('Package '+packageName+' has version ' + pkg.version.major + '.' + pkg.version.minor + ', but version ' + versionObject.major + '.' + versionObject.minor + ' is required.');                                           
                    }
                }
            }

            return true;
        },


        // Has 2 functions:
        // 1.) Ensure that all pre-requisite packages are read.
        // 2.) Run, in order, any auto-run scripts
        // 
        // A package can only be resolved once. Resolving a package again 
        // results in no action taken.
        //
        // Note that resolving a package will resolve its dependencies first for you.
        resolve : ::(packageName) {   
            @pkg = packageDB[packageName];
            if (!pkg) {
                Topaz.log('No such package "' + packageName + '"');
                return false;
            }
            if (pkg.resolved) return true;
            pkg.resolved = true;

            // resolve dependencies first
            for(@i = 0; i < pkg.depends.length; ++i) {
                if (!this.resolve(pkg.depends[i].name)) {
                    Topaz.log('ERROR: Required package for ' + packageName + ' could not be resolved.');
                    pkg.resolved = false;
                    return false;                    
                }

                @:dep = packageDB[pkg.depends[i].name];
                @:majorNeeded = parseInt(pkg.depends[i].version.major);
                @:minorNeeded = parseInt(pkg.depends[i].version.minor);

                @:majorHave = pauseInt(dep.version.major);
                @:minorHave = pauseInt(dep.version.minor);

                if (
                    (majorHave < majorNeeded) 
                    ||
                    (majorHave == majorNeeded &&
                     minorHave <  minorNeeded)
                ) {
                    Topaz.log('ERROR: Required package version for ' + dep.name + 'is ' + majorNeeded + '.' + minorNeeded + ', but found ' + majorHave + ',' + minorHave);
                    pkg.resolved = false;
                    return false;
                } 
            }
            for(@i = 0; i < pkg.autorun.length; ++i) {
                try {
                    Topaz.import(pkg.autorun[i]);
                } catch (e) {
                    Topaz.log('Error while running script ' + pkg.autorun[i] + 'within package:');
                    Topaz.log(Topaz.objectToString(e));
                }
            }

            return true;
        },


        resolveAll : ::() {
            for(@i = 0; i < packages.length; ++i) {
                if (!this.resolve(packages[i].name)) {
                    return false;
                }
            }
            return true;
        },
        
        
        // Convenience function. Given a path to a 
        // directory:
        // - read()s all packages within the directory. Looks for the suffix ".package.json"
        // - calls resolveAll()
        importPath : ::(path) {
            @:obj = Topaz.Filesystem.getPathFromString(Topaz.Filesystem.getPathFromString(Topaz.Resources.path), path);
            @:children = obj.children;
            for(@i = 0; i < children.length; ++i) {
                @:subpath = children[i].string;
                if (subpath.indexOf('.package.json') != -1) {
                    Topaz.Package.read(subpath);
                }
            }
            Topaz.Package.resolveAll();
        },


        getKnownPackages : ::() {
            return packages;
        },


        
        Package Builder for formatVersion 1.
        This program looks for "setup_package.json", which consists of a file 
        familiar in format to a topaz package. However, instead of assetBase64,
        there is a property per-asset of "assetFile" which denotes a path to 
        a real file consisting of the raw data. The builder then reads each of these 
        files, creates a base64 equivalent of the data, and, upon success, returns a 
        package.json with the proper base64 of each asset.
        If debug is true for the package, the all assetFile's will be changed to 
        absolute assetPaths in the resultant package. This will make it quicker to debug
        source files, since they will always point to the filesystem asset.
        
        makePackage : ::(path) {
            @allPackages = [];
            @output = (::() {
                try {
                    @mainPath = Topaz.Filesystem.getPathFromString(
                        Topaz.Filesystem.getPath(TOPAZ.FILESYSTEM.DEFAULT_NODE.RESOURCES),
                        path
                    );
                    if (mainPath == empty) {
                        return "No such path.";
                    }
                    Topaz.Resources.path = mainPath.string;
        
                    @indata = Topaz.Resources.loadAsset('', 'setup_package.json', 'setup_package.json');
                    if (!(indata && indata.byteCount)) {
                        return ('Input file "setup_package.json" is empty or could not be opened. Exiting');
                    }
                    allPackages.push(indata.name);
                    @injson = JSON.parse(indata.string);    
                    @outjson = {};
                    outjson.formatVersion = 1;
        
                    outjson.name = injson.name;
        
                    @packageOutName = "package.json";
                    if (injson.outputName != empty) {
                        packageOutName = injson.outputName;
                    }
                    if (outjson.name == empty) {
                        return ('setup_package.json: missing "name" property!');
                    }
                    outjson.version = injson.version;
                    if (outjson.version == empty) {
                        return ('setup_package.json: missing "version" property!');
                    }
                    if (outjson.version.major == empty) {
                        return ('setup_package.json: missing "version.major" property!');
                    }
                    if (outjson.version.minor == empty) {
                        return ('setup_package.json: missing "version.major" property!');
                    }
                    if (outjson.version.micro == empty) {
                        Topaz.log('WARNING setup_package.json: missing "version.micro" property');
                    }
                    if (outjson.version.build == empty) {
                        Topaz.log('WARNING setup_package.json: missing "version.build" property');
                    }
        
                    @:debug = injson.debug == empty ? false : injson.debug;
        
                    outjson.depends = [];
                    if (injson.depends && injson.depends.length) {
                        for(@i = 0; i < injson.depends.length; ++i) {
                            outjson.depends.push(injson.depends[i]);
                        }
                    }
        
        
        
        
                    outjson.assets = [];
                    if (!(injson.assets && injson.assets.length)) {
                        Topaz.log('WARNING: setup_package.json specifies no assets!');
                    }
        
        
                    for(@i = 0; i < injson.assets.length; ++i) {
                        @asset = {};
                        asset.assetName = injson.assets[i].assetName;
                        asset.assetType = injson.assets[i].assetType;
                        if (injson.assets[i].autorun != empty) {
                            asset.autorun = injson.assets[i].autorun;
                        }
        
        
                        if (debug) {
                            asset.assetPath = Topaz.Filesystem.getPathFromString(
                                mainPath,
                                injson.assets[i].assetFile
                            ).string;
        
                            outjson.assets.push(asset);
                            Topaz.log(injson.assets[i].assetName + ' -> Added');
        
                        } else {
                            @:bufferIn = Topaz.Resources.loadAsset('', injson.assets[i].assetFile, injson.assets[i].assetName);
                            if (!(bufferIn && bufferIn.byteCount)) {
                                return ('setup_package.json: could not open asset ' + injson.assets[i].assetFile);
                            }
                            allPackages.push(bufferIn.name);
                            Topaz.log('Processing asset ' + injson.assets[i].assetName, false);
                            Topaz.log('.', false);
        
                            @:byteCount = bufferIn.byteCount;
                            @:bytes = bufferIn.bytes;
                            @:partition = Math.floor(byteCount/5);
        
                            Topaz.log('++.', false);
                            Topaz.log(' ', false);
        
        
                            asset.assetBase64 = Topaz.toBase64(bytes);                        
                            outjson.assets.push(asset);
                            Topaz.log('OK (' + Math.ceil(byteCount/1024) + '.' + Math.floor(((byteCount%1024) / 1024.0)*100) + 'KB)');
                        }
                    }
        
        
                    @output = JSON.stringify(outjson);
                    @outputAsset = Topaz.Resources.fetchAsset(TOPAZ.ASSET.TYPE.DATA, '__ASSET__39245s$');
                    @outputData = [];
                    Topaz.log('Generating output buffer', false);
                    @:length = output.length;
                    @:partition = Math.floor(length / 5);
                    for(@i = 0; i < length; ++i) {
                        if (i%partition == 0) {
                            Topaz.log('.', false);
                        }
                        outputData.push(output.charCodeAt(i));
                    }
                    outputAsset.bytes = outputData;    
                    Topaz.Resources.writeAsset(outputAsset, '', packageOutName);
                    return packageOutName+' written (' + Math.ceil(length/1024) + '.' + Math.floor(((length%1024) / 1024.0)*100) + 'KB)';
                } catch(e) {
                    return "Errors occured:" + e;
                }
            })();
            //cleanup
            for(@i = 0; i < allPackages.length; ++i) {
                Topaz.Resources.removeAsset(allPackages[i]);
            }
            return output;
        }
    }
})();
*/








