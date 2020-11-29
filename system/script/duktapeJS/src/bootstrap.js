topaz = {
    uniqueObjectPool : 0,
    run : topaz__run,
    pause : topaz__pause,
    'break' : topaz__break,
    resume : topaz__resume,
    iterate : topaz__iterate,
    step : topaz__step,
    draw : topaz__draw,
    attachPreManager : function(v){topaz__attach_pre_manager(v.impl);},        
    attachPreManagerUnpausable : function(v){topaz__attach_pre_manager_unpausable(v.impl);},        
    attachPostManager : function(v){topaz__attach_post_manager(v.impl);},        
    attachPostManagerUnpausable : function(v){topaz__attach_post_manager_unpausable(v.impl);},        
    quit : topaz__quit,
    wait : topaz__wait,
    log : topaz__log,
    objectToString : function(obj, levelSrc) {
        var checked = [];
        var levelG = levelSrc ? levelSrc : 10;
        var helper = function(obj, level) {
            if (obj === undefined) return 'undefined';
            if (obj === null) return 'null';
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

    input : {
        addKeyboardListener : function(obj) {
            topaz_input__add_keyboard_listener(obj);
        },
        addPadListener : function(obj, pad) {
            topaz_input__add_pad_listener(obj, pad);
        },
        addPointerListener : function(obj) {
            topaz_input__add_pointer_listener(obj);
        },
        addMappedListener : function(obj, str) {
            topaz_input__add_mappded_listener(obj, str);
        },
        removeListener : function(obj) {
            topaz_input__remove_listener(obj);
        },


        getState : function(i) {
            return topaz_input__get_state(i);
        },

        getPadState : function(i, p) {
            return topaz_input__get_pad_state(i, p);
        },

        getMappedState : function(i) {
            return topaz_input__get_mapped_state(i);
        },

        setDeadzone : function(a, b, c) {
            topaz_input__set_deadzone(a, b, c);
        },

        queryPads : function() {
            const len = topaz_input__query_pad_count();
            var out = [];
            for(var i = 0; i < len; ++i) {
                out.push(input_api__query_pad_id(i));
            }
            return out;
        },

        addUnicodeListener : function(l) {
            topaz_input__add_unicode_listener(l);
        },

        removeUnicodeListener : function(l) {
            topaz_input__remove_unicode_listener(l);
        },

        
        topazNotAnInput : 0,
        topazKey_0: 1, ///< 0
        topazKey_1: 2, ///< 1
        topazKey_2: 3, ///< 2
        topazKey_3: 4, ///< 3
        topazKey_4: 5, ///< 4
        topazKey_5: 6, ///< 5
        topazKey_6: 7, ///< 6
        topazKey_7: 8, ///< 7
        topazKey_8: 9, ///< 8
        topazKey_9: 10, ///< 9
        topazKey_a: 11, ///< a
        topazKey_b: 12, ///< b
        topazKey_c: 13, ///< c
        topazKey_d: 14, ///< d
        topazKey_e: 15, ///< e
        topazKey_f: 16, ///< f
        topazKey_g: 17, ///< g
        topazKey_h: 18, ///< h
        topazKey_i: 19, ///< i
        topazKey_j: 20, ///< j
        topazKey_k: 21, ///< k
        topazKey_l: 22, ///< l
        topazKey_m: 23, ///< m
        topazKey_n: 24, ///< n
        topazKey_o: 25, ///< o
        topazKey_p: 26, ///< p
        topazKey_q: 27, ///< q
        topazKey_r: 28, ///< r
        topazKey_s: 29, ///< s
        topazKey_t: 30, ///< t
        topazKey_u: 31, ///< u
        topazKey_v: 32, ///< v
        topazKey_w: 33, ///< w
        topazKey_x: 34, ///< x
        topazKey_y: 35, ///< y
        topazKey_z: 36, ///< z
        topazKey_lshift: 37, ///< Left shift key
        topazKey_rshift: 38, ///< Right shift key
        topazKey_lctrl: 39,  ///< Left control key
        topazKey_rctrl: 40,  ///< Right control key
        topazKey_lalt: 41,   ///< Left alt key
        topazKey_ralt: 42,   ///< Right alt key
        topazKey_tab: 43,    ///< Tab
        topazKey_F1: 44,     ///< F1
        topazKey_F2: 45,     ///< F2
        topazKey_F3: 46,     ///< F3
        topazKey_F4: 47,     ///< F4
        topazKey_F5: 48,     ///< F5
        topazKey_F6: 49,     ///< F6
        topazKey_F7: 50,     ///< F7
        topazKey_F8: 51,     ///< F8
        topazKey_F9: 52,     ///< F9
        topazKey_F10: 53,    ///< F10
        topazKey_F11: 54,    ///< F11
        topazKey_F12: 55,    ///< F12
        topazKey_up: 100,     ///< Up arrow
        topazKey_down: 101,   ///< Down arrow
        topazKey_left: 102,   ///< Left arrow
        topazKey_right: 103,  ///< Right arrow
        topazKey_minus: 104,  ///< -
        topazKey_equal: 105,  ///< = 
        topazKey_backspace: 106,  ///< Backspace
        topazKey_grave: 107,  ///< `
        topazKey_esc: 108,    ///< Escape
        topazKey_home: 109,   ///< Home key
        topazKey_pageUp: 110, ///< Page up key
        topazKey_pageDown: 111,  ///< Page down key
        topazKey_end: 112,    ///< End key
        topazKey_backslash: 113, ///< '\'
        topazKey_lbracket: 114, ///< [
        topazKey_rbracket: 115, ///< ]
        topazKey_semicolon: 116, ///< ;
        topazKey_apostrophe: 117, ///< '
        topazKey_frontslash: 118, ///< /
        topazKey_enter: 119, ///< Enter
        topazKey_delete: 120, ///< Delete
        topazKey_numpad0: 121, ///< Numpad 0
        topazKey_numpad1: 122, ///< Numpad 1
        topazKey_numpad2: 123, ///< Numpad 2
        topazKey_numpad3: 124, ///< Numpad 3
        topazKey_numpad4: 125, ///< Numpad 4
        topazKey_numpad5: 126, ///< Numpad 5
        topazKey_numpad6: 127, ///< Numpad 6
        topazKey_numpad7: 128, ///< Numpad 7
        topazKey_numpad8: 129, ///< Numpad 8
        topazKey_numpad9: 130, ///< Numpad 9
        topazKey_prtscr: 131, ///< Print screen button
        topazKey_lsuper: 132, ///< Left Super key (Windows key)
        topazKey_rsuper: 133, ///< Right Super key (Windows key)
        topazKey_space: 134,  ///< Space
        topazKey_insert: 135, ///< Insert key
        topazKey_comma: 136, ///< ,
        topazKey_period: 137 , ///< .
        topazKey_world1: 138, /// I8n key0
        topazKey_world2: 139, /// I8n key1
        topazKey_world3: 140, /// I8n key2
        topazKey_world4: 141, /// I8n key3
        topazKey_world5: 142, /// I8n key4
        topazKey_world6: 143, /// I8n key5
        topazKey_world7: 144, /// I8n key6
        topazKey_world8: 145, /// I8n key7
        topazKey_world9: 146, /// I8n key8
    
        topazPointer_0: 256, ///< Left click
        topazPointer_1: 257, ///< Right click
        topazPointer_2: 258, ///< Middle click
    
        topazPointer_X: 259, ///< Horizontal axis. Usually for the X axis of the pointer
        topazPointer_Y: 260, ///< Horizontal axis. Usually for the X axis of the pointer
        topazPointer_Wheel: 261, ///< Mouse wheel.
    
        topazPad_a: 300,     ///< Button 0
        topazPad_b: 301,     ///< Button 1
        topazPad_c: 302,     ///< Button 2
        topazPad_x: 303,     ///< Button 3
        topazPad_y: 304,     ///< Button 4
        topazPad_r: 305,     ///< Button 5
        topazPad_l: 306,     ///< Button 6
        topazPad_r2: 307,    ///< Button 7
        topazPad_l2: 308,    ///< Button 8
        topazPad_r3: 309,    ///< Button 9
        topazPad_l3: 310,    ///< Button 10
        topazPad_start: 311,    ///< Button 11
        topazPad_select: 312,///< Button 12
        topazPad_b13: 313,///< Button 13
        topazPad_b14: 314,///< Button 14
        topazPad_b15: 315,///< Button 15
        topazPad_b16: 316,///< Button 16
        topazPad_b17: 317,///< Button 17
        topazPad_b18: 318,///< Button 18
        topazPad_b19: 319,///< Button 19
        topazPad_b20: 320,///< Button 20
        topazPad_b21: 321,///< Button 21
        topazPad_b22: 322,///< Button 22
        topazPad_b23: 323,///< Button 23
        topazPad_b24: 324,///< Button 24
        topazPad_b25: 325,///< Button 25
        topazPad_b26: 326,///< Button 26
        topazPad_b27: 327,///< Button 27
        topazPad_b28: 328,///< Button 28
        topazPad_b29: 329,///< Button 29
        topazPad_b30: 330,///< Button 30
        topazPad_b31: 331,///< Button 31
        topazPad_b32: 332,///< Button 32
    
        topazPad_axisX: 400, ///< X button
        topazPad_axisY: 401, ///< Y button
        topazPad_axisZ: 402, ///< Z button
        topazPad_axisX2: 403,///< X2 button 
        topazPad_axisY2: 404,///< Y2 button
        topazPad_axisZ2: 405,///< Z2 button
        topazPad_axisX3: 406,///< X3 button
        topazPad_axisY3: 407,///< Y3 button
        topazPad_axisZ3: 408,///< Z3 button
        topazPad_axisX4: 409,///< X4 button
        topazPad_axisY4: 410,///< Y4 button
        topazPad_axisZ4: 411,///< Z4 button
        topazPad_axisX5: 412,///< X4 button
        topazPad_axisY5: 413,///< Y4 button
        topazPad_axisZ5: 414,///< Z4 button
        topazPad_axisX6: 415,///< X4 button
        topazPad_axisY6: 416,///< Y4 button
        topazPad_axisZ6: 417,///< Z4 button
    
        topazPad_axisR: 450,   
        topazPad_axisL: 451,    
        topazPad_axisR2: 452,    
        topazPad_axisL2: 453,    
        topazPad_axisR3: 454,    
        topazPad_axisL3: 455,    
        topazPad_axisR4: 456,    
        topazPad_axisL4: 457,    
    
        topazPad_options: 511,
        topazInput_Count: 512




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
        impl.__ctx = this;



        Object.defineProperty(this, 'isValid', {get : function(){return topaz_entity__is_valid(impl);}}); 
        
        this.remove = function() {
            topaz_entity__remove(impl);
        }


        Object.defineProperty(this, 'childCount', {get : function(){return topaz_entity__get_child_count(impl);}});

        this.nthChild = function(n) {
            var f = topaz_entity__get_nth_child(impl, n);
            if (f.__ctx) return f.__ctx;
            return new topaz.entity(undefined, f);
        }

        
        Object.defineProperty(this, 'children', {
            get : function(){
                var children = [];
                const len = topaz_entity__get_child_count(impl);
                for(var i = 0; i < len; ++i) {
                    children.push(topaz_entity__get_nth_child(impl, i).__ctx);
                }
                return children;
            }
        });



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
            this.name = props.name;
            if (props.onStep) topaz_entity__set_on_step(impl, function(e){props.onStep(e.__ctx.props)});
            if (props.onDraw) topaz_entity__set_on_draw(impl, function(e){props.onDraw(e.__ctx.props)});
            if (props.onPreStep) topaz_entity__set_on_pre_step(impl, function(e){props.onPreStep(e.__ctx.props)});
            if (props.onPreDraw) topaz_entity__set_on_pre_draw(impl, function(e){props.onPreDraw(e.__ctx.props)});
            if (props.onAttach) topaz_entity__set_on_attach(impl, function(e){props.onAttach(e.__ctx.props)});
            if (props.onDetach) topaz_entity__set_on_detach(impl, function(e){props.onDetach(e.__ctx.props)});
            if (props.onRemove) topaz_entity__set_on_remove(impl, function(e){props.onRemove(e.__ctx.props)});
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
        //topaz.log('REFERENCE CREATED:\n' + topaz.objectToString(this));

    },
    componentNull : function() {return topaz_component__null();},
    component : function(defineProps, implPre) {
        //if (implPre)
            //topaz.log('REFERENCE CREATED:\n' + topaz.objectToString(this));

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
            ctx.events = props.events;

            


            ctx.props.component = this;
            ctx.tag = props.tag;
            impl.onStep = props.onStep ? function(e){props.onStep(e.__ctx.props);} : undefined;
            impl.onDraw = props.onDraw ? function(e){props.onDraw(e.__ctx.props);} : undefined;
            impl.onAttach = props.onAttach ? function(e){props.onAttach(e.__ctx.props);} : undefined;
            impl.onDetach = props.onDetach ? function(e){props.onDetach(e.__ctx.props);} : undefined;
            impl.onDestroy = props.onDestroy ? function(e){props.onDestroy(e.__ctx.props);} : undefined;

            
            var keys = Object.keys(props.events);
            for(var i = 0; i < keys.length; ++i) {
                topaz_component__install_event(
                    impl, 
                    keys[i],
                    (function(fn){ // proper reference capture
                        return function(c) {
                            fn(ctx.props);
                        }
                    })(props.events[keys[i]])
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
            topaz_component__install_event(impl, event, function(component, entity) {
                callback(component.__ctx, entity.__ctx);
            });
        }

        this.uninstallEvent = function(event) {
            topaz_component__uninstall_event(impl, event);
        }

        this.installHook = function(event, callback) {
            return topaz_component__install_hook(impl, event, function(component, entity) {
                callback(component.__ctx, entity.__ctx);
            });
        }
        this.uninstallHook = function(event, id) {
            topaz_component__uninstall_hook(impl, event, id);
        }

        this.installHandler = function(event, callback) {
            return topaz_component__install_handler(impl, event, function(component, entity) {
                callback(component.__ctx, entity.__ctx);
            });
        }
        this.uninstallHandler = function(event, id) {
            topaz_component__uninstall_handler(impl, event, id);
        }



        this.impl = impl;
        if (defineProps) {
            ctx.define(defineProps);
        }

    },
    object2d : function(implPre) {
        var ctx = this;
        var impl;

        if (implPre) 
            impl = implPre;
        else {
            impl = topaz_object2d__create();
        }
        impl.__ctx = this;

        
        // initialize with component properties
        const componentInit = topaz.component.bind(this);;
        componentInit(undefined, impl);

        this.addVelocity = function(a, b) {
            topaz_object2d__add_velocity(impl, a, b);
        }

        this.addVelocityTowards = function(a, b, c) {
            topaz_object2d__add_velocity_towards(impl, a, b, c);
        }

        this.setVelocity = function(a, b) {
            topaz_object2d__set_velocity(impl, a, b);
        }

        this.setVelocityTowards = function(a, b, c) {
            topaz_object2d__set_velocity_towards(impl, a, b, c);
        }

        
        Object.defineProperty(
            this,
            'frictionX', {
                get : function() { return topaz_object2d__get_friction_x(impl);},
                set : function(v){ topaz_object2d__set_friction_x(impl, v);}
            }
        );

        Object.defineProperty(
            this,
            'frictionY', {
                get : function() { return topaz_object2d__get_friction_y(impl);},
                set : function(v){ topaz_object2d__set_friction_y(impl, v);}
            }
        );

        Object.defineProperty(
            this,
            'direction', {
                get : function() { return topaz_object2d__get_direction(impl);},
                set : function(v){
                    topaz_object2d__set_velocity(
                        impl,
                        topaz_object2d__get_speed(impl),
                        topaz_object2d__get_direction(impl)
                    );
                }
            }
        );


        this.halt = function() {topaz_object2d__halt(impl);}
        this.resetMotion = function() {topaz_object2d__reset_motion(imple);}

        Object.defineProperty(
            this,
            'velocityX', {
                get : function() { return topaz_object2d__get_velocity_x(impl);},
                set : function(v){ topaz_object2d__set_velocity_x(impl, v);}
            }
        );

        
        Object.defineProperty(
            this,
            'velocityY', {
                get : function() { return topaz_object2d__get_velocity_y(impl);},
                set : function(v){ topaz_object2d__set_velocity_y(impl, v);}
            }
        );


        Object.defineProperty(
            this,
            'speed', {
                get : function() { return topaz_object2d__get_speed(impl);},
                set : function(v){ topaz_object2d__set_speed(impl, v);}
            }
        );

        Object.defineProperty(
            this,
            'nextPosition', {
                get : function() { return topaz_object2d__get_next_position(impl);}
            }
        );

        Object.defineProperty(
            this,
            'group', {
                get : function() { return topaz_object2d__get_group(impl);},
                set : function(v){ topaz_object2d__set_group(impl, v);}
            }
        );


        this.setGroupInteraction = function(a, b, c) {
            topaz_object2d__set_group_interaction(a, b, c);
        }

        var collider = [];
        Object.defineProperty(
            this,
            'collider', {
                get : function() { return collider; },
                set : function(v){ collider=v; topaz_object2d__set_collider(impl, v);}
            }
        );
        

        this.setColliderRadial = function(a, b) {
            topaz_object2d__set_collider_radial(impl, a, b);
            collider = [];
            const len = topaz_object2d__get_collider_len(impl);
            for(var i = 0; i < len; ++i) {
                const pt = topaz_object2d__get_collider_point(impl, i);
                collider.push(pt.x);
                collider.push(pt.y);
            }
        }

        Object.defineProperty(
            this,
            'lastCollided', {
                get : function() { return new topaz.entity(undefined, topaz_object2d__get_last_collided(impl));}
            }
        );

        

    },
    shape2d : function(implPre) {
        var ctx = this;
        var impl;

        if (implPre) 
            impl = implPre;
        else {
            impl = topaz_shape2d__create();
        }
        impl.__ctx = this;

        
        // initialize with component properties
        const componentInit = topaz.component.bind(this);;
        componentInit(undefined, impl);
        Object.defineProperty(
            this,
            'color', {
                get : function() {return new topaz.color('', topaz_shape2d__get_color(impl));},
                set : function(v){topaz_shape2d__set_color(impl, v.impl);}
            }
        );

        Object.defineProperty(
            this,
            'animSpeed', {
                get : function() {return topaz_shape2d__get_anim_speed(impl);},
                set : function(v){topaz_shape2d__set_anim_speed(impl, v);}
            }
        );

        Object.defineProperty(
            this,
            'center', {
                get : function() {return new topaz.vector(0, 0, 0, topaz_shape2d__get_center(impl));},
                set : function(v){topaz_shape2d__set_center(impl, v.impl);}
            }
        );


        Object.defineProperty(
            this,
            'position', {
                get : function() {return new topaz.vector(0, 0, 0, topaz_shape2d__get_position(impl));},
                set : function(v){topaz_shape2d__set_position(impl, v.impl);}
            }
        );

        Object.defineProperty(
            this,
            'rotation', {
                get : function() {return new topaz.vector(0, 0, 0, topaz_shape2d__get_rotation(impl));},
                set : function(v){topaz_shape2d__set_rotation(impl, v.impl);}
            }
        );

        Object.defineProperty(
            this,
            'scale', {
                get : function() {return new topaz.vector(0, 0, 0, topaz_shape2d__get_scale(impl));},
                set : function(v){topaz_shape2d__set_scale(impl, v.impl);}
            }
        );


        this.formRectangle = function(a, b) {
            topaz_shape2d__form_rectangle(impl, a, b);
        }

        this.formRadial = function(a, b) {
            topaz_shape2d__form_radial(impl, a, b);
        }


        var lines;
        Object.defineProperty(
            this,
            'lines', {
                get : function() {return lines;},
                set : function(v){topaz_shape2d__form_lines(impl, v); lines = v;}
            }
        );


        var tris;
        Object.defineProperty(
            this,
            'triangles', {
                get : function() {return tris;},
                set : function(v){topaz_shape2d__form_triangles(impl, v); tris = v;}
            }
        );

        

    },
    text2d : function(implPre) {
        var ctx = this;
        var impl;

        if (implPre) 
            impl = implPre;
        else {
            impl = topaz_text2d__create();
        }
        impl.__ctx = this;

        
        // initialize with component properties
        const componentInit = topaz.component.bind(this);;
        componentInit(undefined, impl);
        

        Object.defineProperty(
            this,
            'text', {
                get : function() {return topaz_text2d__get_text(impl);},
            }
        );

        this.setText = function(text, c) {
            topaz_text2d__set_text(impl, text, c);
        }
        this.setTextMonospace = function(text, c) {
            topaz_text2d__set_text_monospace(impl, text, c);
        }


        this.setColor = function(c) {
            topaz_text2d__set_color(impl, c.impl);
        }

        this.setColorSection = function(from, to, c) {
            topaz_text2d__set_color_section(impl, from, to, c.impl);
        }


        Object.defineProperty(
            this,
            'extentWidth', {
                get : function() {return topaz_text2d__get_extent_width(impl);}
            }
        );

        Object.defineProperty(
            this,
            'extentHeight', {
                get : function() {return topaz_text2d__get_extent_height(impl);}
            }
        );


        this.getCharX = function(i) {
            return topaz_text2d__get_char_x(impl, i);
        }

        this.getCharY = function(i) {
            return topaz_text2d__get_char_y(impl, i);
        }


        Object.defineProperty(
            this,
            'position', {
                get : function() {return new topaz.vector(0, 0, 0, topaz_text2d__get_position(impl));},
                set : function(v){topaz_text2d__set_position(impl, v.impl);}
            }
        );

        Object.defineProperty(
            this,
            'rotation', {
                get : function() {return new topaz.vector(0, 0, 0, topaz_text2d__get_rotation(impl));},
                set : function(v){topaz_text2d__set_rotation(impl, v.impl);}
            }
        );

        Object.defineProperty(
            this,
            'scale', {
                get : function() {return new topaz.vector(0, 0, 0, topaz_text2d__get_scale(impl));},
                set : function(v){topaz_text2d__set_scale(impl, v.impl);}
            }
        );



        

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
Object.defineProperty(topaz.input, 'mouseX', {get : function(){return topaz_input__mouse_x();}});
Object.defineProperty(topaz.input, 'mouseY', {get : function(){return topaz_input__mouse_y();}});
Object.defineProperty(topaz.input, 'mouseDeltaX', {get : function(){return topaz_input__mouse_delta_x();}});
Object.defineProperty(topaz.input, 'mouseDeltaY', {get : function(){return topaz_input__mouse_delta_y();}});
Object.defineProperty(topaz.input, 'mouseWheel', {get : function(){return topaz_input__mouse_wheel();}});


