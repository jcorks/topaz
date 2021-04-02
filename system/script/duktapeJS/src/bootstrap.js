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
    import : topaz_script__import,
    log : topaz__log,

    topazRender2D_Parameter_AlphaRule : 0,
    topazRender2D_Parameter_DepthTest : 1,
    topazRender2D_Parameter_EtchRule : 2,
    topazRender2D_Parameter_TextureFilterHint : 3,

    topazRenderer_EtchRule_NoEtching : 0,
    topazRenderer_EtchRule_Define : 1,
    topazRenderer_EtchRule_Undefine : 2,
    topazRenderer_EtchRule_In : 3,
    topazRenderer_EtchRule_Out : 4,

    topazRenderer_DepthTest_Less : 0,
    topazRenderer_DepthTest_LEQ : 1,
    topazRenderer_DepthTest_Greater : 2,
    topazRenderer_DepthTest_GEQ : 3,
    topazRenderer_DepthTest_Equal : 4,
    topazRenderer_DepthTest_None : 5,

    topazRenderer_AlphaRule_Allow : 0,
    topazRenderer_AlphaRule_Opaque : 1,
    topazRenderer_AlphaRule_Translucent : 2,
    topazRenderer_AlphaRule_Invisible : 3,

    topazRenderer_TextureFilterHint_Linear : 0,
    topazRenderer_TextureFilterHint_None : 1,


    topazScheduler_Mode_Time : 0,
    topazScheduler_Mode_Frame : 1,

    topazAutomation_Function_None : 0,
    topazAutomation_Function_Linear : 1,
    topazAutomation_Function_Square : 2,
    topazAutomation_Function_Cube : 3,
    topazAutomation_Function_SquareRoot : 4,
    topazAutomation_Function_CubeRoot : 5,
    topazAutomation_Function_Random : 6,


    topazParticle_Property_Duration : 0,
    topazParticle_Property_ScaleX : 1,
    topazParticle_Property_ScaleY : 2,
    topazParticle_Property_ScaleMultiplier : 3,
    topazParticle_Property_Rotation : 4,
    topazParticle_Property_Direction : 5,
    topazParticle_Property_SpeedX : 6,
    topazParticle_Property_SpeedY : 7,
    topazParticle_Property_Red : 8,
    topazParticle_Property_Green : 9,
    topazParticle_Property_Blue : 10,
    topazParticle_Property_Alpha : 11,


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
    filesystem : {
        defaultNode : {
            resources : 0,
            topaz : 1,
            userData : 2
        },
        
        getPath : function(n) {
            return new topaz.filesystem.path(topaz_filesystem__get_path(n));
        },
        
        getPathFromString : function(pth, str) {
            if (pth) {
                return new topaz.filesystem.path(topaz_filesystem__get_path_from_string(pth, str));            
            } else {
                return new topaz.filesystem.path(topaz_filesystem__get_path_from_string(str));                        
            }
        },
        
        
        path : function(implPre) {
            var impl;
            this.uniqueID = topaz.uniqueObjectPool++;
            if (implPre) 
                impl = implPre;
            else {
                throw new Error("path object cannot be make without a LL instance.");
            }
            impl.__ctx = this;
            this.impl = impl;          
        }

        
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
    
    audio : {
        playSound : function(asset, channel) {
            return new topaz.audioPlaybackSound(topaz_audio__play_sound(asset.impl, channel == undefined ? 0 : channel));
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

    resources : {
        assetType_None : 0,
        assetType_Image : 1,
        assetType_Sound : 2,
        assetType_Model : 3,
        assetType_Particle : 4,
        assetType_Data : 5,
        assetType_Actor : 6,
        _rawAssetToInstance : function(impl) {
            if (!impl) return undefined;
            switch(topaz_asset__get_type(impl)) {
              case topaz.resources.assetType_Image: return new topaz.image(impl); break;
              case topaz.resources.assetType_Data:  return new topaz.data (impl); break;
              case topaz.resources.assetType_Sound:  return new topaz.sound(impl); break;
            }
            return new topaz.asset(impl);

        },

        fetchAsset : function(type, name) {
            return topaz.resources._rawAssetToInstance(topaz_resources__fetch_asset(type, name));
        },

        loadAsset : function(ext, path, name) {
            return topaz.resources._rawAssetToInstance(topaz_resources__load_asset(ext, path, name));
        },

        loadAssetData : function(ext, data, name) {
            return topaz.resources._rawAssetToInstance(topaz_resources__load_asset_data(ext, data, name));
        },


        writeAsset : function(asset, ext, path) {
            return topaz_resources__write_asset(asset.impl, ext, path);
        },

        removeAsset : function(asset) {
            topaz_resources__remove_asset(asset.impl);
            asset.impl = undefined;
        },

        queryAssetPaths : function() {
            topaz_resources__query_asset_paths();
        },

        isExtensionSupported : function(ext) {
            return topaz_resources__is_extension_supported(ext);
        }
    },

    fontManager : {
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
                get : function()  { return topaz_asset__get_type(impl);},
                set : function(v) {        topaz_asset__get_type(impl, v);}
            }
        );

        Object.defineProperty(
            obj,
            'name', {
                get : function()  { return topaz_asset__get_type(impl);},
                set : function(v) {        topaz_asset__get_type(impl, v);}
            }
        );
    },

    image : function(implPre) {
        var impl;

        if (implPre)
            impl = implPre;
        else {
            throw new Error("Image asset cannot be make without a LL asset instance.");
        }
        impl.__ctx = this;
        this.uniqueID = topaz.uniqueObjectPool++;
        this.impl = impl;


    },

    data : function(implPre) {
        var impl;

        if (implPre)
            impl = implPre;
        else {
            throw new Error("Data asset cannot be make without a LL asset instance.");
        }
        impl.__ctx = this;
        this.uniqueID = topaz.uniqueObjectPool++;
        this.impl = impl;

    },

    sound : function(implPre) {
        var impl;

        if (implPre)
            impl = implPre;
        else {
            throw new Error("Sound asset cannot be make without a LL asset instance.");
        }
        impl.__ctx = this;
        this.uniqueID = topaz.uniqueObjectPool++;
        this.impl = impl;

    },



    entityNull : function() {return new topaz_entity__null();},
    entity : function(defineProps, implPre) {
        this.uniqueID = topaz.uniqueObjectPool++;
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
    
        //topaz.log('REFERENCE CREATED:\n' + topaz.objectToString(this));

        
    },
    componentNull : function() {return topaz_component__null();},



    text2d : function(implPre) {
        var impl;
        this.uniqueID = topaz.uniqueObjectPool++;
        if (implPre) 
            impl = implPre;
        else {

            impl = topaz_text2d__create();
        }
        impl.__ctx = this;
        this.impl = impl;
        

    },



    scheduler : function(type, implPre) {
        var impl;
        this.uniqueID = topaz.uniqueObjectPool++;
        if (implPre) 
            impl = implPre;
        else {

            impl = topaz_scheduler__create(type);
        }
        impl.__ctx = this;
        this.impl = impl;
    },


    stateControl : function(implPre) {
        var impl;
        this.uniqueID = topaz.uniqueObjectPool++;
        if (implPre) 
            impl = implPre;
        else {

            impl = topaz_state_control__create();
        }
        impl.__ctx = this;
        this.impl = impl;

    },



    component : function(defineProps, implPre) {
        //if (implPre)
            //topaz.log('REFERENCE CREATED:\n' + topaz.objectToString(this));

        this.uniqueID = topaz.uniqueObjectPool++;
        var impl;

        if (implPre) 
            impl = implPre;
        else {
            impl = topaz_component__create('');
        }
        impl.__ctx = this;

        this.impl = impl;
        if (defineProps) {
            ctx.define(defineProps);
        }

    },



    object2d : function(implPre) {
        var impl;
        this.uniqueID = topaz.uniqueObjectPool++;
        if (implPre) 
            impl = implPre;
        else {
            impl = topaz_object2d__create();
        }
        impl.__ctx = this;
        this.impl = impl;

    },
    shape2d : function(implPre) {
        var impl;
        this.uniqueID = topaz.uniqueObjectPool++;
        if (implPre) 
            impl = implPre;
        else {
            impl = topaz_shape2d__create();
        }
        impl.__ctx = this;
        this.impl = impl;

    },
    text2d : function(implPre) {
        var impl;
        this.uniqueID = topaz.uniqueObjectPool++;
        if (implPre) 
            impl = implPre;
        else {
            impl = topaz_text2d__create();
        }
        impl.__ctx = this;
        this.impl = impl;

    },
    automation : function(implPre) {
        var impl;
        this.uniqueID = topaz.uniqueObjectPool++;
        if (implPre) 
            impl = implPre;
        else {
            impl = topaz_automation__create();
        }
        impl.__ctx = this;
        this.impl = impl;
    },


    color : function(name, implPre) {
        var impl;
        this.uniqueID = topaz.uniqueObjectPool++;
        if (implPre)
            impl = implPre;
        else 
            impl = topaz_color__create();

        this.impl = impl;
        if (name)
            this.setFromString(name);

    },


    vector : function(x, y, z, implPre) {
        var impl; 
        if (implPre)
            impl = implPre;
        else 
            impl = topaz_vector__create(x, y, z);



        this.impl = impl;
    },
    
    audioPlaybackSound : function(implPre) {
        this.impl = implPre;
        if (!this.impl) {
            this.impl = 0; // not a real sound, probably.
        }
    },

    particle : function(implPre) {
        var impl; 
        if (implPre)
            impl = implPre;
        else 
            impl = topaz_particle__create();

        this.impl = impl;
    },

    particleEmitter2D : function(implPre) {
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
                    return new topaz.entity(undefined, f);
                }
            }
        );

        obj.emitEvent = function(eventName, entity) {
            topaz_component__emit_event(this.impl, eventName, entity ? entity.impl : topaz_entity__null());
        }

        obj.emitEventAnonymous = function(eventName) {
            topaz_component__emit_event_anonymous(this.impl, eventName);
        }

        obj.canHandleEvent = function(name) {
            return topaz_component__can_handle_event(this.impl, name);
        }

        obj.installEvent = function(event, callback) {
            topaz_component__install_event(this.impl, event, function(component, entity) {
                if (callback)
                    callback(component.__ctx, entity.__ctx);
            });
        }

        obj.uninstallEvent = function(event) {
            topaz_component__uninstall_event(this.impl, event);
        }

        obj.installHook = function(event, callback) {
            return topaz_component__install_hook(this.impl, event, function(component, entity) {
                callback(component.__ctx, entity.__ctx);
            });
        }
        obj.uninstallHook = function(event, id) {
            topaz_component__uninstall_hook(this.impl, event, id);
        }

        obj.installHandler = function(event, callback) {
            return topaz_component__install_handler(this.impl, event, function(component, entity) {
                callback(component.__ctx, entity.__ctx);
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
            topaz.deadEntityPool.push(this);
        }
    
    
        Object.defineProperty(obj, 'childCount', {get : function(){return topaz_entity__get_child_count(this.impl);}});
    
        obj.nthChild = function(n) {
            var f = topaz_entity__get_nth_child(this.impl, n);
            if (f.__ctx) return f.__ctx;
            return new topaz.entity(undefined, f);
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
                return new topaz.entity(undefined, f);
            }, 
            set : function(v){
                topaz_entity__attach(v.impl, this.impl);
            }
        });
    
    
    
        obj.query = function(name) {
            var f = topaz_entity__query(this.impl, name);
            if (f.__ctx) return f.ctx;
            return new topaz.entity(undefined, f);
        }
    
    
        obj.search = function(name) {
            var f = topaz_entity__search(this.impl, name);
            if (f.__ctx) return f.ctx;
            return new topaz.entity(undefined, f);
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
                get : function()  {return new topaz.vector(0, 0, 0, topaz_entity__get_rotation(this.impl));},
                set : function(v) {topaz_entity__set_rotation(this.impl, v.impl);}
            }
        );
    
        Object.defineProperty(
            obj,
            'position', {
                get : function()  {return new topaz.vector(0, 0, 0, topaz_entity__get_position(this.impl));},
                set : function(v) {topaz_entity__set_position(this.impl, v.impl);}
            }
        );
    
        Object.defineProperty(
            obj,
            'scale', {
                get : function()  {return new topaz.vector(0, 0, 0, topaz_entity__get_scale(this.impl));},
                set : function(v) {topaz_entity__set_scale(this.impl, v.impl);}
            }
        );
    
        Object.defineProperty(
            obj,
            'globalPosition', {
                get : function()  {return new topaz.vector(0, 0, 0, topaz_entity__get_global_position(this.impl));}
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
                            out.push(topaz.component(undefined, f));
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
            return new topaz.component(undefined, f);
        }
    
        obj.removeComponent = function(tag) {
            return topaz_entity__remove_component(this.impl, tag);
        }
    
    }


};

(function(){

    /// particle emitter
    Object.defineProperty(topaz.particleEmitter2D.prototype, 'particle', {set : function(v){topaz_particle_emitter_2d__set_particle(this.impl, v.impl);}});
    Object.defineProperty(topaz.particleEmitter2D.prototype, 'independent', {set : function(v){topaz_particle_emitter_2d__set_independent(this.impl, v);}});

    topaz.particleEmitter2D.prototype.emit = function(c) {
        topaz_particle_emitter_2d__emit(this.impl, c);
    }



    /// particle 
    topaz.particle.prototype.setParam = function(param, val) {
        topaz_particle__set_param(this.impl, param, val);
    }
    topaz.particle.prototype.setNoiseMin = function(p, val) {
        topaz_particle__set_noise_min(this.impl, p, val);
    }
    topaz.particle.prototype.setNoiseMax = function(p, val) {
        topaz_particle__set_noise_max(this.impl, p, val);
    }
    topaz.particle.prototype.setFunction = function(p, val) {
        topaz_particle__set_function(this.impl, p, val);
    }

    Object.defineProperty(topaz.particle.prototype, 'string', {get : function(){return topaz_particle__to_string(this.impl)}, set : function(v){topaz_particle__set_from_string(this.impl, v);}})
    Object.defineProperty(topaz.particle.prototype, 'image',  {set : function(v){return topaz_particle__set_image(this.impl, v)}});







    /// color 

    topaz.color.prototype.setFromString = function(str) {
        topaz_color__set_from_string(this.impl, str);
    }

    topaz.color.prototype.getHex = function() {
        return topaz_color__to_hex_string(this.impl);
    }

    topaz.color.prototype.remove = function() {
        topaz_color__destroy(this.impl);
        this.impl = {};
    }

    Object.defineProperty(topaz.color.prototype, 'r', {get : function(){return this.impl.r;}, set : function(v){this.impl.r = v;}});
    Object.defineProperty(topaz.color.prototype, 'g', {get : function(){return this.impl.g;}, set : function(v){this.impl.g = v;}});
    Object.defineProperty(topaz.color.prototype, 'b', {get : function(){return this.impl.b;}, set : function(v){this.impl.b = v;}});
    Object.defineProperty(topaz.color.prototype, 'a', {get : function(){return this.impl.a;}, set : function(v){this.impl.a = v;}});



    /// automation


    topaz.automation.prototype.addKeyframe = function(value, fn, offset) {
        topaz_automation__add_keyframe(this.impl, value, fn, offset);
    }

    topaz.automation.prototype.addVectorKeyframe = function(value, fn, offset) {
        topaz_automation__add_vector_keyframe(this.impl, value, fn, offset);
    }

    topaz.automation.prototype.clear = function() {
        topaz_automation__clear(this.impl);
    }

    topaz.automation.prototype.blend = function(other) {
        topaz_automation__blend(this.impl, other.impl);
    }

    topaz.automation.prototype.smooth = function() {
        topaz_automation__smooth(this.impl);
    }

    topaz.automation.prototype.addFromString = function(str) {
        topaz_automation__add_from_string(this.impl, str);
    }


    

    Object.defineProperty(
        topaz.automation.prototype,
        'length', {
            get : function() {return topaz_automation__get_length(this.impl);},
        }
    );
    Object.defineProperty(
        topaz.automation.prototype,
        'durationFrames', {
            set : function(v) {topaz_automation__set_duration_frames(this.impl, v);}
        }
    );
    Object.defineProperty(
        topaz.automation.prototype,
        'durationSeconds', {
            set : function(v) {topaz_automation__set_duration_seconds(this.impl, v);}
        }
    );

    Object.defineProperty(
        topaz.automation.prototype,
        'duration', {
            get : function() {return topaz_automation__get_duration(this.impl);},
        }
    );

    Object.defineProperty(
        topaz.automation.prototype,
        'looped', {
            get : function() {return topaz_automation__get_looped(this.impl);},
            set : function(v) {return topaz_automation__set_looped(this.impl, v);},
        }
    );
    Object.defineProperty(
        topaz.automation.prototype,
        'speed', {
            get : function() {return topaz_automation__get_speed(this.impl);},
            set : function(v) {return topaz_automation__set_speed(this.impl, v);},
        }
    );


    topaz.automation.prototype.resume = function() {
        topaz_automation__resume(this.impl);
    }
    topaz.automation.prototype.pause = function() {
        topaz_automation__pause(this.impl);
    }


    Object.defineProperty(
        topaz.automation.prototype,
        'string', {
            get : function() {return topaz_automation__to_string(this.impl);},
            set : function(v) {return topaz_automation__set_from_string(this.impl, v);},
        }
    );

    topaz.automation.prototype.vectorAt = function(at) {
        return new topaz.vector(0, 0, 0, topaz_automation__vector_at(this.impl, at));
    }

    topaz.automation.prototype.at = function(a) {
        return topaz_automation__at(this.impl, at);
    }



    Object.defineProperty(
        topaz.automation.prototype,
        'vector', {
            get : function() {return new topaz.vector(0, 0, 0, topaz_automation__current_vector(this.impl))},
        }
    );

    Object.defineProperty(
        topaz.automation.prototype,
        'value', {
            get : function() {return topaz_automation__current(this.impl);},
        }
    );
    topaz._componentAddSymbols(topaz.automation.prototype);


    /// text2d

    Object.defineProperty(
        topaz.text2d.prototype,
        'text', {
            get : function() {return topaz_text2d__get_text(this.impl);},
            set : function(v) {topaz_text2d__set_text(this.impl, v);}
        }
    );

    topaz.text2d.prototype.setFont = function(f, c) {
        topaz_text2d__set_font(this.impl, f, c);
    }
    

    topaz.text2d.prototype.setColor = function(c) {
        topaz_text2d__set_color(this.impl, c.impl);
    }

    topaz.text2d.prototype.setColorSection = function(from, to, c) {
        topaz_text2d__set_color_section(this.impl, from, to, c.impl);
    }


    Object.defineProperty(
        topaz.text2d.prototype,
        'extentWidth', {
            get : function() {return topaz_text2d__get_extent_width(this.impl);}
        }
    );

    Object.defineProperty(
        topaz.text2d.prototype,
        'extentHeight', {
            get : function() {return topaz_text2d__get_extent_height(this.impl);}
        }
    );


    topaz.text2d.prototype.getCharX = function(i) {
        return topaz_text2d__get_char_x(this.impl, i);
    }

    topaz.text2d.prototype.getCharY = function(i) {
        return topaz_text2d__get_char_y(this.impl, i);
    }


    Object.defineProperty(
        topaz.text2d.prototype,
        'position', {
            get : function() {return new topaz.vector(0, 0, 0, topaz_text2d__get_position(this.impl));},
            set : function(v){topaz_text2d__set_position(this.impl, v.impl);}
        }
    );

    Object.defineProperty(
        topaz.text2d.prototype,
        'rotation', {
            get : function() {return new topaz.vector(0, 0, 0, topaz_text2d__get_rotation(this.impl));},
            set : function(v){topaz_text2d__set_rotation(this.impl, v.impl);}
        }
    );

    Object.defineProperty(
        topaz.text2d.prototype,
        'scale', {
            get : function() {return new topaz.vector(0, 0, 0, topaz_text2d__get_scale(this.impl));},
            set : function(v){topaz_text2d__set_scale(this.impl, v.impl);}
        }
    );


    Object.defineProperty(
        topaz.text2d.prototype,
        'alphaRule', {
            get : function() {return topaz_text2d__get_parameter(this.impl, 0)},
            set : function(v){topaz_text2d__set_parameter(this.impl, 0, v);}
        }
    );

    Object.defineProperty(
        topaz.text2d.prototype,
        'depthTest', {
            get : function() {return topaz_text2d__get_parameter(this.impl, 1)},
            set : function(v){topaz_text2d__set_parameter(this.impl, 1, v);}
        }
    );

    Object.defineProperty(
        topaz.text2d.prototype,
        'etchRule', {
            get : function() {return topaz_text2d__get_parameter(this.impl, 2)},
            set : function(v){topaz_text2d__set_parameter(this.impl, 2, v);}
        }
    );

    Object.defineProperty(
        topaz.text2d.prototype,
        'textureFilter', {
            get : function() {return topaz_text2d__get_parameter(this.impl, 3)},
            set : function(v){topaz_text2d__set_parameter(this.impl, 3, v);}
        }
    );
    topaz._componentAddSymbols(topaz.text2d.prototype);



    /// shape2d
    Object.defineProperty(
        topaz.shape2d.prototype,
        'color', {
            get : function() {return new topaz.color('', topaz_shape2d__get_color(this.impl));},
            set : function(v){topaz_shape2d__set_color(this.impl, v.impl);}
        }
    );

    Object.defineProperty(
        topaz.shape2d.prototype,
        'animSpeed', {
            get : function() {return topaz_shape2d__get_anim_speed(this.impl);},
            set : function(v){topaz_shape2d__set_anim_speed(this.impl, v);}
        }
    );

    Object.defineProperty(
        topaz.shape2d.prototype,
        'center', {
            get : function() {return new topaz.vector(0, 0, 0, topaz_shape2d__get_center(this.impl));},
            set : function(v){topaz_shape2d__set_center(this.impl, v.impl);}
        }
    );


    Object.defineProperty(
        topaz.shape2d.prototype,
        'position', {
            get : function() {return new topaz.vector(0, 0, 0, topaz_shape2d__get_position(this.impl));},
            set : function(v){topaz_shape2d__set_position(this.impl, v.impl);}
        }
    );

    Object.defineProperty(
        topaz.shape2d.prototype,
        'rotation', {
            get : function() {return new topaz.vector(0, 0, 0, topaz_shape2d__get_rotation(this.impl));},
            set : function(v){topaz_shape2d__set_rotation(this.impl, v.impl);}
        }
    );

    Object.defineProperty(
        topaz.shape2d.prototype,
        'scale', {
            get : function() {return new topaz.vector(0, 0, 0, topaz_shape2d__get_scale(this.impl));},
            set : function(v){topaz_shape2d__set_scale(this.impl, v.impl);}
        }
    );


    topaz.shape2d.prototype.formRectangle = function(a, b) {
        topaz_shape2d__form_rectangle(this.impl, a, b);
    }

    topaz.shape2d.prototype.formRadial = function(a, b) {
        topaz_shape2d__form_radial(this.impl, a, b);
    }


    var lines;
    Object.defineProperty(
        topaz.shape2d.prototype,
        'lines', {
            get : function() {return lines;},
            set : function(v){topaz_shape2d__form_lines(this.impl, v); lines = v;}
        }
    );


    var tris;
    Object.defineProperty(
        topaz.shape2d.prototype,
        'triangles', {
            get : function() {return tris;},
            set : function(v){topaz_shape2d__form_triangles(this.impl, v); tris = v;}
        }
    );

    Object.defineProperty(
        topaz.shape2d.prototype,
        'alphaRule', {
            get : function() {return topaz_shape2d__get_parameter(this.impl, 0)},
            set : function(v){topaz_shape2d__set_parameter(this.impl, 0, v);}
        }
    );

    Object.defineProperty(
        topaz.shape2d.prototype,
        'depthTest', {
            get : function() {return topaz_shape2d__get_parameter(this.impl, 1)},
            set : function(v){topaz_shape2d__set_parameter(this.impl, 1, v);}
        }
    );

    Object.defineProperty(
        topaz.shape2d.prototype,
        'etchRule', {
            get : function() {return topaz_shape2d__get_parameter(this.impl, 2)},
            set : function(v){topaz_shape2d__set_parameter(this.impl, 2, v);}
        }
    );

    Object.defineProperty(
        topaz.shape2d.prototype,
        'textureFilter', {
            get : function() {return topaz_shape2d__get_parameter(this.impl, 3)},
            set : function(v){topaz_shape2d__set_parameter(this.impl, 3, v);}
        }
    );
    topaz._componentAddSymbols(topaz.shape2d.prototype);





    /// object2d

    topaz.object2d.prototype.addVelocity = function(a, b) {
        topaz_object2d__add_velocity(this.impl, a, b);
    }

    topaz.object2d.prototype.addVelocityTowards = function(a, b, c) {
        topaz_object2d__add_velocity_towards(this.impl, a, b, c);
    }

    topaz.object2d.prototype.setVelocity = function(a, b) {
        topaz_object2d__set_velocity(this.impl, a, b);
    }

    topaz.object2d.prototype.setVelocityTowards = function(a, b, c) {
        topaz_object2d__set_velocity_towards(this.impl, a, b, c);
    }

    
    Object.defineProperty(
        topaz.object2d.prototype,
        'frictionX', {
            get : function() { return topaz_object2d__get_friction_x(this.impl);},
            set : function(v){ topaz_object2d__set_friction_x(this.impl, v);}
        }
    );

    Object.defineProperty(
        topaz.object2d.prototype,
        'frictionY', {
            get : function() { return topaz_object2d__get_friction_y(this.impl);},
            set : function(v){ topaz_object2d__set_friction_y(this.impl, v);}
        }
    );

    Object.defineProperty(
        topaz.object2d.prototype,
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


    topaz.object2d.prototype.halt = function() {topaz_object2d__halt(this.impl);}
    topaz.object2d.prototype.resetMotion = function() {topaz_object2d__reset_motion(this.impl);}

    Object.defineProperty(
        topaz.object2d.prototype,
        'velocityX', {
            get : function() { return topaz_object2d__get_velocity_x(this.impl);},
            set : function(v){ topaz_object2d__set_velocity_x(this.impl, v);}
        }
    );

    
    Object.defineProperty(
        topaz.object2d.prototype,
        'velocityY', {
            get : function() { return topaz_object2d__get_velocity_y(this.impl);},
            set : function(v){ topaz_object2d__set_velocity_y(this.impl, v);}
        }
    );


    Object.defineProperty(
        topaz.object2d.prototype,
        'speed', {
            get : function() { return topaz_object2d__get_speed(this.impl);},
            set : function(v){ topaz_object2d__set_speed(this.impl, v);}
        }
    );

    Object.defineProperty(
        topaz.object2d.prototype,
        'nextPosition', {
            get : function() { return topaz_object2d__get_next_position(this.impl);}
        }
    );

    Object.defineProperty(
        topaz.object2d.prototype,
        'group', {
            get : function() { return topaz_object2d__get_group(this.impl);},
            set : function(v){ topaz_object2d__set_group(this.impl, v);}
        }
    );


    topaz.object2d.prototype.setGroupInteraction = function(a, b, c) {
        topaz_object2d__set_group_interaction(a, b, c);
    }

    var collider = [];
    Object.defineProperty(
        topaz.object2d.prototype,
        'collider', {
            get : function() { return collider; },
            set : function(v){ collider=v; topaz_object2d__set_collider(this.impl, v);}
        }
    );
    

    topaz.object2d.prototype.setColliderRadial = function(a, b) {
        topaz_object2d__set_collider_radial(this.impl, a, b);
        collider = [];
        const len = topaz_object2d__get_collider_len(this.impl);
        for(var i = 0; i < len; ++i) {
            const pt = topaz_object2d__get_collider_point(this.impl, i);
            collider.push(pt.x);
            collider.push(pt.y);
        }
    }

    Object.defineProperty(
        topaz.object2d.prototype,
        'lastCollided', {
            get : function() { return new topaz.entity(undefined, topaz_object2d__get_last_collided(this.impl));}
        }
    );
    topaz._componentAddSymbols(topaz.object2d.prototype);







    /// state control
    

    topaz.stateControl.prototype.add = function(name, state) {
        topaz_state_control__add(this.impl, name, state.onStep, state.onDraw, state.onInit);
    }

    topaz.stateControl.prototype.remove = function(name) {
        topaz_state_control__remove(this.impl, name);
    }

    topaz.stateControl.prototype.execute = function(name) {
        topaz_state_control__execute(this.impl, name);
    }

    topaz.stateControl.prototype.halt = function(name) {
        topaz_state_control__halt(this.impl, name)
    }


    Object.defineProperty(
        topaz.stateControl.prototype,
        'isHalted', {
            get : function() {
                return topaz_state_control__is_halted(this.impl);
            },
        }
    );

    Object.defineProperty(
        topaz.stateControl.prototype,
        'state', {
            get : function() {
                return topaz_state_control__get_current(this.impl);
            },
        }
    );
    topaz._componentAddSymbols(topaz.stateControl.prototype);




    /// scheduler 
    topaz.scheduler.prototype.startTask = function(taskName, interval, idelay, cb) {
        topaz_scheduler__start_task(this.impl, taskName, interval, idelay, cb);
    }

    topaz.scheduler.prototype.startTaskSimple = function(interval, cb) {
        return topaz_scheduler__start_task_simple(this.impl, interval, cb);
    }


    topaz.scheduler.prototype.endTask = function(name) {
        topaz_scheduler__end_task(this.impl, name);
    }

    topaz.scheduler.prototype.pause = function(name) {
        topaz_scheduler__pause(this.impl, name);
    }

    topaz.scheduler.prototype.resume = function(name) {
        topaz_scheduler__resume(this.impl, name);
    }

    topaz.scheduler.prototype.getTaskIntervalRemaining = function(name) {
        return topaz_scheduler__get_task_interval_remaining(this.impl, name);
    }

    Object.defineProperty(
        topaz.scheduler.prototype,
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
    topaz._componentAddSymbols(topaz.scheduler.prototype);




    /// topaz.component
    topaz._componentAddSymbols(topaz.component.prototype);







    /// topaz.entity.
    topaz._entityAddSymbols(topaz.entity.prototype);



    // topaz.sound 
    Object.defineProperty(
        topaz.sound.prototype,
        'sampleCount', {
            get : function()  { return topaz_sound__get_sample_count(this.impl);}
        }
    );

    Object.defineProperty(
        topaz.sound.prototype,
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
    topaz._assetSetCommonSymbols(topaz.sound.prototype);



    // filesystem.path
    Object.defineProperty(
        topaz.filesystem.path.prototype,
        'string', {
            get : function()  { return topaz_filesystem_path__as_string(this.impl);}
        }
    );

    Object.defineProperty(
        topaz.filesystem.path.prototype,
        'parent', {
            get : function()  { return new topaz.filesystem.path(topaz_filesystem_path__get_parent(this.impl));}
        }
    );


    Object.defineProperty(
        topaz.filesystem.path.prototype,
        'children', {
            get : function()  {
                var out = [];
                const len = topaz_filesystem_path__get_child_count(this.impl);
                for(i = 0; i < len; ++i) {
                    var p = new topaz.filesystem.path(topaz_filesystem_path__get_nth_child(this.impl, i));
                    if (p != undefined) {
                        out.push(p);
                    }                
                }
                
                return out;
            }
        }
    );



    // topaz.data

    
    Object.defineProperty(
        topaz.data.prototype,
        'byteCount', {
            get : function()  { return topaz_data__get_byte_count(this.impl);}
        }
    );

    Object.defineProperty(
        topaz.data.prototype,
        'string', {
            get : function()  { return topaz_data__get_as_string(this.impl);}
        }
    );

    Object.defineProperty(
        topaz.data.prototype,
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
    topaz._assetSetCommonSymbols(topaz.data.prototype);








    // image 

        
    Object.defineProperty(
        topaz.image.prototype,
        'width', {
            get : function()  { return topaz_image__get_width(this.impl);}
        }
    );

    Object.defineProperty(
        topaz.image.prototype,
        'height', {
            get : function()  { return topaz_image__get_height(this.impl);}
        }
    );

    Object.defineProperty(
        topaz.image.prototype,
        'frameCount', {
            get : function()  { return topaz_image__get_frame_count(this.impl);}
        }
    );




    topaz.image.prototype.resize = function(w, h) {
        topaz_image__resize(this.impl, w, h);
    }

    topaz.image.prototype.addFrame = function() {
        topaz_image__add_frame(this.impl);
    }

    topaz.image.prototype.removeFrame = function(f) {
        topaz_image__remove_frame(this.impl, f);
    }

    topaz.image.prototype.setRGBA = function(i, rgbaData) {
        topaz_image__frame_set_rgba(this.impl, i, rgbaData);
    }
    topaz._assetSetCommonSymbols(topaz.image.prototype);



    // audioPlaybackSound 
    
    Object.defineProperty(
        topaz.audioPlaybackSound.prototype,
        'volume', {
            set : function(v)  { topaz_audio__playback_set_volume(this.impl, v);}
        }
    );
    Object.defineProperty(
        topaz.audioPlaybackSound.prototype,
        'panning', {
            set : function(v)  { topaz_audio__playback_set_panning(this.impl, v);}
        }
    );
    Object.defineProperty(
        topaz.audioPlaybackSound.prototype,
        'repeat', {
            set : function(v)  { topaz_audio__playback_set_repeat(this.impl, v);}
        }
    );
    Object.defineProperty(
        topaz.audioPlaybackSound.prototype,
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
        topaz.audioPlaybackSound.prototype,
        'seek', {
            set : function(v)  { 
                topaz_audio__playback_seek(this.impl, v);
            }
        }
    );
    
    
    topaz.audioPlaybackSound.prototype.stop = function() {
        topaz_audio__playback_stop(this.impl);
    }







    // vector base properties




    topaz.vector.prototype.getDistance = function(other) {
        return topaz_vector__get_distance(this.impl, other.impl);
    }        

    topaz.vector.prototype.normalize = function() {
        this.topaz_vector__normalize(this.impl);   
    }

    topaz.vector.prototype.cross = function(other) {
        var out = topaz_vector__cross(this.impl, other.impl);
        return new topaz.vector(0, 0, 0, out);
    }

    topaz.vector.prototype.floor = function() {
        topaz_vector__floor(this.impl);
    }

    topaz.vector.prototype.rotationXDiff = function(other) {
        return topaz_vector__rotation_x_diff(this.impl, other.impl);
    }

    topaz.vector.prototype.rotationXDiffRelative = function(other) {
        return topaz_vector__rotation_x_diff_relative(this.impl, other.impl);
    }

    topaz.vector.prototype.rotationX = function() {
        return topaz_vector__rotation_x(this.impl);
    }

    topaz.vector.prototype.rotationYDiff = function(other) {
        return topaz_vector__rotation_y_diff(this.impl, other.impl);
    }

    topaz.vector.prototype.rotationYDiffRelative = function(other) {
        return topaz_vector__rotation_y_diff_relative(this.impl, other.impl);
    }

    topaz.vector.prototype.rotationY = function() {
        return topaz_vector__rotation_y(this.impl);
    }


    topaz.vector.prototype.rotationZDiff = function(other) {
        return topaz_vector__rotation_z_diff(this.impl, other.impl);
    }

    topaz.vector.prototype.rotationZDiffRelative = function(other) {
        return topaz_vector__rotation_z_diff_relative(this.impl, other.impl);
    }

    topaz.vector.prototype.rotationZ = function() {
        return topaz_vector__rotation_z(this.impl);
    }

    topaz.vector.prototype.rotateX = function(val) {
        topaz_vector__rotate_x(this.impl, val);
    }

    topaz.vector.prototype.rotateY = function(val) {
        topaz_vector__rotate_y(this.impl, val);
    }

    topaz.vector.prototype.rotateZ = function(val) {
        topaz_vector__rotate_z(this.impl, val);
    }

    topaz.vector.prototype.remove = function() {
        topaz_vector__destroy(this.impl);
        impl = {};
    }

    topaz.vector.prototype.add = function(b) {
        return new topaz.vector(this.x + b.x, this.y + b.y, this.z + b.z);
    }
    topaz.vector.prototype.subtract = function(b) {
        return new topaz.vector(this.x - b.x, this.y - b.y, this.z - b.z);
    }
    topaz.vector.prototype.multiply = function(b) {
        return new topaz.vector(this.x * b.x, this.y * b.y, this.z * b.z);
    }
    topaz.vector.prototype.divide = function(b) {
        return new topaz.vector(this.x / b.x, this.y / b.y, this.z / b.z);
    }


    Object.defineProperty(topaz.vector.prototype, 'length', {get : function(){return topaz_vector__get_length(impl)}});
    Object.defineProperty(topaz.vector.prototype, 'x', {get : function(){return topaz_vector__get_x(this.impl);}, set : function(v){topaz_vector__set_x(this.impl, v);}});
    Object.defineProperty(topaz.vector.prototype, 'y', {get : function(){return topaz_vector__get_y(this.impl);}, set : function(v){topaz_vector__set_y(this.impl, v);}});
    Object.defineProperty(topaz.vector.prototype, 'z', {get : function(){return topaz_vector__get_z(this.impl);}, set : function(v){topaz_vector__set_z(this.impl, v);}});


}());



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
Object.defineProperty(topaz.resources, 'assetPaths', {
        get : function(){
            var paths = [];
            const len = topaz_resources__asset_path_count();
            for(var i = 0; i < len; ++i) {
                paths.push(topaz_resources__nth_asset_path(i))                
            }
            return paths;
        }
    }
);
Object.defineProperty(topaz.resources, 'path', {
    set : function(v){topaz_resources__set_path(v);},
    get : function(){return topaz_resources__get_path();}
});


topaz.attachPreManager(
    new topaz.entity({
        name : 'TOPAZEntityCleaner',
        onStep : function() {
            const pool = topaz.deadEntityPool;
            if (pool.length) {
                for(var i = 0; i < pool.length; ++i) {
                    if (pool[i].impl) {
                        pool[i].impl.__ctx.props = undefined;
                        pool[i].impl.__ctx = undefined;
                        pool[i].impl = undefined;
                    }
                }
                topaz.deadEntityPool = [];
            }
        }
    })
);
