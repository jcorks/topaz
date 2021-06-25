"use strict"
const TOPAZ = {
    INPUT : {
        NOT_AN_INPUT : 0,
        KEY_0: 1, ///< 0
        KEY_1: 2, ///< 1
        KEY_2: 3, ///< 2
        KEY_3: 4, ///< 3
        KEY_4: 5, ///< 4
        KEY_5: 6, ///< 5
        KEY_6: 7, ///< 6
        KEY_7: 8, ///< 7
        KEY_8: 9, ///< 8
        KEY_9: 10, ///< 9
        KEY_a: 11, ///< a
        KEY_b: 12, ///< b
        KEY_c: 13, ///< c
        KEY_d: 14, ///< d
        KEY_e: 15, ///< e
        KEY_f: 16, ///< f
        KEY_g: 17, ///< g
        KEY_h: 18, ///< h
        KEY_i: 19, ///< i
        KEY_j: 20, ///< j
        KEY_k: 21, ///< k
        KEY_l: 22, ///< l
        KEY_m: 23, ///< m
        KEY_n: 24, ///< n
        KEY_o: 25, ///< o
        KEY_p: 26, ///< p
        KEY_q: 27, ///< q
        KEY_r: 28, ///< r
        KEY_s: 29, ///< s
        KEY_t: 30, ///< t
        KEY_u: 31, ///< u
        KEY_v: 32, ///< v
        KEY_w: 33, ///< w
        KEY_x: 34, ///< x
        KEY_y: 35, ///< y
        KEY_z: 36, ///< z
        KEY_lshift: 37, ///< Left shift key
        KEY_rshift: 38, ///< Right shift key
        KEY_lctrl: 39,  ///< Left control key
        KEY_rctrl: 40,  ///< Right control key
        KEY_lalt: 41,   ///< Left alt key
        KEY_ralt: 42,   ///< Right alt key
        KEY_tab: 43,    ///< Tab
        KEY_F1: 44,     ///< F1
        KEY_F2: 45,     ///< F2
        KEY_F3: 46,     ///< F3
        KEY_F4: 47,     ///< F4
        KEY_F5: 48,     ///< F5
        KEY_F6: 49,     ///< F6
        KEY_F7: 50,     ///< F7
        KEY_F8: 51,     ///< F8
        KEY_F9: 52,     ///< F9
        KEY_F10: 53,    ///< F10
        KEY_F11: 54,    ///< F11
        KEY_F12: 55,    ///< F12
        KEY_up: 100,     ///< Up arrow
        KEY_down: 101,   ///< Down arrow
        KEY_left: 102,   ///< Left arrow
        KEY_right: 103,  ///< Right arrow
        KEY_minus: 104,  ///< -
        KEY_equal: 105,  ///< = 
        KEY_backspace: 106,  ///< Backspace
        KEY_grave: 107,  ///< `
        KEY_esc: 108,    ///< Escape
        KEY_home: 109,   ///< Home key
        KEY_pageUp: 110, ///< Page up key
        KEY_pageDown: 111,  ///< Page down key
        KEY_end: 112,    ///< End key
        KEY_backslash: 113, ///< '\'
        KEY_lbracket: 114, ///< [
        KEY_rbracket: 115, ///< ]
        KEY_semicolon: 116, ///< ;
        KEY_apostrophe: 117, ///< '
        KEY_frontslash: 118, ///< /
        KEY_enter: 119, ///< Enter
        KEY_delete: 120, ///< Delete
        KEY_numpad0: 121, ///< Numpad 0
        KEY_numpad1: 122, ///< Numpad 1
        KEY_numpad2: 123, ///< Numpad 2
        KEY_numpad3: 124, ///< Numpad 3
        KEY_numpad4: 125, ///< Numpad 4
        KEY_numpad5: 126, ///< Numpad 5
        KEY_numpad6: 127, ///< Numpad 6
        KEY_numpad7: 128, ///< Numpad 7
        KEY_numpad8: 129, ///< Numpad 8
        KEY_numpad9: 130, ///< Numpad 9
        KEY_prtscr: 131, ///< Print screen button
        KEY_lsuper: 132, ///< Left Super key (Windows key)
        KEY_rsuper: 133, ///< Right Super key (Windows key)
        KEY_space: 134,  ///< Space
        KEY_insert: 135, ///< Insert key
        KEY_comma: 136, ///< ,
        KEY_period: 137 , ///< .
        KEY_world1: 138, /// I8n key0
        KEY_world2: 139, /// I8n key1
        KEY_world3: 140, /// I8n key2
        KEY_world4: 141, /// I8n key3
        KEY_world5: 142, /// I8n key4
        KEY_world6: 143, /// I8n key5
        KEY_world7: 144, /// I8n key6
        KEY_world8: 145, /// I8n key7
        KEY_world9: 146, /// I8n key8
    
        POINTER_0: 256, ///< Left click
        POINTER_1: 257, ///< Right click
        POINTER_2: 258, ///< Middle click
    
        POINTER_X: 259, ///< Horizontal axis. Usually for the X axis of the pointer
        POINTER_Y: 260, ///< Horizontal axis. Usually for the X axis of the pointer
        POINTER_Wheel: 261, ///< Mouse wheel.
    
        PAD_a: 300,     ///< Button 0
        PAD_b: 301,     ///< Button 1
        PAD_c: 302,     ///< Button 2
        PAD_x: 303,     ///< Button 3
        PAD_y: 304,     ///< Button 4
        PAD_r: 305,     ///< Button 5
        PAD_l: 306,     ///< Button 6
        PAD_r2: 307,    ///< Button 7
        PAD_l2: 308,    ///< Button 8
        PAD_r3: 309,    ///< Button 9
        PAD_l3: 310,    ///< Button 10
        PAD_start: 311,    ///< Button 11
        PAD_select: 312,///< Button 12
        PAD_b13: 313,///< Button 13
        PAD_b14: 314,///< Button 14
        PAD_b15: 315,///< Button 15
        PAD_b16: 316,///< Button 16
        PAD_b17: 317,///< Button 17
        PAD_b18: 318,///< Button 18
        PAD_b19: 319,///< Button 19
        PAD_b20: 320,///< Button 20
        PAD_b21: 321,///< Button 21
        PAD_b22: 322,///< Button 22
        PAD_b23: 323,///< Button 23
        PAD_b24: 324,///< Button 24
        PAD_b25: 325,///< Button 25
        PAD_b26: 326,///< Button 26
        PAD_b27: 327,///< Button 27
        PAD_b28: 328,///< Button 28
        PAD_b29: 329,///< Button 29
        PAD_b30: 330,///< Button 30
        PAD_b31: 331,///< Button 31
        PAD_b32: 332,///< Button 32
    
        PAD_axisX: 400, ///< X button
        PAD_axisY: 401, ///< Y button
        PAD_axisZ: 402, ///< Z button
        PAD_axisX2: 403,///< X2 button 
        PAD_axisY2: 404,///< Y2 button
        PAD_axisZ2: 405,///< Z2 button
        PAD_axisX3: 406,///< X3 button
        PAD_axisY3: 407,///< Y3 button
        PAD_axisZ3: 408,///< Z3 button
        PAD_axisX4: 409,///< X4 button
        PAD_axisY4: 410,///< Y4 button
        PAD_axisZ4: 411,///< Z4 button
        PAD_axisX5: 412,///< X4 button
        PAD_axisY5: 413,///< Y4 button
        PAD_axisZ5: 414,///< Z4 button
        PAD_axisX6: 415,///< X4 button
        PAD_axisY6: 416,///< Y4 button
        PAD_axisZ6: 417,///< Z4 button
    
        PAD_axisR: 450,   
        PAD_axisL: 451,    
        PAD_axisR2: 452,    
        PAD_axisL2: 453,    
        PAD_axisR3: 454,    
        PAD_axisL3: 455,    
        PAD_axisR4: 456,    
        PAD_axisL4: 457,    
    
        PAD_options: 511,
        COUNT: 512
    },

  
    RENDERER : {
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
            INVISIBLE : 3,
        },
        
        TEXTURE_FILTER_HINT : {
            LINEAR : 0,
            NONE : 1,
        },
        
        LAYER : {
            COLOR : 1,
            DEPTH : 2,
            ETCH : 4,
            ALL  : 7
        }

    },
    
    ASSET : { 
        TYPE : {
            NONE : 0,
            IMAGE : 1,
            SOUND : 2,
            MATERIAL : 3,
            PARTICLE : 4,
            DATA : 5,
            MESH : 6
        }
    },
    
    
    PARTICLE : {
        PROPERTY : {
            DURATION : 0,
            SCALE_X : 1,
            SCALE_Y : 2,
            SCALE_MULTIPLIER : 3,
            ROTATION : 4,
            DIRECTION : 5,
            SPEED_X : 6,
            SPEED_Y : 7,
            RED : 8,
            GREEN : 9,
            BLUE : 10,
            ALPHA : 11,
        }
    },
    
    FILESYSTEM : {
        DEFAULT_NODE : {
            RESOURCES : 0,
            TOPAZ : 1,
            USER_DATA : 2
        },
    
    },
    
    AUTOMATION : {
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


    SHAPE3D : {
        TEXTURE : {
            SLOT_0 : 0,
            SLOT_1 : 1,
            SLOT_2 : 2
        }
    },


    OBJECT2D : {
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
        }
    },
    
    SCHEDULER : {
        MODE : {
            TIME : 0,
            FRAME : 1
        }
    },
    
    DISPLAY : {
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
            INPUT_FOCUS : 9
        },
        
        FRAMEBUFFER : {
            A : 0,
            B : 1,
            C : 2,
            D : 3
        }
    },
    

}

// prototype: tclass
//
// Gives a strict class definition with 
// easy introspection.
var tclass = function(d) {
    const classinst = {};
    if (d.declare != undefined) {
        d.declare(classinst);
    }
    const define = d.define;
    const inherits = d.inherits;

    // isa query data. since every inherited class has to 
    // be declared first, all base classes will already have a proper 
    // inheritsFrom. We take our base ones and add to it.
    classinst.interfaces = [];
    const allclass = classinst.interfaces;
    if (inherits != undefined) {
        const addbase = function(other) {
            for(var n = 0; n < other.length; ++n) {
                const g = other[n];
                
                var found = false;
                for(var i = 0; i < allclass.length; ++i) {
                    if (allclass[i] === g) {
                        found = true;
                        break;
                    }
                }
                if (found == false)
                    allclass.push(g);
            }
        }
        if (inherits.length) {
            for(var i = 0; i < inherits.length; ++i)
                addbase(inherits[i].interfaces);
        } else {
            addbase(inherits.interfaces);
        }
    }    
    allclass.push(classinst);
    

    // returns whether the isntance inherits from the given class.
    const isa = function(d) {
        for(var i = 0; i < allclass.length; ++i) 
            if (allclass[i] === d) return true;
        return false;
    }
    
    classinst.new = function(argobj, noseal, outsrc) {
        if (argobj == undefined) argobj = {};
        var out = outsrc;
        if (inherits != undefined) {
            if (out == undefined) out = {};
            if (inherits.length) {
                for(var i = 0; i < inherits.length; ++i) 
                    inherits[i].new(argobj, true, out);
            } else {
                inherits.new(argobj, true, out);
            }
        }
        if (out == undefined) out = {};
        
        var varnames;
        var mthnames;
        if (out.publicVars != undefined) {
            mthnames = {
                inherited : out.publicMethods
            };
    
            varnames = {
                inherited : out.publicVars
            };
            out.publicVars = varnames;
            out.publicMethods = mthnames;
        } else {
            varnames = {};
            mthnames = {};
            out.publicVars = varnames;
            out.publicMethods = mthnames;
        }
        
        out.interface = function(obj) {
            const keys = Object.keys(obj);
            for(var i = 0; i < keys.length; ++i) {
                const v = obj[keys[i]];
                switch(typeof(v)) {
                  case 'function':
                    out[keys[i]] = v; 
                    mthnames[keys[i]] = 'function';
                    break;
                    
                  case 'object':
                    Object.defineProperty(out,
                        keys[i], {
                            get : v.get,
                            set : v.set,
                        }
                    );
                    if (v.get == undefined && v.set != undefined) {
                        varnames[keys[i]] = 'Write-only';                        
                    } else if (v.get != undefined && v.set == undefined) {
                        varnames[keys[i]] = 'Read-only';                                            
                    } else {
                        varnames[keys[i]] = 'Read/Write';                                                                
                    }
                    break;
                    
                  default:
                    throw new Error('tclass interfaces can only have getters/setters and methods.');
                }
            }
        }
        define(out, argobj, classinst);
        delete out.interface;

        if (d.toString != undefined) {
            const a = d.toString;
            out.toString = function(){return a(out);};
        }

        if (!noseal) {
            out.introspect = function() {
                return {
                    'public' : {
                        'variables' : varnames,
                        'methods' : mthnames
                    }
                };
            }
            out.isa = isa;
            delete out.publicMethods;
            delete out.publicVars;

            Object.seal(out);
        }
        return out;        
    }
    
    return classinst
}

const Vector = (function(){
    const statepush = function(v) {
        if (v.native == undefined) {
            if (isNaN(v.x)) v.x = 0;
            if (isNaN(v.y)) v.y = 0;
            if (isNaN(v.z)) v.z = 0;
            v.native = topaz_vector__create(v.x, v.y, v.z);
        } else {
            topaz_vector__set_xyz(v.native, v.x, v.y, v.z);
        }
    }

    const statepull = function(v) {
        v.x = topaz_vector__get_x(v.native);
        v.y = topaz_vector__get_y(v.native);
        v.z = topaz_vector__get_z(v.native);
    }

    return {
        statepush : statepush,
        statepull : statepull,
        fromnative : function(native) {
            const out = {native:native};
            statepull(out);
            return out;
        },
        getDistance : function(a, b){ 
            statepush(a); statepush(b);
            return topaz_vector__get_distance(a.native, b.native);
        },
        
        normalize : function(a){ 
            statepush(a);
            topaz_vector__normalize(a.native);   
            statepull(a);
        },
        
        cross : function(a, b){ 
            statepush(a);
            statepush(b);
            return cnew({native:topaz_vector__cross(a.native, b.native)});
        },

        floor : function(a){ 
            statepush(a);
            topaz_vector__floor(a.native);
            statepull(a);
        },
        
        rotationXDiff : function(a, b){ 
            statepush(a);
            statepush(b);
            return topaz_vector__rotation_x_diff(a.native, b.native);
        },
        
        rotationXDiffRelative : function(a, b){ 
            statepush(a);
            statepush(b);
            return topaz_vector__rotation_x_diff_relative(a.native, b.native);
        },
        
        rotationX : function(a) { 
            statepush(a);
            return topaz_vector__rotation_x(a.native);
        },
        
        rotationYDiff : function(a, b) { 
            statepush(a);
            statepush(b);
            return topaz_vector__rotation_y_diff(a.native, b.native);
        },
        
        rotationYDiffRelative : function(a, b){ 
            statepush(a);
            statepush(b);
            return topaz_vector__rotation_y_diff_relative(a.native, b.native);
        },
        
        rotationY : function(a){ 
            statepush(a);
            return topaz_vector__rotation_y(a.native);
        },
        
        rotationZDiff : function(a, b){
            statepush(a);
            statepush(b); 
            return topaz_vector__rotation_z_diff(a.native, b.native);
        },
        
        rotationZDiffRelative : function(a, b){
            statepush(a);
            statepush(b);
            return topaz_vector__rotation_z_diff_relative(a.native, b.native);
        },
        
        rotationZ : function(a){ 
            statepush(a);
            return topaz_vector__rotation_z(a.native);
        },
        
        rotateX : function(a, val){ 
            statepush(a);
            topaz_vector__rotate_x(a.native, val);
            statepull(a);
        },
        
        rotateY : function(a, val){ 
            statepush(a);
            topaz_vector__rotate_y(a.native, val);
            statepull(a);
        },
        
        rotateZ : function(a, val){ 
            statepush(a);
            topaz_vector__rotate_y(a.native, val);
            statepull(a);
        },

        
        add : function(a, b){ 
            return {x:a.x + b.x, y:a.y + b.y, z:a.z + b.z};
        },
        
        subtract : function(a, b){ 
            return {x:a.x - b.x, y:a.y - b.y, z:a.z - b.z};
        },

        multiply : function(a, b){ 
            return {x:a.x * b.x, y:a.y * b.y, z:a.z * b.z};
        },
        
        divide : function(a, b){ 
            return {x:a.x / b.x, y:a.y / b.y, z:a.z / b.z};
        },
        
        length : function(a){
            statepush(a);
            return topaz_vector__get_length(a.native)
        }
    }
})();


// similar to vector, color is a "light object" where 
// the state is held in-script and no real methods are 
// bound to it. When it interacts with 
// the native context, a wrapper is added and the data syncd
const Color = (function(){
    const refColor = topaz_color__create(0, 0, 0, 0);

    const statepush = function(v) {
        if (v.native == undefined) {
            if (isNaN(v.r)) v.r = 0;
            if (isNaN(v.g)) v.g = 0;
            if (isNaN(v.b)) v.b = 0;
            if (isNaN(v.a)) v.a = 0;
            v.native = topaz_color__create(v.r, v.g, v.b, v.a);
        } else {
            topaz_color__set_rgba(v.native, v.r, v.g, v.b, v.a);
        }
    }

    const statepull = function(v) {
        v.r = topaz_color__get_r(v.native);
        v.g = topaz_color__get_g(v.native);
        v.b = topaz_color__get_b(v.native);
        v.a = topaz_color__get_a(v.native);
    }

    return {
        fromnative : function(native) {
            const out = {native:native};
            statepull(out);
            return out;
        },
        statepush : statepush,
        statepull : statepull,
        asString : function(a) {
            statepush(a);
            return topaz_color__to_hex_string(a.native);
        },

        fromString : function(str) {
            topaz_color__set_from_string(refColor, str);
            return {
                r: topaz_color__get_r(refColor),
                g: topaz_color__get_g(refColor),
                b: topaz_color__get_b(refColor),
                a: topaz_color__get_a(refColor)
            }
        }
    }
})();


var Topaz = tclass({
    define : function(this_, args, classinst){ 
        var __Topaz__ = this_;

        // Declares a class which has external, native data.
        // Instances of this_ class act as "shells" or interfaces for 
        // this_ external data and usually have little to no state 
        // outside of this_.
        var __Native__ = tclass({
            declare  : function(this_class){ 
                this_class.uniqueObjectPool = 0;    
            },
            
            define : function(this_, args, this_class){ 
                var impl = undefined;
                var id = this_class.uniqueObjectPool;
                this_class.uniqueObjectPool = 1 + this_class.uniqueObjectPool;       
                
                this_.interface({
                    // binds a native instance to this_ table. Will throw an error 
                    // if no since native instance can be bound. Binding happens through 2 
                    // mechanisms: 1) instData.nativeCreate is a native function that can { 
                    // return a new object for us to use or 2) instData.instance contains 
                    // a preexisting native reference.
                    bindNative : function(instData, args) { 
                        
                        if (instData.instance == undefined) { 
                            impl = undefined                        
                        } else {
                            impl = instData.instance.native;
                        } 
                        
                        
                        if (impl == undefined && instData.nativeCreate != undefined) { 
                            if (args == undefined) {
                                impl = instData.nativeCreate();
                            } else {
                                switch(args.length) {
                                  case 1:
                                    impl = instData.nativeCreate(args[0]);
                                    break;
                                  case 2:
                                    impl = instData.nativeCreate(args[0], args[1]);
                                    break;
                                  case 3:
                                    impl = instData.nativeCreate(args[0], args[1], args[2]);
                                    break;
                                  defaul:
                                    throw new Error('Too many arguments to dispatch native create call');
                                }
                            }
                        }
                        if (impl == undefined) {
                            error('Topaz.Native instance cannot have a undefined native reference.'+ tostring(debug.traceback('')));
                            return;
                        }
                        impl.__ctx = this_;
                        return impl;
                    },
                    
                    
                    // A unique id that identifies this_ instance.
                    uniqueID : {
                        get : function(){ 
                            return id;
                        }            
                    },
                    
                    
                    //Gets the external reference to this_ external class
                    native : { 
                        get : function() { 
                            return impl;
                        }
                    }          
                });
            
            }
        });

        // Must be separate since Topaz inherit from the asset ahead of time before Topaz is computed.
        var __Asset__ = tclass({
            inherits : __Native__,
            define : function(this_, args, this_class){ 
                var impl = this_.bindNative({
                    instance : args,
                });

                this_.interface({
                    type : {
                        get : function()   {return topaz_asset__get_type(impl);},
                        set : function(v)  {      topaz_asset__set_type(impl, v);}
                    },

                    name : {
                        get : function()   {return topaz_asset__get_name(impl);},
                        set : function(v)  {      topaz_asset__set_name(impl, v);} 
                    }
                });
            }
        });


        var __Filesystem__ = {        
            getPath : function(n) { 
                return __Topaz__.Filesystem.Path.new({native:topaz_filesystem__get_path(n)});
            },
            
            getPathFromString : function(pth, str){ 
                if (str == undefined) {
                    return __Topaz__.Filesystem.Path.new({native:topaz_filesystem__get_path_from_string(pth)});            
                } else {
                    return __Topaz__.Filesystem.Path.new({native:topaz_filesystem__get_path_from_string(pth.native, str)});                        
                }
            },
            
            Path : tclass({
                inherits : __Native__,
                define   : function(this_class, this_, args){ 
                    var impl = this_.bindNative({instance : args});

                    this_.interface({
                        string : {
                            get : function(){ 
                                return topaz_filesystem_path__as_string(impl);
                            }
                        },
                        
                        parent : {
                            get : function(){ 
                                return __Topaz__.Filesystem.Path.new({native:topaz_filesystem_path__get_parent(impl)});
                            }
                        },
                        
                        children : {
                            get : function(){ 
                                var out = [];
                                var len = topaz_filesystem_path__get_child_count(impl);
                                for(var i = 0; i < len; ++i) { 
                                    var p = __Topaz__.Filesystem.Path.new({native:topaz_filesystem_path__get_nth_child(impl, i)});
                                    if (p != undefined) {
                                        out.push(p);
                                    }
                                }
                                return out;
                            }
                        }
                    });
                }
            }),
        };

        var __RNG__ = tclass({
            inherits : __Native__,
            define : function(this_, args, this_class){ 
    
                var impl = this_.bindNative({
                    instance : args, 
                    nativeCreate : topaz_rng__create
                }, [args.seed]);
    
                
                this_.interface({
                    seed : {
                        set : function(s) { 
                            topaz_rng__set_seed(impl, s);
                        }
                    },
                    
                    integer : {
                        get : function() { 
                            return topaz_rng__next_int(impl);
                        }
                    },
                    
                    value : {
                        get : function() { 
                            return topaz_rng__next_value(impl);
                        }
                    }
                });
            }
        });

        var __ViewManager__ = (tclass({
            define : function(this_, args, this_class){ 
                this_.interface({
                    mainDisplay : {
                        set : function(v){ 
                            topaz_view_manager__set_main(v.native);
                        },
                        get : function(){ 
                            return __Topaz__.Display.new({native:topaz_view_manager__get_main()});
                        }
                    },
    
                    clipboard : {
                        set : function(v){ 
                            topaz_view_manager__set_clipboard_from_string(v);
                        },
                        get : function(){ 
                            return topaz_view_manager__get_clipboard_as_string();
                        }
                    }
                });
            }
        })).new();

        var __Display__ =  tclass({
            inherits : __Native__,
            define : function(this_, args, this_class) { 
                var impl = this_.bindNative({
                    instance : args,
                    nativeCreate : topaz_view_manager__create_display
                });
                
                this_.interface({
                    destroy : function(){ 
                        topaz_display__destroy(impl);
                    },
    
                    resize : function(w, h){ 
                        topaz_display__set_parameter(impl, 2, w);
                        topaz_display__set_parameter(impl, 3, h);
                    },
    
                    addParameterCallback : function(cb){ 
                        return topaz_display__add_parameter_callback(impl, cb);
                    },
    
                    addCloseCallback : function(cb){ 
                        return topaz_display__add_close_callback(impl, cb);
                    },
    
                    removeCallback : function(i){ 
                        topaz_display__remove_callback(impl, i);
                    },
    
                    getParameter : function(p){ 
                        return topaz_display__get_parameter(impl, p);
                    },
    
                    setParameter : function(p, v){ 
                        topaz_display__set_parameter(impl, p, v);
                    },
    
    
                    getFramebuffer : function(a){ 
                        return __Topaz__.Framebuffer.new({native:topaz_display__get_framebuffer(impl, a)});
                    },
    
                    useFramebuffer : function(a){ 
                        return topaz_display__use_framebuffer(impl, a);
                    },
    
                    clearMainFramebuffer : function(a) { 
                        topaz_display__clear_main_framebuffer(impl, a);
                    },
    
                    captureMainFramebuffer : function(a){ 
                        topaz_display__capture_main_framebuffer(impl, a.native);
                    },
    
                    width : {
                        get : function(){ 
                            return topaz_display__get_parameter(impl, 2);
                        },
                        set : function(v){ 
                            topaz_display__set_parameter(impl, 2, v);
                        }
                    },
    
                    height : {
                        get : function(){ 
                            return topaz_display__get_parameter(impl, 3);
                        },
                        set : function(v){ 
                            topaz_display__set_parameter(impl, 3, v);
                        }
                    },
    
    
                    camera2d : {
                        get : function(){ 
                            return __Topaz__.Entity.new({native:topaz_display__get_camera_2d(impl)});
                        }
                    },
    
                    camera3d : {
                        get : function(){ 
                            return __Topaz__.Entity.new({native:topaz_display__get_camera_3d(impl)});
                        }
                    },
    
                    framebuffer : {
                        get : function(){ 
                            return __Topaz__.Framebuffer.new({native:topaz_display__get_main_framebuffer(impl)});
                        }
                    }
                });
            }
        });
    
        var __Framebuffer__ = tclass({
            inherits : __Native__,
            define : function(this_, args, this_class){ 
                var impl = this_.bindNative({
                    instance : args
                });
    
                this_.interface({
                    width : {
                        get : function(){ 
                            return topaz_framebuffer__get_width(impl);
                        }
                    },
    
                    height : {
                        get : function(){ 
                            return topaz_framebuffer__get_height(impl);
                        }
                    },
    
                    resize : function(w, h){ 
                        topaz_framebuffer__resize(impl, w, h);
                    },
    
                    filteredHint : {
                        get : function(){ 
                            return topaz_framebuffer__get_filtered_hint(impl);
                        },
                        set : function(b){ 
                            return topaz_framebuffer__set_filtered_hint(impl, b);
                        }
                    }
                });
            }
        });

        var __Mesh__ = tclass({
            inherits : __Asset__,
            define : function(this_, implPre, this_class){ 
                var impl = this_.native;    
                this_.interface({
                    vertexCount : {
                        get : function()   {return topaz_mesh__get_vertex_count(impl);},
                        set : function(v)  {return topaz_mesh__set_vertex_count(impl, v);} 
                    },
    
    
                    vertices : {
                        set : function(v){ 
                            topaz_mesh__define_vertices(impl, v); 
                        },
                        
                        get : function(){ 
                            var out = {};
                            var len = this_.vertexCount-1;
                            for(var i = 0; i < len; ++i) {
                                table.append(out, topaz_mesh__get_vertex(impl, 0, i, 0));
                                table.append(out, topaz_mesh__get_vertex(impl, 0, i, 1));
                                table.append(out, topaz_mesh__get_vertex(impl, 0, i, 2));
            
                                table.append(out, topaz_mesh__get_vertex(impl, 1, i, 0));
                                table.append(out, topaz_mesh__get_vertex(impl, 1, i, 1));
                                table.append(out, topaz_mesh__get_vertex(impl, 1, i, 2));
            
                                table.append(out, topaz_mesh__get_vertex(impl, 2, i, 0));
                                table.append(out, topaz_mesh__get_vertex(impl, 2, i, 1));
            
                                table.append(out, topaz_mesh__get_vertex(impl, 3, i, 0));
                                table.append(out, topaz_mesh__get_vertex(impl, 3, i, 1));
                                table.append(out, topaz_mesh__get_vertex(impl, 3, i, 2));
                                table.append(out, topaz_mesh__get_vertex(impl, 3, i, 3));
                            }      
                            
                            return out;
                        }
                    },
    
                    setVertex : function(i, v){ 
                        topaz_mesh__set_vertex(impl, i, v);
                    },
    
                    getVertex : function(i){ 
                        var out = {};
                        table.append(out, topaz_mesh__get_vertex(impl, 0, i, 0));
                        table.append(out, topaz_mesh__get_vertex(impl, 0, i, 1));
                        table.append(out, topaz_mesh__get_vertex(impl, 0, i, 2));
                
                        table.append(out, topaz_mesh__get_vertex(impl, 1, i, 0));
                        table.append(out, topaz_mesh__get_vertex(impl, 1, i, 1));
                        table.append(out, topaz_mesh__get_vertex(impl, 1, i, 2));
                
                        table.append(out, topaz_mesh__get_vertex(impl, 2, i, 0));
                        table.append(out, topaz_mesh__get_vertex(impl, 2, i, 1));
                
                        table.append(out, topaz_mesh__get_vertex(impl, 3, i, 0));
                        table.append(out, topaz_mesh__get_vertex(impl, 3, i, 1));
                        table.append(out, topaz_mesh__get_vertex(impl, 3, i, 2));
                        table.append(out, topaz_mesh__get_vertex(impl, 3, i, 3));
                        return out;            
                    },
    
                    addObject : function(obj){ 
                        topaz_mesh__add_object(impl);
                        if (obj != undefined) {
                            topaz_mesh__set_object(this_.objectCount-1, obj);
                        }
                    },
    
                    objectCount : {
                        get : function() { 
                            return topaz_mesh__get_object_count(impl);
                        }
                    },
    
                    removeObject : function(i){ 
                        topaz_mesh__remove_object(impl, i);
                    },
    
                    setObject : function(i, v){ 
                        topaz_mesh__set_object(impl, i, v);
                    },
    
    
                });
            }
        });

        var __Input__ = tclass({
            define : function(this_, args, this_class) { 
                this_.interface({
                    mouse : {   
                        get : function(){ 
                            return {x:topaz_input__mouse_x(), y:topaz_input__mouse_y()};
                        }
                    },
    
                    mouseDelta : {
                        get : function(){ 
                            return {x:topaz_input__mouse_delta_x(), y:topaz_input__mouse_delta_y()};
                        } 
                    },
    
                    mouseWheel : {
                        get : function() {return topaz_input__mouse_wheel(); } 
                    },
    
    
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
                        var len = topaz_input__query_pad_count();
                        var out = [];
                        for(var i = 0; i < len; ++i) {
                            table.append(out, input_api__query_pad_id(i));
                        }
                        return out;
                    },
            
                    addUnicodeListener : function(l) { 
                        topaz_input__add_unicode_listener(l);
                    },
            
                    removeUnicodeListener : function(l) { 
                        topaz_input__remove_unicode_listener(l);
                    }
            
                })
            }
        }).new();


        var __Audio__ = tclass({
            define : function(this_, this_class){ 
                var ps = tclass({
                    inherits : __Native__,
                    define : function(this_, args, this_class) { 
                        var impl = this_.bindNative({
                            instance : args
                        });
    
    
                        this_.interface({
                            volume : {
                                set : function(v)  {topaz_audio__playback_set_volume(impl, v);} 
                            },
    
                            panning : {
                                set : function(v)  {topaz_audio__playback_set_panning(impl, v);} 
                            },
    
                            repeatSound : {
                                set : function(v)  {topaz_audio__playback_set_repeat(impl, v);} 
                            },
    
                            paused : {
                                set : function(v) { 
                                    if (v != undefined) { 
                                        topaz_audio__playback_pause(impl);
                                    } else {
                                        topaz_audio__playback_resume(impl);
                                    }
                                }
                            },
    
                            seek : {
                                set : function(v) { 
                                    topaz_audio__playback_seek(impl, v);
                                }
                            },
    
                            stop : function(){ 
                                topaz_audio__playback_stop(impl);
                            }
                        });
                    }
                });
                this_.interface({
                    // get class ref
                    PlaybackSound : {
                        get : function(){ 
                            return ps;
                        }
                    },
                
                    playSound : function(asset, channel) { 
                        if (channel == undefined) { 
                            return ps(topaz_audio__play_sound(asset.native, 0));
                        } else {
                            return ps(topaz_audio__play_sound(asset.native, channel));
                        }
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
                })
            }
        }).new();

        var __Material__ = tclass({
            inherits : __Asset__,
            define : function(this_, args, this_class){ 
                var impl = this_.native;
                this_.interface({
                    setProgramData : function(a, b){ 
                        topaz_material__set_program_data(impl, a, b);
                    },
    
                    getProgramData : function(a){ 
                        return topaz_material__set_program_data(impl, a);
                    },
    
                    clearSources : function(){ 
                        topaz_material__clear_sources(impl);
                    },
    
                    setProgramSource : function(a, b, c){ 
                        topaz_material__set_program_source(impl, a, b, c); 
                    },
    
    
                });
            }
        });
        
        var __Image__ = tclass({
            inherits : __Asset__,
            define : function(this_, args, this_class){ 
                var impl = this_.native;
                this_.interface({
                    width : {
                        get : function()   {return topaz_image__get_width(impl);} 
                    },
                    
                    height : {
                        get : function()   {return topaz_image__get_height(impl);}
                    },
                    
                    frameCount : {
                        get : function()   {return topaz_image__get_frame_count(impl);} 
                    },
                    
                    resize : function(w, h) { 
                        topaz_image__resize(impl, w, h);
                    },
                    
                    addFrame : function() { 
                        topaz_image__add_frame(impl);
                    },
                    
                    removeFrame : function(f) { 
                        topaz_image__remove_frame(impl, f);
                    },
                    
                    setRGBA : function(i, rgbaData) { 
                        topaz_image__frame_set_rgba(impl, i, rgbaData);
                    }
                });
            }
        });
        
        var __Data__ = tclass({
            inherits : __Asset__,
            define : function(this_, args, this_class) { 
                var impl = this_.native;
                
                this_.interface({
                    byteCount : {
                        get : function()   {return topaz_data__get_byte_count(impl);} 
                    },
                    
                    string : {
                        get : function()   {return topaz_data__get_as_string(impl);} 
                    },
                    
                    bytes : {
                        get : function() { 
                            var bytes = [];
                            var len = topaz_data__get_byte_count(impl);
                            for(var i = 0; i < len; ++i) {
                                bytes.push(topaz_data__get_nth_byte(impl, i));
                            }
                            return bytes;
                        },

                        set : function(v){ 
                            topaz_data__set(impl, v);
                        }
                    }
               }); 
            }
        });
        
        
        var __Sound__ = tclass({
            inherits : __Asset__,
            define : function(this_, args, this_class){ 
                var impl = this_.native;            
                
                this_.interface({
                    sampleCount : {
                        get : function() {return topaz_sound__get_sample_count(impl);} 
                    },
                    
                    samples : {
                        get : function()  { 
                            var out = [];
                            var len = this_.sampleCount;
                            for(var i = 0; i < len; ++i) {
                                out.push(topaz_sound__get_nth_sample_left(impl, i));
                                out.push(topaz_sound__get_nth_sample_right(impl, i));
                            }
                            return out;
                        },
                        
                        
                        set : function(v) { 
                            topaz_sound__set_samples(impl, v);
                        }
                    }
                });
            }
        });
        
        
        
        var __Resources__ = tclass({
            define : function(this_, args, this_class){ 
                var swtch = {};
                swtch[TOPAZ.ASSET.TYPE.IMAGE] = function(i){return __Topaz__.Image.new({native:i})} 
                swtch[TOPAZ.ASSET.TYPE.DATA] = function(i){return __Topaz__.Data.new({native:i})} 
                swtch[TOPAZ.ASSET.TYPE.SOUND] = function(i){return __Topaz__.Sound.new({native:i})} 
                swtch[TOPAZ.ASSET.TYPE.MATERIAL] = function(i){return __Topaz__.Material.new({native:i})} 
                swtch[TOPAZ.ASSET.TYPE.MESH] = function(i){return __Topaz__.Mesh.new({native:i})} 

                var _rawAssetToInstance = function(impl) { 
                    if (impl == undefined) { return undefined; }
                    var fn = swtch[topaz_asset__get_type(impl)];
                    if (fn == undefined) { return __Topaz__.Asset.new({native:impl}); }
                    return fn(impl);
                }
            
            
                this_.interface({
                    fetchAsset : function(type, name){ 
                        return _rawAssetToInstance(topaz_resources__fetch_asset(type, name));
                    },

                    createAsset : function(type) { 
                        return _rawAssetToInstance(topaz_resources__create_asset(type));
                    },

                    loadAsset : function(ext, path, name) { 
                        return _rawAssetToInstance(topaz_resources__load_asset(ext, path, name));
                    },

                    loadAssetData : function(ext, data, name) { 
                        return _rawAssetToInstance(topaz_resources__load_asset_data(ext, data, name));
                    },
                    loadAssetBase64 : function(ext, b64, name) { 
                        return _rawAssetToInstance(topaz_resources__load_asset_base64(ext, b64, name));
                    },


                    writeAsset : function(asset, ext, path) { 
                        return topaz_resources__write_asset(asset.native, ext, path);
                    },

                    removeAsset : function(name) { 
                        topaz_resources__remove_asset(name);
                    },

                    

                    isExtensionSupported : function(ext) { 
                        return topaz_resources__is_extension_supported(ext);
                    },
                    
                    path : {
                        get : function() { 
                            return topaz_resources__get_path();                        
                        },
                        
                        set : function(v) { 
                            repoaz_resources__set_path(v);             
                        }
                    }
                });
            }
        }).new();
        
        var __FontManager__ = tclass({
            define : function(this_, args, this_class) { 
                this_.interface({
                    registerFont : function(n){ 
                        topaz_font_manager__register_font(n);
                    },

                    preloadGlyphs : function(a, c, b){ 
                        topaz_font_manager__preload_glyphs(a, c, b);
                    },

                    unregisterFont : function(n){ 
                        topaz_font_manager__unregister_font(n);
                    },            
                });
            }
        }).new();



                

        var __Entity__ = tclass({
            inherits : __Native__,
            define : function(this_, args, this_class)    { 
                this_.bindNative({
                    instance : args,
                    nativeCreate : topaz_entity__create
                });
                
                this_.interface({
                    isValid : {
                        get : function(){ 
                            return topaz_entity__is_valid(this_.native);
                        }
                    },
                    
                    remove : function(){ 
                        topaz_entity__remove(this_.native);
                    },
                    
                    childCount : {
                        get : function(){ 
                            return topaz_entity__get_child_count(this_.native);
                        }
                    },
                    
                    nthChild : function(n){ 
                        var f = topaz_entity__get_nth_child(this_.native, n);
                        if (f.__ctx != undefined) { return f.__ctx };
                        return this_class.new({native:f});                
                    },
                    
                    
                    children : {
                        get : function(){ 
                            var children = [];
                            var len = topaz_entity__get_child_count(this_.native);
                            for(var i = 0; i < len; ++i) { 
                                children.push(topaz_entity__get_nth_child(this_.native, i).__ctx);
                            }
                            return children;
                        },
                        
                        set : function(c){ 
                            while(topaz_entity__get_child_count(this_.native) > 0) {
                                topaz_entity__detach(topaz_entity__get_nth_child(this_.native, 0));
                            }
                                
                            for(var i = 0; i < c.length; ++i) {
                                topaz_entity__attach(this_.native, c[i].native);
                            }
                        }
                    },
                    
                    
                    //TODO: the rest, also onStep, onDraw, etc <3
                    step : function() { 
                        topaz_entity__step(this_.native);
                    },
                    
                    draw : function() { 
                        topaz_entity__step(this_.native);
                    },
    
                    attach : function(other) { 
                        topaz_entity__attach(this_.native, other.native);
                    },
    
                    detach : function() { 
                        topaz_entity__detach(this_.native);
                    },
                    
                    parent : {
                        get : function() { 
                            var f = topaz_entity__get_parent(this_.native);
                            if (f.__ctx != undefined) { return f.__ctx; }
                            return this_class.new({native:f});
                        },
                        
                        set : function(v){ 
                            topaz_entity__attach(v.native, this_.native);
                        }
                    },
                    
                    query : function(name) { 
                        var f = topaz_entity__query(this_.native, name);
                        if (f.__ctx != undefined) { return f.ctx };
                        return this_class.new({native:f});
                    },
                    
                    
                    search : function(name){ 
                        var f = topaz_entity__search(this_.native, name);
                        if (f.__ctx != undefined) { return f.ctx };
                        return this_class.new({native:f});                
                    },
                    
                    priority : {
                        get : function() {return topaz_entity__get_priority(this_.native);}, 
                        set : function(v){topaz_entity__set_priority(this_.native, v);}
                    },
                    
        
                    setPriorityLast : function(){ 
                        topaz_entity__set_priority_last(this_.native);
                    },
        
                    setPriorityFirst : function(){ 
                        topaz_entity__set_priority_first(this_.native);
                    },
    
                    rotation : {
                        get : function()  {return Vector.fromnative(topaz_entity__get_rotation(this_.native));}, 
                        set : function(v) {Vector.statepush(v);topaz_entity__set_rotation(this_.native, v.native);} 
                    },

    
                    position : {
                        get : function()   {return Vector.fromnative(topaz_entity__get_position(this_.native)); }, 
                        set : function(v)  {Vector.statepush(v);topaz_entity__set_position(this_.native, v.native);} 
                    },
                    
                    scale : {
                        get : function()  {return Vector.fromnative(topaz_entity__get_scale(this_.native));}, 
                        set : function(v) {Vector.statepush(v);topaz_entity__set_scale(this_.native, v.native);}
                    },
                    
                    globalPosition : {
                        get : function() {return Vector.fromnative(topaz_entity__get_global_position(this_.native));} 
                    },
                    
                    isStepping : {
                        get : function() {return topaz_entity__is_stepping(this_.native);}
                    },
                    
                    isDrawing : {
                        get : function() {return topaz_entity__is_drawing(this_.native) } 
                    },
                    
                    stepping : {
                        get : function()  {return topaz_entity__get_stepping(this_.native);},
                        set : function(v) {return topaz_entity__set_stepping(this_.native, v);} 
                    },
                    
                    drawing : {
                        get : function()  {return topaz_entity__get_drawing(this_.native);}, 
                        set : function(v) {return topaz_entity__set_drawing(this_.native, v);} 
                    },
                    
                    name : {
                        get : function()  {return topaz_entity__get_name(this_.native);},
                        set : function(v) {return topaz_entity__set_name(this_.native, v);} 
                    },
    
                    addComponent : function(c){ 
                        topaz_entity__add_component(this_.native, c.native);
                    },
                
                    addComponentAfter : function(c){ 
                        topaz_entity__add_component_after(this_.native, c.native);
                    },
                        
                    components : {
                        get : function(){ 
                            var len = topaz_entity__get_component_count(this_.native);
                            var out = [];
                            for(var i = 0; i < len; ++i) {
                                var f = topaz_entity__get_nth_component(this_.native, i);
                                if (f.__ctx != undefined) {
                                    out.push(f.__ctx);
                                } else {
                                    out.push(__Topaz__.Component.new({native:f}));
                                }
                            }
                            return out;
                        },
                        set : function(c){ 
                            
                            while(topaz_entity__get_component_count(this_.native) > 0) {
                                topaz_entity__remove_component(this_.native, topaz_entity__get_nth_component(this_.native, 0));
                            }
    
                            for(var i = 0; i < c.length; ++i) {
                                topaz_entity__add_component(this_.native, c[i].native);
                            }
                        }
                    },
    
    
                    queryComponent : function(tag) { 
                        var f = topaz_entity__query_component(this_.native, tag);
                        if (f.__ctx != undefined) { return f.__ctx; }
                        return __Topaz__.Component({native:f});
                    },
                
                    removeComponent : function(c){ 
                        return topaz_entity__remove_component(this_.native, c.native);
                    },
                    
                    onStep : {set : function(v){ 
                            topaz_entity__set_on_step(this_.native, function(){ v() });
    
                    }},
    
                    onDraw : {set : function(v){ 
                            topaz_entity__set_on_draw(this_.native, function(){v()});
                    }},
    
                    onPreStep : {set : function(v){ 
                            topaz_entity__set_on_pre_step(this_.native, function(){v()});
                    }},
    
                    onPreDraw : {set : function(v){ 
                            topaz_entity__set_on_pre_draw(this_.native, function(){v()});
                    }},
                    
                    onAttach : {set : function(v){ 
                            topaz_entity__set_on_attach(this_.native, function(){v()});
                    }},
    
                    onDetach : {set : function(v){ 
                            topaz_entity__set_on_detach(this_.native, function(){v()});
                    }},
    
                    onRemove : {set : function(v){ 
                            topaz_entity__set_on_remove(this_.native, function(){v()});
                    }}
    
                    
    
    
                
                });        
            }    
        });


        var __Component__ = tclass({
            inherits : __Native__,
            define : function(this_, args, this_class){ 
                // some classes will inherit and overwrite the native instance, so a 
                // var impl doesnt make sense.
                this_.bindNative({
                    instance : args,
                    nativeCreate : topaz_component__create
                }, [args.tag]);
    
                this_.interface({
                    destroy : function(){ 
                        topaz_component__destroy(this_.native);
                    },
                    
                    step : function(){ 
                        topaz_component__run(this_.native);
                    },
                    
                    draw : function(){ 
                        topaz_component__draw(this_.native);
                    },
                    
                    stepping : {
                        get : function()  {return topaz_component__get_stepping(this_.native);},
                        set : function(v) {topaz_component__set_stepping(this_.native, v);} 
                    },
                    
                    drawing : {
                        get : function()  {return topaz_component__get_drawing(this_.native);},
                        set : function(v) {topaz_component__set_drawing(this_.native, v);} 
                    },
                    
                    tag : {
                        get : function()  {return topaz_component__get_tag(this_.native);}, 
                        set : function(v) {topaz_component__set_tag(this_.native, v);} 
                    },
                    
                    host : {
                        get : function(){ 
                            var f = topaz_component__get_host(this_.native);
                            if (f.__ctx != undefined) { return f.__ctx };
                            return __Topaz__.Entity.new({native:f});
                        }
                    },
                    
                    emitEvent : function(eventName, ent){ 
                        if (ent == undefined) {
                            return topaz_component__emit_event_anonymous(this_.native, eventName);
                        } else {
                            return topaz_component__emit_event(this_.native, eventName, ent.native);
                        }
                    },
                    
                    canHandleEvent : function(name){ 
                        return topaz_component__can_handle_event(this_.native, name);
                    },
                    
                    installEvent : function(event, callback){ 
                        var cb;
                        if (callback != undefined) {
                            cb = function(component, ent){callback(component.__ctx, ent.__ctx)} 
                        } else { 
                            cb = function(){} 
                        }
                            
                        topaz_component__install_event(this_.native, event, cb);
                    },
                    
                    uninstallEvent : function(event){ 
                        topaz_component__uninstall_event(this_.native, event);
                    },
                    
                    installHook : function(event, callback){ 
                        return topaz_component__install_hook(this_.native, event, function(component, ent){ 
                            callback(component.__ctx, ent.__ctx);
                        });
                    },
                    
                    uninstallHook : function(event, id){ 
                        topaz_component__uninstall_hook(this_.native, event, id);
                    },
                    
                    installHandler : function(event, callback){ 
                        return topaz_component__install_handler(this_.native, event, function(component, ent){ 
                            callback(component.__ctx, ent.__ctx);
                        });
                    },
                    
                    uninstallHandler : function(event, id){ 
                        topaz_component__uninstall_handler(this_.native, event, id);
                    },
                    
                    onStep : {set : function(v){ 
                            topaz_component__set_on_step(this_.native, function(){ v() });
    
                    }},
    
                    onDraw : {set : function(v){ 
                            topaz_component__set_on_draw(this_.native, function(){v()});
                    }},
                        
                    onAttach : {set : function(v){ 
                            topaz_component__set_on_attach(this_.native, function(){v()});
                    }},
    
                    onDetach : {set : function(v){ 
                            topaz_component__set_on_detach(this_.native, function(){v()});
                    }},
    
                    onDestroy : {set : function(v){ 
                            topaz_component__set_on_destroy(this_.native, function(){v()});
                    }}

                });
            }
        });
        

        
        
        var __Text2D__ = tclass({
            inherits :__Component__,
            define : function(this_, args, this_class){ 
                // whoops, the component constructor already made a generic component native.
                // destroy it and make a real one
                this_.native.__ctx.destroy();
                
                var impl = this_.bindNative({
                    instance : args,
                    nativeCreate : topaz_text2d__create
                });
                
                var fontState;
                var sizeState;
                
                this_.interface({
                    text : {
                        get : function() {return topaz_text2d__get_text(impl);},
                        set : function(v){ topaz_text2d__set_text(impl, v);} 
                    },
                    
                    font : {
                        get : function()  {return this_.fontState;},
                        set : function(v) { 
                            fontState = v;
                            topaz_text2d__set_font(impl, fontState, sizeState);
                        }
                    },
                    
                    size :  {
                        get : function()  {return this_.sizeState;},
                        set : function(v) { 
                            sizeState = v;
                            topaz_text2d__set_font(impl, fontState, sizeState);
                        }
                    },
                    
                    setColor : function(c) { 
                        Color.statepush(c);
                        topaz_text2d__set_color(impl, c.native);
                    },
                    
                    setColorSection : function(from, to, c) { 
                        Color.statepush(c);
                        topaz_text2d__set_color_section(impl, from, to, c.native);
                    },
                    
                    extentWidth : {
                        get : function() {return topaz_text2d__get_extent_width(impl);} 
                    },
                    
                    extentHeight : {
                        get : function() {return topaz_text2d__get_extent_height(impl);} 
                    },
                    
                    getCharX : function(i) { 
                        return topaz_text2d__get_char_x(impl, i);
                    },
                    
                    getCharY : function(i) { 
                        return topaz_text2d__get_char_y(impl, i);
                    },
                    
                    position : {
                        get : function() {return Vector.fromnative(topaz_text2d__get_position(impl));}, 
                        set : function(v){Vector.statepush(v);topaz_text2d__set_position(impl, v.native);} 
                    },
                    
                    rotation : {
                        get : function() {return Vector.fromnative(topaz_text2d__get_rotation(impl));},
                        set : function(v){Vector.statepush(v);topaz_text2d__set_rotation(impl, v.native);} 
                    },
                    
                    scale : {
                        get : function() {return Vector.fromnative(topaz_text2d__get_scale(impl));}, 
                        set : function(v){Vector.statepush(v);topaz_text2d__set_scale(impl, v.native);} 
                    },
                    
                    setAttribute : function(a, b){ 
                        topaz_text2d__set_attribute(impl, a, b);
                    },
                    
                    getAttribute : function(a){ 
                        return topaz_text2d__get_attribute(impl, a);
                    }
                });

            }
        });
        
        var __Scheduler__ = tclass({
            inherits :__Component__,
            define : function(this_, args, this_class){ 
                // whoops, the component constructor already made a generic component native.
                // destroy it and make a real one
                this_.native.__ctx.destroy();
                
                var impl = this_.bindNative({
                    instance : args,
                    nativeCreate : topaz_scheduler__create
                }, [args.type]);
                
                this_.interface({
                    startTask : function(args){ 
                        if (args["taskName"] != undefined) {
                            topaz_scheduler__start_task(
                                impl,
                                args.taskName,
                                args.interval,
                                args.intervalDelay,
                                args.callback
                            );
                            return args.taskName;
                        } else {
                            return topaz_scheduler__start_task_simple(
                                impl,
                                args.interval,
                                args.callback
                            );
                        }
                    },
                    
                    endTask : function(name){ 
                        topaz_scheduler__end_task(impl, name);
                    },
                
                    pause : function(name){ 
                        topaz_scheduler__pause(impl, name);
                    },
                    
                    resume : function(name) { 
                        topaz_scheduler__resume(impl, name);
                    },
                    
                    getTaskIntervalRemaining : function(name){ 
                        return topaz_scheduler__get_task_interval_remaining(impl, name);
                    },
                    
                    tasks : {
                        get : function(){ 
                            var out = [];
                            var len = topaz_scheduler__get_task_count(impl);
                            for(var i = 0; i < len; ++i) {
                                out.push(topaz_schedluer__get_task(impl, i));
                            }
                            return out;
                        }
                    },
                    
                    
                });
            } 
        });
        
        var __StateControl__ = tclass({
            inherits :__Component__,
            define : function(this_, args, this_class){ 
                // whoops, the component constructor already made a generic component native.
                // destroy it and make a real one
                this_.native.__ctx.destroy();
                
                var impl = this_.bindNative({
                    instance : args,
                    nativeCreate : topaz_state_control__create
                });
                
                this_.interface({
                    add : function(name, state){ 
                        topaz_state_control__add(impl, state.state, state.onStep, state.onDraw, state.onInit);
                    },
                    
                    remove : function(name){ 
                        topaz_state_control__remove(impl, name);
                    },
                    
                    execute : function(name){ 
                        topaz_state_control__execute(impl, name);
                    },
                    
                    halt : function(){ 
                        topaz_state_control__halt(impl)
                    },
                    
                    isHalted : {
                        get : function(){ 
                            return topaz_state_control__is_halted(impl);
                        },
                    },
                    
                    state : {
                        get : function(){ 
                            return topaz_state_control__get_current(impl);
                        },
                    }                    
                    
                           
                });
            }
        });
        
        var __Object2D__ = tclass({
            inherits :__Component__,
            define : function(this_, args, this_class){ 
                // whoops, the component constructor already made a generic component native.
                // destroy it and make a real one
                this_.native.__ctx.destroy();
                
                var impl = this_.bindNative({
                    instance : args,
                    nativeCreate : topaz_oject2d__create
                });
                
                this_class.setGroupInteraction = function(a, b, c){ 
                    topaz_object2d__set_group_interaction(a, b, c);
                }
                
                var collider_;
                
                this_.interface({
                    addVelocity : function(a, b) { 
                        topaz_object2d__add_velocity(impl, a, b);
                    },

                    addVelocityTowards : function(a, b, c) { 
                        topaz_object2d__add_velocity_towards(impl, a, b, c);
                    },

                    setVelocity : function(a, b) { 
                        topaz_object2d__set_velocity(impl, a, b);
                    },

                    setVelocityTowards : function(a, b, c) { 
                        topaz_object2d__set_velocity_towards(impl, a, b, c);
                    },




                    frictionX : {
                        get : function()  {return topaz_object2d__get_friction_x(impl);}, 
                        set : function(v) {topaz_object2d__set_friction_x(impl, v);} 
                    },

                    frictionY : {
                        get : function()  {return topaz_object2d__get_friction_y(impl);},
                        set : function(v) {topaz_object2d__set_friction_y(impl, v);} 
                    },

                    direction : {
                        get : function()  {return topaz_object2d__get_direction(impl);},
                        set : function(v){ 
                            topaz_object2d__set_velocity(
                                impl,
                                topaz_object2d__get_speed(impl),
                                topaz_object2d__get_direction(impl)
                            );
                        }
                    },

                    halt : function() {topaz_object2d__halt(impl);},
                    resetMotion : function() {topaz_object2d__reset_motion(impl);},

                    velocityX : {
                        get : function()  {return topaz_object2d__get_velocity_x(impl);},
                        set : function(v) {topaz_object2d__set_velocity_x(impl, v);} 
                    },

                    velocityY : {
                        get : function()  {return topaz_object2d__get_velocity_y(impl);},
                        set : function(v) {topaz_object2d__set_velocity_y(impl, v);} 
                    },

                    speed : {
                        get : function()  {return topaz_object2d__get_speed(impl);},
                        set : function(v) {topaz_object2d__set_speed(impl, v);} 
                    },

                    nextPosition : {
                        get : function()  {return topaz_object2d__get_next_position(impl);} 
                    },

                    group : {
                        get : function()  {return topaz_object2d__get_group(impl);}, 
                        set : function(v) {topaz_object2d__set_group(impl, v);} 
                    },

                    collider : {
                        get : function()  {return _collider; },
                        set : function(v) {_collider=v; topaz_object2d__set_collider(impl, v);} 
                    },

                    setColliderRadial : function(a, b) { 
                        topaz_object2d__set_collider_radial(impl, a, b);
                        _collider = [];
                        var len = topaz_object2d__get_collider_len(impl);
                        for(var i = 0; i < len; ++i) {
                            var pt = topaz_object2d__get_collider_point(impl, i);
                            _collider.push(pt.x);
                            _collider.push(pt.y);
                        }
                    },

                    lastCollided : {
                        get : function()  {return __Topaz__.Entity.new({native:topaz_object2d__get_last_collided(impl)});} 
                    }
                });                
            }
        });
        
        var __Shape2D__ = tclass({
            inherits :__Component__,
            define : function(this_, args, this_class){ 
                // whoops, the component constructor already made a generic component native.
                // destroy it and make a real one
                this_.native.__ctx.destroy();
                
                var impl = this_.bindNative({
                    instance : args,
                    nativeCreate : topaz_shape2d__create
                });
                
                var _lines;
                var _tris;
                
                this_.interface({
                    color : {
                        get : function() {return Color.fromnative(topaz_shape2d__get_color(impl));}, 
                        set : function(v){Color.statepush(v);topaz_shape2d__set_color(impl, v.native);} 
                    },


                    animSpeed : {
                        get : function() {return topaz_shape2d__get_anim_speed(impl);},
                        set : function(v){topaz_shape2d__set_anim_speed(impl, v);} 
                    },

                    center : {
                        get : function() {return Vector.fromnative(topaz_shape2d__get_center(impl));},
                        set : function(v){Vector.statepush(v);topaz_shape2d__set_center(impl, v.native);} 
                    },

                    position : {
                        get : function() {return Vector.fromnative(topaz_shape2d__get_position(impl));},
                        set : function(v){Vector.statepush(v);topaz_shape2d__set_position(impl, v.native);} 
                    },

                    rotation : {
                        get : function() {return Vector.fromnative(topaz_shape2d__get_rotation(impl));},
                        set : function(v){Vector.statepush(v);topaz_shape2d__set_rotation(impl, v.native);} 
                    },

                    scale : {
                        get : function() {return Vector.fromnative(topaz_shape2d__get_scale(impl));}, 
                        set : function(v){Vector.statepush(v);topaz_shape2d__set_scale(impl, v.native);} 
                    },

                    lines : {
                        get : function() {return _lines;},
                        set : function(v){topaz_shape2d__form_lines(impl, v); _lines = v;} 
                    },

                    triangles : {
                        get : function() {return _tris;}, 
                        set : function(v){topaz_shape2d__form_triangles(impl, v); _tris = v;} 
                    },

                    setAttribute : function(a, b) { 
                        topaz_shape2d__set_attribute(impl, a, b);
                    },

                    getAttribute : function(a) { 
                        return topaz_shape2d__set_attribute(impl, a);
                    },

                    formRectangle : function(a, b){ 
                        topaz_shape2d__form_rectangle(impl, a, b);
                    },

                    formImage : function(a) { 
                        topaz_shape2d__form_image(impl, a.native);
                    },

                    formImageFrame : function(a, b) { 
                        topaz_shape2d__form_image_frame(impl, a.native, b);
                    },

                    formImageScaled : function(a, b, c) { 
                        topaz_shape2d__form_image(impl, a.native, b, c);
                    },

                    formRadial : function(a, b) { 
                        topaz_shape2d__form_radial(impl, a, b);
                    },








                });
            }
        });
        
        
        var __Shape3D__ = tclass({
            inherits :__Component__,
            define : function(this_, args, this_class){ 
                // whoops, the component constructor already made a generic component native.
                // destroy it and make a real one
                this_.native.__ctx.destroy();
                
                var impl = this_.bindNative({
                    instance : args,
                    nativeCreate : topaz_shape3d__create
                });
                
                
                this_.interface({
                    position : {
                        get : function() {return Vector.fromnative(topaz_shape3d__get_position(impl));},
                        set : function(v){Vector.statepush(v);topaz_shape3d__set_position(impl, v.native);} 
                    },

                    rotation : {
                        get : function() {return Vector.fromnative(topaz_shape3d__get_rotation(impl));}, 
                        set : function(v){Vector.statepush(v);topaz_shape3d__set_rotation(impl, v.native);} 
                    },

                    scale : {
                        get : function() {return Vector.fromnative(topaz_shape3d__get_scale(impl));},
                        set : function(v){Vector.statepush(v);topaz_shape3d__set_scale(impl, v.native);} 
                    },

                    setAttribute : function(a, b){ 
                        topaz_shape3d__set_attribute(impl, a, b);
                    },

                    getAttribute : function(a){ 
                        return topaz_shape3d__set_attribute(impl, a);
                    },

                    setTexture : function(slot, t){ 
                        topaz_shape3d__set_texture(impl, slot, t.native);
                    },

                    mesh : {
                        set : function(v){ 
                            topaz_shape3d__set_mesh(impl, v.native);            
                        }
                    },

                    sampleFramebuffer : {
                        set : function(v) { 
                            topaz_shape3d__set_sample_framebuffer(impl, v.native);
                        }
                    },

                    material : {
                        set : function(v){ 
                            topaz_shape3d__set_material(impl, v.native);
                        }
                    }
                });  
                
            }
        });
        
        var __Automation__ = tclass({
            inherits :__Component__,
            define : function(this_, args, this_class){ 
                // whoops, the component constructor already made a generic component native.
                // destroy it and make a real one
                this_.native.__ctx.destroy();
                
                var impl = this_.bindNative({
                    instance : args,
                    nativeCreate : topaz_automation__create
                });
                
                this_.interface({
                    addKeyframe : function(value, fn, offset){ 
                        topaz_automation__add_keyframe(impl, value, fn, offset);
                    },

                    addVectorKeyframe : function(value, fn, offset){ 
                        topaz_automation__add_vector_keyframe(impl, value.native, fn, offset);
                    },

                    clear : function(){ 
                        topaz_automation__clear(impl);
                    },

                    blend : function(other){ 
                        topaz_automation__blend(impl, other.native);
                    },

                    smooth : function(){ 
                        topaz_automation__smooth(impl);
                    },

                    addFromString : function(str){ 
                        topaz_automation__add_from_string(impl, str);
                    },

                    skipTo : function(n){ 
                        topaz_automation__skip_to(impl, n);
                    },

                    resume : function() { 
                        topaz_automation__resume(impl);
                    },

                    pause : function() { 
                        topaz_automation__pause(impl);
                    },

                    vectorAt : function(at) { 
                        return Vector.fromnative(topaz_automation__vector_at(impl, at));
                    },

                    at : function(a) { 
                        return topaz_automation__at(impl, at);
                    },

                    vector : {
                        get : function() {return Vector.fromnative(topaz_automation__current_vector(impl))},
                    },

                    value : {
                        get : function() {return topaz_automation__current(impl);},
                    },

                    string : {
                        get : function() {return topaz_automation__to_string(impl);},
                        set : function(v) {return topaz_automation__set_from_string(impl, v);},
                    },

                    length : {
                        get : function() {return topaz_automation__get_length(impl);},
                    },

                    durationFrames : {
                        set : function(v){ topaz_automation__set_duration_frames(impl, v);} 
                    },

                    durationSeconds : {
                        set : function(v){ topaz_automation__set_duration_seconds(impl, v);} 
                    },

                    duration : {
                        get : function() {return topaz_automation__get_duration(impl);},
                    },

                    looped : {
                        get : function() {return topaz_automation__get_looped(impl);},
                        set : function(v) {return topaz_automation__set_looped(impl, v);},
                    },

                    speed : {
                        get : function() {return topaz_automation__get_speed(impl);},
                        set : function(v) {return topaz_automation__set_speed(impl, v);},
                    }
                });
            }
        });
        
        var __Particle__ = tclass({
            inherits : __Native__,
            define : function(this_, args, this_class){ 

                var impl = this_.bindNative({
                    nativeCreate : topaz_particle__create
                });
                
                this_.interface({
                    setAttribute : function(param, val) { 
                        topaz_particle__set_attribute(impl, param, val);
                    },

                    setNoiseMin : function(p, val) { 
                        topaz_particle__set_noise_min(impl, p, val);
                    },

                    setNoiseMax : function(p, val) { 
                        topaz_particle__set_noise_max(impl, p, val);
                    },

                    setFunction : function(p, val) { 
                        topaz_particle__set_function(impl, p, val);
                    },

                    string : {get : function() {return topaz_particle__to_string(impl)}, set : function(v){topaz_particle__set_from_string(impl, v);}}, 
                    image :  {set : function(v) {return topaz_particle__set_image(impl, v)}}
                });
            }        
        });
        
        var __ParticleEmitter2D__  = tclass({
            inherits :__Entity__,
            define : function(this_, args, this_class) { 
                // whoops, the component constructor already made a generic component native.
                // destroy it and make a real one
                this_.native.__ctx.remove();
                
                var impl = this_.bindNative({
                    nativeCreate : topaz_particle_emitter_2d__create
                });
                this_.interface({

                    particle : {set : function(v){topaz_particle_emitter_2d__set_particle(impl, v.native);}}, 
                    independent : {set : function(v){topaz_particle_emitter_2d__set_independent(impl, v);}}, 

                    emit : function(c){ 
                        topaz_particle_emitter_2d__emit(impl, c);
                    },
                });
            }
        });

        this_.interface({
            run   : topaz__run,
            pause   : topaz__pause,
            pauseNow : topaz__pause_now,
            resume : topaz__resume,
            iterate : topaz__iterate,
            step : topaz__step,
            draw : topaz__draw,
            attachPreManager : function(v){topaz__attach_pre_manager(v.native);},        
            attachPreManagerUnpausable : function(v){topaz__attach_pre_manager_unpausable(v.native);},        
            attachPostManager : function(v){topaz__attach_post_manager(v.native);},        
            attachPostManagerUnpausable : function(v){topaz__attach_post_manager_unpausable(v.native);},        
            quit : topaz__quit,
            wait : topaz__wait,
            import : topaz_script__import,
            log : topaz__log,
            toBase64 : topaz__to_base64,
            fromBase64 : function(f) { 
                return __Topaz__.Data.new({native:topaz__from_base64(f)});
            },
            
            root : {
                set : function(e){ 
                    return topaz__set_root(e.native)
                },

                get : function(e){ 
                    
                }
            },
            
            isPaused : {
                get : function(){ 
                    return topaz__is_paused();
                }            
            },
            
            time : {
                get : function(){ 
                    return topaz__get_time();
                }
            },
            
            versionMicro : {
                get : function() { 
                    return topaz__get_version_micro();                
                }
            },
            versionMajor : {
                get : function() { 
                    return topaz__get_version_major();                
                }
            },
            versionMinor : {
                get : function() { 
                    return topaz__get_version_minor();                
                }
            },
            

            
            
            
            
            
            objectToString : function(obj, levelSrc) {
                var checked = [];
                var levelG = levelSrc ? levelSrc : 10;
                var helper = function(obj, level) {
                    if (obj === undefined) return 'undefined';
                    if (obj === null) return 'null';
                    if (!(typeof obj === 'object')) return '\''+obj + '\'';
                    if (checked.indexOf(obj) != -1) return '[Already Printed]'
                    checked.push(obj);
                    var strOut = '{\n';
                    var keys = Object.keys(obj);
                    var levelInv = levelG - level;
                    for(var i = 0; i < keys.length; ++i) {
                        var subStr = levelInv ? helper(obj[keys[i]], level+1) : obj[keys[i]];
                        for(var n = 0; n < level; ++n) strOut += '  ';
                        strOut += '  \'' + keys[i] + '\' : ' + subStr + ',\n'; 
                    }
                    for(var n = 0; n < level; ++n) strOut += '  ';
                    strOut += '}';
                    return strOut;
                }
                
                return helper(obj, 0) + '\n';
            },
    
            Filesystem  : {get : function(){return __Filesystem__ }},
            RNG         : {get : function(){return __RNG__ }},
            ViewManager : {get : function(){return __ViewManager__ }},
            Display     : {get : function(){return __Display__ }},
            Framebuffer : {get : function(){return __Framebuffer__ }},
            Asset       : {get : function(){return __Asset__ }},
            Mesh        : {get : function(){return __Mesh__ }},
            Input       : {get : function(){return __Input__ }},
            Audio       : {get : function(){return __Audio__ }},
            Material    : {get : function(){return __Material__ }},
            Entity      : {get : function(){return __Entity__ }},
            Component   : {get : function(){return __Component__ }},
            Resources   : {get : function(){return __Resources__ }},
            FontManager : {get : function(){return __FontManager__ }},
            Image       : {get : function(){return __Image__ }},
            Data        : {get : function(){return __Data__ }},
            Sound       : {get : function(){return __Sound__ }},
            Text2D      : {get : function(){return __Text2D__ }},
            Scheduler   : {get : function(){return __Scheduler__ }},
            StateControl: {get : function(){return __StateControl__ }},
            Object2D    : {get : function(){return __Object2D__ }},
            Shape2D     : {get : function(){return __Shape2D__ }},
            Shape3D     : {get : function(){return __Shape3D__ }},
            Automation  : {get : function(){return __Automation__ }},
            Particle    : {get : function(){return __Particle__ }},
            ParticleEmitter2D : {get : function(){return __ParticleEmitter2D__ }}
       });
    }
}).new();

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
var Package = (function(){
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
        // and version at or greater, this_ function will return true.
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
        
        
        // Convenience function. Given a path to a 
        // directory:
        // - read()s all packages within the directory. Looks for the suffix ".package.json"
        // - calls resolveAll()
        importPath : function(path) {
            const obj = Topaz.Filesystem.getPathFromString(Topaz.Filesystem.getPathFromString(Topaz.Resources.path), path);
            const children = obj.children;
            for(var i = 0; i < children.length; ++i) {
                const subpath = children[i].string;
                if (subpath.indexOf('.package.json') != -1) {
                    Topaz.Package.read(subpath);
                }
            }
            Topaz.Package.resolveAll();
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
                        Topaz.Filesystem.getPath(TOPAZ.FILESYSTEM.DEFAULT_NODE.RESOURCES),
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
        
                            Topaz.log('++.', false);
                            Topaz.log(' ', false);
        
        
                            asset.assetBase64 = Topaz.toBase64(bytes);                        
                            outjson.assets.push(asset);
                            Topaz.log('OK (' + Math.ceil(byteCount/1024) + '.' + Math.floor(((byteCount%1024) / 1024.0)*100) + 'KB)');
                        }
                    }
        
        
                    var output = JSON.stringify(outjson);
                    var outputAsset = Topaz.Resources.fetchAsset(TOPAZ.ASSET.TYPE.DATA, '__ASSET__39245s$');
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









