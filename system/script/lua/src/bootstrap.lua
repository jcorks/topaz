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
  define = function(instance, arg) 
  
    local privateVar = arg.a1+arg.a2;

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

instance = MyClass({a1=2, a2=4});

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

    local inherits = d.inherits;
    local define = d.define;
    
    local classinst = {};
    if d.declare ~= nil then 
        d.declare(classinst);
    end
    
    -- isa query data. since every inherited class has to 
    -- be declared first, all base classes will already have a proper 
    -- inheritsFrom. We take our base ones and add to it.
    local allclass = {};
    
    if inherits ~= nil then 
        local addbase = function(other)

            for n = 1, #other do 
                local g = other[n]
                
                local found = false 
                for i = 1, #allclass do 
                    if allclass[i] == g then 
                        found = true
                        break                    
                    end
                end
                if found == false then 

                    table.insert(allclass, g)                
                end
            end
        end
        
        if inherits[1]~=nil then
            for i = 1, #inherits do 
                addbase(inherits[i].interfaces);
            end
        else 
            addbase(inherits.interfaces);
        end
    end
    table.insert(allclass, classinst);
    classinst.interfaces = allclass;
    

    

    local isa = function(d) 
        for i = 0, #allclass do 
            if allclass[i] == d then return true end;        
        end
        return false
    end
    
    
    classinst.new = function(args, outsrc) 
        if args == nil then args = {} end
        local out = outsrc;
        if inherits ~= nil then
            if out == nil then out = {} end
            if inherits[1] ~= nil then 
                for i = 1, #inherits do 
                    inherits[i].new(args, out);
                end
            else 
                inherits.new(args, out);            
            end
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
            varnames = {inherited=out.__class.varnames};
            mthnames = {inherited=out.__class.mthnames};
            out.__class.varnames = varnames;
            out.__class.mthnames = mthnames;

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
                    mthnames[k] = 'function';
                    out[k] = v;
                elseif type(v) == 'table' then 
                    makeprop(k, v);
                else 
                    error('tclass interfaces can only have getters/setters and methods.');
                end
            end
            out.isa = isa;
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
        define(out, args, classinst); 
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
            __tostring = function()return '[tclass]'end
        }
    );
    return classinst;
end




Vector = (function()
    local isNaN = function(d)
        return d == undefined or d ~= d 
    end
    local denil = function(v)
        if isNaN(v.x) == true then v.x = 0; end
        if isNaN(v.y) == true then v.y = 0; end
        if isNaN(v.z) == true then v.z = 0; end
    end
    local statepush = function(v)
        if v.native == nil then
            denil(v);
            v.native = topaz_vector__create(v.x, v.y, v.z);
        else
            topaz_vector__set_xyz(v.native, v.x, v.y, v.z);
        end
    end

    local statepull = function(v)
        v.x = topaz_vector__get_x(v.native);
        v.y = topaz_vector__get_y(v.native);
        v.z = topaz_vector__get_z(v.native);
    end

    return {
        statepush = statepush,
        statepull = statepull,
        fromnative = function(native)
            local out = {native=native};
            statepull(out);
            return out;
        end,
        getDistance = function(a, b) 
            statepush(a); statepush(b);
            return topaz_vector__get_distance(a.native, b.native);
        end,
        
        normalize = function(a) 
            statepush(a);
            topaz_vector__normalize(a.native);   
            statepull(a);
        end,
        
        cross = function(a, b) 
            statepush(a);
            statepush(b);
            return cnew({native:topaz_vector__cross(a.native, b.native)});
        end,

        floor = function(a) 
            statepush(a);
            topaz_vector__floor(a.native);
            statepull(a);
        end,
        
        rotationXDiff = function(a, b) 
            statepush(a);
            statepush(b);
            return topaz_vector__rotation_x_diff(a.native, b.native);
        end,
        
        rotationXDiffRelative = function(a, b) 
            statepush(a);
            statepush(b);
            return topaz_vector__rotation_x_diff_relative(a.native, b.native);
        end,
        
        rotationX = function(a)
            statepush(a);
            return topaz_vector__rotation_x(a.native);
        end,
        
        rotationYDiff = function(a, b) 
            statepush(a);
            statepush(b);
            return topaz_vector__rotation_y_diff(a.native, b.native);
        end,
        
        rotationYDiffRelative = function(a, b) 
            statepush(a);
            statepush(b);
            return topaz_vector__rotation_y_diff_relative(a.native, b.native);
        end,
        
        rotationY = function(a) 
            statepush(a);
            return topaz_vector__rotation_y(a.native);
        end,
        
        rotationZDiff = function(a, b)
            statepush(a);
            statepush(b); 
            return topaz_vector__rotation_z_diff(a.native, b.native);
        end,
        
        rotationZDiffRelative = function(a, b)
            statepush(a);
            statepush(b);
            return topaz_vector__rotation_z_diff_relative(a.native, b.native);
        end,
        
        rotationZ = function(a) 
            statepush(a);
            return topaz_vector__rotation_z(a.native);
        end,
        
        rotateX = function(a, val) 
            statepush(a);
            topaz_vector__rotate_x(a.native, val);
            statepull(a);
        end,
        
        rotateY = function(a, val) 
            statepush(a);
            topaz_vector__rotate_y(a.native, val);
            statepull(a);
        end,
        
        rotateZ = function(a, val) 
            statepush(a);
            topaz_vector__rotate_y(a.native, val);
            statepull(a);
        end,

        
        add = function(a, b) 
            denil(a); denil(b);
            return {x=a.x + b.x, y=a.y + b.y, z=a.z + b.z};
        end,
        
        subtract = function(a, b) 
            denil(a); denil(b);
            return {x=a.x - b.x, y=a.y - b.y, z=a.z - b.z};
        end,

        multiply = function(a, b) 
            denil(a); denil(b);
            return {x=a.x * b.x, y=a.y * b.y, z=a.z * b.z};
        end,
        
        divide = function(a, b) 
            denil(a); denil(b);
            return {x=a.x / b.x, y=a.y / b.y, z=a.z / b.z};
        end,
        
        length = function(a)
            statepush(a);
            return topaz_vector__get_length(a.native)
        end
    }
end)();

Color = (function()
    local isNaN = function(d)
        return d == undefined or d ~= d 
    end
    local refColor = topaz_color__create(0, 0, 0, 0);

    local statepush = function(v)
        if v.native == undefined then
            if isNaN(v.r) == true then v.r = 0; end
            if isNaN(v.g) == true then  v.g = 0;end
            if isNaN(v.b) == true then  v.b = 0;end
            if isNaN(v.a) == true then  v.a = 0;end
            v.native = topaz_color__create(v.r, v.g, v.b, v.a);
        else
            topaz_color__set_rgba(v.native, v.r, v.g, v.b, v.a);
        end
    end

    local statepull = function(v)
        v.r = topaz_color__get_r(v.native);
        v.g = topaz_color__get_g(v.native);
        v.b = topaz_color__get_b(v.native);
        v.a = topaz_color__get_a(v.native);
    end

    return {
        fromnative = function(native)
            local out = {native=native};
            statepull(out);
            return out;
        end,
        statepush = statepush,
        statepull = statepull,
        asString = function(a)
            statepush(a);
            return topaz_color__to_hex_string(a.native);
        end,

        fromString = function(str)
            topaz_color__set_from_string(refColor, str);
            return {
                r= topaz_color__get_r(refColor),
                g= topaz_color__get_g(refColor),
                b= topaz_color__get_b(refColor),
                a= topaz_color__get_a(refColor)
            }
        end
    }
end)();






Topaz = tclass({
    define = function(this, args, thisclass)
        local __Topaz__ = this;

        -- Declares a class which has external, native data.
        -- Instances of this class act as "shells" or interfaces for 
        -- this external data and usually have little to no state 
        -- outside of this.
        local __Native__ = tclass({
            
            define = function(this, args, thisclass)
                local impl = nil;
                
                this.interface({
                    -- binds a native instance to this table. Will throw an error 
                    -- if no since native instance can be bound. Binding happens through 2 
                    -- mechanisms: 1) instData.nativeCreate is a native function that can 
                    -- return a new object for us to use or 2) instData.instance contains 
                    -- a preexisting native reference.
                    bindNative = function(instData, ...) 
                        
                        if instData.instance == nil then 
                            impl = nil                        
                        else
                            impl = instData.instance.native;
                        end 
                        
                        
                        if impl == nil and instData.nativeCreate ~= nil then 
                            impl = instData.nativeCreate(...);
                        end
                        if impl == nil then
                            error('Topaz.Native instance cannot have a nil native reference.'.. tostring(debug.traceback('')));
                            return;
                        end
                        impl.__ctx = this;
                        return impl;
                    end,
                    
                    
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
            define = function(this, args, thisclass)
                local impl = this.bindNative({
                    instance = args,
                });

                this.interface({
                    type = {
                        get = function()   return topaz_asset__get_type(impl);end,
                        set = function(v)         topaz_asset__set_type(impl, v);end
                    },

                    name = {
                        get = function()   return topaz_asset__get_name(impl);end,
                        set = function(v)         topaz_asset__set_name(impl, v);end
                    }
                });
            end
        });


        local __Filesystem__ = {        
            getPath = function(n) 
                return __Topaz__.Filesystem.Path.new({native=topaz_filesystem__get_path(n)});
            end,
            
            getPathFromString = function(pth, str)
                if str == nil then
                    return __Topaz__.Filesystem.Path.new({native=topaz_filesystem__get_path_from_string(pth)});            
                else
                    return __Topaz__.Filesystem.Path.new({native=topaz_filesystem__get_path_from_string(pth.native, str)});                        
                end
            end,
            
            Path = tclass({
                inherits = __Native__,
                define   = function(thisclass, this, args)
                    local impl = this.bindNative({instance = args});

                    this.interface({
                        string = {
                            get = function()
                                return topaz_filesystem_path__as_string(impl);
                            end
                        },
                        
                        parent = {
                            get = function()
                                return __Topaz__.Filesystem.Path.new({native=topaz_filesystem_path__get_parent(impl)});
                            end
                        },
                        
                        children = {
                            get = function()
                                local out = {};
                                local len = topaz_filesystem_path__get_child_count(impl);
                                for i = 0, len-1 do 
                                    local p = __Topaz__.Filesystem.Path.new({native=topaz_filesystem_path__get_nth_child(impl, i)});
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
            define = function(this, args, thisclass)
    
                local impl = this.bindNative({
                    instance = args, 
                    nativeCreate = topaz_rng__create
                }, args.seed);
    
                
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
            define = function(this, args, thisclass)
                this.interface({
                    mainDisplay = {
                        set = function(v)
                            topaz_view_manager__set_main(v.native);
                        end,
                        get = function()
                            return __Topaz__.Display.new({native=topaz_view_manager__get_main()});
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
            define = function(this, args, thisclass)
                local impl = this.bindNative({
                    instance = args,
                    nativeCreate = topaz_view_manager__create_display
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
                        return __Topaz__.Framebuffer.new({native=topaz_display__get_framebuffer(impl, a)});
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
                            return __Topaz__.Entity.new({native=topaz_display__get_camera_2d(impl)});
                        end
                    },
    
                    camera3d = {
                        get = function()
                            return __Topaz__.Entity.new({native=topaz_display__get_camera_3d(impl)});
                        end
                    },
    
                    framebuffer = {
                        get = function()
                            return __Topaz__.Framebuffer.new({native=topaz_display__get_main_framebuffer(impl)});
                        end
                    }
                });
            end
        });
    
        local __Framebuffer__ = tclass({
            inherits = __Native__,
            define = function(this, args, thisclass)
                local impl = this.bindNative({
                    instance = args
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
            define = function(this, args, thisclass)
                local impl = this.native;    
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
                                table.insert(out, topaz_mesh__get_vertex(impl, 0, i, 0));
                                table.insert(out, topaz_mesh__get_vertex(impl, 0, i, 1));
                                table.insert(out, topaz_mesh__get_vertex(impl, 0, i, 2));
            
                                table.insert(out, topaz_mesh__get_vertex(impl, 1, i, 0));
                                table.insert(out, topaz_mesh__get_vertex(impl, 1, i, 1));
                                table.insert(out, topaz_mesh__get_vertex(impl, 1, i, 2));
            
                                table.insert(out, topaz_mesh__get_vertex(impl, 2, i, 0));
                                table.insert(out, topaz_mesh__get_vertex(impl, 2, i, 1));
            
                                table.insert(out, topaz_mesh__get_vertex(impl, 3, i, 0));
                                table.insert(out, topaz_mesh__get_vertex(impl, 3, i, 1));
                                table.insert(out, topaz_mesh__get_vertex(impl, 3, i, 2));
                                table.insert(out, topaz_mesh__get_vertex(impl, 3, i, 3));
                            end      
                            
                            return out;
                        end
                    },
    
                    setVertex = function(i, v)
                        topaz_mesh__set_vertex(impl, i, v);
                    end,
    
                    getVertex = function(i)
                        local out = {};
                        table.insert(out, topaz_mesh__get_vertex(impl, 0, i, 0));
                        table.insert(out, topaz_mesh__get_vertex(impl, 0, i, 1));
                        table.insert(out, topaz_mesh__get_vertex(impl, 0, i, 2));
                
                        table.insert(out, topaz_mesh__get_vertex(impl, 1, i, 0));
                        table.insert(out, topaz_mesh__get_vertex(impl, 1, i, 1));
                        table.insert(out, topaz_mesh__get_vertex(impl, 1, i, 2));
                
                        table.insert(out, topaz_mesh__get_vertex(impl, 2, i, 0));
                        table.insert(out, topaz_mesh__get_vertex(impl, 2, i, 1));
                
                        table.insert(out, topaz_mesh__get_vertex(impl, 3, i, 0));
                        table.insert(out, topaz_mesh__get_vertex(impl, 3, i, 1));
                        table.insert(out, topaz_mesh__get_vertex(impl, 3, i, 2));
                        table.insert(out, topaz_mesh__get_vertex(impl, 3, i, 3));
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


        local __Input__ = tclass({
            define = function(this, args, thisclass)
                this.interface({
                    mouse = {   
                        get = function()
                            return {x=topaz_input__mouse_x(), y=topaz_input__mouse_y()};
                        end
                    },
    
                    mouseDelta = {
                        get = function()
                            return {x=topaz_input__mouse_delta_x(), y=topaz_input__mouse_delta_y()};
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
                            table.insert(out, input_api__query_pad_id(i));
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
            define = function(this, args, thisclass)
                local ps = tclass({
                    inherits = __Native__,
                    define = function(this, args, thisclass)
                        local impl = this.bindNative({
                            instance = args
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
                            return ps(topaz_audio__play_sound(asset.native, 0));
                        else
                            return ps(topaz_audio__play_sound(asset.native, channel));
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
            define = function(this, args, thisclass)
                local impl = this.native;
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
            define = function(this, args, thisclass)
                local impl = this.native;
                this.interface({
                    width = {
                        get = function()   return topaz_image__get_width(impl);end
                    },
                    
                    height = {
                        get = function()   return topaz_image__get_height(impl);end
                    },
                    
                    frameCount = {
                        get = function()   return topaz_image__get_frame_count(impl);end
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
            define = function(this, args, thisclass)
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
                                table.insert(bytes, topaz_data__get_nth_byte(impl, i));
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
            define = function(this, args, thisclass)
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
                                table.insert(out, topaz_sound__get_nth_sample_left(impl, i));
                                table.insert(out, topaz_sound__get_nth_sample_right(impl, i));
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
            define = function(this, args, thisclass)
                local swtch = {};
                swtch[TOPAZ.ASSET.TYPE.IMAGE] = function(i)return __Topaz__.Image.new({native=i})end
                swtch[TOPAZ.ASSET.TYPE.DATA] = function(i)return __Topaz__.Data.new({native=i})end
                swtch[TOPAZ.ASSET.TYPE.SOUND] = function(i)return __Topaz__.Sound.new({native=i})end
                swtch[TOPAZ.ASSET.TYPE.MATERIAL] = function(i)return __Topaz__.Material.new({native=i})end
                swtch[TOPAZ.ASSET.TYPE.MESH] = function(i)return __Topaz__.Mesh.new({native=i})end

                local _rawAssetToInstance = function(impl) 
                    if impl == nil then return nil; end
                    local fn = swtch[topaz_asset__get_type(impl)];
                    if fn == nil then return __Topaz__.Asset.new({native=impl}); end
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
            define = function(this, args, thisclass)
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
            define = function(this, args, thisclass)   
                this.bindNative({
                    instance = args,
                    nativeCreate = topaz_entity__create
                });
                
                this.interface({
                    isValid = {
                        get = function()
                            return topaz_entity__is_valid(this.native);
                        end
                    },
                    
                    remove = function()
                        topaz_entity__remove(this.native);
                    end,
                    
                    childCount = {
                        get = function()
                            return topaz_entity__get_child_count(this.native);
                        end
                    },
                    
                    nthChild = function(n)
                        local f = topaz_entity__get_nth_child(this.native, n);
                        if f.__ctx ~= nil then return f.__ctx end;
                        return thisclass.new({native=f});                
                    end,
                    
                    
                    children = {
                        get = function()
                            local children = {};
                            local len = topaz_entity__get_child_count(this.native);
                            for i = 0, len-1 do 
                                table.insert(children, topaz_entity__get_nth_child(this.native, i).__ctx);
                            end
                            return children;
                        end,
                        
                        set = function(c)
                            while topaz_entity__get_child_count(this.native) > 0 do
                                topaz_entity__detach(topaz_entity__get_nth_child(this.native, 0));
                            end
                                
                            for i = 1, #c do
                                topaz_entity__attach(this.native, c[i].native);
                            end
                        end
                    },
                    
                    
                    --TODO: the rest, also onStep, onDraw, etc <3
                    step = function() 
                        topaz_entity__step(this.native);
                    end,
                    
                    draw = function() 
                        topaz_entity__step(this.native);
                    end,
    
                    attach = function(other) 
                        topaz_entity__attach(this.native, other.native);
                    end,
    
                    detach = function() 
                        topaz_entity__detach(this.native);
                    end,
                    
                    parent = {
                        get = function() 
                            local f = topaz_entity__get_parent(this.native);
                            if f.__ctx ~= nil then return f.__ctx; end
                            return thisclass.new({native=f});
                        end,
                        
                        set = function(v)
                            topaz_entity__attach(v.native, this.native);
                        end
                    },
                    
                    query = function(name) 
                        local f = topaz_entity__query(this.native, name);
                        if f.__ctx ~= nil then return f.__ctx end;
                        return thisclass.new({native=f});
                    end,
                    
                    
                    search = function(name)
                        local f = topaz_entity__search(this.native, name);
                        if f.__ctx ~= nil then return f.__ctx end;
                        return thisclass.new({native=f});                
                    end,
                    
                    priority = {
                        get = function() return topaz_entity__get_priority(this.native);end, 
                        set = function(v)topaz_entity__set_priority(this.native, v);end               
                    },
                    
        
                    setPriorityLast = function()
                        topaz_entity__set_priority_last(this.native);
                    end,
        
                    setPriorityFirst = function()
                        topaz_entity__set_priority_first(this.native);
                    end,
    
                    rotation = {
                        get = function()  return Vector.fromnative(topaz_entity__get_rotation(this.native));end,
                        set = function(v) Vector.statepush(v);topaz_entity__set_rotation(this.native, v.native);end
                    },

    
                    position = {
                        get = function()   return Vector.fromnative(topaz_entity__get_position(this.native)); end,
                        set = function(v)  Vector.statepush(v);topaz_entity__set_position(this.native, v.native); end
                    },
                    
                    scale = {
                        get = function()  return Vector.fromnative(topaz_entity__get_scale(this.native));end,
                        set = function(v) Vector.statepush(v);topaz_entity__set_scale(this.native, v.native);end
                    },
                    
                    globalPosition = {
                        get = function() return Vector.fromnative(topaz_entity__get_global_position(this.native));end
                    },
                    
                    isStepping = {
                        get = function() return topaz_entity__is_stepping(this.native); end
                    },
                    
                    isDrawing = {
                        get = function() return topaz_entity__is_drawing(this.native) end
                    },
                    
                    stepping = {
                        get = function()  return topaz_entity__get_stepping(this.native);end,
                        set = function(v) return topaz_entity__set_stepping(this.native, v);end
                    },
                    
                    drawing = {
                        get = function()  return topaz_entity__get_drawing(this.native);end,
                        set = function(v) return topaz_entity__set_drawing(this.native, v);end
                    },
                    
                    name = {
                        get = function()  return topaz_entity__get_name(this.native);end,
                        set = function(v) return topaz_entity__set_name(this.native, v);end
                    },
    
                    addComponent = function(c)
                        topaz_entity__add_component(this.native, c.native);
                    end,
                
                    addComponentAfter = function(c)
                        topaz_entity__add_component_after(this.native, c.native);
                    end,
                        
                    components = {
                        get = function()
                            local len = topaz_entity__get_component_count(this.native);
                            local out = {};
                            for i = 1, len do
                                local f = topaz_entity__get_nth_component(this.native, i-1);
                                if f.__ctx ~= nil then
                                    table.insert(out, f.__ctx);
                                else 
                                    table.insert(out, __Topaz__.Component.new({native=f}));
                                end
                            end
                            return out;
                        end,
                        set = function(c)
                            
                            while topaz_entity__get_component_count(this.native) > 0 do
                                topaz_entity__remove_component(this.native, topaz_entity__get_nth_component(this.native, 0));
                            end
    
                            for i = 1, #c do
                                topaz_entity__add_component(this.native, c[i].native);
                            end
                        end
                    },
    
    
                    queryComponent = function(tag) 
                        local f = topaz_entity__query_component(this.native, tag);
                        if f.__ctx ~= nil then return f.__ctx; end
                        return __Topaz__.Component({native=f});
                    end,
                
                    removeComponent = function(c)
                        return topaz_entity__remove_component(this.native, c.native);
                    end,
                    
                    onStep = {set = function(v)
                            topaz_entity__set_on_step(this.native, function() v() end);
    
                    end},
    
                    onDraw = {set = function(v)
                            topaz_entity__set_on_draw(this.native, function()v()end);
                    end},
    
                    onPreStep = {set = function(v)
                            topaz_entity__set_on_pre_step(this.native, function()v()end);
                    end},
    
                    onPreDraw = {set = function(v)
                            topaz_entity__set_on_pre_draw(this.native, function()v()end);
                    end},
                    
                    onAttach = {set = function(v)
                            topaz_entity__set_on_attach(this.native, function()v()end);
                    end},
    
                    onDetach = {set = function(v)
                            topaz_entity__set_on_detach(this.native, function()v()end);
                    end},
    
                    onRemove = {set = function(v)
                            topaz_entity__set_on_remove(this.native, function()v()end);
                    end}
    
                    
    
    
                
                });        
            end    
        });


        local __Component__ = tclass({
            inherits = __Native__,
            define = function(this, args, thisclass)
                -- some classes will inherit and overwrite the native instance, so a 
                -- local impl doesnt make sense.
                this.bindNative({
                    instance = args,
                    nativeCreate = topaz_component__create
                }, args.tag);
    
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
                            return __Topaz__.Entity.new({native=f});
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
                    end,
                    
                    
                    onStep = {set : function(v) 
                            topaz_component__set_on_step(this.native, function() v() end);
    
                    end},
    
                    onDraw = {set : function(v)
                            topaz_component__set_on_draw(this.native, function()v()end);
                    end},
                        
                    onAttach = {set : function(v) 
                            topaz_component__set_on_attach(this.native, function()v()end);
                    end},
    
                    onDetach = {set : function(v) 
                            topaz_component__set_on_detach(this.native, function()v()end);
                    end},
    
                    onDestroy = {set : function(v)
                            topaz_component__set_on_destroy(this.native, function()v()end);
                    end}
                    
                });
            end
        });
        

        
        
        local __Text2D__ = tclass({
            inherits = __Component__,
            define = function(this, args, thisclass)
                -- whoops, the component constructor already made a generic component native.
                -- destroy it and make a real one
                this.native.__ctx.destroy();
                
                local impl = this.bindNative({
                    instance = args,
                    nativeCreate = topaz_text2d__create
                });
                
                local fontState;
                local sizeState;
                
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
                        Color.statepush(c);
                        topaz_text2d__set_color(impl, c.native);
                    end,
                    
                    setColorSection = function(from, to, c) 
                        Color.statepush(c);
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
                        get = function() return Vector.fromnative(topaz_text2d__get_position(impl));end,
                        set = function(v)Vector.statepush(v);topaz_text2d__set_position(impl, v.native);end
                    },
                    
                    rotation = {
                        get = function() return Vector.fromnative(topaz_text2d__get_rotation(impl));end,
                        set = function(v)Vector.statepush(v);topaz_text2d__set_rotation(impl, v.native);end
                    },
                    
                    scale = {
                        get = function() return Vector.fromnative(topaz_text2d__get_scale(impl));end,
                        set = function(v)Vector.statepush(v);topaz_text2d__set_scale(impl, v.native);end
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
        
        local __Scheduler__ = tclass({
            inherits = __Component__,
            define = function(this, args, thisclass)
                -- whoops, the component constructor already made a generic component native.
                -- destroy it and make a real one
                this.native.__ctx.destroy();
                
                local impl = this.bindNative({
                    instance = args,
                    nativeCreate = topaz_scheduler__create
                }, args.type);
                
                this.interface({
                    startTask = function(args)
                        if args["taskName"] ~= nil then
                            topaz_scheduler__start_task(
                                impl,
                                args.taskName,
                                args.interval,
                                args.intervalDelay,
                                args.callback
                            );
                            return args.taskName;
                        else
                            return topaz_scheduler__start_task_simple(
                                impl,
                                args.interval,
                                args.callback
                            );
                        end
                    end,
                    
                    endTask = function(name)
                        topaz_scheduler__end_task(impl, name);
                    end,
                
                    pause = function(name)
                        topaz_scheduler__pause(impl, name);
                    end,
                    
                    resume = function(name) 
                        topaz_scheduler__resume(impl, name);
                    end,
                    
                    getTaskIntervalRemaining = function(name)
                        return topaz_scheduler__get_task_interval_remaining(impl, name);
                    end,
                    
                    tasks = {
                        get = function()
                            local out = {};
                            local len = topaz_scheduler__get_task_count(impl);
                            for i = 0, len-1 do
                                table.insert(out, topaz_schedluer__get_task(impl, i));
                            end
                            return out;
                        end
                    },
                    
                    
                });
            end 
        });
        
        local __StateControl__ = tclass({
            inherits = __Component__,
            define = function(this, args, thisclass)
                -- whoops, the component constructor already made a generic component native.
                -- destroy it and make a real one
                this.native.__ctx.destroy();
                
                local impl = this.bindNative({
                    instance = args,
                    nativeCreate = topaz_state_control__create
                });
                
                this.interface({
                    add = function(name, state)
                        topaz_state_control__add(impl, state.state, state.onStep, state.onDraw, state.onInit);
                    end,
                    
                    remove = function(name)
                        topaz_state_control__remove(impl, name);
                    end,
                    
                    execute = function(name)
                        topaz_state_control__execute(impl, name);
                    end,
                    
                    halt = function()
                        topaz_state_control__halt(impl)
                    end,
                    
                    isHalted = {
                        get = function()
                            return topaz_state_control__is_halted(impl);
                        end,
                    },
                    
                    state = {
                        get = function()
                            return topaz_state_control__get_current(impl);
                        end,
                    }                    
                    
                           
                });
            end
        });
        
        local __Object2D__ = tclass({
            inherits = __Component__,
            define = function(this, args, thisclass)
                -- whoops, the component constructor already made a generic component native.
                -- destroy it and make a real one
                this.native.__ctx.destroy();
                
                local impl = this.bindNative({
                    instance = args,
                    nativeCreate = topaz_object2d__create
                });
                
                thisclass.setGroupInteraction = function(a, b, c)
                    topaz_object2d__set_group_interaction(a, b, c);
                end
                
                local _collider;
                
                this.interface({
                    addVelocity = function(a, b) 
                        topaz_object2d__add_velocity(impl, a, b);
                    end,

                    addVelocityTowards = function(a, b, c) 
                        topaz_object2d__add_velocity_towards(impl, a, b, c);
                    end,

                    setVelocity = function(a, b) 
                        topaz_object2d__set_velocity(impl, a, b);
                    end,

                    setVelocityTowards = function(a, b, c) 
                        topaz_object2d__set_velocity_towards(impl, a, b, c);
                    end,




                    frictionX = {
                        get = function()  return topaz_object2d__get_friction_x(impl);end,
                        set = function(v) topaz_object2d__set_friction_x(impl, v);end
                    },

                    frictionY = {
                        get = function()  return topaz_object2d__get_friction_y(impl);end,
                        set = function(v) topaz_object2d__set_friction_y(impl, v);end
                    },

                    direction = {
                        get = function()  return topaz_object2d__get_direction(impl);end,
                        set = function(v)
                            topaz_object2d__set_velocity(
                                impl,
                                topaz_object2d__get_speed(impl),
                                topaz_object2d__get_direction(impl)
                            );
                        end
                    },

                    halt = function() topaz_object2d__halt(impl);end,
                    resetMotion = function() topaz_object2d__reset_motion(impl);end,

                    velocityX = {
                        get = function()  return topaz_object2d__get_velocity_x(impl);end,
                        set = function(v) topaz_object2d__set_velocity_x(impl, v);end
                    },

                    velocityY = {
                        get = function()  return topaz_object2d__get_velocity_y(impl);end,
                        set = function(v) topaz_object2d__set_velocity_y(impl, v);end
                    },

                    speed = {
                        get = function()  return topaz_object2d__get_speed(impl);end,
                        set = function(v) topaz_object2d__set_speed(impl, v);end
                    },

                    nextPosition = {
                        get = function()  return topaz_object2d__get_next_position(impl);end
                    },

                    group = {
                        get = function()  return topaz_object2d__get_group(impl);end,
                        set = function(v) topaz_object2d__set_group(impl, v);end
                    },

                    collider = {
                        get = function()  return _collider; end,
                        set = function(v) _collider=v; topaz_object2d__set_collider(impl, v);end
                    },

                    setColliderRadial = function(a, b) 
                        topaz_object2d__set_collider_radial(impl, a, b);
                        _collider = {};
                        local len = topaz_object2d__get_collider_len(impl);
                        for i = 0, len-1 do
                            local pt = topaz_object2d__get_collider_point(impl, i);
                            table.insert(_collider, pt.x);
                            table.insert(_collider, pt.y);
                        end
                    end,

                    lastCollided = {
                        get = function()  return __Topaz__.Entity.new({native=topaz_object2d__get_last_collided(impl)});end
                    }
                });                
            end
        });
        
        local __Shape2D__ = tclass({
            inherits = __Component__,
            define = function(this, args, thisclass)
                -- whoops, the component constructor already made a generic component native.
                -- destroy it and make a real one
                this.native.__ctx.destroy();
                
                local impl = this.bindNative({
                    instance = args,
                    nativeCreate = topaz_shape2d__create
                });
                
                local _lines;
                local _tris;
                
                this.interface({
                    color = {
                        get = function() return Color.fromnative(topaz_shape2d__get_color(impl));end,
                        set = function(v)Color.statepush(v);topaz_shape2d__set_color(impl, v.native);end
                    },


                    animSpeed = {
                        get = function() return topaz_shape2d__get_anim_speed(impl);end,
                        set = function(v)topaz_shape2d__set_anim_speed(impl, v);end
                    },

                    center = {
                        get = function() return Vector.fromnative(topaz_shape2d__get_center(impl));end,
                        set = function(v)Vector.statepush(v);topaz_shape2d__set_center(impl, v.native);end
                    },

                    position = {
                        get = function() return Vector.fromnative(topaz_shape2d__get_position(impl));end,
                        set = function(v)Vector.statepush(v);topaz_shape2d__set_position(impl, v.native);end
                    },

                    rotation = {
                        get = function() return Vector.fromnative(topaz_shape2d__get_rotation(impl));end,
                        set = function(v)Vector.statepush(v);topaz_shape2d__set_rotation(impl, v.native);end
                    },

                    scale = {
                        get = function() return Vector.fromnative(topaz_shape2d__get_scale(impl));end,
                        set = function(v)Vector.statepush(v);topaz_shape2d__set_scale(impl, v.native);end
                    },

                    lines = {
                        get = function() return _lines;end,
                        set = function(v)topaz_shape2d__form_lines(impl, v); _lines = v;end
                    },

                    triangles = {
                        get = function() return _tris;end,
                        set = function(v)topaz_shape2d__form_triangles(impl, v); _tris = v;end
                    },

                    setAttribute = function(a, b) 
                        topaz_shape2d__set_attribute(impl, a, b);
                    end,

                    getAttribute = function(a) 
                        return topaz_shape2d__set_attribute(impl, a);
                    end,

                    formRectangle = function(a, b)
                        topaz_shape2d__form_rectangle(impl, a, b);
                    end,

                    formImage = function(a) 
                        topaz_shape2d__form_image(impl, a.native);
                    end,

                    formImageFrame = function(a, b) 
                        topaz_shape2d__form_image_frame(impl, a.native, b);
                    end,

                    formImageScaled = function(a, b, c) 
                        topaz_shape2d__form_image(impl, a.native, b, c);
                    end,

                    formRadial = function(a, b) 
                        topaz_shape2d__form_radial(impl, a, b);
                    end,








                });
            end
        });
        
        
        local __Shape3D__ = tclass({
            inherits = __Component__,
            define = function(this, args, thisclass)
                -- whoops, the component constructor already made a generic component native.
                -- destroy it and make a real one
                this.native.__ctx.destroy();
                
                local impl = this.bindNative({
                    instance = args,
                    nativeCreate = topaz_shape3d__create
                });
                
                
                this.interface({
                    position = {
                        get = function() return Vector.fromnative(topaz_shape3d__get_position(impl));end,
                        set = function(v)Vector.statepush(v);topaz_shape3d__set_position(impl, v.native);end
                    },

                    rotation = {
                        get = function() return Vector.fromnative(topaz_shape3d__get_rotation(impl));end,
                        set = function(v)Vector.statepush(v);topaz_shape3d__set_rotation(impl, v.native);end
                    },

                    scale = {
                        get = function() return Vector.fromnative(topaz_shape3d__get_scale(impl));end,
                        set = function(v)Vector.statepush(v);topaz_shape3d__set_scale(impl, v.native);end
                    },

                    setAttribute = function(a, b)
                        topaz_shape3d__set_attribute(impl, a, b);
                    end,

                    getAttribute = function(a)
                        return topaz_shape3d__set_attribute(impl, a);
                    end,

                    setTexture = function(slot, t)
                        topaz_shape3d__set_texture(impl, slot, t.native);
                    end,

                    mesh = {
                        set = function(v)
                            topaz_shape3d__set_mesh(impl, v.native);            
                        end
                    },

                    sampleFramebuffer = {
                        set = function(v) 
                            topaz_shape3d__set_sample_framebuffer(impl, v.native);
                        end
                    },

                    material = {
                        set = function(v)
                            topaz_shape3d__set_material(impl, v.native);
                        end
                    }
                });  
                
            end
        });
        
        local __Automation__ = tclass({
            inherits = __Component__,
            define = function(this, args, thisclass)
                -- whoops, the component constructor already made a generic component native.
                -- destroy it and make a real one
                this.native.__ctx.destroy();
                
                local impl = this.bindNative({
                    instance = args,
                    nativeCreate = topaz_automation__create
                });
                
                this.interface({
                    addKeyframe = function(value, fn, offset)
                        topaz_automation__add_keyframe(impl, value, fn, offset);
                    end,

                    addVectorKeyframe = function(value, fn, offset)
                        topaz_automation__add_vector_keyframe(impl, value.native, fn, offset);
                    end,

                    clear = function()
                        topaz_automation__clear(impl);
                    end,

                    blend = function(other)
                        topaz_automation__blend(impl, other.native);
                    end,

                    smooth = function()
                        topaz_automation__smooth(impl);
                    end,

                    addFromString = function(str)
                        topaz_automation__add_from_string(impl, str);
                    end,

                    skipTo = function(n)
                        topaz_automation__skip_to(impl, n);
                    end,

                    resume = function() 
                        topaz_automation__resume(impl);
                    end,

                    pause = function() 
                        topaz_automation__pause(impl);
                    end,

                    vectorAt = function(at) 
                        return Vector.fromnative(topaz_automation__vector_at(impl, at));
                    end,

                    at = function(a) 
                        return topaz_automation__at(impl, at);
                    end,

                    vector = {
                        get = function() return Vector.fromnative(topaz_automation__current_vector(impl))end,
                    },

                    value = {
                        get = function() return topaz_automation__current(impl);end,
                    },

                    string = {
                        get = function() return topaz_automation__to_string(impl);end,
                        set = function(v) return topaz_automation__set_from_string(impl, v);end,
                    },

                    length = {
                        get = function() return topaz_automation__get_length(impl);end,
                    },

                    durationFrames = {
                        set = function(v) topaz_automation__set_duration_frames(impl, v);end
                    },

                    durationSeconds = {
                        set = function(v) topaz_automation__set_duration_seconds(impl, v);end
                    },

                    duration = {
                        get = function() return topaz_automation__get_duration(impl);end,
                    },

                    looped = {
                        get = function() return topaz_automation__get_looped(impl);end,
                        set = function(v) return topaz_automation__set_looped(impl, v);end,
                    },

                    speed = {
                        get = function() return topaz_automation__get_speed(impl);end,
                        set = function(v) return topaz_automation__set_speed(impl, v);end,
                    }
                });
            end
        });
        
        local __Particle__ = tclass({
            inherits = __Native__,
            define = function(this, args, thisclass)

                local impl = this.bindNative({
                    nativeCreate = topaz_particle__create
                });
                
                this.interface({
                    setAttribute = function(param, val) 
                        topaz_particle__set_attribute(impl, param, val);
                    end,

                    setNoiseMin = function(p, val) 
                        topaz_particle__set_noise_min(impl, p, val);
                    end,

                    setNoiseMax = function(p, val) 
                        topaz_particle__set_noise_max(impl, p, val);
                    end,

                    setFunction = function(p, val) 
                        topaz_particle__set_function(impl, p, val);
                    end,

                    string = {get = function() return topaz_particle__to_string(impl)end, set = function(v)topaz_particle__set_from_string(impl, v);end},
                    image =  {set = function(v) return topaz_particle__set_image(impl, v)end}
                });
            end        
        });
        
        local __ParticleEmitter2D__  = tclass({
            inherits = __Entity__,
            define = function(this, args, thisclass)
                local impl = this.native;

                this.interface({

                    particle = {set = function(v)topaz_particle_emitter_2d__set_particle(impl, v.native);end},
                    independent = {set = function(v)topaz_particle_emitter_2d__set_independent(impl, v);end},

                    emit = function(c)
                        topaz_particle_emitter_2d__emit(impl, c);
                    end,
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
            attachPreManager = function(v)topaz__attach_pre_manager(v.native);end,        
            attachPreManagerUnpausable = function(v)topaz__attach_pre_manager_unpausable(v.native);end,        
            attachPostManager = function(v)topaz__attach_post_manager(v.native);end,        
            attachPostManagerUnpausable = function(v)topaz__attach_post_manager_unpausable(v.native);end,        
            quit = topaz__quit,
            wait = topaz__wait,
            import = topaz_script__import,
            log = topaz__log,
            toBase64 = topaz__to_base64,
            fromBase64 = function(f) 
                return __Topaz__.Data.new({native=topaz__from_base64(f)});
            end,
            
            root = {
                set = function(e)
                    return topaz__set_root(e.native)
                end,

                get = function(e)
                    local o = topaz__get_root();
                    if o ~= nil 
                        return o.__ctx;
                    end
                    return nil;
                end
            },
            
            isPaused = {
                get = function()
                    return topaz__is_paused();
                end            
            },
            
            time = {
                get = function()
                    return topaz__get_time();
                end
            },
            
            versionMicro = {
                get = function() 
                    return topaz__get_version_micro();                
                end
            },
            versionMajor = {
                get = function() 
                    return topaz__get_version_major();                
                end
            },
            versionMinor = {
                get = function() 
                    return topaz__get_version_minor();                
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
                        return tostring(obj);            
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
            Component   = {get = function()return __Component__ end},
            Resources   = {get = function()return __Resources__ end},
            FontManager = {get = function()return __FontManager__ end},
            Image       = {get = function()return __Image__ end},
            Data        = {get = function()return __Data__ end},
            Sound       = {get = function()return __Sound__ end},
            Text2D      = {get = function()return __Text2D__ end},
            Scheduler   = {get = function()return __Scheduler__ end},
            StateControl= {get = function()return __StateControl__ end},
            Object2D    = {get = function()return __Object2D__ end},
            Shape2D     = {get = function()return __Shape2D__ end},
            Shape3D     = {get = function()return __Shape3D__ end},
            Automation  = {get = function()return __Automation__ end},
            Particle    = {get = function()return __Particle__ end},
            ParticleEmitter2D = {get = function()return __ParticleEmitter2D__ end}
       });
    end
}).new();
