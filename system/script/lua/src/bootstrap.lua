TOPAZ = {
    INPUT = {
        NOT_AN_INPUT = 0,
        KEY_0= 1, --< 0
        KEY_1= 2, --< 1
        KEY_2= 3, --< 2
        KEY_3= 4, --< 3
        KEY_4= 5, --< 4
        KEY_5= 6, --< 5
        KEY_6= 7, --< 6
        KEY_7= 8, --< 7
        KEY_8= 9, --< 8
        KEY_9= 10, --< 9
        KEY_a= 11, --< a
        KEY_b= 12, --< b
        KEY_c= 13, --< c
        KEY_d= 14, --< d
        KEY_e= 15, --< e
        KEY_f= 16, --< f
        KEY_g= 17, --< g
        KEY_h= 18, --< h
        KEY_i= 19, --< i
        KEY_j= 20, --< j
        KEY_k= 21, --< k
        KEY_l= 22, --< l
        KEY_m= 23, --< m
        KEY_n= 24, --< n
        KEY_o= 25, --< o
        KEY_p= 26, --< p
        KEY_q= 27, --< q
        KEY_r= 28, --< r
        KEY_s= 29, --< s
        KEY_t= 30, --< t
        KEY_u= 31, --< u
        KEY_v= 32, --< v
        KEY_w= 33, --< w
        KEY_x= 34, --< x
        KEY_y= 35, --< y
        KEY_z= 36, --< z
        KEY_lshift= 37, --< Left shift key
        KEY_rshift= 38, --< Right shift key
        KEY_lctrl= 39,  --< Left control key
        KEY_rctrl= 40,  --< Right control key
        KEY_lalt= 41,   --< Left alt key
        KEY_ralt= 42,   --< Right alt key
        KEY_tab= 43,    --< Tab
        KEY_F1= 44,     --< F1
        KEY_F2= 45,     --< F2
        KEY_F3= 46,     --< F3
        KEY_F4= 47,     --< F4
        KEY_F5= 48,     --< F5
        KEY_F6= 49,     --< F6
        KEY_F7= 50,     --< F7
        KEY_F8= 51,     --< F8
        KEY_F9= 52,     --< F9
        KEY_F10= 53,    --< F10
        KEY_F11= 54,    --< F11
        KEY_F12= 55,    --< F12
        KEY_up= 100,     --< Up arrow
        KEY_down= 101,   --< Down arrow
        KEY_left= 102,   --< Left arrow
        KEY_right= 103,  --< Right arrow
        KEY_minus= 104,  --< -
        KEY_equal= 105,  --< = 
        KEY_backspace= 106,  --< Backspace
        KEY_grave= 107,  --< `
        KEY_esc= 108,    --< Escape
        KEY_home= 109,   --< Home key
        KEY_pageUp= 110, --< Page up key
        KEY_pageDown= 111,  --< Page down key
        KEY_end= 112,    --< End key
        KEY_backslash= 113, --< '\'
        KEY_lbracket= 114, --< [
        KEY_rbracket= 115, --< ]
        KEY_semicolon= 116, --< ;
        KEY_apostrophe= 117, --< '
        KEY_frontslash= 118, --< /
        KEY_enter= 119, --< Enter
        KEY_delete= 120, --< Delete
        KEY_numpad0= 121, --< Numpad 0
        KEY_numpad1= 122, --< Numpad 1
        KEY_numpad2= 123, --< Numpad 2
        KEY_numpad3= 124, --< Numpad 3
        KEY_numpad4= 125, --< Numpad 4
        KEY_numpad5= 126, --< Numpad 5
        KEY_numpad6= 127, --< Numpad 6
        KEY_numpad7= 128, --< Numpad 7
        KEY_numpad8= 129, --< Numpad 8
        KEY_numpad9= 130, --< Numpad 9
        KEY_prtscr= 131, --< Print screen button
        KEY_lsuper= 132, --< Left Super key (Windows key)
        KEY_rsuper= 133, --< Right Super key (Windows key)
        KEY_space= 134,  --< Space
        KEY_insert= 135, --< Insert key
        KEY_comma= 136, --< ,
        KEY_period= 137 , --< .
        KEY_world1= 138, -- I8n key0
        KEY_world2= 139, -- I8n key1
        KEY_world3= 140, -- I8n key2
        KEY_world4= 141, -- I8n key3
        KEY_world5= 142, -- I8n key4
        KEY_world6= 143, -- I8n key5
        KEY_world7= 144, -- I8n key6
        KEY_world8= 145, -- I8n key7
        KEY_world9= 146, -- I8n key8
    
        POINTER_0= 256, --< Left click
        POINTER_1= 257, --< Right click
        POINTER_2= 258, --< Middle click
    
        POINTER_X= 259, --< Horizontal axis. Usually for the X axis of the pointer
        POINTER_Y= 260, --< Horizontal axis. Usually for the X axis of the pointer
        POINTER_Wheel= 261, --< Mouse wheel.
    
        PAD_a= 300,     --< Button 0
        PAD_b= 301,     --< Button 1
        PAD_c= 302,     --< Button 2
        PAD_x= 303,     --< Button 3
        PAD_y= 304,     --< Button 4
        PAD_r= 305,     --< Button 5
        PAD_l= 306,     --< Button 6
        PAD_r2= 307,    --< Button 7
        PAD_l2= 308,    --< Button 8
        PAD_r3= 309,    --< Button 9
        PAD_l3= 310,    --< Button 10
        PAD_start= 311,    --< Button 11
        PAD_select= 312,--< Button 12
        PAD_b13= 313,--< Button 13
        PAD_b14= 314,--< Button 14
        PAD_b15= 315,--< Button 15
        PAD_b16= 316,--< Button 16
        PAD_b17= 317,--< Button 17
        PAD_b18= 318,--< Button 18
        PAD_b19= 319,--< Button 19
        PAD_b20= 320,--< Button 20
        PAD_b21= 321,--< Button 21
        PAD_b22= 322,--< Button 22
        PAD_b23= 323,--< Button 23
        PAD_b24= 324,--< Button 24
        PAD_b25= 325,--< Button 25
        PAD_b26= 326,--< Button 26
        PAD_b27= 327,--< Button 27
        PAD_b28= 328,--< Button 28
        PAD_b29= 329,--< Button 29
        PAD_b30= 330,--< Button 30
        PAD_b31= 331,--< Button 31
        PAD_b32= 332,--< Button 32
    
        PAD_axisX= 400, --< X button
        PAD_axisY= 401, --< Y button
        PAD_axisZ= 402, --< Z button
        PAD_axisX2= 403,--< X2 button 
        PAD_axisY2= 404,--< Y2 button
        PAD_axisZ2= 405,--< Z2 button
        PAD_axisX3= 406,--< X3 button
        PAD_axisY3= 407,--< Y3 button
        PAD_axisZ3= 408,--< Z3 button
        PAD_axisX4= 409,--< X4 button
        PAD_axisY4= 410,--< Y4 button
        PAD_axisZ4= 411,--< Z4 button
        PAD_axisX5= 412,--< X4 button
        PAD_axisY5= 413,--< Y4 button
        PAD_axisZ5= 414,--< Z4 button
        PAD_axisX6= 415,--< X4 button
        PAD_axisY6= 416,--< Y4 button
        PAD_axisZ6= 417,--< Z4 button
    
        PAD_axisR= 450,   
        PAD_axisL= 451,    
        PAD_axisR2= 452,    
        PAD_axisL2= 453,    
        PAD_axisR3= 454,    
        PAD_axisL3= 455,    
        PAD_axisR4= 456,    
        PAD_axisL4= 457,    
    
        PAD_options= 511,
        COUNT= 512
    },

  
    RENDERER = {
        ATTRIBUTE = {
            PRIMITIVE = 0,
            ALPHA_RULE = 1,
            DEPTH_TEST = 2,
            ETCH_RULE = 3,
            TEXTURE_FILTER_HINT = 4
        },
        
        PRIMITIVE = {
            TRIANGLE = 0,
            LINE = 1
        },

        ETCH_RULE = {
            NO_ETCHING = 0,
            DEFINE = 1,
            UNDEFINE = 2,
            IN = 3,
            OUT = 4
        },
        
        DEPTH_TEST = {
            LESS = 0,
            LEQ = 1,
            GREATER = 2,
            GEQ = 3,
            EQUAL = 4,
            NONE = 5
        },

        ALPHA_RULE = {
            ALLOW = 0,
            OPAQUE = 1,
            TRANSLUCENT = 2,
            INVISIBLE = 3,
        },
        
        TEXTURE_FILTER_HINT = {
            LINEAR = 0,
            NONE = 1,
        },
        
        LAYER = {
            COLOR = 1,
            DEPTH = 2,
            ETCH = 4,
            ALL  = 7
        }

    },
    
    ASSET = { 
        TYPE = {
            NONE = 0,
            IMAGE = 1,
            SOUND = 2,
            MATERIAL = 3,
            PARTICLE = 4,
            DATA = 5,
            MESH = 6
        }
    },
    
    
    PARTICLE = {
        PROPERTY = {
            DURATION = 0,
            SCALE_X = 1,
            SCALE_Y = 2,
            SCALE_MULTIPLIER = 3,
            ROTATION = 4,
            DIRECTION = 5,
            SPEED_X = 6,
            SPEED_Y = 7,
            RED = 8,
            GREEN = 9,
            BLUE = 10,
            ALPHA = 11,
        }
    },
    
    FILESYSTEM = {
        DEFAULT_NODE = {
            RESOURCES = 0,
            TOPAZ = 1,
            USER_DATA = 2
        },
    
    },
    
    AUTOMATION = {
        FUNCTION = {
            NONE = 0,
            LINEAR = 1,
            ACCEL = 2,
            SLOW = 3,
            SOFT_ACCEL = 4,
            SOFT_SLOW = 5,
            RANDOM = 6    
        }      
    },


    SHAPE3D = {
        TEXTURE = {
            SLOT_0 = 0,
            SLOT_1 = 1,
            SLOT_2 = 2
        }
    },


    OBJECT2D = {
        GROUP = {
            A = 0,
            B = 1,
            C = 2,
            D = 3,
            E = 4,
            F = 5,
            G = 6,
            H = 7,
            I = 8,
            J = 9,
            K = 10,
            L = 11,
            M = 12,
            N = 13,
            O = 14,
            P = 15,
            Q = 16,
            R = 17,
            S = 18,
            T = 19,
            U = 20,
            V = 21,
            W = 22,
            X = 23,
            Y = 24,
            Z = 25
        }
    },
    
    SCHEDULER = {
        MODE = {
            TIME = 0,
            FRAME = 1
        }
    },
    
    DISPLAY = {
        VIEW_POLICY = {
            NONE      = 0,
            MATCH_SIZE = 1
        },

        PARAMETER = {
            X = 0,
            Y = 1,
            WIDTH = 2,
            HEIGHT = 3,
            SHOW = 4,
            FULLSCREEN = 5,
            LOCK_CLIENT_RESIZE = 6,
            LOCK_CLIENT_POSITION = 7,
            VIEW_POLICY = 8,
            INPUT_FOCUS = 9
        },
        
        FRAMEBUFFER = {
            A = 0,
            B = 1,
            C = 2,
            D = 3
        }
    },
}






-- Topaz's own class system.
-- Its basic, but workable!!
--
-- Usage:
--[[
MyClass = tclass({
  define = function(instance, arg1, arg2) 
  
    local privateVar = arg1+arg2;

    instance.interface({
    
      publicFunction = function()
        return privateVar
      end
      
      publicValue = {
        get = function()
          return privateVar
        end
      }
    }); 
 
  end
})

instance = MyClass(2, 4);

print(instance.publicFunction()) --> 6
print(instance.publicValue)      --> 6

--]]
--
-- Features:
--[[

- Inheritance with access to superclass members 
- Throws error if unrecognized members are accessed
- toString
- Explicitly defined public interface 


]]--

function tclass(d) 

    local data = {
        inherits = d.inherits,
        constructor = d.define
    };
    
    local classinst = {};
    if d.declare ~= nil then 
        d.declare(classinst);
    end
    
    classinst.new = function(...) 
        local out = nil;
        if data.inherits ~= nil then
            out = data.inherits(...);
        end
        if (out == nil) then out = {} end
        

        local getters;            
        local setters;        
        local varnames;
        local mthnames;


        -- for inheritance, this will already 
        -- exist and can be appended.
        if out.__class ~= nil then 
            getters = out.__class.getters;
            setters = out.__class.setters;
            varnames  = out.__class.varnames;
            mthnames = out.__class.mthnames;
        else 
            getters = {};
            setters = {};
            mthnames = {};
            varnames  = {};
        end

        
        -- captured set/get.
        local makeprop = function(name, pdefine)
            local r = false;
            local w = false;

            if pdefine.set ~= nil then 
                setters[name] = pdefine.set;                
                w = true;
            end
            if pdefine.get ~= nil then 
                getters[name] = pdefine.get;                
                r = true;
            end
            
            if r == true and w == false then 
                varnames[name] = 'Read-only'            
            elseif r == false and w == true then 
                varnames[name] = 'Write-only'                        
            else 
                varnames[name] = 'Read/Write'            
            end
                
        end

        -- TODO: preserve old metatable better. This is sloppy!
        local oldmt = getmetatable(out);
        setmetatable(out, {});
        -- one function that defines the class interface
        out.interface = function(obj)
            setmetatable(out, {});
            for k, v in pairs(obj) do
                if type(v) == 'function' then
                    if mthnames[k] == nil then 
                        mthnames[k] = 'default';
                    else 
                        mthnames[k] = 'overidden';                    
                    end
                    out[k] = v;
                elseif type(v) == 'table' then 
                    makeprop(k, v);
                end
            end
        end
        
        out.introspect = function()
            return {
                public = {
                    variables = varnames,
                    methods = mthnames
                }
            }
        end
        setmetatable(out, oldmt);

        -- actually define interface
        data.constructor(classinst, out, ...); 
        out.interface = nil;
        if out.__class == nil then 
            out.__class = {
                getters = getters,
                setters = setters,
                mthnames = mthnames,
                varnames = varnames
            }            
        end
        
        
        local mt = {
            __index = function(table, key)
                local g = getters[key];
                if g ~= nil then 
                    return g()
                end
                error('tclass: Unknown public member "'..key..'"'..debug.traceback());
             end,
             
            __newindex = function(table, key, value)
                local s = setters[key];
                if s ~= nil then 
                    s(value)  
                    return;                  
                end

                error('tclass: Unknown public member "'..key..'"' ..debug.traceback());
            end,
        };            
        if oldmt ~= nil then 
            mt.__tostring = oldmt.__tostring
        end


        if d.toString ~= nil then mt.__tostring = function()return d.toString(out)end; end
        setmetatable(out, mt);            

        return out;
    end

    setmetatable(
        classinst, {
            __call = function(nu, ...) return classinst.new(...) end,
            __tostring = function()return '[tclass]'end
        }
    );
    return classinst;
end







Topaz = tclass({
    define = function(thisclass, this)
        local __Topaz__ = this;

        -- Declares a class which has external, native data.
        -- Instances of this class act as "shells" or interfaces for 
        -- this external data and usually have little to no state 
        -- outside of this.
        local __Native__ = tclass({
            declare = function(thisclass)
                thisclass.uniqueObjectPool = 0;    
            end,
            
            define = function(thisclass, this)
                local impl = nil;
                local id = thisclass.uniqueObjectPool;
                thisclass.uniqueObjectPool = 1 + thisclass.uniqueObjectPool;       
                
                this.interface({
                    -- binds a native instance to this table. Will throw an error 
                    -- if no since native instance can be bound. Binding happens through 2 
                    -- mechanisms: 1) instData.nativeCreate is a native function that can 
                    -- return a new object for us to use or 2) instData.instance contains 
                    -- a preexisting native reference.
                    bindNative = function(instData, ...) 
                        impl = instData.instance;
                        if instData.impl == nil and instData.nativeCreate ~= nil then 
                            impl = instData.nativeCreate(...);
                        end
                        if impl == nil then
                            error('Topaz.Native instance cannot have a nil native reference.'.. tostring(debug.traceback('')));
                            return;
                        end
                        impl.__ctx = this;
                        return impl;
                    end,
                    
                    
                    -- A unique id that identifies this instance.
                    uniqueID = {
                        get = function()
                            return id;
                        end            
                    },
                    
                    
                    --Gets the external reference to this external class
                    native = { 
                        get = function() 
                            return impl;
                        end
                    }          
                });
            
            end
        });

        -- Must be separate since Topaz inherit from the asset ahead of time before Topaz is computed.
        local __Asset__ = tclass({
            inherits = __Native__,
            define = function(thisclass, this, implePre)
                local impl = this.bindNative({
                    instance = implePre,
                });

                this.interface({
                    type = {
                        get = function()   return topaz_asset__get_type(impl);end,
                        set = function(v)         topaz_asset__set_type(impl, v);end
                    },

                    name = {
                        get = function()   return topaz_asset__get_name(this.impl);end,
                        set = function(v)         topaz_asset__set_name(this.impl, v);end
                    }
                });
            end
        });


        local __Filesystem__ = {        
            getPath = function(n) 
                return __Topaz__.Filesystem.Path(topaz_filesystem__get_path(n));
            end,
            
            getPathFromString = function(pth, str)
                if str == undefined then
                    return __Topaz__.Filesystem.Path(topaz_filesystem__get_path_from_string(pth));            
                else
                    return __Topaz__.Filesystem.Path(topaz_filesystem__get_path_from_string(pth.impl, str));                        
                end
            end,
            
            Path = tclass({
                inherits = __Native__,
                define   = function(thisclass, this, implePre)
                    local impl = this.bindNative({instance = implePre});

                    this.interface({
                        string = {
                            get = function()
                                return topaz_filesystem_path__as_string(impl);
                            end
                        },
                        
                        parent = {
                            get = function()
                                return __Topaz__.Filesystem.Path(topaz_filesystem_path__get_parent(impl));
                            end
                        },
                        
                        children = {
                            get = function()
                                local out = {};
                                local len = topaz_filesystem_path__get_child_count(impl);
                                for i = 0, len-1 do 
                                    local p = __Topaz__.Filesystem.Path(topaz_filesystem_path__get_nth_child(impl, i));
                                    if p ~= nil then
                                        table.insert(out, p);
                                    end
                                end
                                return out;
                            end
                        }
                    });
                end
            }),
        };

        local __RNG__ = tclass({
            inherits = __Native__,
            define = function(thisclass, this, implePre, ...);
    
                local impl = this.bindNative({
                    instance = implePre, 
                    nativeCreate = topaz_rng__create
                }, ...);
    
                
                this.interface({
                    seed = {
                        set = function(s) 
                            topaz_rng__set_seed(impl, s);
                        end
                    },
                    
                    integer = {
                        get = function() 
                            return topaz_rng__next_int(impl);
                        end
                    },
                    
                    value = {
                        get = function() 
                            return topaz_rng__next_value(impl);
                        end
                    }
                });
            end
        });

        local __ViewManager__ = (tclass({
            define = function(thisclass, this)
                this.interface({
                    mainDisplay = {
                        set = function(v)
                            topaz_view_manager__set_main(v.native);
                        end,
                        get = function()
                            return __Topaz__.Display(topaz_view_manager__get_main());
                        end
                    },
    
                    clipboard = {
                        set = function(v)
                            topaz_view_manager__set_clipboard_from_string(v);
                        end,
                        get = function()
                            return topaz_view_manager__get_clipboard_as_string();
                        end
                    }
                });
            end
        })).new();

        local __Display__ =  tclass({
            inherits = __Native__,
            define = function(thisclass, this, implePre) 
                local impl = this.bindNative({
                    instance = implePre,
                    nativeCreate = topaz_view_manager__create_display()
                });
                
                this.interface({
                    destroy = function()
                        topaz_display__destroy(impl);
                    end,
    
                    resize = function(w, h)
                        topaz_display__set_parameter(impl, 2, w);
                        topaz_display__set_parameter(impl, 3, h);
                    end,
    
                    addParameterCallback = function(cb)
                        return topaz_display__add_parameter_callback(impl, cb);
                    end,
    
                    addCloseCallback = function(cb)
                        return topaz_display__add_close_callback(impl, cb);
                    end,
    
                    removeCallback = function(i)
                        topaz_display__remove_callback(impl, i);
                    end,
    
                    getParameter = function(p)
                        return topaz_display__get_parameter(impl, p);
                    end,
    
                    setParameter = function(p, v)
                        topaz_display__set_parameter(impl, p, v);
                    end,
    
    
                    getFramebuffer = function(a)
                        return __Topaz__.Framebuffer(topaz_display__get_framebuffer(impl, a));
                    end,
    
                    useFramebuffer = function(a)
                        return topaz_display__use_framebuffer(impl, a);
                    end,
    
                    clearMainFramebuffer = function(a) 
                        topaz_display__clear_main_framebuffer(impl, a);
                    end,
    
                    captureMainFramebuffer = function(a)
                        topaz_display__capture_main_framebuffer(impl, a.native);
                    end,
    
                    width = {
                        get = function()
                            return topaz_display__get_parameter(impl, 2);
                        end,
                        set = function(v)
                            topaz_display__set_parameter(impl, 2, v);
                        end
                    },
    
                    height = {
                        get = function()
                            return topaz_display__get_parameter(impl, 3);
                        end,
                        set = function(v)
                            topaz_display__set_parameter(impl, 3, v);
                        end
                    },
    
    
                    camera2d = {
                        get = function()
                            return __Topaz__.Entity(topaz_display__get_camera_2d(impl));
                        end
                    },
    
                    camera3d = {
                        get = function()
                            return __Topaz__.Entity(topaz_display__get_camera_3d(impl));
                        end
                    },
    
                    framebuffer = {
                        get = function()
                            return __Topaz__.Framebuffer(topaz_display__get_main_framebuffer(impl));
                        end
                    }
                });
            end
        });
    
        local __Framebuffer__ = tclass({
            inherits = __Native__,
            define = function(thisclass, this, implePre)
                local impl = this.bindNative({
                    instance = implePre
                });
    
                this.interface({
                    width = {
                        get = function()
                            return topaz_framebuffer__get_width(impl);
                        end
                    },
    
                    height = {
                        get = function()
                            return topaz_framebuffer__get_height(impl);
                        end
                    },
    
                    resize = function(w, h)
                        topaz_framebuffer__resize(impl, w, h);
                    end,
    
                    filteredHint = {
                        get = function()
                            return topaz_framebuffer__get_filtered_hint(impl);
                        end,
                        set = function(b)
                            return topaz_framebuffer__set_filtered_hint(impl, b);
                        end
                    }
                });
            end
        });

        local __Mesh__ = tclass({
            inherits = __Asset__,
            define = function(thisclass, this, implPre)
                local impl = this.bindNative({
                    instance = implePre,
                });
    
                this.interface({
                    vertexCount = {
                        get = function()   return topaz_mesh__get_vertex_count(impl);end,
                        set = function(v)  return topaz_mesh__set_vertex_count(impl, v);end
                    },
    
    
                    vertices = {
                        set = function(v)
                            topaz_mesh__define_vertices(impl, v); 
                        end,
                        
                        get = function()
                            local out = {};
                            local len = this.vertexCount-1;
                            for i = 0, len do
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
                            end      
                            
                            return out;
                        end
                    },
    
                    setVertex = function(i, v)
                        topaz_mesh__set_vertex(impl, i, v);
                    end,
    
                    getVertex = function(i)
                        local out = {};
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
                    end,
    
                    addObject = function(obj)
                        topaz_mesh__add_object(impl);
                        if obj ~= nil then
                            topaz_mesh__set_object(this.objectCount-1, obj);
                        end
                    end,
    
                    objectCount = {
                        get = function() 
                            return topaz_mesh__get_object_count(impl);
                        end
                    },
    
                    removeObject = function(i)
                        topaz_mesh__remove_object(impl, i);
                    end,
    
                    setObject = function(i, v)
                        topaz_mesh__set_object(impl, i, v);
                    end,
    
    
                });
            end
        });


        local Input = tclass({
            define = function(thisclass, this) 
                this.interface({
                    mouse = {   
                        get = function()
                            return __Topaz__.Vector(topaz_input__mouse_x(), topaz_input__mouse_y());
                        end
                    },
    
                    mouseDelta = {
                        get = function()
                            return __Topaz__.Vector(topaz_input__mouse_delta_x(), topaz_input__mouse_delta_y());
                        end 
                    },
    
                    mouseWheel = {
                        get = function() return topaz_input__mouse_wheel(); end 
                    },
    
    
                    addKeyboardListener = function(obj) 
                        return topaz_input__add_keyboard_listener(obj);
                    end,
                    addPadListener = function(obj, pad) 
                        return topaz_input__add_pad_listener(obj, pad);
                    end,
                    addPointerListener = function(obj) 
                        return topaz_input__add_pointer_listener(obj);
                    end,
                    addMappedListener = function(obj, str) 
                        return topaz_input__add_mappded_listener(obj, str);
                    end,
                    removeListener = function(obj) 
                        topaz_input__remove_listener(obj);
                    end,
            
            
                    getState = function(i) 
                        return topaz_input__get_state(i);
                    end,
            
                    getPadState = function(i, p) 
                        return topaz_input__get_pad_state(i, p);
                    end,
            
                    getMappedState = function(i) 
                        return topaz_input__get_mapped_state(i);
                    end,
            
                    setDeadzone = function(a, b, c) 
                        topaz_input__set_deadzone(a, b, c);
                    end,
            
                    queryPads = function() 
                        local len = topaz_input__query_pad_count();
                        local out = {};
                        for i = 0, len-1 do
                            table.append(out, input_api__query_pad_id(i));
                        end
                        return out;
                    end,
            
                    addUnicodeListener = function(l) 
                        topaz_input__add_unicode_listener(l);
                    end,
            
                    removeUnicodeListener = function(l) 
                        topaz_input__remove_unicode_listener(l);
                    end
            
                })
            end
        }).new();


        local __Audio__ = tclass({
            define = function(thisclass, this)
                local ps = tclass({
                    inherits = __Native__,
                    define = function(thisclass, this) 
                        local impl = this.bindNative({
                            instance = implePre
                        });
    
    
                        this.interface({
                            volume = {
                                set = function(v)  topaz_audio__playback_set_volume(impl, v);end
                            },
    
                            panning = {
                                set = function(v)  topaz_audio__playback_set_panning(impl, v);end
                            },
    
                            repeatSound = {
                                set = function(v)  topaz_audio__playback_set_repeat(impl, v);end
                            },
    
                            paused = {
                                set = function(v) 
                                    if v ~= nil then 
                                        topaz_audio__playback_pause(impl);
                                    else
                                        topaz_audio__playback_resume(impl);
                                    end
                                end
                            },
    
                            seek = {
                                set = function(v) 
                                    topaz_audio__playback_seek(impl, v);
                                end
                            },
    
                            stop = function()
                                topaz_audio__playback_stop(impl);
                            end
                        });
                    end
                });
                this.interface({
                    -- get class ref
                    PlaybackSound = {
                        get = function()
                            return ps;
                        end
                    },
                
                    playSound = function(asset, channel) 
                        if channel == nil then 
                            return ps(topaz_audio__play_sound(asset.impl, 0));
                        else
                            return ps(topaz_audio__play_sound(asset.impl, channel));
                        end
                    end,
                    
                    channelHalt = function(channel) 
                        topaz_audio__channel_halt(channel);
                    end,
                    
                    channelSetVolume = function(channel, volume) 
                        topaz_audio__channel_set_volume(channel, volume);
                    end,
            
                    channelSetPanning = function(channel, panning) 
                        topaz_audio__channel_set_panning(channel, panning);
                    end
                })
            end
        }).new();

        local __Material__ = tclass({
            inherits = __Asset__,
            define = function(thisclass, this, implePre)

                this.interface({
                    setProgramData = function(a, b)
                        topaz_material__set_program_data(impl, a, b);
                    end,
    
                    getProgramData = function(a)
                        return topaz_material__set_program_data(impl, a);
                    end,
    
                    clearSources = function()
                        topaz_material__clear_sources(impl);
                    end,
    
                    setProgramSource = function(a, b, c)
                        topaz_material__set_program_source(impl, a, b, c); 
                    end,
    
    
                });
            end
        });
        
        local __Image__ = tclass({
            inherits = __Asset__,
            define = function(thisclass, this, implPre)
                local impl = this.native;
                this.interface({
                    width = {
                        get = function()   return topaz_image__get_width(this.impl);end
                    },
                    
                    height = {
                        get = function()   return topaz_image__get_height(this.impl);end
                    },
                    
                    frameCount = {
                        get = function()   return topaz_image__get_frame_count(this.impl);end
                    },
                    
                    resize = function(w, h) 
                        topaz_image__resize(impl, w, h);
                    end,
                    
                    addFrame = function() 
                        topaz_image__add_frame(impl);
                    end,
                    
                    removeFrame = function(f) 
                        topaz_image__remove_frame(impl, f);
                    end,
                    
                    setRGBA = function(i, rgbaData) 
                        topaz_image__frame_set_rgba(impl, i, rgbaData);
                    end
                });
            end
        });
        
        local __Data__ = tclass({
            inherits = __Asset__,
            define = function(thisclass, this, implPre) 
                local impl = this.native;
                
                this.interface({
                    byteCount = {
                        get = function()   return topaz_data__get_byte_count(impl);end
                    },
                    
                    string = {
                        get = function()   return topaz_data__get_as_string(impl); end
                    },
                    
                    bytes = {
                        get = function() 
                            local bytes = {};
                            local len = topaz_data__get_byte_count(impl);
                            for i = 0, len-1 do
                                table.append(bytes, topaz_data__get_nth_byte(impl, i));
                            end
                            return bytes;
                        end,

                        set = function(v)
                            topaz_data__set(impl, v);
                        end
                    }
               }); 
            end
        });
        
        
        local __Sound__ = tclass({
            inherits = __Asset__,
            define = function(thisclass, this, implPre)
                local impl = this.native;            
                
                this.interface({
                    sampleCount = {
                        get = function() return topaz_sound__get_sample_count(impl); end
                    },
                    
                    samples = {
                        get = function()  
                            local out = {};
                            local len = this.sampleCount;
                            for i = 0, len-1 do
                                table.append(out, topaz_sound__get_nth_sample_left(impl, i));
                                table.append(out, topaz_sound__get_nth_sample_right(impl, i));
                            end
                            return out;
                        end,
                        
                        
                        set = function(v) 
                            topaz_sound__set_samples(impl, v);
                        end
                    }
                });
            end
        });
        
        
        
        local __Resources__ = tclass({
            define = function(thisclass, this)
                local swtch = {};
                swtch[TOPAZ.ASSET.TYPE.IMAGE] = function(i)return __Topaz__.Image(i)end
                swtch[TOPAZ.ASSET.TYPE.DATA] = function(i)return __Topaz__.Data(i)end
                swtch[TOPAZ.ASSET.TYPE.SOUND] = function(i)return __Topaz__.Sound(i)end
                swtch[TOPAZ.ASSET.TYPE.MATERIAL] = function(i)return __Topaz__.Material(i)end
                swtch[TOPAZ.ASSET.TYPE.MESH] = function(i)return __Topaz__.Mesh(i)end

                local _rawAssetToInstance = function(impl) 
                    if impl == nil then return undefined; end
                    local fn = swtch[topaz_asset__get_type(impl)];
                    if fn == nil then return __Topaz__.Asset(impl); end
                    return fn(impl);
                end
            
            
                this.interface({
                    fetchAsset = function(type, name)
                        return _rawAssetToInstance(topaz_resources__fetch_asset(type, name));
                    end,

                    createAsset = function(type) 
                        return _rawAssetToInstance(topaz_resources__create_asset(type));
                    end,

                    loadAsset = function(ext, path, name) 
                        return _rawAssetToInstance(topaz_resources__load_asset(ext, path, name));
                    end,

                    loadAssetData = function(ext, data, name) 
                        return _rawAssetToInstance(topaz_resources__load_asset_data(ext, data, name));
                    end,
                    loadAssetBase64 = function(ext, b64, name) 
                        return _rawAssetToInstance(topaz_resources__load_asset_base64(ext, b64, name));
                    end,


                    writeAsset = function(asset, ext, path) 
                        return topaz_resources__write_asset(asset.native, ext, path);
                    end,

                    removeAsset = function(name) 
                        topaz_resources__remove_asset(name);
                    end,

                    

                    isExtensionSupported = function(ext) 
                        return topaz_resources__is_extension_supported(ext);
                    end,
                    
                    path = {
                        get = function() 
                            return topaz_resources__get_path();                        
                        end,
                        
                        set = function(v) 
                            repoaz_resources__set_path(v);             
                        end
                    }
                });
            end
        }).new();
        
        local __FontManager__ = tclass({
            define = function(thisclass, this) 
                this.interface({
                    registerFont = function(n)
                        topaz_font_manager__register_font(n);
                    end,

                    preloadGlyphs = function(a, c, b)
                        topaz_font_manager__preload_glyphs(a, c, b);
                    end,

                    unregisterFont = function(n)
                        topaz_font_manager__unregister_font(n);
                    end,            
                });
            end
        }).new();

        

        local __Entity__ = tclass({
            inherits = __Native__,
            define = function(thisclass, this, implePre, ...)
                local impl = this.bindNative({
                    instance = implePre,
                    nativeCreate = topaz_entity__create
                });
                
                this.interface({
                    isValid = {
                        get = function()
                            return topaz_entity__is_valid(impl);
                        end
                    },
                    
                    remove = function()
                        topaz_entity__remove(impl);
                    end,
                    
                    childCount = {
                        get = function()
                            return topaz_entity__get_child_count(impl);
                        end
                    },
                    
                    nthChild = function(n)
                        local f = topaz_entity__get_nth_child(impl, n);
                        if f.__ctx ~= nil then return f.__ctx end;
                        return thisclass.new(f);                
                    end,
                    
                    
                    children = {
                        get = function()
                            local children = {};
                            local len = topaz_entity__get_child_count(impl);
                            for i = 1, len do 
                                table.append(children, topaz_entity__get_nth_child(this.impl, i).__ctx);
                            end
                            return children;
                        end,
                        
                        set = function(c)
                            while topaz_entity__get_child_count(impl) do
                                topaz_entity__detach(topaz_entity__get_nth_child(impl, 0));
                            end
                                
                            for i = 1, #c do
                                topaz_entity__attach(impl, c[i].native);
                            end
                        end
                    },
                    
                    
                    --TODO: the rest, also onStep, onDraw, etc <3
                    step = function() 
                        topaz_entity__step(impl);
                    end,
                    
                    draw = function() 
                        topaz_entity__step(impl);
                    end,
    
                    attach = function(other) 
                        topaz_entity__attach(impl, other.native);
                    end,
    
                    detach = function() 
                        topaz_entity__detach(impl);
                    end,
                    
                    parent = {
                        get = function() 
                            local f = topaz_entity__get_parent(impl);
                            if f.__ctx ~= nil then return f.__ctx; end
                            return thisclass.new(f);
                        end,
                        
                        set = function(v)
                            topaz_entity__attach(v.native, impl);
                        end
                    },
                    
                    query = function(name) 
                        local f = topaz_entity__query(impl, name);
                        if f.__ctx ~= nil then return f.ctx end;
                        return thisclass.new(f);
                    end,
                    
                    
                    search = function(name)
                        local f = topaz_entity__search(impl, name);
                        if f.__ctx ~= nil then return f.ctx end;
                        return thisclass.new(f);                
                    end,
                    
                    priority = {
                        get = function() return topaz_entity__get_priority(impl);end, 
                        set = function(v)topaz_entity__set_priority(impl, v);end               
                    },
                    
        
                    setPriorityLast = function()
                        topaz_entity__set_priority_last(impl);
                    end,
        
                    setPriorityFirst = function()
                        topaz_entity__set_priority_first(impl);
                    end,
    
                    position = {
                        get = function()   return __Topaz__.Vector(0, 0, 0, topaz_entity__get_position(impl)); end,
                        set = function(v)  topaz_entity__set_position(impl, v.native); end
                    },
                    
                    scale = {
                        get = function()  return __Topaz__.Vector(0, 0, 0, topaz_entity__get_scale(impl));end,
                        set = function(v) topaz_entity__set_scale(impl, v.native);end
                    },
                    
                    globalPosition = {
                        get = function() return __Topaz__.Vector(0, 0, 0, topaz_entity__get_global_position(impl));end
                    },
                    
                    isStepping = {
                        get = function() return topaz_entity__is_stepping(impl); end
                    },
                    
                    isDrawing = {
                        get = function() return topaz_entity__is_drawing(impl) end
                    },
                    
                    stepping = {
                        get = function()  return topaz_entity__get_stepping(impl);end,
                        set = function(v) return topaz_entity__set_stepping(impl, v);end
                    },
                    
                    drawing = {
                        get = function()  return topaz_entity__get_drawing(impl);end,
                        set = function(v) return topaz_entity__set_drawing(impl, v);end
                    },
                    
                    name = {
                        get = function()  return topaz_entity__get_name(impl);end,
                        set = function(v) return topaz_entity__set_name(impl, v);end
                    },
    
                    addComponent = function(c)
                        topaz_entity__add_component(impl, c.native);
                    end,
                
                    addComponentAfter = function(c)
                        topaz_entity__add_component_after(impl, c.native);
                    end,
                        
                    components = {
                        get = function()
                            local len = topaz_entity__get_component_count(impl);
                            local out = {};
                            for i = 1, len do
                                local f = topaz_entity__get_nth_component(impl, i-1);
                                if f.__ctx ~= nil then
                                    table.append(out, f.__ctx);
                                else 
                                    out.push(__Topaz__.Component(f));
                                end
                            end
                            return out;
                        end,
                        set = function(c)
                            
                            while topaz_entity__get_component_count(impl) > 0 do
                                topaz_entity__remove_component(impl, topaz_entity__get_nth_component(impl, 0));
                            end
    
                            for i = 1, #c do
                                topaz_entity__add_component(impl, c[i].native);
                            end
                        end
                    },
    
    
                    queryComponent = function(tag) 
                        local f = topaz_entity__query_component(impl, tag);
                        if f.__ctx ~= nil then return f.__ctx; end
                        return __Topaz__.Component(f);
                    end,
                
                    removeComponent = function(c)
                        return topaz_entity__remove_component(impl, c.native);
                    end,
                    
                    onStep = {set = function(v)
                            topaz_entity__set_on_step(impl, function() v() end);
    
                    end},
    
                    onDraw = {set = function(v)
                            topaz_entity__set_on_draw(impl, function()v()end);
                    end},
    
                    onPreStep = {set = function(v)
                            topaz_entity__set_on_pre_step(impl, function()v()end);
                    end},
    
                    onPreDraw = {set = function(v)
                            topaz_entity__set_on_pre_draw(impl, function()v()end);
                    end},
                    
                    onAttach = {set = function(v)
                            topaz_entity__set_on_attach(impl, function()v()end);
                    end},
    
                    onDetach = {set = function(v)
                            topaz_entity__set_on_detach(impl, function()v()end);
                    end},
    
                    onRemove = {set = function(v)
                            topaz_entity__set_on_remove(impl, function()v()end);
                    end}
    
                    
    
    
                
                });        
            end    
        });

        local __Vector__ = tclass({
            inherits = __Native__,
            define = function(thisclass, this, x_, y_, z_, implPre)
    
                local impl = this.bindNative({
                    instance = implePre,
                    nativeCreate = topaz_vector__create
                }, x_, y_, z_);
                
                this.interface({
                    getDistance = function(other)
                        return topaz_vector__get_distance(impl, other.native);
                    end,
                    
                    normalize = function()
                        this.topaz_vector__normalize(impl);   
                    end,
                    
                    cross = function(other)
                        return __Topaz__.Vector(0, 0, 0, topaz_vector__cross(impl, other.native));
                    end,
    
                    floor = function()
                        topaz_vector__floor(impl);
                    end,
                    
                    rotationXDiff = function(other)
                        return topaz_vector__rotation_x_diff(impl, other.native);
                    end,
                    
                    rotationXDiffRelative = function(other)
                        return topaz_vector__rotation_x_diff_relative(impl, other.native);
                    end,
                    
                    rotationX = function() 
                        return topaz_vector__rotation_x(impl);
                    end,
                    
                    rotationYDiff = function(other) 
                        return topaz_vector__rotation_y_diff(impl, other.native);
                    end,
                    
                    rotationYDiffRelative = function(other)
                        return topaz_vector__rotation_y_diff_relative(impl, other.native);
                    end,
                    
                    rotationY = function()
                        return topaz_vector__rotation_y(impl);
                    end,
                    
                    rotationZDiff = function(other)
                        return topaz_vector__rotation_z_diff(impl, other.native);
                    end,
                    
                    rotationZDiffRelative = function(other)
                        return topaz_vector__rotation_z_diff_relative(impl, other.native);
                    end,
                    
                    rotationZ = function()
                        return topaz_vector__rotation_z(impl);
                    end,
                    
                    rotateX = function(val)
                        topaz_vector__rotate_x(impl, val);
                    end,
                    
                    rotateY = function(val)
                        topaz_vector__rotate_y(impl, val);
                    end,
                    
                    rotateZ = function(val)
                        topaz_vector__rotate_z(impl, val);
                    end,
                    
                    remove = function()
                        topaz_vector__destroy(impl);
                        impl = {};
                    end,
                    
                    add = function(b)
                        return __Topaz__.Vector(this.x + b.x, this.y + b.y, this.z + b.z);
                    end,
                    
                    subtract = function(b)
                        return __Topaz__.Vector(this.x - b.x, this.y - b.y, this.z - b.z);
                    end,
    
                    multiply = function(b)
                        return __Topaz__.Vector(this.x * b.x, this.y * b.y, this.z * b.z);
                    end,
                    
                    divide = function(b)
                        return __Topaz__.Vector(this.x / b.x, this.y / b.y, this.z / b.z);
                    end,
                    
                    length = {get = function()return topaz_vector__get_length(impl)end},
                    x = {get = function()return topaz_vector__get_x(impl);end, set = function(v)topaz_vector__set_x(impl, v);end},
                    y = {get = function()return topaz_vector__get_y(impl);end, set = function(v)topaz_vector__set_y(impl, v);end},
                    z = {get = function()return topaz_vector__get_z(impl);end, set = function(v)topaz_vector__set_z(impl, v);end}
    
                });        
            end,
            toString = function(this)
                return '{'..this.x..','..this.y..',' ..this.z..'}';        
            end
        });
        

        local __Component__ = tclass({
            inherits = __Native__,
            define = function(thisclass, this, implePre)
                -- some classes will inherit and overwrite the native instance, so a 
                -- local impl doesnt make sense.
                this.bindNative({
                    instance = implePre,
                    nativeCreate = topaz_component__create
                }, '');
    
                this.interface({
                    destroy = function()
                        topaz_component__destroy(this.native);
                    end,
                    
                    step = function()
                        topaz_component__run(this.native);
                    end,
                    
                    draw = function()
                        topaz_component__draw(this.native);
                    end,
                    
                    stepping = {
                        get = function()  return topaz_component__get_stepping(this.native);end,
                        set = function(v) topaz_component__set_stepping(this.native, v);end
                    },
                    
                    drawing = {
                        get = function()  return topaz_component__get_drawing(this.native);end,
                        set = function(v) topaz_component__set_drawing(this.native, v);end
                    },
                    
                    tag = {
                        get = function()  return topaz_component__get_tag(this.native);end,
                        set = function(v) topaz_component__set_tag(this.native, v);end
                    },
                    
                    host = {
                        get = function()
                            local f = topaz_component__get_host(this.native);
                            if f.__ctx ~= nil then return f.__ctx end;
                            return __Topaz__.Entity(f);
                        end
                    },
                    
                    emitEvent = function(eventName, ent)
                        if ent == nil then
                            return topaz_component__emit_event_anonymous(this.native, eventName);
                        else
                            return topaz_component__emit_event(this.native, eventName, ent.native);
                        end
                    end,
                    
                    canHandleEvent = function(name)
                        return topaz_component__can_handle_event(this.native, name);
                    end,
                    
                    installEvent = function(event, callback)
                        local cb;
                        if callback ~= nil then
                            cb = function(component, ent)callback(component.__ctx, ent.__ctx)end
                        else 
                            cb = function()end
                        end
                            
                        topaz_component__install_event(this.native, event, cb);
                    end,
                    
                    uninstallEvent = function(event)
                        topaz_component__uninstall_event(this.native, event);
                    end,
                    
                    installHook = function(event, callback)
                        return topaz_component__install_hook(this.native, event, function(component, ent)
                            callback(component.__ctx, ent.__ctx);
                        end);
                    end,
                    
                    uninstallHook = function(event, id)
                        topaz_component__uninstall_hook(this.native, event, id);
                    end,
                    
                    installHandler = function(event, callback)
                        return topaz_component__install_handler(this.native, event, function(component, ent)
                            callback(component.__ctx, ent.__ctx);
                        end);
                    end,
                    
                    uninstallHandler = function(event, id)
                        topaz_component__uninstall_handler(this.native, event, id);
                    end
                });
            end
        });
        
        local __Color__ = tclass({
            define = function(thisclass, this, implPre, name)
                local impl = this.bindNative({
                    instance = implPre,
                    nativeCreate = topaz_color__create
                });
                topaz_color__set_from_string(impl, name);
                
                this.interface({
                    string =
                    {
                        get = function()
                            return topaz_color__to_hex_string(impl);
                        end,

                        set = function(str)
                            topaz_color__set_from_string(impl, str);
                        end
                    },
                    r= {
                        get = function() return topaz_color__get_r(impl);end, 
                        set = function(v)       topaz_color__set_r(impl, v);end
                    },
                    
                    g= {
                        get = function() return topaz_color__get_g(impl);end, 
                        set = function(v)       topaz_color__set_g(impl, v);end
                    },
                    
                    b= {
                        get = function() return topaz_color__get_b(impl);end, 
                        set = function(v)       topaz_color__set_b(impl, v);end
                    },
                    
                    a= {
                        get = function() return topaz_color__get_a(impl);end, 
                        set = function(v)       topaz_color__set_a(impl, v);end
                    }
                });
            end,


            
            toString = function()
                return 'RGBA: '..math.floor(this.r*255)..','..math.floor(this.g*255)..','..math.floor(this.b*255)..':'..math.floor(this.a*255);    
            end   
        });
        
        
        local __Text2D__ = tclass({
            inherits = __Component__,
            define = function(thisclass, this, implPre)
                -- whoops, the component constructor already made a generic component native.
                -- destroy it and make a real one
                this.native.__ctx.destroy();
                
                local impl = this.bindNative({
                    instance = implePre,
                    nativeCreate = topaz_text2d__create
                });
                
                local fontState;
                local fontSize;
                
                this.interface({
                    text = {
                        get = function() return topaz_text2d__get_text(impl);end,
                        set = function(v) topaz_text2d__set_text(impl, v);end
                    },
                    
                    font = {
                        get = function()  return this.fontState;end,
                        set = function(v) 
                            fontState = v;
                            topaz_text2d__set_font(impl, fontState, sizeState);
                        end
                    },
                    
                    size =  {
                        get = function()  return this.sizeState;end,
                        set = function(v) 
                            sizeState = v;
                            topaz_text2d__set_font(impl, fontState, sizeState);
                        end
                    },
                    
                    setColor = function(c) 
                        topaz_text2d__set_color(impl, c.native);
                    end,
                    
                    setColorSection = function(from, to, c) 
                        topaz_text2d__set_color_section(impl, from, to, c.native);
                    end,
                    
                    extentWidth = {
                        get = function() return topaz_text2d__get_extent_width(impl);end
                    },
                    
                    extentHeight = {
                        get = function() return topaz_text2d__get_extent_height(impl);end
                    },
                    
                    getCharX = function(i) 
                        return topaz_text2d__get_char_x(impl, i);
                    end,
                    
                    getCharY = function(i) 
                        return topaz_text2d__get_char_y(impl, i);
                    end,
                    
                    position = {
                        get = function() return Topaz.Vector(0, 0, 0, topaz_text2d__get_position(this.impl));end,
                        set = function(v)topaz_text2d__set_position(impl, v.native);end
                    },
                    
                    rotation = {
                        get = function() return Topaz.Vector(0, 0, 0, topaz_text2d__get_rotation(this.impl));end,
                        set = function(v)topaz_text2d__set_rotation(impl, v.native);end
                    },
                    
                    scale = {
                        get = function() return Topaz.Vector(0, 0, 0, topaz_text2d__get_scale(this.impl));end,
                        set = function(v)topaz_text2d__set_scale(impl, v.native);end
                    },
                    
                    setAttribute = function(a, b)
                        topaz_text2d__set_attribute(impl, a, b);
                    end,
                    
                    getAttribute = function(a)
                        return topaz_text2d__get_attribute(impl, a);
                    end
                });

            end
        });

        this.interface({
            run   = topaz__run,
            pause   = topaz__pause,
            pauseNow = topaz__pause_now,
            resume = topaz__resume,
            iterate = topaz__iterate,
            step = topaz__step,
            draw = topaz__draw,
            attachPreManager = function(v)topaz__attach_pre_manager(v.impl);end,        
            attachPreManagerUnpausable = function(v)topaz__attach_pre_manager_unpausable(v.impl);end,        
            attachPostManager = function(v)topaz__attach_post_manager(v.impl);end,        
            attachPostManagerUnpausable = function(v)topaz__attach_post_manager_unpausable(v.impl);end,        
            quit = topaz__quit,
            wait = topaz__wait,
            import = topaz_script__import,
            log = topaz__log,
            toBase64 = topaz__to_base64,
            fromBase64 = function(f) 
                return __Topaz__.Data(topaz__from_base64(f));
            end,
            
            root = {
                set = function(e)
                    return topaz__set_root(e.native)
                end,

                get = function(e)
                    
                end
            },
            objectToString = function(obj, levelSrc)
                local checked = {};
                local levelG;
                if levelSrc ~= nil then 
                    levelG = levelSrc
                else
                    levelG = 10;
                end
                local other;
                local helper = function(obj, level)
                    if obj == nil then return '<nil>'; end
                    if type(obj) == 'function' then return '<function()>'
                    elseif type(obj) == 'userdata' then return '<internal>'
                    elseif type(obj) == 'string' then return obj
                    elseif type(obj) == 'table' then

                        if checked[obj] ~= nil then return '[Already Printed]' end
                        
                        
                        checked[obj] = 1;

                        local strOut = '|'..tostring(obj)..'|{\n';
                        local levelInv = levelG - level;    

                                        
                        for k,v in pairs(obj) do 
                            local subStr;
                            if levelInv > 0 then
                                subStr = other(v, level+1)
                            else 
                                subStr = v;
                            end
                            for n = 0, level do strOut = strOut .. '  '; end
                            strOut = strOut .. '  ' .. k .. ' : ' .. subStr .. ',\n'; 
                        
                        end
                        for n = 0, level do strOut = strOut .. '  '; end
                        strOut = strOut..'}';
                        return strOut;
                    else 
                        return '<data>';            
                    end
                    return '';
                end
                other = helper;
                
                return helper(obj, 0) .. '\n';
            end,  
    
            Filesystem  = {get = function()return __Filesystem__ end},
            RNG         = {get = function()return __RNG__ end},
            ViewManager = {get = function()return __ViewManager__ end},
            Display     = {get = function()return __Display__ end},
            Framebuffer = {get = function()return __Framebuffer__ end},
            Asset       = {get = function()return __Asset__ end},
            Mesh        = {get = function()return __Mesh__ end},
            Input       = {get = function()return __Input__ end},
            Audio       = {get = function()return __Audio__ end},
            Material    = {get = function()return __Material__ end},
            Entity      = {get = function()return __Entity__ end},
            Vector      = {get = function()return __Vector__ end},
            Component   = {get = function()return __Component__ end},
            Resources   = {get = function()return __Resources__ end},
            FontManager = {get = function()return __FontManager__ end},
            Image       = {get = function()return __Image__ end},
            Data        = {get = function()return __Data__ end},
            Sound       = {get = function()return __Sound__ end},
            Color       = {get = function()return __Color__ end},
            Text2D      = {get = function()return __Text2D__ end}
            
       });
    end
}).new();
