"use strict"
var Topaz = {
    uniqueObjectPool : 0,
    run : topaz__run,
    pause : topaz__pause,
    pauseNow : topaz__pause_now,
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
    import : topaz_script__import,
    log : topaz__log,
    toBase64 : topaz__to_base64,
    fromBase64 : function(f) {
        return new Topaz.Data(topaz__from_base64(f));
    },

    Key :{
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
    
    Render2D : {
        Parameter : {
            alphaRule : 0,
            depthTest : 1,
            etchRule : 2,
            textureFilterHint : 3
        }    
    },
    
    Renderer : {
        EtchRule : {
            noEtching : 0,
            define : 1,
            undefine : 2,
            in : 3,
            out : 4
        },
        
        DepthTest : {
            less : 0,
            LEQ : 1,
            greater : 2,
            GEQ : 3,
            equal : 4,
            none : 5
        },

        AlphaRule : {
            allow : 0,
            opaque : 1,
            translucent : 2,
            invisible : 3,
        },
        
        TextureFilterHint : {
            linear : 0,
            none : 1,
        }

    },




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
    deadEntityPool : [],
    Filesystem : {
        DefaultNode : {
            resources : 0,
            topaz : 1,
            userData : 2
        },
        
        getPath : function(n) {
            return new Topaz.Filesystem.Path(topaz_filesystem__get_path(n));
        },
        
        getPathFromString : function(pth, str) {
            if (str == undefined) {
                return new Topaz.Filesystem.Path(topaz_filesystem__get_path_from_string(pth));            
            } else {
                return new Topaz.Filesystem.Path(topaz_filesystem__get_path_from_string(pth.impl, str));                        
            }
        },
        
        
        Path : function(implPre) {
            var impl;
            this.uniqueID = Topaz.uniqueObjectPool++;
            if (implPre) 
                impl = implPre;
            else {
                throw new Error("path object cannot be make without a LL instance.");
            }
            impl.__ctx = this;
            this.impl = impl;          
        }

        
    },
    ViewManager : {

    },

    Display : function(implPre) {
        var impl;

        if (implPre)
            impl = implPre;
        else 
            impl = topaz_view_manager__create_display();

        impl.__ctx = this;
        this.impl = impl;
    
    },
    Input : {
        addKeyboardListener : function(obj) {
            return topaz_input__add_keyboard_listener(obj);
        },
        addPadListener : function(obj, pad) {
            return topaz_input__add_pad_listener(obj, pad);
        },
        addPointerListener : function(obj) {
            return topaz_input__add_pointer_listener(obj);
        },
        addMappedListener : function(obj, str) {
            return topaz_input__add_mappded_listener(obj, str);
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

        
    },
    
    Audio : {
        PlaybackSound : function(implPre) {
            this.impl = implPre;
            if (!this.impl) {
                this.impl = 0; // not a real sound, probably.
            }
        },
    
        playSound : function(asset, channel) {
            return new Topaz.Audio.PlaybackSound(topaz_audio__play_sound(asset.impl, channel == undefined ? 0 : channel));
        },
        
        channelHalt : function(channel) {
            topaz_audio__channel_halt(channel);
        },
        
        channelSetVolume : function(channel, volume) {
            topaz_audio__channel_set_volume(channel, volume);
        },

        channelSetPanning : function(channel, panning) {
            topaz_audio__channel_set_panning(channel, panning);
        }

    },

    Resources : {
        AssetType : {
            none : 0,
            image : 1,
            sound : 2,
            model : 3,
            particle : 4,
            data : 5,
            actor : 6
        },
        
        _rawAssetToInstance : function(impl) {
            if (!impl) return undefined;
            switch(topaz_asset__get_type(impl)) {
              case Topaz.Resources.AssetType.image: return new Topaz.Image(impl); break;
              case Topaz.Resources.AssetType.data:  return new Topaz.Data (impl); break;
              case Topaz.Resources.AssetType.sound:  return new Topaz.Sound(impl); break;
            }
            return new Topaz.Asset(impl);

        },

        fetchAsset : function(type, name) {
            return Topaz.Resources._rawAssetToInstance(topaz_resources__fetch_asset(type, name));
        },

        loadAsset : function(ext, path, name) {
            return Topaz.Resources._rawAssetToInstance(topaz_resources__load_asset(ext, path, name));
        },

        loadAssetData : function(ext, data, name) {
            return Topaz.Resources._rawAssetToInstance(topaz_resources__load_asset_data(ext, data, name));
        },
        loadAssetBase64 : function(ext, b64, name) {
            return Topaz.Resources._rawAssetToInstance(topaz_resources__load_asset_base64(ext, b64, name));
        },


        writeAsset : function(asset, ext, path) {
            return topaz_resources__write_asset(asset.impl, ext, path);
        },

        removeAsset : function(name) {
            topaz_resources__remove_asset(name);
        },

        

        isExtensionSupported : function(ext) {
            return topaz_resources__is_extension_supported(ext);
        }
    },

    FontManager : {
        registerFont : function(n) {
            topaz_font_manager__register_font(n);
        },

        preloadGlyphs : function(a, c, b) {
            topaz_font_manager__preload_glyphs(a, c, b);
        },

        unregisterFont : function(n) {
            topaz_font_manager__unregister_font(n);
        },

    },
    _assetSetCommonSymbols : function(obj) {

        Object.defineProperty(
            obj,
            'type', {
                get : function()  { return topaz_asset__get_type(this.impl);},
                set : function(v) {        topaz_asset__set_type(this.impl, v);}
            }
        );

        Object.defineProperty(
            obj,
            'name', {
                get : function()  { return topaz_asset__get_name(this.impl);},
                set : function(v) {        topaz_asset__set_name(this.impl, v);}
            }
        );
    },

    Image : function(implPre) {
        var impl;

        if (implPre)
            impl = implPre;
        else {
            throw new Error("Image asset cannot be make without a LL asset instance.");
        }
        impl.__ctx = this;
        this.uniqueID = Topaz.uniqueObjectPool++;
        this.impl = impl;


    },

    Data : function(implPre) {
        var impl;

        if (implPre)
            impl = implPre;
        else {
            throw new Error("Data asset cannot be make without a LL asset instance.");
        }
        impl.__ctx = this;
        this.uniqueID = Topaz.uniqueObjectPool++;
        this.impl = impl;

    },

    Sound : function(implPre) {
        var impl;

        if (implPre)
            impl = implPre;
        else {
            throw new Error("Sound asset cannot be make without a LL asset instance.");
        }
        impl.__ctx = this;
        this.uniqueID = Topaz.uniqueObjectPool++;
        this.impl = impl;

    },



    Entity : function(defineProps, implPre) {
        this.uniqueID = Topaz.uniqueObjectPool++;
        var impl;

        if (implPre)
            impl = implPre;
        else 
            impl = topaz_entity__create();
        impl.__ctx = this;
        this.impl = impl;

        if (defineProps) {
            this.define(defineProps);
        }
    
        //Topaz.log('REFERENCE CREATED:\n' + Topaz.objectToString(this));

        
    },



    Text2D : function(implPre) {
        var impl;
        this.uniqueID = Topaz.uniqueObjectPool++;
        this.sizeState = 12;
        this.fontState = '';
        if (implPre) 
            impl = implPre;
        else {

            impl = topaz_text2d__create();
        }
        impl.__ctx = this;
        this.impl = impl;
        

    },



    Scheduler : function(type, implPre) {
        var impl;
        this.uniqueID = Topaz.uniqueObjectPool++;
        if (implPre) 
            impl = implPre;
        else {

            impl = topaz_scheduler__create(type);
        }
        impl.__ctx = this;
        this.impl = impl;
    },


    StateControl : function(implPre) {
        var impl;
        this.uniqueID = Topaz.uniqueObjectPool++;
        if (implPre) 
            impl = implPre;
        else {

            impl = topaz_state_control__create();
        }
        impl.__ctx = this;
        this.impl = impl;

    },



    Component : function(defineProps, implPre) {
        //if (implPre)
            //Topaz.log('REFERENCE CREATED:\n' + Topaz.objectToString(this));

        this.uniqueID = Topaz.uniqueObjectPool++;
        var impl;

        if (implPre) 
            impl = implPre;
        else {
            impl = topaz_component__create('');
        }
        impl.__ctx = this;

        this.impl = impl;
        if (defineProps) {
            this.define(defineProps);
        }

    },



    Object2D : function(implPre) {
        var impl;
        this.uniqueID = Topaz.uniqueObjectPool++;
        if (implPre) 
            impl = implPre;
        else {
            impl = topaz_object2d__create();
        }
        impl.__ctx = this;
        this.impl = impl;

    },
    Shape2D : function(implPre) {
        var impl;
        this.uniqueID = Topaz.uniqueObjectPool++;
        if (implPre) 
            impl = implPre;
        else {
            impl = topaz_shape2d__create();
        }
        impl.__ctx = this;
        this.impl = impl;

    },

    Automation : function(implPre) {
        var impl;
        this.uniqueID = Topaz.uniqueObjectPool++;
        if (implPre) 
            impl = implPre;
        else {
            impl = topaz_automation__create();
        }
        impl.__ctx = this;
        this.impl = impl;
    },


    Color : function(name, implPre) {
        var impl;
        this.uniqueID = Topaz.uniqueObjectPool++;
        if (implPre)
            impl = implPre;
        else 
            impl = topaz_color__create();

        this.impl = impl;
        if (name)
            this.string = name;

    },


    Vector : function(x, y, z, implPre) {
        var impl; 
        if (implPre)
            impl = implPre;
        else 
            impl = topaz_vector__create(x, y, z);



        this.impl = impl;
    },
    

    Particle : function(implPre) {
        var impl; 
        if (implPre)
            impl = implPre;
        else 
            impl = topaz_particle__create();

        this.impl = impl;
    },

    ParticleEmitter2D : function(implPre) {
        var impl; 
        if (implPre)
            impl = implPre;
        else 
            impl = topaz_particle_emitter_2d__create();

        this.impl = impl;

    },

    _componentAddSymbols : function(obj) {
        
        obj.destroy = function() {
            topaz_component__destroy(this.impl);
        }

        obj.step = function() {
            topaz_component__run(this.impl);
        }

        obj.draw = function() {
            topaz_component__draw(this.impl);
        }

        // sets onStep, onDraw, etc.
        obj.define = function(props) {
            this.props = {};
            var keys = Object.keys(props.props);
            for(var i = 0; i < keys.length; ++i) {
                this.props[keys[i]] = props.props[keys[i]];
            }
            this.events = props.events;
            if (!this.events) this.events = {};
            


            this.props.component = this;
            this.tag = props.tag;
            this.impl.onStep = props.onStep ? function(e){props.onStep(e.__ctx.props);} : undefined;
            this.impl.onDraw = props.onDraw ? function(e){props.onDraw(e.__ctx.props);} : undefined;
            this.impl.onAttach = props.onAttach ? function(e){props.onAttach(e.__ctx.props);} : undefined;
            this.impl.onDetach = props.onDetach ? function(e){props.onDetach(e.__ctx.props);} : undefined;
            this.impl.onDestroy = props.onDestroy ? function(e){props.onDestroy(e.__ctx.props);} : undefined;

            
            var keys = Object.keys(this.events);
            for(var i = 0; i < keys.length; ++i) {
                topaz_component__install_event(
                    impl, 
                    keys[i],
                    (function(fn){ // proper reference capture
                        return function(c) {
                            fn(this.props);
                        }
                    })(this.events[keys[i]])
                );
            }

            if (props.onReady) {
                props.onReady(this.props);
            }
        }


        Object.defineProperty(
            obj,
            'stepping', {
                get : function() {return topaz_component__get_stepping(this.impl);},
                set : function(v){topaz_component__set_stepping(this.impl, v);}
            }
        );

        Object.defineProperty(
            obj,
            'drawing', {
                get : function() {return topaz_component__get_drawing(this.impl);},
                set : function(v){topaz_component__set_drawing(this.impl, v);}
            }
        );


        Object.defineProperty(
            obj,
            'tag', {
                get : function() { return topaz_component__get_tag(this.impl);},
                set : function(v){ topaz_component__set_tag(this.impl, v);}
            }
        );

        Object.defineProperty(
            obj,
            'host', {
                get : function() {
                    var f = topaz_component__get_host(this.impl);
                    if (f.__ctx) return f.__ctx;
                    return new Topaz.Entity(undefined, f);
                }
            }
        );

        obj.emitEvent = function(eventName, ent) {
            if (ent == undefined)
                return topaz_component__emit_event_anonymous(this.impl, eventName);
            else
                return topaz_component__emit_event(this.impl, eventName, ent.impl);
        }


        obj.canHandleEvent = function(name) {
            return topaz_component__can_handle_event(this.impl, name);
        }

        obj.installEvent = function(event, callback) {
            topaz_component__install_event(this.impl, event, function(component, ent) {
                if (callback)
                    callback(component.__ctx, ent.__ctx);
            });
        }

        obj.uninstallEvent = function(event) {
            topaz_component__uninstall_event(this.impl, event);
        }

        obj.installHook = function(event, callback) {
            return topaz_component__install_hook(this.impl, event, function(component, ent) {
                callback(component.__ctx, ent.__ctx);
            });
        }
        obj.uninstallHook = function(event, id) {
            topaz_component__uninstall_hook(this.impl, event, id);
        }

        obj.installHandler = function(event, callback) {
            return topaz_component__install_handler(this.impl, event, function(component, ent) {
                callback(component.__ctx, ent.__ctx);
            });
        }
        obj.uninstallHandler = function(event, id) {
            topaz_component__uninstall_handler(this.impl, event, id);
        }




    },

    _entityAddSymbols : function(obj) {

        Object.defineProperty(obj, 'isValid', {get : function(){return topaz_entity__is_valid(this.impl);}}); 
        
        obj.remove = function() {
            topaz_entity__remove(this.impl);
            Topaz.deadEntityPool.push(this);
        }
    
    
        Object.defineProperty(obj, 'childCount', {get : function(){return topaz_entity__get_child_count(this.impl);}});
    
        obj.nthChild = function(n) {
            var f = topaz_entity__get_nth_child(this.impl, n);
            if (f.__ctx) return f.__ctx;
            return new Topaz.Entity(undefined, f);
        }
    
        
        Object.defineProperty(obj, 'children', {
            get : function(){
                var children = [];
                const len = topaz_entity__get_child_count(this.impl);
                for(var i = 0; i < len; ++i) {
                    children.push(topaz_entity__get_nth_child(this.impl, i).__ctx);
                }
                return children;
            },
    
            set : function(c) {
                while(topaz_entity__get_child_count(this.impl)) {
                    topaz_entity__detach(topaz_entity__get_nth_child(this.impl, 0));
                }
    
                for(var i = 0; i < c.length; ++i) {
                    topaz_entity__attach(this.impl, c[i].impl);
                }
            }
        });
    
    
    
        obj.step = function() {topaz_entity__step(n);}
        obj.draw = function() {topaz_entity__draw(n);}
        
        obj.attach = function(other) {
            topaz_entity__attach(this.impl, other.impl);
        }
    
        obj.detach = function() {
            topaz_entity__detach(this.impl);
        }
    
    
        Object.defineProperty(obj, 'parent', {
            get : function() {
                var f = topaz_entity__get_parent(this.impl);
                if (f.__ctx) return f.__ctx;
                return new Topaz.Entity(undefined, f);
            }, 
            set : function(v){
                topaz_entity__attach(v.impl, this.impl);
            }
        });
    
    
    
        obj.query = function(name) {
            var f = topaz_entity__query(this.impl, name);
            if (f.__ctx) return f.ctx;
            return new Topaz.Entity(undefined, f);
        }
    
    
        obj.search = function(name) {
            var f = topaz_entity__search(this.impl, name);
            if (f.__ctx) return f.ctx;
            return new Topaz.Entity(undefined, f);
        }
    
    
    
        Object.defineProperty(obj, 'priority', {
            get : function() {return topaz_entity__get_priority(this.impl);}, 
            set : function(v){topaz_entity__set_priority(this.impl, v);}
        });
    
    
        obj.setPriorityLast = function() {
            topaz_entity__set_priority_last(this.impl);
        }
    
        obj.setPriorityFirst = function() {
            topaz_entity__set_priority_first(this.impl);
        }
    
        obj.define = function(props, prevProps) {
    
            this.props = prevProps == undefined ? {} : prevProps;
            // REALLY simple inheritence
            if (props.inherits) {
                this.define(props.inherits, this.props);
            }
    
            // todo: shallow copy
            if (props.props) {
                var keys = Object.keys(props.props);
                for(var i = 0; i < keys.length; ++i) {
                    this.props[keys[i]] = props.props[keys[i]];
                }
            }
    
            this.props.entity = this;
            this.name = props.name;
            if (props.onStep) topaz_entity__set_on_step(this.impl, function(e){props.onStep(e.__ctx.props)});
            if (props.onDraw) topaz_entity__set_on_draw(this.impl, function(e){props.onDraw(e.__ctx.props)});
            if (props.onPreStep) topaz_entity__set_on_pre_step(this.impl, function(e){props.onPreStep(e.__ctx.props)});
            if (props.onPreDraw) topaz_entity__set_on_pre_draw(this.impl, function(e){props.onPreDraw(e.__ctx.props)});
            if (props.onAttach) topaz_entity__set_on_attach(this.impl, function(e){props.onAttach(e.__ctx.props)});
            if (props.onDetach) topaz_entity__set_on_detach(this.impl, function(e){props.onDetach(e.__ctx.props)});
            if (props.onRemove) topaz_entity__set_on_remove(this.impl, function(e){props.onRemove(e.__ctx.props)});
            if (props.onReady) {
                props.onReady(this.props);
            }
        }
    
    
    
        Object.defineProperty(
            obj,
            'rotation', {
                get : function()  {return new Topaz.Vector(0, 0, 0, topaz_entity__get_rotation(this.impl));},
                set : function(v) {topaz_entity__set_rotation(this.impl, v.impl);}
            }
        );
    
        Object.defineProperty(
            obj,
            'position', {
                get : function()  {return new Topaz.Vector(0, 0, 0, topaz_entity__get_position(this.impl));},
                set : function(v) {topaz_entity__set_position(this.impl, v.impl);}
            }
        );
    
        Object.defineProperty(
            obj,
            'scale', {
                get : function()  {return new Topaz.Vector(0, 0, 0, topaz_entity__get_scale(this.impl));},
                set : function(v) {topaz_entity__set_scale(this.impl, v.impl);}
            }
        );
    
        Object.defineProperty(
            obj,
            'globalPosition', {
                get : function()  {return new Topaz.Vector(0, 0, 0, topaz_entity__get_global_position(this.impl));}
            }
        );
    
    
        Object.defineProperty(
            obj,
            'isStepping', {
                get : function() {return topaz_entity__is_stepping(this.impl);}
            }
        );
    
        Object.defineProperty(
            obj,
            'isDrawing', {
                get : function() {return topaz_entity__is_drawing(this.impl);}
            }
        );
    
    
        Object.defineProperty(
            obj,
            'stepping', {
                get : function()  {return topaz_entity__get_stepping(this.impl);},
                set : function(v) {return topaz_entity__set_stepping(this.impl, v);}
            }
        );
    
    
    
        Object.defineProperty(
            obj,
            'drawing', {
                get : function()  {return topaz_entity__get_drawing(this.impl);},
                set : function(v) {return topaz_entity__set_drawing(this.impl, v);}
            }
        );
    
        Object.defineProperty(
            obj,
            'name', {
                get : function()  {return topaz_entity__get_name(this.impl);},
                set : function(v) {return topaz_entity__set_name(this.impl, v);}
            }
        );
    
        obj.addComponent = function(c) {
            topaz_entity__add_component(this.impl, c.impl);
        }
    
        obj.addComponentAfter = function(c) {
            topaz_entity__add_component_after(this.impl, c.impl);
        }
    
    
    
        Object.defineProperty(
            obj,
            'components', {
                get : function()  {
                    const len = topaz_entity__get_component_count(this.impl);
                    var out = [];
                    for(var i = 0; i < len; ++i) {
                        var f = topaz_entity__get_nth_component(this.impl, index);
                        if (f.__ctx) {
                            out.push(f.__ctx);
                        } else { 
                            out.push(Topaz.Component(undefined, f));
                        }
                    }
                    return out;
                },
                set : function(c) {
                    while(topaz_entity__get_component_count(this.impl)) {
                        topaz_entity__remove_component(topaz_component__get_tag(topaz_entity__get_nth_component(this.impl, 0)));
                    }
    
                    for(var i = 0; i < c.length; ++i) {
                        topaz_entity__add_component(this.impl, c[i].impl);
                    }
                }
            }
        );
    
    
        obj.queryComponent = function(tag) {
            var f = topaz_entity__query_component(this.impl, tag);
            if (f.__ctx) return f.__ctx;
            return new Topaz.Component(undefined, f);
        }
    
        obj.removeComponent = function(c) {
            return topaz_entity__remove_component(this.impl, c.impl);
        }
    
    }


};

(function(){

    /// particle emitter
    Object.defineProperty(Topaz.ParticleEmitter2D.prototype, 'particle', {set : function(v){topaz_particle_emitter_2d__set_particle(this.impl, v.impl);}});
    Object.defineProperty(Topaz.ParticleEmitter2D.prototype, 'independent', {set : function(v){topaz_particle_emitter_2d__set_independent(this.impl, v);}});

    Topaz.ParticleEmitter2D.prototype.emit = function(c) {
        topaz_particle_emitter_2d__emit(this.impl, c);
    }
    Topaz._entityAddSymbols(Topaz.ParticleEmitter2D.prototype);




    /// particle 
    Topaz.Particle.prototype.setParameter = function(param, val) {
        topaz_particle__set_param(this.impl, param, val);
    }
    Topaz.Particle.prototype.setNoiseMin = function(p, val) {
        topaz_particle__set_noise_min(this.impl, p, val);
    }
    Topaz.Particle.prototype.setNoiseMax = function(p, val) {
        topaz_particle__set_noise_max(this.impl, p, val);
    }
    Topaz.Particle.prototype.setFunction = function(p, val) {
        topaz_particle__set_function(this.impl, p, val);
    }
    
    
    Topaz.Particle.Property = {
        duration : 0,
        scaleX : 1,
        scaleY : 2,
        scaleMultiplier : 3,
        rotation : 4,
        direction : 5,
        speedX : 6,
        speedY : 7,
        red : 8,
        green : 9,
        blue : 10,
        alpha : 11,
    };

    Object.defineProperty(Topaz.Particle.prototype, 'string', {get : function(){return topaz_particle__to_string(this.impl)}, set : function(v){topaz_particle__set_from_string(this.impl, v);}})
    Object.defineProperty(Topaz.Particle.prototype, 'image',  {set : function(v){return topaz_particle__set_image(this.impl, v)}});







    /// color 

    Topaz.Color.prototype.setFromString = 



    Object.defineProperty(Topaz.Color.prototype, 'string', 
        {
            get : function() {
                return topaz_color__to_hex_string(this.impl);
            },

            set : function(str) {
                topaz_color__set_from_string(this.impl, str);
            }
        }
    );
    Object.defineProperty(Topaz.Color.prototype, 'r', {
        get : function(){ return topaz_color__get_r(this.impl);}, 
        set : function(v){       topaz_color__set_r(this.impl, v);}
    });
    Object.defineProperty(Topaz.Color.prototype, 'g', {
        get : function(){ return topaz_color__get_g(this.impl);}, 
        set : function(v){       topaz_color__set_g(this.impl, v);}
    });
    Object.defineProperty(Topaz.Color.prototype, 'b', {
        get : function(){ return topaz_color__get_b(this.impl);}, 
        set : function(v){       topaz_color__set_b(this.impl, v);}
    });
    Object.defineProperty(Topaz.Color.prototype, 'a', {
        get : function(){ return topaz_color__get_a(this.impl);}, 
        set : function(v){       topaz_color__set_a(this.impl, v);}
    });


    /// automation


    Topaz.Automation.prototype.addKeyframe = function(value, fn, offset) {
        topaz_automation__add_keyframe(this.impl, value, fn, offset);
    }

    Topaz.Automation.prototype.addVectorKeyframe = function(value, fn, offset) {
        topaz_automation__add_vector_keyframe(this.impl, value, fn, offset);
    }

    Topaz.Automation.prototype.clear = function() {
        topaz_automation__clear(this.impl);
    }

    Topaz.Automation.prototype.blend = function(other) {
        topaz_automation__blend(this.impl, other.impl);
    }

    Topaz.Automation.prototype.smooth = function() {
        topaz_automation__smooth(this.impl);
    }

    Topaz.Automation.prototype.addFromString = function(str) {
        topaz_automation__add_from_string(this.impl, str);
    }
    Topaz.Automation.prototype.skipTo = function(n) {
        topaz_automation__skip_to(this.impl, n);
    }
    




    Topaz.Automation.Function = {
        none : 0,
        linear : 1,
        square : 2,
        cube : 3,
        squareRoot : 4,
        cubeRoot : 5,
        random : 6    
    };




    

    Object.defineProperty(
        Topaz.Automation.prototype,
        'length', {
            get : function() {return topaz_automation__get_length(this.impl);},
        }
    );
    Object.defineProperty(
        Topaz.Automation.prototype,
        'durationFrames', {
            set : function(v) {topaz_automation__set_duration_frames(this.impl, v);}
        }
    );
    Object.defineProperty(
        Topaz.Automation.prototype,
        'durationSeconds', {
            set : function(v) {topaz_automation__set_duration_seconds(this.impl, v);}
        }
    );

    Object.defineProperty(
        Topaz.Automation.prototype,
        'duration', {
            get : function() {return topaz_automation__get_duration(this.impl);},
        }
    );

    Object.defineProperty(
        Topaz.Automation.prototype,
        'looped', {
            get : function() {return topaz_automation__get_looped(this.impl);},
            set : function(v) {return topaz_automation__set_looped(this.impl, v);},
        }
    );
    Object.defineProperty(
        Topaz.Automation.prototype,
        'speed', {
            get : function() {return topaz_automation__get_speed(this.impl);},
            set : function(v) {return topaz_automation__set_speed(this.impl, v);},
        }
    );


    Topaz.Automation.prototype.resume = function() {
        topaz_automation__resume(this.impl);
    }
    Topaz.Automation.prototype.pause = function() {
        topaz_automation__pause(this.impl);
    }


    Object.defineProperty(
        Topaz.Automation.prototype,
        'string', {
            get : function() {return topaz_automation__to_string(this.impl);},
            set : function(v) {return topaz_automation__set_from_string(this.impl, v);},
        }
    );

    Topaz.Automation.prototype.vectorAt = function(at) {
        return new Topaz.Vector(0, 0, 0, topaz_automation__vector_at(this.impl, at));
    }

    Topaz.Automation.prototype.at = function(a) {
        return topaz_automation__at(this.impl, at);
    }



    Object.defineProperty(
        Topaz.Automation.prototype,
        'vector', {
            get : function() {return new Topaz.Vector(0, 0, 0, topaz_automation__current_vector(this.impl))},
        }
    );

    Object.defineProperty(
        Topaz.Automation.prototype,
        'value', {
            get : function() {return topaz_automation__current(this.impl);},
        }
    );
    Topaz._componentAddSymbols(Topaz.Automation.prototype);


    /// text2d

    Object.defineProperty(
        Topaz.Text2D.prototype,
        'text', {
            get : function() {return topaz_text2d__get_text(this.impl);},
            set : function(v) {topaz_text2d__set_text(this.impl, v);}
        }
    );
    Object.defineProperty(
        Topaz.Text2D.prototype,
        'font', {
            get : function()  {return this.fontState;},
            set : function(v) {
                this.fontState = v;
                topaz_text2d__set_font(this.impl, this.fontState, this.sizeState);
            }
        }  
    );
    Object.defineProperty(
        Topaz.Text2D.prototype,
        'size', {
            get : function()  {return this.sizeState;},
            set : function(v) {
                this.sizeState = v;
                topaz_text2d__set_font(this.impl, this.fontState, this.sizeState);
            }
        }  
    );
    

    Topaz.Text2D.prototype.setColor = function(c) {
        topaz_text2d__set_color(this.impl, c.impl);
    }

    Topaz.Text2D.prototype.setColorSection = function(from, to, c) {
        topaz_text2d__set_color_section(this.impl, from, to, c.impl);
    }


    Object.defineProperty(
        Topaz.Text2D.prototype,
        'extentWidth', {
            get : function() {return topaz_text2d__get_extent_width(this.impl);}
        }
    );

    Object.defineProperty(
        Topaz.Text2D.prototype,
        'extentHeight', {
            get : function() {return topaz_text2d__get_extent_height(this.impl);}
        }
    );


    Topaz.Text2D.prototype.getCharX = function(i) {
        return topaz_text2d__get_char_x(this.impl, i);
    }

    Topaz.Text2D.prototype.getCharY = function(i) {
        return topaz_text2d__get_char_y(this.impl, i);
    }


    Object.defineProperty(
        Topaz.Text2D.prototype,
        'position', {
            get : function() {return new Topaz.Vector(0, 0, 0, topaz_text2d__get_position(this.impl));},
            set : function(v){topaz_text2d__set_position(this.impl, v.impl);}
        }
    );

    Object.defineProperty(
        Topaz.Text2D.prototype,
        'rotation', {
            get : function() {return new Topaz.Vector(0, 0, 0, topaz_text2d__get_rotation(this.impl));},
            set : function(v){topaz_text2d__set_rotation(this.impl, v.impl);}
        }
    );

    Object.defineProperty(
        Topaz.Text2D.prototype,
        'scale', {
            get : function() {return new Topaz.Vector(0, 0, 0, topaz_text2d__get_scale(this.impl));},
            set : function(v){topaz_text2d__set_scale(this.impl, v.impl);}
        }
    );


    Topaz.Text2D.prototype.setParameter = function(a, b) {
        topaz_text2d__set_parameter(this.impl, a, b);
    }
    Topaz._componentAddSymbols(Topaz.Text2D.prototype);



    /// shape2d
    Object.defineProperty(
        Topaz.Shape2D.prototype,
        'color', {
            get : function() {return new Topaz.Color('', topaz_shape2d__get_color(this.impl));},
            set : function(v){topaz_shape2d__set_color(this.impl, v.impl);}
        }
    );

    Object.defineProperty(
        Topaz.Shape2D.prototype,
        'animSpeed', {
            get : function() {return topaz_shape2d__get_anim_speed(this.impl);},
            set : function(v){topaz_shape2d__set_anim_speed(this.impl, v);}
        }
    );

    Object.defineProperty(
        Topaz.Shape2D.prototype,
        'center', {
            get : function() {return new Topaz.Vector(0, 0, 0, topaz_shape2d__get_center(this.impl));},
            set : function(v){topaz_shape2d__set_center(this.impl, v.impl);}
        }
    );


    Object.defineProperty(
        Topaz.Shape2D.prototype,
        'position', {
            get : function() {return new Topaz.Vector(0, 0, 0, topaz_shape2d__get_position(this.impl));},
            set : function(v){topaz_shape2d__set_position(this.impl, v.impl);}
        }
    );

    Object.defineProperty(
        Topaz.Shape2D.prototype,
        'rotation', {
            get : function() {return new Topaz.Vector(0, 0, 0, topaz_shape2d__get_rotation(this.impl));},
            set : function(v){topaz_shape2d__set_rotation(this.impl, v.impl);}
        }
    );

    Object.defineProperty(
        Topaz.Shape2D.prototype,
        'scale', {
            get : function() {return new Topaz.Vector(0, 0, 0, topaz_shape2d__get_scale(this.impl));},
            set : function(v){topaz_shape2d__set_scale(this.impl, v.impl);}
        }
    );


    Topaz.Shape2D.prototype.formRectangle = function(a, b) {
        topaz_shape2d__form_rectangle(this.impl, a, b);
    }

    Topaz.Shape2D.prototype.formImage = function(a) {
        topaz_shape2d__form_image(this.impl, a);
    }

    Topaz.Shape2D.prototype.formImageScaled = function(a, b, c) {
        topaz_shape2d__form_image(this.impl, a, b, c);
    }

    Topaz.Shape2D.prototype.formRadial = function(a, b) {
        topaz_shape2d__form_radial(this.impl, a, b);
    }


    Object.defineProperty(
        Topaz.Shape2D.prototype,
        'lines', {
            get : function() {return this._lines;},
            set : function(v){topaz_shape2d__form_lines(this.impl, v); this._lines = v;}
        }
    );



    Object.defineProperty(
        Topaz.Shape2D.prototype,
        'triangles', {
            get : function() {return this._tris;},
            set : function(v){topaz_shape2d__form_triangles(this.impl, v); this._tris = v;}
        }
    );

    Topaz.Shape2D.prototype.setParameter = function(a, b) {
        topaz_shape2d__set_parameter(this.impl, a, b);
    }
    
    Topaz._componentAddSymbols(Topaz.Shape2D.prototype);





    /// object2d

    Topaz.Object2D.prototype.addVelocity = function(a, b) {
        topaz_object2d__add_velocity(this.impl, a, b);
    }

    Topaz.Object2D.prototype.addVelocityTowards = function(a, b, c) {
        topaz_object2d__add_velocity_towards(this.impl, a, b, c);
    }

    Topaz.Object2D.prototype.setVelocity = function(a, b) {
        topaz_object2d__set_velocity(this.impl, a, b);
    }

    Topaz.Object2D.prototype.setVelocityTowards = function(a, b, c) {
        topaz_object2d__set_velocity_towards(this.impl, a, b, c);
    }
    
    Topaz.Object2D.Group = {
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
    };

    
    Object.defineProperty(
        Topaz.Object2D.prototype,
        'frictionX', {
            get : function() { return topaz_object2d__get_friction_x(this.impl);},
            set : function(v){ topaz_object2d__set_friction_x(this.impl, v);}
        }
    );

    Object.defineProperty(
        Topaz.Object2D.prototype,
        'frictionY', {
            get : function() { return topaz_object2d__get_friction_y(this.impl);},
            set : function(v){ topaz_object2d__set_friction_y(this.impl, v);}
        }
    );

    Object.defineProperty(
        Topaz.Object2D.prototype,
        'direction', {
            get : function() { return topaz_object2d__get_direction(this.impl);},
            set : function(v){
                topaz_object2d__set_velocity(
                    this.impl,
                    topaz_object2d__get_speed(this.impl),
                    topaz_object2d__get_direction(this.impl)
                );
            }
        }
    );


    Topaz.Object2D.prototype.halt = function() {topaz_object2d__halt(this.impl);}
    Topaz.Object2D.prototype.resetMotion = function() {topaz_object2d__reset_motion(this.impl);}

    Object.defineProperty(
        Topaz.Object2D.prototype,
        'velocityX', {
            get : function() { return topaz_object2d__get_velocity_x(this.impl);},
            set : function(v){ topaz_object2d__set_velocity_x(this.impl, v);}
        }
    );

    
    Object.defineProperty(
        Topaz.Object2D.prototype,
        'velocityY', {
            get : function() { return topaz_object2d__get_velocity_y(this.impl);},
            set : function(v){ topaz_object2d__set_velocity_y(this.impl, v);}
        }
    );


    Object.defineProperty(
        Topaz.Object2D.prototype,
        'speed', {
            get : function() { return topaz_object2d__get_speed(this.impl);},
            set : function(v){ topaz_object2d__set_speed(this.impl, v);}
        }
    );

    Object.defineProperty(
        Topaz.Object2D.prototype,
        'nextPosition', {
            get : function() { return topaz_object2d__get_next_position(this.impl);}
        }
    );

    Object.defineProperty(
        Topaz.Object2D.prototype,
        'group', {
            get : function() { return topaz_object2d__get_group(this.impl);},
            set : function(v){ topaz_object2d__set_group(this.impl, v);}
        }
    );


    Topaz.Object2D.setGroupInteraction = function(a, b, c) {
        topaz_object2d__set_group_interaction(a, b, c);
    }

    Object.defineProperty(
        Topaz.Object2D.prototype,
        'collider', {
            get : function() { return this._collider; },
            set : function(v){ this._collider=v; topaz_object2d__set_collider(this.impl, v);}
        }
    );
    

    Topaz.Object2D.prototype.setColliderRadial = function(a, b) {
        topaz_object2d__set_collider_radial(this.impl, a, b);
        this._collider = [];
        const len = topaz_object2d__get_collider_len(this.impl);
        for(var i = 0; i < len; ++i) {
            const pt = topaz_object2d__get_collider_point(this.impl, i);
            this._collider.push(pt.x);
            this._collider.push(pt.y);
        }
    }

    Object.defineProperty(
        Topaz.Object2D.prototype,
        'lastCollided', {
            get : function() { return new Topaz.Entity(undefined, topaz_object2d__get_last_collided(this.impl));}
        }
    );
    Topaz._componentAddSymbols(Topaz.Object2D.prototype);







    /// state control
    

    Topaz.StateControl.prototype.add = function(name, state) {
        topaz_state_control__add(this.impl, state.state, state.onStep, state.onDraw, state.onInit);
    }

    Topaz.StateControl.prototype.remove = function(name) {
        topaz_state_control__remove(this.impl, name);
    }

    Topaz.StateControl.prototype.execute = function(name) {
        topaz_state_control__execute(this.impl, name);
    }

    Topaz.StateControl.prototype.halt = function() {
        topaz_state_control__halt(this.impl)
    }


    Object.defineProperty(
        Topaz.StateControl.prototype,
        'isHalted', {
            get : function() {
                return topaz_state_control__is_halted(this.impl);
            },
        }
    );

    Object.defineProperty(
        Topaz.StateControl.prototype,
        'state', {
            get : function() {
                return topaz_state_control__get_current(this.impl);
            },
        }
    );
    Topaz._componentAddSymbols(Topaz.StateControl.prototype);




    /// scheduler 
    Topaz.Scheduler.prototype.startTask = function(args) {
        if (args["taskName"] != undefined) {
            topaz_scheduler__start_task(
                this.impl,
                args.taskName,
                args.interval,
                args.intervalDelay,
                args.callback
            );
            return args.taskName;
        } else {
            return topaz_scheduler__start_task_simple(
                this.impl,
                args.interval,
                args.callback
            );
        }
    }

    Topaz.Scheduler.prototype.endTask = function(name) {
        topaz_scheduler__end_task(this.impl, name);
    }

    Topaz.Scheduler.prototype.pause = function(name) {
        topaz_scheduler__pause(this.impl, name);
    }

    Topaz.Scheduler.prototype.resume = function(name) {
        topaz_scheduler__resume(this.impl, name);
    }

    Topaz.Scheduler.prototype.getTaskIntervalRemaining = function(name) {
        return topaz_scheduler__get_task_interval_remaining(this.impl, name);
    }
    
    Topaz.Scheduler.Mode = {
        time : 0,
        frame : 1
    }
    

    Object.defineProperty(
        Topaz.Scheduler.prototype,
        'tasks', {
            get : function() {
                var out = [];
                const len = topaz_scheduler__get_task_count(this.impl);
                for(var i = 0; i < len; ++i) {
                    out.push(topaz_schedluer__get_task(this.impl, i));
                }
                return out;
            },
        }
    );
    Topaz._componentAddSymbols(Topaz.Scheduler.prototype);




    /// Topaz.component
    Topaz._componentAddSymbols(Topaz.Component.prototype);
    Topaz.Component.null = function() {return topaz_component__null();};







    /// Topaz.entity.
    Topaz._entityAddSymbols(Topaz.Entity.prototype);
    Topaz.Entity.null = function() {
        return new topaz_entity__null();
    }



    // Topaz.sound 
    Object.defineProperty(
        Topaz.Sound.prototype,
        'sampleCount', {
            get : function()  { return topaz_sound__get_sample_count(this.impl);}
        }
    );

    Object.defineProperty(
        Topaz.Sound.prototype,
        'samples', {
            get : function()  { 
                var out = [];
                const len = this.sampleCount;
                for(var i = 0; i < len; ++i) {
                    out.push(topaz_sound__get_nth_sample_left(this.impl, i));
                    out.push(topaz_sound__get_nth_sample_right(this.impl, i));
                }
                return out;
            },
            
            
            set : function(v) {
                topaz_sound__set_samples(this.impl, v);
            }
        }
    );
    Topaz._assetSetCommonSymbols(Topaz.Sound.prototype);


    // ViewManager 
    Object.defineProperty(
        Topaz.ViewManager,
        'mainDisplay', {
            set : function(v) {
                topaz_view_manager__set_main(v.impl);
            },
            get : function() {
                return new Topaz.Display(topaz_view_manager__get_main());
            }
        }
    );


    Object.defineProperty(
        Topaz.ViewManager,
        'clipboard', {
            set : function(v) {
                topaz_view_manager__set_clipboard_from_string(v);
            },
            get : function() {
                return topaz_view_manager__get_clipboard_as_string();
            }
        }
    );






    Topaz.Display.ViewPolicy = {
        none      : 0,
        matchSize : 1
    }
    
    Topaz.Display.Parameter = {
        x : 0,
        y : 1,
        width : 2,
        height : 3,
        show : 4,
        fullscreen : 5,
        lockClientResize : 6,
        lockClientPosition : 7,
        viewPolicy : 8,
        inputFocus : 9
    }

    Topaz.Display.prototype.destroy = function() {
        topaz_display__destroy(this.impl);
    }

    Topaz.Display.prototype.resize = function(w, h) {
        topaz_display__set_parameter(this.impl, 2, w);
        topaz_display__set_parameter(this.impl, 3, h);
    }

    Topaz.Display.prototype.addParameterCallback = function(cb) {
        return topaz_display__add_parameter_callback(this.impl, cb);
    }

    Topaz.Display.prototype.addCloseCallback = function(cb) {
        return topaz_display__add_close_callback(this.impl, cb);
    }

    Topaz.Display.prototype.removeCallback = function(i) {
        topaz_display__remove_callback(i);
    }

    Topaz.Display.prototype.setRenderResolution = function(w, h) {
        topaz_display__set_render_resolution(this.impl, w, h);
    }

    Topaz.Display.prototype.getParameter = function(p) {
        return topaz_display__get_parameter(this.impl, p);
    }

    Topaz.Display.prototype.setParameter = function(p, v) {
        topaz_display__set_parameter(this.impl, p, v);
    }


    Object.defineProperty(
        Topaz.Display.prototype,
        'width', {
            get : function() {
                return topaz_display__get_parameter(this.impl, 2);
            },
            set : function(v) {
                topaz_display__set_parameter(this.impl, 2, v);
            }
        }
    );

    Object.defineProperty(
        Topaz.Display.prototype,
        'height', {
            get : function() {
                return topaz_display__get_parameter(this.impl, 3);
            },
            set : function(v) {
                topaz_display__set_parameter(this.impl, 3, v);
            }
        }
    );

    Object.defineProperty(
        Topaz.Display.prototype,
        'renderWidth', {
            get : function() {
                return topaz_display__get_render_width(this.impl);
            }
        }
    );

    Object.defineProperty(
        Topaz.Display.prototype,
        'renderHeight', {
            get : function() {
                return topaz_display__get_render_height(this.impl);
            }
        }
    );


    Object.defineProperty(
        Topaz.Display.prototype,
        'camera2d', {
            get : function() {
                return new Topaz.Entity(topaz_display__get_camera_2d(this.impl));
            }
        }
    );

    Object.defineProperty(
        Topaz.Display.prototype,
        'camera3d', {
            get : function() {
                return new Topaz.Entity(topaz_display__get_camera_3d(this.impl));
            }
        }
    );




    // Filesystem.path
    Object.defineProperty(
        Topaz.Filesystem.Path.prototype,
        'string', {
            get : function()  { return topaz_filesystem_path__as_string(this.impl);}
        }
    );

    Object.defineProperty(
        Topaz.Filesystem.Path.prototype,
        'parent', {
            get : function()  { return new Topaz.Filesystem.Path(topaz_filesystem_path__get_parent(this.impl));}
        }
    );


    Object.defineProperty(
        Topaz.Filesystem.Path.prototype,
        'children', {
            get : function()  {
                var out = [];
                const len = topaz_filesystem_path__get_child_count(this.impl);
                for(var i = 0; i < len; ++i) {
                    var p = new Topaz.Filesystem.Path(topaz_filesystem_path__get_nth_child(this.impl, i));
                    if (p != undefined) {
                        out.push(p);
                    }                
                }
                
                return out;
            }
        }
    );



    // Topaz.data

    
    Object.defineProperty(
        Topaz.Data.prototype,
        'byteCount', {
            get : function()  { return topaz_data__get_byte_count(this.impl);}
        }
    );

    Object.defineProperty(
        Topaz.Data.prototype,
        'string', {
            get : function()  { return topaz_data__get_as_string(this.impl);}
        }
    );

    Object.defineProperty(
        Topaz.Data.prototype,
        'bytes', {
            get : function()  { 
                const bytes = [];
                const len = topaz_data__get_byte_count(this.impl);
                for(var i = 0; i < len; ++i) {
                    bytes.push(topaz_data__get_nth_byte(this.impl, i));
                }
                return bytes;
            },

            set : function(v) {
                topaz_data__set(this.impl, v);
            }
        }
    );
    Topaz._assetSetCommonSymbols(Topaz.Data.prototype);








    // image 

        
    Object.defineProperty(
        Topaz.Image.prototype,
        'width', {
            get : function()  { return topaz_image__get_width(this.impl);}
        }
    );

    Object.defineProperty(
        Topaz.Image.prototype,
        'height', {
            get : function()  { return topaz_image__get_height(this.impl);}
        }
    );

    Object.defineProperty(
        Topaz.Image.prototype,
        'frameCount', {
            get : function()  { return topaz_image__get_frame_count(this.impl);}
        }
    );




    Topaz.Image.prototype.resize = function(w, h) {
        topaz_image__resize(this.impl, w, h);
    }

    Topaz.Image.prototype.addFrame = function() {
        topaz_image__add_frame(this.impl);
    }

    Topaz.Image.prototype.removeFrame = function(f) {
        topaz_image__remove_frame(this.impl, f);
    }

    Topaz.Image.prototype.setRGBA = function(i, rgbaData) {
        topaz_image__frame_set_rgba(this.impl, i, rgbaData);
    }
    Topaz._assetSetCommonSymbols(Topaz.Image.prototype);



    // audioPlaybackSound 
    
    Object.defineProperty(
        Topaz.Audio.PlaybackSound.prototype,
        'volume', {
            set : function(v)  { topaz_audio__playback_set_volume(this.impl, v);}
        }
    );
    Object.defineProperty(
        Topaz.Audio.PlaybackSound.prototype,
        'panning', {
            set : function(v)  { topaz_audio__playback_set_panning(this.impl, v);}
        }
    );
    Object.defineProperty(
        Topaz.Audio.PlaybackSound.prototype,
        'repeatSound', {
            set : function(v)  { topaz_audio__playback_set_repeat(this.impl, v);}
        }
    );
    Object.defineProperty(
        Topaz.Audio.PlaybackSound.prototype,
        'paused', {
            set : function(v)  { 
                if (v)
                    topaz_audio__playback_pause(this.impl);
                else
                    topaz_audio__playback_resume(this.impl);
            }
        }
    );

    Object.defineProperty(
        Topaz.Audio.PlaybackSound.prototype,
        'seek', {
            set : function(v)  { 
                topaz_audio__playback_seek(this.impl, v);
            }
        }
    );
    
    
    Topaz.Audio.PlaybackSound.prototype.stop = function() {
        topaz_audio__playback_stop(this.impl);
    }







    // vector base properties




    Topaz.Vector.prototype.getDistance = function(other) {
        return topaz_vector__get_distance(this.impl, other.impl);
    }        

    Topaz.Vector.prototype.normalize = function() {
        this.topaz_vector__normalize(this.impl);   
    }

    Topaz.Vector.prototype.cross = function(other) {
        var out = topaz_vector__cross(this.impl, other.impl);
        return new Topaz.Vector(0, 0, 0, out);
    }

    Topaz.Vector.prototype.floor = function() {
        topaz_vector__floor(this.impl);
    }

    Topaz.Vector.prototype.rotationXDiff = function(other) {
        return topaz_vector__rotation_x_diff(this.impl, other.impl);
    }

    Topaz.Vector.prototype.rotationXDiffRelative = function(other) {
        return topaz_vector__rotation_x_diff_relative(this.impl, other.impl);
    }

    Topaz.Vector.prototype.rotationX = function() {
        return topaz_vector__rotation_x(this.impl);
    }

    Topaz.Vector.prototype.rotationYDiff = function(other) {
        return topaz_vector__rotation_y_diff(this.impl, other.impl);
    }

    Topaz.Vector.prototype.rotationYDiffRelative = function(other) {
        return topaz_vector__rotation_y_diff_relative(this.impl, other.impl);
    }

    Topaz.Vector.prototype.rotationY = function() {
        return topaz_vector__rotation_y(this.impl);
    }


    Topaz.Vector.prototype.rotationZDiff = function(other) {
        return topaz_vector__rotation_z_diff(this.impl, other.impl);
    }

    Topaz.Vector.prototype.rotationZDiffRelative = function(other) {
        return topaz_vector__rotation_z_diff_relative(this.impl, other.impl);
    }

    Topaz.Vector.prototype.rotationZ = function() {
        return topaz_vector__rotation_z(this.impl);
    }

    Topaz.Vector.prototype.rotateX = function(val) {
        topaz_vector__rotate_x(this.impl, val);
    }

    Topaz.Vector.prototype.rotateY = function(val) {
        topaz_vector__rotate_y(this.impl, val);
    }

    Topaz.Vector.prototype.rotateZ = function(val) {
        topaz_vector__rotate_z(this.impl, val);
    }

    Topaz.Vector.prototype.remove = function() {
        topaz_vector__destroy(this.impl);
        impl = {};
    }

    Topaz.Vector.prototype.add = function(b) {
        return new Topaz.Vector(this.x + b.x, this.y + b.y, this.z + b.z);
    }
    Topaz.Vector.prototype.subtract = function(b) {
        return new Topaz.Vector(this.x - b.x, this.y - b.y, this.z - b.z);
    }
    Topaz.Vector.prototype.multiply = function(b) {
        return new Topaz.Vector(this.x * b.x, this.y * b.y, this.z * b.z);
    }
    Topaz.Vector.prototype.divide = function(b) {
        return new Topaz.Vector(this.x / b.x, this.y / b.y, this.z / b.z);
    }


    Object.defineProperty(Topaz.Vector.prototype, 'length', {get : function(){return topaz_vector__get_length(impl)}});
    Object.defineProperty(Topaz.Vector.prototype, 'x', {get : function(){return topaz_vector__get_x(this.impl);}, set : function(v){topaz_vector__set_x(this.impl, v);}});
    Object.defineProperty(Topaz.Vector.prototype, 'y', {get : function(){return topaz_vector__get_y(this.impl);}, set : function(v){topaz_vector__set_y(this.impl, v);}});
    Object.defineProperty(Topaz.Vector.prototype, 'z', {get : function(){return topaz_vector__get_z(this.impl);}, set : function(v){topaz_vector__set_z(this.impl, v);}});


}());



Object.defineProperty(Topaz, 'isPaused', {get : function(){return topaz__is_paused();}});
Object.defineProperty(Topaz, 'root', {
    get : function(){return new Topaz.Entity(undefined, topaz__get_root());},
    set : function(v){return topaz__set_root(v.impl);}
});
Object.defineProperty(Topaz, 'time', {get : function(){return topaz__get_time();}});
Object.defineProperty(Topaz, 'versionMicro', {get : function(){return topaz__get_version_micro();}});
Object.defineProperty(Topaz, 'versionMajor', {get : function(){return topaz__get_version_major();}});
Object.defineProperty(Topaz, 'versionMinor', {get : function(){return topaz__get_version_minor();}});
Object.defineProperty(Topaz.Input, 'mouse',  {get : function(){ 
    return new Topaz.Vector(topaz_input__mouse_x(), topaz_input__mouse_y());
}});
Object.defineProperty(Topaz.Input, 'mouseDelta', {get : function(){
    return new Topaz.Vector(topaz_input__mouse_delta_x(), topaz_input__mouse_delta_y());
}});
Object.defineProperty(Topaz.Input, 'mouseWheel', {get : function(){return topaz_input__mouse_wheel();}});
Object.defineProperty(Topaz.Resources, 'path', {
    set : function(v){topaz_resources__set_path(v);},
    get : function(){return topaz_resources__get_path();}
});


Topaz.attachPreManager(
    new Topaz.Entity({
        name : 'TOPAZEntityCleaner',
        onStep : function() {
            const pool = Topaz.deadEntityPool;
            if (pool.length) {
                for(var i = 0; i < pool.length; ++i) {
                    if (pool[i].impl) {
                        pool[i].impl.__ctx.props = undefined;
                        pool[i].impl.__ctx = undefined;
                        pool[i].impl = undefined;
                    }
                }
                Topaz.deadEntityPool = [];
            }
        }
    })
);



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
            "assetBase64" : "AAAAA79a9d0ef0x...",
            "autorun" : "true"
        },
        {
            "assetName" : "topazAssetName2",
            "assetType" : "png",
            "assetBase64" : "AFBBA79a9d0ef0x...",
        },
        {
            "assetName" : "topazAssetName4",
            "assetType" : "font",
            "assetPath" : "./mydata.ttf",
        }
    ]
}
*/

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
Topaz.Package = (function(){
    var packages = [];
    var packagesActive = [];
    var packageDB = {}; // indexed by string.

    const loadAsset = function(jsonAsset, ext) {
        if (jsonAsset.assetBase64 != undefined) {
            var success = Topaz.Resources.loadAssetBase64(
                ext,
                jsonAsset.assetBase64,
                jsonAsset.assetName
            );
            if (success == undefined) {
                throw new Error('Error loading ' + assetSrc.assetName + '!');
            }

        }
        if (jsonAsset.assetPath != undefined) {
            var success = Topaz.Resources.loadAsset(
                ext,
                jsonAsset.assetPath,
                jsonAsset.assetName
            );    
            if (success == undefined) {
                throw new Error('Error loading ' + jsonAsset.assetPath + '!');
            }
        }
    }

    // can throw, watch out
    const makePackageFromJSON = function(json) {

        if (json.formatVersion != 1) {
            throw new Error('Package version unrecognized (' + json.formatVersion + ')');
        }


        var pkg = {};
        pkg.name = json.name;
        pkg.version = json.version;
        pkg.assets = []; // strings of asset names
        pkg.depends = json.depends;
        if (!pkg.depends) {
            pkg.depends = [];
        }
        // not a valid array, tolerate it.
        if (pkg.depends.length == undefined) {
            pkg.depends = [];
        }
        pkg.autorun = []; // strings of asset names
        pkg.resolved = false;

        for(var i = 0; i < json.assets.length; ++i) {
            const assetSrc = json.assets[i];
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
        read : function(path) {
            var asset = Topaz.Resources.loadAsset('', path, path);
            if (!asset) {
                Topaz.log('Could not read package ' + path + '!');
                return;
            }
            try {
                const pkg = makePackageFromJSON(JSON.parse(asset.string));
                packageDB[pkg.name] = pkg;
                packages.push(pkg);
            } catch(e) {
                Topaz.log(e);
            }
            Topaz.Resources.removeAsset(path);
        },

        // Adds all assets for the package from a package JSON string.
        readData : function(jsonStr) {
            try {
                const pkg = makePackageFromJSON(JSON.parse(jsonStr));
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
        // and version at or greater, this function will return true.
        // If not, an error is thrown.
        require : function(packageName, versionObject) {
            const pkg = packageDB[packageName];
            if (!pkg) {
                throw new Error('Unknown package ' + packageName);                
            }

            if (!pkg.resolved) {
                throw new Error('Package '+packageName+' has been pre-loaded but not resolved.');                
            }


            if (versionObject != undefined) {

                if (versionObject.major != undefined) {
                    if (versionObject.major > pkg.version.major) {
                        throw new Error('Package '+packageName+' has major version ' + pkg.version.major + ', but version ' + versionObject.major + ' is required.');                
                    }
                }

                if (versionObject.minor != undefined &&
                    versionObject.major != undefined) {
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
        resolve : function(packageName) {   
            var pkg = packageDB[packageName];
            if (!pkg) {
                Topaz.log('No such package "' + packageName + '"');
                return false;
            }
            if (pkg.resolved) return true;
            pkg.resolved = true;

            // resolve dependencies first
            for(var i = 0; i < pkg.depends.length; ++i) {
                if (!this.resolve(pkg.depends[i].name)) {
                    Topaz.log('ERROR: Required package for ' + packageName + ' could not be resolved.');
                    pkg.resolved = false;
                    return false;                    
                }

                const dep = packageDB[pkg.depends[i].name];
                const majorNeeded = parseInt(pkg.depends[i].version.major);
                const minorNeeded = parseInt(pkg.depends[i].version.minor);

                const majorHave = pauseInt(dep.version.major);
                const minorHave = pauseInt(dep.version.minor);

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
            for(var i = 0; i < pkg.autorun.length; ++i) {
                try {
                    Topaz.import(pkg.autorun[i]);
                } catch (e) {
                    Topaz.log('Error while running script ' + pkg.autorun[i] + 'within package:');
                    Topaz.log(Topaz.objectToString(e));
                }
            }

            return true;
        },


        resolveAll : function() {
            for(var i = 0; i < packages.length; ++i) {
                if (!this.resolve(packages[i].name)) {
                    return false;
                }
            }
            return true;
        },


        getKnownPackages : function() {
            return packages;
        },


        /*
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
        */
        makePackage : function(path) {
            var allPackages = [];
            var output = (function() {
                try {
                    var mainPath = Topaz.Filesystem.getPathFromString(
                        Topaz.Filesystem.getPath(Topaz.Filesystem.DefaultNode.resources),
                        path
                    );
                    if (mainPath == undefined) {
                        return "No such path.";
                    }
                    Topaz.Resources.path = mainPath.string;
        
                    var indata = Topaz.Resources.loadAsset('', 'setup_package.json', 'setup_package.json');
                    if (!(indata && indata.byteCount)) {
                        return ('Input file "setup_package.json" is empty or could not be opened. Exiting');
                    }
                    allPackages.push(indata.name);
                    var injson = JSON.parse(indata.string);    
                    var outjson = {};
                    outjson.formatVersion = 1;
        
                    outjson.name = injson.name;
        
                    var packageOutName = "package.json";
                    if (injson.outputName != undefined) {
                        packageOutName = injson.outputName;
                    }
                    if (outjson.name == undefined) {
                        return ('setup_package.json: missing "name" property!');
                    }
                    outjson.version = injson.version;
                    if (outjson.version == undefined) {
                        return ('setup_package.json: missing "version" property!');
                    }
                    if (outjson.version.major == undefined) {
                        return ('setup_package.json: missing "version.major" property!');
                    }
                    if (outjson.version.minor == undefined) {
                        return ('setup_package.json: missing "version.major" property!');
                    }
                    if (outjson.version.micro == undefined) {
                        Topaz.log('WARNING setup_package.json: missing "version.micro" property');
                    }
                    if (outjson.version.build == undefined) {
                        Topaz.log('WARNING setup_package.json: missing "version.build" property');
                    }
        
                    const debug = injson.debug == undefined ? false : injson.debug;
        
                    outjson.depends = [];
                    if (injson.depends && injson.depends.length) {
                        for(var i = 0; i < injson.depends.length; ++i) {
                            outjson.depends.push(injson.depends[i]);
                        }
                    }
        
        
        
        
                    outjson.assets = [];
                    if (!(injson.assets && injson.assets.length)) {
                        Topaz.log('WARNING: setup_package.json specifies no assets!');
                    }
        
        
                    for(var i = 0; i < injson.assets.length; ++i) {
                        var asset = {};
                        asset.assetName = injson.assets[i].assetName;
                        asset.assetType = injson.assets[i].assetType;
                        if (injson.assets[i].autorun != undefined) {
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
                            const bufferIn = Topaz.Resources.loadAsset('', injson.assets[i].assetFile, injson.assets[i].assetName);
                            if (!(bufferIn && bufferIn.byteCount)) {
                                return ('setup_package.json: could not open asset ' + injson.assets[i].assetFile);
                            }
                            allPackages.push(bufferIn.name);
                            Topaz.log('Processing asset ' + injson.assets[i].assetName, false);
                            Topaz.log('.', false);
        
                            const byteCount = bufferIn.byteCount;
                            const bytes = bufferIn.bytes;
                            const partition = Math.floor(byteCount/5);
        
                            Topaz.log('.....', false);
                            Topaz.log(' ', false);
        
        
                            asset.assetBase64 = Topaz.toBase64(bytes);                        
                            outjson.assets.push(asset);
                            Topaz.log('OK (' + Math.ceil(byteCount/1024) + '.' + Math.floor(((byteCount%1024) / 1024.0)*100) + 'KB)');
                        }
                    }
        
        
                    var output = JSON.stringify(outjson);
                    var outputAsset = Topaz.Resources.fetchAsset(Topaz.Resources.AssetType.data, '__ASSET__39245s$');
                    var outputData = [];
                    Topaz.log('Generating output buffer', false);
                    const length = output.length;
                    const partition = Math.floor(length / 5);
                    for(var i = 0; i < length; ++i) {
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
            for(var i = 0; i < allPackages.length; ++i) {
                Topaz.Resources.removeAsset(allPackages[i]);
            }
            return output;
        }
    }
})();
