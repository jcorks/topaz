extern var topaz_;

/*
    Some maintenance notes for gravity:
    
        
    -   "self" is the keyword for self-refrential 

    -   function object instances (closures) do NOT have default bindings even 
        if those closures come from instances i.e. "myObject.method".
        the closure needs to either be bound manually with .bind or 
        an explicit binding with .apply
        
    -   obj.load(arg) where arg is a closure WILL change the scope context to that 
        closure, so watch out.
        
    -   Nested classes are privately accessible to its owning class only.
    
    -   Namespaces can be implemented using Maps in conjunction with 
        meta class objects. I.e.:
        
            class Interface_Implementation {
                class InternalStuff {
                    public var a;        
                    
                    func init() {
                        a = 100;
                    }       
                }

                func make() {
                    return [
                        "Test" : InternalStuff
                    ];    
                }
            }

            var Interface = Interface_Implementation().make();





            func main() {
                var test = Interface.Test();
                Topaz.log(test.a);
            }
        

*/

class Topz {
    var run;
    var pause;
    var resume;
    var iterate;
    var step;
    var draw;
    var toBase64;
    func fromBase64(f) {
        var out = Data();
        out.impl_ = topaz_.topaz__from_base64(f);
        return out;
    }

    var isPaused {
        get {
            return topaz_.topaz__is_paused();
        }
    }

    var root {
        get {
            return topaz_.topaz__get_root();
        }
        set {
            topaz_.topaz__set_root(value.impl_);
        }
    }

    var attachPreManager;
    var attachPreManagerUnpausable;
    var attachPostManager;
    var attachPostManagerUnpausable;
    var quit;
    var wait;

    var time {
        get {
            return topaz_.topaz__get_time();
        }
    }

    var versionMicro {
        get { return topaz_.topaz__get_version_micro(); }
    }

    var versionMinor {
        get { return topaz_.topaz__get_version_minor(); }
    }

    var versionMajor {
        get { return topaz_.topaz__get_version_major(); }
    }

    func log(a, b) {
        if (b == false) {
            topaz_.topaz__log(''+a, false);
        } else {
            topaz_.topaz__log(''+a, true);
        }
    }    




    /// vector 
    class Vector_Definition {
        // effectively private.
        var impl_;

        func init(x_, y_, z_) {
            if (x_ is String) {
                impl_ = topaz_.topaz_vector__create(x_);                    
            } else {
                if (z_ == undefined) {
                    impl_ = topaz_.topaz_vector__create(x_, y_, 0);                                    
                } else {
                    impl_ = topaz_.topaz_vector__create(x_, y_, z_);
                }
            }

        }    
        func getDistance(other) {
            return topaz_.topaz_vector__get_distance(impl_, other.impl_);
        }

        func normalize() {
            topaz_.topaz_vector__normalize(impl_);
        }

        func cross(other) {
            var out = Vector();
            out.impl_ = topaz_.topaz_vector__cross(impl_, other.impl_);
            return out;
        }

        func floor(other) {
            topaz_.topaz_vector__floor(impl_);
        }


        var length {
            get {return topaz_.topaz_vector__get_length(impl_);}
        }    


        

        
        
        
        func rotationXdiff(other) {
            return topaz_.topaz_vector__rotation_x_diff(impl_, other.impl_);
        }   
        
        func rotationXDiffRelative(other) {
            return topaz_.topaz_vector__rotation_x_diff_relative(impl_, other.impl_);
        }   

        func rotationX() {
            return topaz_.topaz_vector__rotation_x(impl_);
        }        
        


        func rotationYdiff(other) {
            return topaz_.topaz_vector__rotation_y_diff(impl_, other.impl_);
        }   
        
        func rotationYDiffRelative(other) {
            return topaz_.topaz_vector__rotation_y_diff_relative(impl_, other.impl_);
        }   

        func rotationY() {
            return topaz_.topaz_vector__rotation_y(impl_);
        }        




        func rotationZdiff(other) {
            return topaz_.topaz_vector__rotation_z_diff(impl_, other.impl_);
        }   
        
        func rotationZDiffRelative(other) {
            return topaz_.topaz_vector__rotation_z_diff_relative(impl_, other.impl_);
        }   

        func rotationZ() {
            return topaz_.topaz_vector__rotation_z(impl_);
        }        




        func rotateX() {
            topaz_.topaz_vector__rotate_x(impl_);
        }
        func rotateY() {
            topaz_.topaz_vector__rotate_y(impl_);
        }
        func rotateZ() {
            topaz_.topaz_vector__rotate_z(impl_);
        }


        func String() {
            return '(' + x + ',' + y + ',' + z + ')';
        }

        func + (other) {
            if (other is Topaz.Vector) {
                return Topaz.Vector(
                    x + other.x,
                    y + other.y,
                    z + other.z
                );
            } else {
                return Topaz.Vector(
                    x + other,
                    y + other,
                    z + other
                );
            }
        }

        func * (other) {
            if (other is Topaz.Vector) {
                return Topaz.Vector(
                    x * other.x,
                    y * other.y,
                    z * other.z
                );
            } else {
                return Topaz.Vector(
                    x * other,
                    y * other,
                    z * other
                );
            }
        }

        func - (other) {
            if (other is Topaz.Vector) {
                return Topaz.Vector(
                    x - other.x,
                    y - other.y,
                    z - other.z
                );
            } else {
                return Topaz.Vector(
                    x - other,
                    y - other,
                    z - other
                );
            }
        }

        func / (other) {
            if (other is Topaz.Vector) {
                return Topaz.Vector(
                    x / other.x,
                    y / other.y,
                    z / other.z
                );
            } else {
                return Topaz.Vector(
                    x / other,
                    y / other,
                    z / other
                );
            }
        }




        
        var x {
            set {topaz_.topaz_vector__set_x(impl_, value);}
            get {return topaz_.topaz_vector__get_x(impl_);}
        }
        
        var y {
            set {topaz_.topaz_vector__set_y(impl_, value);}
            get {return topaz_.topaz_vector__get_y(impl_);}
        }

        var z {
            set {topaz_.topaz_vector__set_z(impl_, value);}
            get {return topaz_.topaz_vector__get_z(impl_);}
        }


    }
    var Vector;



    /////// color 
    class Color_Definition {
        var impl_;
        func init(r_, g_, b_, a_) {
            impl_ = topaz_.topaz_color__create();
            if (r_.length > 0) {
                string = r_;
            } else  {
                if (r_ != null) r = r_;
                if (g_ != null) g = g_;
                if (b_ != null) b = b_;
                if (a_ != null) a = a_;
            }
        }

        var string {
            get {
                return topaz_.topaz_color__to_hex_string(impl_);
            }

            set {
                topaz_.topaz_color__set_from_string(impl_, value);
            }
        }

        var r {
            get {return topaz_.topaz_color__get_r(impl_);}
            set {       topaz_.topaz_color__set_r(impl_, value);}
        }
        var g {
            get {return topaz_.topaz_color__get_g(impl_);}
            set {       topaz_.topaz_color__set_g(impl_, value);}
        }
        var b {
            get {return topaz_.topaz_color__get_b(impl_);}
            set {       topaz_.topaz_color__set_b(impl_, value);}
        }
        var a {
            get {return topaz_.topaz_color__get_a(impl_);}
            set {       topaz_.topaz_color__set_a(impl_, value);}
        }


        func String() {
            return string;
        }
    }
    var Color;



    /////////////////
    ////////// audio
    class Audio_Definition {
        class PlaybackSound_Definition {
            var impl_;
            
            func init(id) {
                impl_ = id;
            }   
            
            var volume {
                set {
                    topaz_.topaz_audio__playback_set_volume(impl_, value);
                }
            }

            var panning {
                set {
                    topaz_.topaz_audio__playback_set_panning(impl_, value);
                }
            }

            var repeatSound {
                set {
                    topaz_.topaz_audio__playback_set_repeat(impl_, value);
                }
            }

            var paused {
                set {
                    if (value)
                        topaz_.topaz_audio__playback_pause(impl_);
                    else 
                        topaz_.topaz_audio__playback_resume(impl_);
                }
            }
            
            
            var seek {
                set {
                    topaz_.topaz_audio__playback_seek(impl_, value);            
                }
            }

            func stop() {
                topaz_.topaz_audio__playback_stop(impl_);
            }


        }    

        func playSound(asset, channel) {
            return PlaybackSound_Definition(topaz_.topaz_audio__play_sound(asset.impl_, channel == undefined ? 0 : channel));
        }
        
        var channelHalt;
        var channelSetVolume;
        var channelSetPanning;

        var PlaybackSound;
        func init() {
            channelHalt = topaz_.topaz_audio__channel_halt;
            channelSetPanning = topaz_.topaz_audio__channel_set_volume;
            channelSetPanning = topaz_.topaz_audio__channel_set_panning;
            PlaybackSound = PlaybackSound_Definition;
        }
    };
    var Audio;






    ////////// resources
    /////
    class Resources_Definition {
        var AssetType;

        func init() {
            AssetType = [
                "none"  : 0,
                "image" : 1,
                "sound" : 2,
                "model" : 3,
                "particle" : 4,
                "data" : 5,
                "actor" : 6,
                "count" : 7
            ];
        }



        var path {
            get {
                return topaz_.topaz_resources__get_path();
            }   

            set {
                topaz_.topaz_resources__set_path(value);
            }
        }


        func loadAsset(a, b, c) {
            if (c == '') c = b;
            var impl = topaz_.topaz_resources__load_asset(a, b, c);
            var out;
            if (!impl) return null;

            switch(topaz_.topaz_asset__get_type(impl)) {
            case AssetType.data:
                out = Topaz.Data();
                out.impl_ = impl;
                return out;

            case AssetType.image:
                out = Topaz.Image();
                out.impl_ = impl;
                return out;

            case AssetType.sound:
                out = Topaz.Sound();
                out.impl_ = impl;
                return out;

            }
            return null;
        }

        func loadAssetData(a, b, c) {
            if (c == '') c = b;
            var impl = topaz_.topaz_resources__load_asset_data(a, b, c);
            var out;
            if (!impl) return null;
            switch(topaz_.topaz_asset__get_type(impl)) {
            case AssetType.data:
                out = Topaz.Data();
                out.impl_ = impl;
                return out;

            case AssetType.image:
                out = Topaz.Image();
                out.impl_ = impl;
                return out;

            case AssetType.sound:
                out = Topaz.Sound();
                out.impl_ = impl;
                return out;

            }
            return null;
        }


        func fetchAsset(a, b) {
            var impl = topaz_.topaz_resources__fetch_asset(a, b);
            var out;
            if (!impl) return null;
            switch(topaz_.topaz_asset__get_type(impl)) {
            case AssetType.data:
                out = Topaz.Data();
                out.impl_ = impl;
                return out;

            case AssetType.image:
                out = Topaz.Image();
                out.impl_ = impl;
                return out;

            case AssetType.sound:
                out = Topaz.Sound();
                out.impl_ = impl;
                return out;

            }
            return null;
        }
        
        func writeAsset(asset, name, ext) {
            return topaz_.topaz_resources__write_asset(asset.impl_, name, ext);
        }

        func removeAsset(a) {
            topaz_.topaz_resources__remove_asset(a);
        }

        func isExtensionSupported(a) {
            topaz_.topaz_resources__is_extension_supported(a);
        }    

        func queryAssetPaths() {
            topaz_.topaz_resources__query_asset_paths();
        }

        var assetPaths {
            get {
                var out = [];
                var len = topaz_.topaz_resources__asset_path_count();
                for(var i in 0..<len) {
                    out.push(topaz_.topaz_resources__nth_asset_path(i));
                }
                return out;
            }
        }    

    }
    var Resources;





    class Input_Definition {
        var topazNotAnInput;
        var topazKey_0; ///< 0
        var topazKey_1; ///< 1
        var topazKey_2; ///< 2
        var topazKey_3; ///< 3
        var topazKey_4; ///< 4
        var topazKey_5; ///< 5
        var topazKey_6; ///< 6
        var topazKey_7; ///< 7
        var topazKey_8; ///< 8
        var topazKey_9; ///< 9
        var topazKey_a; ///< a
        var topazKey_b; ///< b
        var topazKey_c; ///< c
        var topazKey_d; ///< d
        var topazKey_e; ///< e
        var topazKey_f; ///< f
        var topazKey_g; ///< g
        var topazKey_h; ///< h
        var topazKey_i; ///< i
        var topazKey_j; ///< j
        var topazKey_k; ///< k
        var topazKey_l; ///< l
        var topazKey_m; ///< m
        var topazKey_n; ///< n
        var topazKey_o; ///< o
        var topazKey_p; ///< p
        var topazKey_q; ///< q
        var topazKey_r; ///< r
        var topazKey_s; ///< s
        var topazKey_t; ///< t
        var topazKey_u; ///< u
        var topazKey_v; ///< v
        var topazKey_w; ///< w
        var topazKey_x; ///< x
        var topazKey_y; ///< y
        var topazKey_z; ///< z
        var topazKey_lshift; ///< Left shift key
        var topazKey_rshift; ///< Right shift key
        var topazKey_lctrl;  ///< Left control key
        var topazKey_rctrl;  ///< Right control key
        var topazKey_lalt;   ///< Left alt key
        var topazKey_ralt;   ///< Right alt key
        var topazKey_tab;    ///< Tab
        var topazKey_F1;     ///< F1
        var topazKey_F2;     ///< F2
        var topazKey_F3;     ///< F3
        var topazKey_F4;     ///< F4
        var topazKey_F5;     ///< F5
        var topazKey_F6;     ///< F6
        var topazKey_F7;     ///< F7
        var topazKey_F8;     ///< F8
        var topazKey_F9;     ///< F9
        var topazKey_F10;    ///< F10
        var topazKey_F11;    ///< F11
        var topazKey_F12;    ///< F12
        var topazKey_up;     ///< Up arrow
        var topazKey_down;   ///< Down arrow
        var topazKey_left;   ///< Left arrow
        var topazKey_right;  ///< Right arrow
        var topazKey_minus;  ///< -
        var topazKey_equal;  ///< = 
        var topazKey_backspace;  ///< Backspace
        var topazKey_grave;  ///< `
        var topazKey_esc;    ///< Escape
        var topazKey_home;   ///< Home key
        var topazKey_pageUp; ///< Page up key
        var topazKey_pageDown;  ///< Page down key
        var topazKey_end;    ///< End key
        var topazKey_backslash; ///< '\'
        var topazKey_lbracket; ///< [
        var topazKey_rbracket; ///< ]
        var topazKey_semicolon; ///< ;
        var topazKey_apostrophe; ///< '
        var topazKey_frontslash; ///< /
        var topazKey_enter; ///< Enter
        var topazKey_delete; ///< Delete
        var topazKey_numpad0; ///< Numpad 0
        var topazKey_numpad1; ///< Numpad 1
        var topazKey_numpad2; ///< Numpad 2
        var topazKey_numpad3; ///< Numpad 3
        var topazKey_numpad4; ///< Numpad 4
        var topazKey_numpad5; ///< Numpad 5
        var topazKey_numpad6; ///< Numpad 6
        var topazKey_numpad7; ///< Numpad 7
        var topazKey_numpad8; ///< Numpad 8
        var topazKey_numpad9; ///< Numpad 9
        var topazKey_prtscr; ///< Print screen button
        var topazKey_lsuper; ///< Left Super key (Windows key)
        var topazKey_rsuper; ///< Right Super key (Windows key)
        var topazKey_space;  ///< Space
        var topazKey_insert; ///< Insert key
        var topazKey_comma;///< ,
        var topazKey_period;///< .
        var topazKey_world1;/// I8n key0
        var topazKey_world2;/// I8n key1
        var topazKey_world3;/// I8n key2
        var topazKey_world4;/// I8n key3
        var topazKey_world5;/// I8n key4
        var topazKey_world6;/// I8n key5
        var topazKey_world7;/// I8n key6
        var topazKey_world8;/// I8n key7
        var topazKey_world9;/// I8n key8
    
        var topazPointer_0;///< Left click
        var topazPointer_1;///< Right click
        var topazPointer_2;///< Middle click
    
        var topazPointer_X;///< Horizontal axis. Usually for the X axis of the pointer
        var topazPointer_Y;///< Horizontal axis. Usually for the X axis of the pointer
        var topazPointer_Wheel;///< Mouse wheel.
    
        var topazPad_a;///< Button 0
        var topazPad_b;///< Button 1
        var topazPad_c;///< Button 2
        var topazPad_x;///< Button 3
        var topazPad_y;///< Button 4
        var topazPad_r;///< Button 5
        var topazPad_l;///< Button 6
        var topazPad_r2;///< Button 7
        var topazPad_l2;///< Button 8
        var topazPad_r3;///< Button 9
        var topazPad_l3;///< Button 10
        var topazPad_start;///< Button 11
        var topazPad_select;///< Button 12
        var topazPad_b13;///< Button 13
        var topazPad_b14;///< Button 14
        var topazPad_b15;///< Button 15
        var topazPad_b16;///< Button 16
        var topazPad_b17;///< Button 17
        var topazPad_b18;///< Button 18
        var topazPad_b19;///< Button 19
        var topazPad_b20;///< Button 20
        var topazPad_b21;///< Button 21
        var topazPad_b22;///< Button 22
        var topazPad_b23;///< Button 23
        var topazPad_b24;///< Button 24
        var topazPad_b25;///< Button 25
        var topazPad_b26;///< Button 26
        var topazPad_b27;///< Button 27
        var topazPad_b28;///< Button 28
        var topazPad_b29;///< Button 29
        var topazPad_b30;///< Button 30
        var topazPad_b31;///< Button 31
        var topazPad_b32;///< Button 32
    
        var topazPad_axisX;///< X button
        var topazPad_axisY;///< Y button
        var topazPad_axisZ;///< Z button
        var topazPad_axisX2;///< X2 button 
        var topazPad_axisY2;///< Y2 button
        var topazPad_axisZ2;///< Z2 button
        var topazPad_axisX3;///< X3 button
        var topazPad_axisY3;///< Y3 button
        var topazPad_axisZ3;///< Z3 button
        var topazPad_axisX4;///< X4 button
        var topazPad_axisY4;///< Y4 button
        var topazPad_axisZ4;///< Z4 button
        var topazPad_axisX5;///< X4 button
        var topazPad_axisY5;///< Y4 button
        var topazPad_axisZ5;///< Z4 button
        var topazPad_axisX6;///< X4 button
        var topazPad_axisY6;///< Y4 button
        var topazPad_axisZ6;///< Z4 button
    
        var topazPad_axisR;
        var topazPad_axisL;
        var topazPad_axisR2;
        var topazPad_axisL2;
        var topazPad_axisR3;
        var topazPad_axisL3;
        var topazPad_axisR4;
        var topazPad_axisL4;
    
        var topazPad_options;
        var topazInput_Count;



        var addKeyboardListener;
        var addPadListener
        var addPointerListener;
        var addMappedListener;
        var getState;
        var getPadState;
        var getMappedState;
        var getDeadzone;
        var queryPadCount
        var queryPadID;
        var addUnicodeListener;


        var mouse {
            get {
                return Topaz.Vector(
                    topaz_.topaz_input__mouse_x(),
                    topaz_.topaz_input__mouse_y()
                );
            }
        }

        var mouseDelta {
            get {
                return Topaz.Vector(
                    topaz_.topaz_input__mouse_delta_x(),
                    topaz_.topaz_input__mouse_delta_y()
                );
            }
        }

        var mouseWheel {
            get {
                return topaz_.topaz_input__mouse_wheel();
            }
        }
    


        func init() {
            addKeyboardListener = topaz_.topaz_input__add_keyboard_listener;
            addUnicodeListener = topaz_.topaz_input__add_unicode_listener;
            queryPadID = topaz_.topaz_input__query_pad_id;
            queryPadCount = topaz_.topaz_input__query_pad_count;
            getDeadzone = topaz_.topaz_input__set_deadzone;
            getMappedState = topaz_.topaz_input__get_mapped_state;
            getPadState = topaz_.topaz_input__get_pad_state;
            getState = topaz_.topaz_input__get_state;
            addMappedListener = topaz_.topaz_input__add_mapped_listener;
            addPointerListener = topaz_.topaz_input__add_pointer_listener;
            addPadListener = topaz_.topaz_input__add_pad_listener;

            
            topazNotAnInput = 0;
            topazKey_0 = 1; ///< 0
            topazKey_1 = 2; ///< 1
            topazKey_2 = 3; ///< 2
            topazKey_3 = 4; ///< 3
            topazKey_4 = 5; ///< 4
            topazKey_5 = 6; ///< 5
            topazKey_6 = 7; ///< 6
            topazKey_7 = 8; ///< 7
            topazKey_8 = 9; ///< 8
            topazKey_9 = 10; ///< 9
            topazKey_a = 11; ///< a
            topazKey_b = 12; ///< b
            topazKey_c = 13; ///< c
            topazKey_d = 14; ///< d
            topazKey_e = 15; ///< e
            topazKey_f = 16; ///< f
            topazKey_g = 17; ///< g
            topazKey_h = 18; ///< h
            topazKey_i = 19; ///< i
            topazKey_j = 20; ///< j
            topazKey_k = 21; ///< k
            topazKey_l = 22; ///< l
            topazKey_m = 23; ///< m
            topazKey_n = 24; ///< n
            topazKey_o = 25; ///< o
            topazKey_p = 26; ///< p
            topazKey_q = 27; ///< q
            topazKey_r = 28; ///< r
            topazKey_s = 29; ///< s
            topazKey_t = 30; ///< t
            topazKey_u = 31; ///< u
            topazKey_v = 32; ///< v
            topazKey_w = 33; ///< w
            topazKey_x = 34; ///< x
            topazKey_y = 35; ///< y
            topazKey_z = 36; ///< z
            topazKey_lshift = 37; ///< Left shift key
            topazKey_rshift = 38; ///< Right shift key
            topazKey_lctrl = 39;  ///< Left control key
            topazKey_rctrl = 40;  ///< Right control key
            topazKey_lalt = 41;   ///< Left alt key
            topazKey_ralt = 42;   ///< Right alt key
            topazKey_tab = 43;    ///< Tab
            topazKey_F1 = 44;     ///< F1
            topazKey_F2 = 45;     ///< F2
            topazKey_F3 = 46;     ///< F3
            topazKey_F4 = 47;     ///< F4
            topazKey_F5 = 48;     ///< F5
            topazKey_F6 = 49;     ///< F6
            topazKey_F7 = 50;     ///< F7
            topazKey_F8 = 51;     ///< F8
            topazKey_F9 = 52;     ///< F9
            topazKey_F10 = 53;    ///< F10
            topazKey_F11 = 54;    ///< F11
            topazKey_F12 = 55;    ///< F12
            topazKey_up = 100;     ///< Up arrow
            topazKey_down = 101;   ///< Down arrow
            topazKey_left = 102;   ///< Left arrow
            topazKey_right = 103;  ///< Right arrow
            topazKey_minus = 104;  ///< -
            topazKey_equal = 105;  ///< = 
            topazKey_backspace = 106;  ///< Backspace
            topazKey_grave = 107;  ///< `
            topazKey_esc = 108;    ///< Escape
            topazKey_home = 109;   ///< Home key
            topazKey_pageUp = 110; ///< Page up key
            topazKey_pageDown = 111;  ///< Page down key
            topazKey_end = 112;    ///< End key
            topazKey_backslash = 113; ///< '\'
            topazKey_lbracket = 114; ///< [
            topazKey_rbracket = 115; ///< ]
            topazKey_semicolon = 116; ///< ;
            topazKey_apostrophe = 117; ///< '
            topazKey_frontslash = 118; ///< /
            topazKey_enter = 119; ///< Enter
            topazKey_delete = 120; ///< Delete
            topazKey_numpad0 = 121; ///< Numpad 0
            topazKey_numpad1 = 122; ///< Numpad 1
            topazKey_numpad2 = 123; ///< Numpad 2
            topazKey_numpad3 = 124; ///< Numpad 3
            topazKey_numpad4 = 125; ///< Numpad 4
            topazKey_numpad5 = 126; ///< Numpad 5
            topazKey_numpad6 = 127; ///< Numpad 6
            topazKey_numpad7 = 128; ///< Numpad 7
            topazKey_numpad8 = 129; ///< Numpad 8
            topazKey_numpad9 = 130; ///< Numpad 9
            topazKey_prtscr = 131; ///< Print screen button
            topazKey_lsuper = 132; ///< Left Super key (Windows key)
            topazKey_rsuper = 133; ///< Right Super key (Windows key)
            topazKey_space = 134;  ///< Space
            topazKey_insert = 135; ///< Insert key
            topazKey_comma = 136; ///< ,
            topazKey_period = 137; ///< .
            topazKey_world1 = 138; /// I8n key0
            topazKey_world2 = 139; /// I8n key1
            topazKey_world3 = 140; /// I8n key2
            topazKey_world4 = 141; /// I8n key3
            topazKey_world5 = 142; /// I8n key4
            topazKey_world6 = 143; /// I8n key5
            topazKey_world7 = 144; /// I8n key6
            topazKey_world8 = 145; /// I8n key7
            topazKey_world9 = 146; /// I8n key8
        
            topazPointer_0 = 256; ///< Left click
            topazPointer_1 = 257; ///< Right click
            topazPointer_2 = 258; ///< Middle click
        
            topazPointer_X = 259; ///< Horizontal axis. Usually for the X axis of the pointer
            topazPointer_Y = 260; ///< Horizontal axis. Usually for the X axis of the pointer
            topazPointer_Wheel = 261; ///< Mouse wheel.
        
            topazPad_a = 300;     ///< Button 0
            topazPad_b = 301;     ///< Button 1
            topazPad_c = 302;     ///< Button 2
            topazPad_x = 303;     ///< Button 3
            topazPad_y = 304;     ///< Button 4
            topazPad_r = 305;     ///< Button 5
            topazPad_l = 306;     ///< Button 6
            topazPad_r2 = 307;    ///< Button 7
            topazPad_l2 = 308;    ///< Button 8
            topazPad_r3 = 309;    ///< Button 9
            topazPad_l3 = 310;    ///< Button 10
            topazPad_start = 311;    ///< Button 11
            topazPad_select = 312;///< Button 12
            topazPad_b13 = 313;///< Button 13
            topazPad_b14 = 314;///< Button 14
            topazPad_b15 = 315;///< Button 15
            topazPad_b16 = 316;///< Button 16
            topazPad_b17 = 317;///< Button 17
            topazPad_b18 = 318;///< Button 18
            topazPad_b19 = 319;///< Button 19
            topazPad_b20 = 320;///< Button 20
            topazPad_b21 = 321;///< Button 21
            topazPad_b22 = 322;///< Button 22
            topazPad_b23 = 323;///< Button 23
            topazPad_b24 = 324;///< Button 24
            topazPad_b25 = 325;///< Button 25
            topazPad_b26 = 326;///< Button 26
            topazPad_b27 = 327;///< Button 27
            topazPad_b28 = 328;///< Button 28
            topazPad_b29 = 329;///< Button 29
            topazPad_b30 = 330;///< Button 30
            topazPad_b31 = 331;///< Button 31
            topazPad_b32 = 332;///< Button 32
        
            topazPad_axisX = 400; ///< X button
            topazPad_axisY = 401; ///< Y button
            topazPad_axisZ = 402; ///< Z button
            topazPad_axisX2 = 403;///< X2 button 
            topazPad_axisY2 = 404;///< Y2 button
            topazPad_axisZ2 = 405;///< Z2 button
            topazPad_axisX3 = 406;///< X3 button
            topazPad_axisY3 = 407;///< Y3 button
            topazPad_axisZ3 = 408;///< Z3 button
            topazPad_axisX4 = 409;///< X4 button
            topazPad_axisY4 = 410;///< Y4 button
            topazPad_axisZ4 = 411;///< Z4 button
            topazPad_axisX5 = 412;///< X4 button
            topazPad_axisY5 = 413;///< Y4 button
            topazPad_axisZ5 = 414;///< Z4 button
            topazPad_axisX6 = 415;///< X4 button
            topazPad_axisY6 = 416;///< Y4 button
            topazPad_axisZ6 = 417;///< Z4 button
        
            topazPad_axisR = 450;   
            topazPad_axisL = 451;    
            topazPad_axisR2 = 452;    
            topazPad_axisL2 = 453;    
            topazPad_axisR3 = 454;    
            topazPad_axisL3 = 455;    
            topazPad_axisR4 = 456;    
            topazPad_axisL4 = 457;    
        
            topazPad_options = 511;
            topazInput_Count = 512;
        }
        
    }
    var Input;



    // asset
    class Asset_Definition {
        var impl_;
        var name {
            get {return topaz_.topaz_asset__get_name(impl_)}
        }

        var type {
            get {return topaz_.topaz_asset__get_type(impl_)}
        }
    }
    var Asset;




    class Entity {    
        var impl_;
        
        
                
        func init() {
            impl_ = topaz_.topaz_entity__create();
            impl_.api_ = self;


            topaz_.topaz_entity__set_on_step(impl_, onStep_wrapper);
            topaz_.topaz_entity__set_on_draw(impl_, onDraw_wrapper);
            topaz_.topaz_entity__set_on_pre_step(impl_, onPreStep_wrapper);
            topaz_.topaz_entity__set_on_pre_draw(impl_, onPreDraw_wrapper);
            topaz_.topaz_entity__set_on_remove(impl_, onRemove_wrapper);
            topaz_.topaz_entity__set_on_attach(impl_, onAttach_wrapper);
            topaz_.topaz_entity__set_on_detach(impl_, onDetach_wrapper);

            onReady();

        }

        func onStep_wrapper(ref) {
            ref.api_.onStep();
        }
        func onDraw_wrapper(ref) {
            ref.api_.onDraw();
        }
        func onPreStep_wrapper(ref) {
            ref.api_.onPreStep();
        }
        func onPreDraw_wrapper(ref) {
            ref.api_.onPreDraw();
        }
        func onRemove_wrapper(ref) {
            ref.api_.onRemove();
        }
        func onAttach_wrapper(ref) {
            ref.api_.onAttach();
        }
        func onDetach_wrapper(ref) {
            ref.api_.onDetach();
        }



        func onDraw(){};
        func onStep(){};
        func onPreDraw(){};
        func onPreStep(){};
        func onAttach(){};
        func onDetach(){};
        func onRemove(){};

        
        func onReady() {
        
        }





        
        var isValid {
            get {return topaz_.topaz_entity__is_valid(impl_);}
        }
        
        func remove() {
            topaz_.topaz_entity__remove(impl_);
        }
        
        var children {
            get {
                var children = [];
                var len = topaz_.topaz_entity__get_child_count(impl_);
                var child;
                for(var i in 0...len) {
                    child = Entity();
                    child.impl_ = topaz_.topaz_entity__get_nth_child(impl_, i);
                    children.push(child);
                }
                return children;
            }


            set {
                var l = children;
                for(var i in 0..<l.count) {
                    l[i].remove();
                }
                for(var i in 0..<value.count) {
                    attach(value[i]);
                }
            }
        }
        
        
        func step() {
            topaz_.topaz_entity__step(impl_);
        }

        func draw() {
            topaz_.topaz_entity__draw(impl_);
        }

        func attach(other) {
            topaz_.topaz_entity__attach(impl_, other.impl_);
        }

        func detach() {
            topaz_.topaz_entity__detach(impl_);
        }

        var parent {
            get {
                return topaz_.topaz_entity__get_parent(impl_);
            }
            
            set {
                topaz_.topaz_entity__attach(value.impl_, impl_);
            }
        }
        
        func search(str) {
            return topaz_.topaz_entity__search(impl_, str);
        }
        
        
        
        var priority {
            get {
                return topaz_.topaz_entity__get_priority(impl_);
            }
            
            set {
                topaz_.topaz_entity__set_priority(impl_, value);
            }
        }
        
        
        func setPriorityFirst() {
            topaz_.topaz_entity__set_priority_first(impl_);
        }
        
        func setPriorityLast() {
            topaz_.topaz_entity__set_priority_last(impl_);
        }


        var rotation {
            get { 
                var out = Topaz.Vector();
                out.impl_ = topaz_.topaz_entity__get_rotation(impl_);
                return out;
            }
            
            set {
                topaz_.topaz_entity__set_rotation(impl_, value.impl_);
            }
        }


        var position {
            get { 
                var out = Topaz.Vector();
                out.impl_ = topaz_.topaz_entity__get_position(impl_);
                return out;
            }
            
            set {
                topaz_.topaz_entity__set_position(impl_, value.impl_);
            }
        }
        
        var scale {
            get { 
                var out = Topaz.Vector();
                out.impl_ = topaz_.topaz_entity__get_scale(impl_);
                return out;
            }
            
            set {
                topaz_.topaz_entity__set_scale(impl_, value.impl_);
            }
        }
        
        
        
        var globalPosition {
            get {
                var out = Topaz.Vector();
                out.impl_ = topaz_.topaz_entity__get_global_position(impl_);
                return out;
            }
        }
        
        var stepping {
            get {
                return topaz_.topaz_entity__get_stepping(impl_);
            }
            
            set {
                topaz_.topaz_entity__set_stepping(impl_, value);
            }
        }


        var drawing {
            get {
                return topaz_.topaz_entity__get_drawing(impl_);
            }
            
            set {
                topaz_.topaz_entity__set_drawing(impl_, value);
            }
        }
        
        
        var isStepping {
            get {
                return topaz_.topaz_entity__is_stepping(impl_);
            }
        }

        var isDrawing {
            get {
                return topaz_.topaz_entity__is_drawing(impl_);
            }
        }

        var name {
            get {
                return topaz_.topaz_entity__get_name(impl_);
            }
            set {
                topaz_.topaz_entity__set_name(impl_, value);
            }

        }


        func addComponent(other) {
            topaz_.topaz_entity__add_component(impl_, other.impl_);
        }

        func addComponentAfter(other) {
            topaz_.topaz_entity__add_component_after(impl_, other.impl_);
        }

        var components {
            get {
                var out = [];
                var count = topaz_.topaz_entity__get_component_count(impl_);
                for(var i in 0..<count) {
                    var c = topaz_.topaz_entity__get_nth_component(impl_, i);
                    out.push(c);
                }
                return out;
            }

            set {
                var l = components;
                for(var i in 0..<l.count) {
                    removeComponent(l[i]);
                }

                for(var i in value) {
                    addComponent(i);
                }
            }
        }

        func queryComponent(name) {
            return topaz_.topaz_entity__query_component(impl_, name);
        }

        func removeComponent(c) {
            topaz_.topaz_entity__remove_component(impl_, c.impl_);
        }
    }



    ////////////////////
    //////////
    class Component {
        var impl_;

        
        func init() {
            impl_ = topaz_.topaz_component__create('');
            impl_.api_ = self;
            topaz_.topaz_component__set_on_step(impl_, onStep_wrapper);
            topaz_.topaz_component__set_on_draw(impl_, onDraw_wrapper);
            topaz_.topaz_component__set_on_destroy(impl_, onDestroy_wrapper);
            topaz_.topaz_component__set_on_attach(impl_, onAttach_wrapper);
            topaz_.topaz_component__set_on_detach(impl_, onDetach_wrapper);
            onReady();
        }

        func onStep_wrapper(ref) {
            ref.api_.onStep();
        }
        func onDraw_wrapper(ref) {
            ref.api_.onDraw();
        }
        func onDestroy_wrapper(ref) {
            ref.api_.onDestroy();
        }
        func onAttach_wrapper(ref) {
            ref.api_.onAttach();
        }
        func onDetach_wrapper(ref) {
            ref.api_.onDetach();
        }


        func onReady(){};
        func onDraw(){};
        func onStep(){};
        func onAttach(){};
        func onDetach(){};
        func onDestroy(){};
        func destroy() {
            topaz_.topaz_component__destroy(impl_);
        }


        func step() {
            topaz_.topaz_component__step(impl_);
        }

        func draw() {
            topaz_.topaz_component__draw(impl_);
        }

        var stepping {
            get {return topaz_.topaz_component__get_stepping(impl_);}
            set {topaz_.topaz_component__set_stepping(impl_, value);}
        }

        var drawing {
            get {return topaz_.topaz_component__get_drawing(impl_);}
            set {topaz_.topaz_component__set_drawing(impl_, value);}
        }

        var tag {
            get {return topaz_.topaz_component__get_tag(impl_);}
            set {topaz_.topaz_component__set_tag(impl_, value);}
        }

        var host {
            get {return topaz_.topaz_component__get_host(impl_).api_;}
        }

        
        static var Null {
            get {
                var out = Component('');
                out.impl_ = topaz_.component__null();
                return out;
            }
        }

        func emitEvent(evName, entSource) {
            if (entSource == undefined) {
                topaz_.topaz_component__emit_event_anonymous(impl_, evName);
            } else {
                topaz_.topaz_component__emit_event(impl_, evName, entSource.impl_);
            }
        }    

        

        func canHandleEvent(evName) {
            return topaz_.topaz_component__can_handle_event(impl_, evName);
        }


        func installEvent(name, handler) {
            topaz_.topaz_component__install_event(impl_, name, func(c, e){handler(c.api_, e.api_)});
        }

        
        
        func uninstallEvent(name) {
            topaz_.topaz_component__uninstall_event(impl_, name);
        }


        func installHook(name, handler) {
            return topaz_.topaz_component__install_hook(impl_, name, func(c, e){
                handler(c.api_, e.api_);
            });
        }

        func uninstallHook(id) {
            topaz_.topaz_component__uninstall_hook(impl_, id);        
        }    
    }


    class Object2D_Definition : Component {
        static var Group;



        func init() {
            impl_ = topaz_.topaz_object2d__create();
        }
        
        func addVelocity(a, p) {
            topaz_.topaz_object2d__add_velocity(impl_, a, p);
        }   

        func addVelocityTowards(a, p, i) {
            topaz_.topaz_object2d__add_velocity_towards(impl_, a, p.impl_, i);
        }

        func setVelocity(a, p) {
            topaz_.topaz_object2d__set_velocity(impl_, a, p);
        }   

        func setVelocityTowards(a, p, i) {
            topaz_.topaz_object2d__set_velocity_towards(impl_, a, p.impl_, i);
        }


        var velocityX {
            get {
                return topaz_.topaz_object2d__get_velocity_x(impl_);
            }

            set {
                topaz_.topaz_object2d__set_velocity_x(impl_, value);
            }
        }


        var velocityY {
            get {
                return topaz_.topaz_object2d__get_velocity_y(impl_);
            }

            set {
                topaz_.topaz_object2d__set_velocity_y(impl_, value);
            }
        }

        var frictionX {
            get {
                return topaz_.topaz_object2d__get_friction_x(impl_);
            }

            set {
                topaz_.topaz_object2d__set_friction_x(impl_, value);
            }
        }

        var frictionY {
            get {
                return topaz_.topaz_object2d__get_friction_y(impl_);
            }

            set {
                topaz_.topaz_object2d__get_friction_y(impl_);
            }
        }

        var direction {
            get {
                return topaz_.topaz_object2d__get_direction(impl_);
            }

            set {
                setVelocity(speed, value);
            }
        }

        func halt() {
            topaz_.topaz_object2d__halt(impl_);
        }

        
        func resetMotion() {
            topaz_.topaz_object2d__reset_motion(impl_);
        }   
        
        var speed {
            get {
                return topaz_.topaz_object2d__get_speed(impl_);
            }
            set {
                topaz_.topaz_object2d__set_speed(impl_, value);
            }
        }

        var nextPosition {
            get {
                var out = Topaz.Vector();
                out.impl_ = topaz_.topaz_object2d__get_next_position(impl_);
                return out;
            }
        }
        

        var group {
            get {
                return topaz_.topaz_object2d__get_group(impl_);
            }

            set {
                topaz_.topaz_object2d__set_group(impl_, value);
            }
        }

        static func setGroupInteraction(a, b, v) {
            topaz_.topaz_object2d__set_group_interaction(a, b, v);
        }

        var collider {
            get {
                var out = [];
                var len = topaz_.topaz_object2d__get_collider_len(impl_);
                for(var i in 0..<len) {
                    var vector = Topaz.Vector();
                    vector.impl_ = topaz_.topaz_object2d__get_collider_point(impl_, i);
                    out.push(vector.x);
                    out.push(vector.y);
                }
            }

            set {
                topaz_.topaz_object2d__set_collider(impl_, value);
            }
        }


        func setColliderRadial(rad, num) {
            topaz_.topaz_object2d__set_collider_radial(impl_, rad, num);
        }


        var lastCollided {
            get {
                var out = topaz_.topaz_object2d__get_last_collided(impl_);
                if (out != null) return out.api_;
                return null;
            }
        }

        func String() {
            return 'Component::Object2D\n  velocity:' + Topaz.Vector(velocityX, velocityY) + '\n  speed:' + speed + '\n  direction:' + direction + '\n  group' + group;
        }
    }
    var Object2D;

    class Data_Definition : Asset_Definition {
        var bytes {
            set {
                topaz_.topaz_data__set(impl_, value);
            }

            get {
                var arr = [];
                var count = topaz_.topaz_data__get_byte_count(impl_);
                for(var i in 0..<count) {
                    arr.push(topaz_.topaz_data__get_nth_byte(impl_, i));
                }
                return arr;
            }
        }

        var byteCount {
            get {
                return topaz_.topaz_data__get_byte_count(impl_);
            }
        }

        var string {
            get {
                return topaz_.topaz_data__get_as_string(impl_);
            }
        }

        func String() {
            return 'Data asset (' + topaz_.topaz_data__get_byte_count(impl_) + ' bytes)';
        }    


    }
    var Data;





    //// sound 
    class Sound_Definition : Asset_Definition {
        var sampleCount {
            get {
                return topaz_.topaz_sound__get_sample_count(impl_);        
            }
        }

        var samples {
            get {
                var out = [];
                var len = topaz_.topaz_sound__get_sample_count(impl_);
                for(var i in 0..<len) {
                    out.push(topaz_.topaz_sound__get_nth_sample_left(impl_, i));
                    out.push(topaz_.topaz_sound__get_nth_sample_right(impl_, i));
                }
                return out;
            }
            
            set {
                topaz_.topaz_sound__set_samples(impl_, value);        
            }
        }

        func String() {
            return 'Sound asset (' + topaz_.topaz_sound__get_sample_count(impl_) + ' samples)';
        }    
    }
    var Sound;




    ////////////
    class Image_Definition : Asset_Definition {
        var height {
            get {
                return topaz_.topaz_image__get_height(impl_);
            }

            set {
                topaz_.topaz_image__resize(impl_, width, value);
            }
        }

        var width {
            get {
                return topaz_.topaz_image__get_width(impl_);
            }

            set {
                topaz_.topaz_image__resize(impl_, value, height);
            }
        }


        func resize(w, h) {
            topaz_.topaz_image__resize(impl_, w, h);
        }

        func addFrame() {
            topaz_.topaz_image__add_frame(impl_);
        }

        func removeFrame(i) {
            topaz_.topaz_image__remove_frame(impl_, i);
        }

        func getFrameCount() {
            return topaz_.topaz_image__get_frame_count(impl_);
        }

        func frameSetRGBA(i, rgba) {
            topaz_.topaz_image__frame_set_rgba(impl_, i, rgba);        
        }
    }
    var Image;


    class FontManager_Definition {
        var registerFont;
        var preloadGlyphs;
        var unregisterFont;

        func init() {
            registerFont = topaz_.topaz_font_manager__register_font;
            preloadGlyphs = topaz_.topaz_font_manager__preload_glyphs;
            unregisterFont = topaz_.topaz_font_manager__unregister_font;
        }
    }
    var FontManager;


    class Filesystem_Definition {
        func getPath(n) {
            return Path_Definition(topaz_.topaz_filesystem__get_path(n));
        }
        
        func getPathFromString(pth, str) {
            if (pth) {
                return Path_Definition(topaz_.topaz_filesystem__get_path_from_string(pth.impl_, str));
            } else {
                return Path_Definition(topaz_.topaz_filesystem__get_path_from_string(str));        
            }
        }
                
        class Path_Definition {
            var impl_;
            
            func init(id) {
                impl_ = id;
            }   
            
            var string {
                get {
                    return topaz_.topaz_filesystem_path__as_string(impl_);
                }
            }
            
            var parent {
                get {
                    return Path_Definition(topaz_.topaz_filesystem_path__get_parent(impl_));
                }
            }
            
            var children {
                get {
                    var out = [];
                    var len = topaz_.topaz_filesystem_path__get_child_count(impl_);
                    for(var i in 0..<len) {
                        out.push(Path_Definition(topaz_.topaz_filesystem_path__get_nth_child(impl_, i)));
                    }
                    return out;
                }
            }
        }
        var Path;


        var DefaultNode;
        func init() {
            Path = Path_Definition;
            DefaultNode = [
                "resources" : 0,
                "topaz" : 1,
                "userData" : 2
            ];
        }
    }
    var Filesystem;

    class Scheduler_Definition : Component {

        static var Mode;

        func init(type) {
            impl_ = topaz_.topaz_scheduler__create(type);
        }

        func startTask(args) {
            if (args.hasKey("taskName")) {
                topaz_.topaz_scheduler__start_task(
                    impl_,
                    args.taskName,
                    args.interval,
                    args.intervalDelay,
                    args.callback
                );
                return args.taskName;
            } else {
                return topaz_.topaz_scheduler__start_task_simple(
                    impl_,
                    args.interval,
                    args.callback
                );
            }
        }

        func endTask(name) {
            topaz_.topaz_scheduler__end_task(impl_, name);
        }

        var tasks {
            get {
                var out = [];
                var len = topaz_.topaz_scheduler__get_task_count(impl_);
                for(var i in 0..<len) {
                    out.push(topaz_.topaz_scheduler__get_task(impl_, i));
                }
                return out;
            }
        }

        func pause() {
            topaz_.topaz_scheduler__pause(impl_);
        }

        func resume() {
            topaz_.topaz_scheduler__pause(impl_);
        }
        
        func String() {
            return 'Component::Scheduler\n  tasks:' + tasks
        }
    }
    var Scheduler;

    var Render2D;


    class Shape2D_Definition : Component {
        




        func init() {
            impl_ = topaz_.topaz_shape2d__create();
        }

        var color {
            get {
                var out = Color();
                out.impl_ = topaz_.topaz_shape2d__get_color(impl_);
                return out;
            }

            set {
                topaz_.topaz_shape2d__set_color(impl_, value.impl_);
            }
        }

        func getParameter(p) {
            return topaz_.topaz_shape2d__get_parameter(impl_, p);
        }

        func setParameter(p, v) {
            topaz_.topaz_shape2d__set_parameter(impl_, p, v);
        }

        var animSpeed {
            get {
                return topaz_.topaz_shape2d__get_anim_speed(impl_);
            }

            set {
                topaz_.topaz_shape2d__set_anim_speed(impl_, value);
            }
        }

        var center {
            get {
                var out = Topaz.Vector();
                out.impl_ = topaz_.topaz_shape2d__get_center(impl_);
                return out; 
            }

            set {
                topaz_.topaz_shape2d__set_center(impl_, value.impl_);
            }
        }

        var rotation {
            get {
                var out = Topaz.Vector();
                out.impl_ = topaz_.topaz_shape2d__get_rotation(impl_);
                return out; 
            }

            set {
                topaz_.topaz_shape2d__set_rotation(impl_, value.impl_);
            }
        }
        
        var position {
            get {
                var out = Topaz.Vector();
                out.impl_ = topaz_.topaz_shape2d__get_position(impl_);
                return out; 
            }

            set {
                topaz_.topaz_shape2d__set_position(impl_, value.impl_);
            }
        }

        var scale {
            get {
                var out = Topaz.Vector();
                out.impl_ = topaz_.topaz_shape2d__get_scale(impl_);
                return out; 
            }

            set {
                topaz_.topaz_shape2d__set_scale(impl_, value.impl_);
            }
        }


        func formRectangle(w, h) {
            topaz_.topaz_shape2d__form_rectangle(impl_, w, h);
        }
        
        func formImage(img) {
            topaz_.topaz_shape2d__form_image(impl_, img.impl_);
        }

        func formRadial(rad, sid) {
            topaz_.topaz_shape2d__form_radial(impl_, rad, sid);
        }
        
        func formTrianges(tris) {
            topaz_.topaz_shape2d__form_triangles(impl_, tris);
        }

        func formLines(lines) {
            topaz_.topaz_shape2d__form_lines(impl_, lines);
        }   

        func String() {
            return 'Component::Shape2D\n  color:' + color +'\n  pos:' + position + '\n  rotation:' + rotation + '\n  scale:' + scale + '\n  center:' + center;
        }

    }
    var Shape2D;


    class Text2D_Definition : Component {
        private var sizeState;
        private var fontState;




        func init() {
            impl_ = topaz_.topaz_text2d__create();
            sizeState = 12;
            fontState = '';
            
        }

        private func applyFont() {
            topaz_.topaz_text2d__set_font(impl_, fontState, sizeState);
        }


        var text {
            get {
                return topaz_.topaz_text2d__get_text(impl_);
            }
            set {
                topaz_.topaz_text2d__set_text(impl_, value);
            }
        }

        var size {
            get {
                return sizeState;
            }
            set {
                sizeState = value;
                applyFont();
            }
        }

        var font {
            get {
                return fontState;
            }
            set {
                fontState = value;
                applyFont();
            }
        }


        func getParameter(p) {
            return topaz_.topaz_shape2d__get_parameter(impl_, p);
        }

        func setParameter(p, v) {
            topaz_.topaz_shape2d__set_parameter(impl_, p, v);
        }



        var rotation {
            get {
                var out = Topaz.Vector();
                out.impl_ = topaz_.topaz_text2d__get_rotation(impl_);
                return out; 
            }

            set {
                topaz_.topaz_text2d__set_rotation(impl_, value.impl_);
            }
        }
        
        var position {
            get {
                var out = Topaz.Vector();
                out.impl_ = topaz_.topaz_text2d__get_position(impl_);
                return out; 
            }

            set {
                topaz_.topaz_text2d__set_position(impl_, value.impl_);
            }
        }

        var scale {
            get {
                var out = Topaz.Vector();
                out.impl_ = topaz_.topaz_text2d__get_scale(impl_);
                return out; 
            }

            set {
                topaz_.topaz_text2d__set_scale(impl_, value.impl_);
            }
        }


        var extentWidth {
            get {return topaz_.topaz_text2d__get_extent_width(impl_);}
        }
        var extentHeight {
            get {return topaz_.topaz_text2d__get_extent_height(impl_);}
        }

        func getCharX(i) {
            return topaz_.topaz_text2d__get_char_x(impl_, i);
        }

        func getCharY(i) {
            return topaz_.topaz_text2d__get_char_y(impl_, i);
        }

        func setColor(color) {
            topaz_.topaz_text2d__set_color(impl_, color.impl_);
        }

        func setColorSection(from, to, color) {
            topaz_.topaz_text2d__set_color_section(impl_, from, to, color.impl_);
        }

        


        func String() {
            return 'Component::Text2D\n  text:' + text +'\n  pos:' + position + '\n  rotation:' + rotation + '\n  scale:' + scale;
        }

        

    }
    var Text2D;


    class StateControl_Definition : Component {
        func init() {
            impl_ = topaz_.topaz_state_control__create();
        }

        func add(args) {
            topaz_.topaz_state_control__add(
                impl_,
                args.state,
                args.onStep,
                args.onDraw,
                args.onInit
            );
        }
        
        func execute(nam) {
            topaz_.topaz_state_control__execute(impl_, nam);
        }

        func halt() {
            topaz_.topaz_state_control__halt(impl_);
        }

        var halted {
            get { return topaz_.topaz_state_control__is_halted(impl_); }
        }

        var current {
            get { return topaz_.topaz_state_control__get_current(impl_); }
        }

        func String() {
            return 'Component::StateControl\n' + '  current:' + current + '\n  halted?' + halted;
        }
        
    }
    var StateControl;




    class Automation_Definition : Component {

        func init() {
            impl_ = topaz_.topaz_automation__create();
        }

        func addKeyframe(value, fn, offset) {
            topaz_.topaz_automation__add_keyframe(impl_, value, fn, offset);
        }

        func addVectorKeyframe(value, fn, offset) {
            topaz_.topaz_automation__add_vector_keyframe(impl_, value, fn, offset);
        }
        
        func clear() {
            topaz_.topaz_automation__clear(impl_);
        }

        func addAutomation(other) {
            topaz_.topaz_automation__add_automation(impl_, other.impl_);
        }

        func blend(other) {
            topaz_.topaz_automation__blend(impl_, other.impl_);
        }

        func smooth() {
            topaz_.topaz_automation__other(impl_);
        }

        func addFromString(str) {
            topaz_.topaz_automation__add_from_string(impl_, str);
        }

        var length {
            get {
                return topaz_.topaz_automation__get_length(impl_);
            }
        }

        var durationFrames {
            set {
                topaz_.topaz_automation__set_duration_frames(impl_, value);
            }
        }


        var durationSeconds {
            set {
                topaz_.topaz_automation__set_duration_seconds(impl_, value);
            }
        }

        var duration {
            get {
                return topaz_.topaz_automation__get_duration(impl_);
            }
        }

        var looped {
            get {
                return topaz_.topaz_automation__get_looped(impl_);
            }
            set {
                topaz_.topaz_automation__set_looped(impl_, value);
            }
        }

        var speed {
            get {
                return topaz_.topaz_automation__get_speed(impl_);
            }
            set {
                topaz_.topaz_automation__set_speed(impl_, value);
            }
        }

        func resume() {
            topaz_.topaz_automation__resume(impl_);
        }
        func pause() {
            topaz_.topaz_automation__pause(impl_);
        }

        var string {
            set {
                return topaz_.topaz_automation__to_string(impl_);
            }

            get {
                topaz_.topaz_automation__set_from_string(impl_, value);
            }
        }

        func vectorAt(val) {
            var out = Topaz.Vector();
            out.impl_ = topaz_.topaz_automation__vector_at(impl_, val);
            return out;
        }

        func at(val) {
            return topaz_.topaz_automation__at(impl_, val);
        }

        var vector {
            get {
                return topaz_.topaz_automation__current_Topaz.Vector(impl_);
            }
        }

        var value {
            get {
                return topaz_.topaz_automation__current(impl_);
            }
        }

        static var Function;

    }
    var Automation;




    class Package_Definition {
        private var packages;
        private var packageDB; // map name to package object

        func init() {
            packageDB=[:];
            packages=[];
        }

        private func loadAsset(jsonAsset, ext) {
            if (jsonAsset.assetBase64 != null) {
                var out = Topaz.fromBase64(jsonAsset.assetBase64).bytes;
                var success = Resources.loadAssetData(
                    ext,
                    out,
                    jsonAsset.assetName
                );
                if (success == null) {
                    Topaz.log("Error loading " + jsonAsset.assetName + "!");
                    return false;
                }

            }
            if (jsonAsset.assetPath != null) {
                var success = Topaz.Resources.loadAsset(
                    ext,
                    jsonAsset.assetPath,
                    jsonAsset.assetName
                );    
                if (success == null) {
                    Topaz.log("Error loading " + jsonAsset.assetPath + "!");
                    return false;
                }
            }
            return true;
        }

        // can throw, watch out
        private func makePackageFromJSON(json) {

            if (json.formatVersion != 1) {
                Topaz.log("Package version unrecognized (" + json.formatVersion + ")");
                return null;
            }


            var pkg = [:];
            pkg.name = json.name;
            pkg.version = json.version;
            pkg.assets = []; // strings of asset names
            pkg.depends = json.depends;
            if (!pkg.depends) {
                pkg.depends = [];
            }
            // not a valid array, tolerate it.
            if (pkg.depends.count == null) {
                pkg.depends = [];
            }
            pkg.resolved = false;

            for(var i in 0..<json.assets.count) {
                const assetSrc = json.assets[i];
                switch(assetSrc.assetType) {
                case "script":
                    loadAsset(assetSrc, "")
                    break;


                case "font":
                    loadAsset(assetSrc, "")
                    FontManager_Definition.registerFont(
                        assetSrc.assetName
                    );

                default:
                    loadAsset(assetSrc, assetSrc.assetType)
                    break;
                }


            }
            return pkg;
        };





        func read(path) {
            var asset = Topaz.Resources.loadAsset('', path, path);
            if (!asset) {
                Topaz.log("Could not read package");
                return;
            }

            var pkg = makePackageFromJSON(JSON.parse(asset.string));
            if (pkg) {
                packageDB[pkg.name] = pkg;
                packages.push(pkg);
            } 
            Topaz.Resources.removeAsset(path);
            if (pkg) return true;
            return false;        
        }

        func readData(jsonStr) {
            var pkg = makePackageFromJSON(JSON.parse(jsonStr));
            if (pkg) {
                packageDB[pkg.name] = pkg;
                packages.push(pkg);
            }
        }


        func require(packageName, versionObject) {
            var pkg = packageDB[packageName];
            if (!pkg) {
                Topaz.log('Unknown package ' + packageName);                
                return false;
            }

            if (!pkg.resolved) {
                Topaz.log('Package '+packageName+' has been pre-loaded but not resolved.');                
                return false;
            }


            if (versionObject != null) {

                if (versionObject.major != null) {
                    if (versionObject.major > pkg.version.major) {
                        Topaz.log('Package '+packageName+' has major version ' + pkg.version.major + ', but version ' + versionObject.major + ' is required.');                
                        return false;
                    }
                }

                if (versionObject.minor != null &&
                    versionObject.major != null) {
                        if (versionObject.major == pkg.version.major && 
                            versionObject.minor >  pkg.version.minor) {
                        Topaz.log('Package '+packageName+' has version ' + pkg.version.major + '.' + pkg.version.minor + ', but version ' + versionObject.major + '.' + versionObject.minor + ' is required.');                                           
                        return false;
                    }
                }
            }

            return true;
        }


        func resolve(packageName) {
            var pkg = packageDB[packageName];
            if (!pkg) {
                Topaz.log("No such package '" + packageName + "'");
                return false;
            }

            if (pkg.resolved) return true;
            pkg.resolved = true;

            for(var i in 0..<pkg.depends.count) {
                if (!resolve(pkg.depends[i].name)) {
                    Topaz.log("Error: Required package for " + packageName + " could not be resolved");
                    pkg.resolved = false;
                    return false;
                }

                var dep = packageDB[pkg.depends[i].version.major];
                var majorNeeded = Int(pkg.depends[i].version.major);
                var minorNeeded = Int(pkg.depends[i].version.minor);

                var majorHave = Int(dep.version.major);
                var minorHave = Int(dep.version.minor);

                if (
                    (majorHave < majorNeeded) 
                    ||
                    (majorHave == majorNeeded &&
                    minorHave <  minorNeeded)
                ) {
                    Topaz.log("ERROR: Required package version for " + dep.name + " is " + majorNeeded + "." + minorNeeded + ", but found " + majorHave + "," + minorHave);
                    pkg.resolved = false;
                    return false;
                } 
            }

            return true;
        }

        func resolveAll() {
            for(var i in 0..<packages.count) {
                if (!resolve(packages[i].name)) {
                    return false;
                }
            }
            return true;
        }


        private func genPackage(allPackages, path) {
            var mainPath = Topaz.Filesystem.getPathFromString(
                Topaz.Filesystem.getPath(Topaz.Filesystem.DefaultNode.resources),
                path
            );
            if (mainPath == null) {
                Topaz.log("No such path.");
                return false;
            }

            Topaz.Resources.path = mainPath.string;

            var indata = Topaz.Resources.loadAsset('', 'setup_package.json', 'setup_package.json');
            if (!(indata != null && indata.byteCount)) {
                return ("Input file 'setup_package.json' is empty or could not be opened. Exiting");
            }
            allPackages.push(indata.name);
            var injson = JSON.parse(indata.string);    
            var outjson = [:];
            outjson.formatVersion = 1;
            outjson.name = injson.name;
            var packageOutName = "package.json";
            if (injson.outputName != null) {
                packageOutName = injson.outputName;
            }
            if (outjson.name == null) {
                return ("setup_package.json: missing 'name' property!");
            }
            outjson.version = injson.version;
            if (outjson.version == null) {
                return ("setup_package.json: missing 'version' property!");
            }


            if (!outjson.version.hasKey("major")) {
                return ("setup_package.json: missing 'version.major' property!");
            }
            if (!outjson.version.hasKey("minor")) {
                return ("setup_package.json: missing 'version.major' property!");
            }
            if (!outjson.version.hasKey("micro")) {
                Topaz.log("WARNING setup_package.json: missing 'version.micro' property");
            }
            if (!outjson.version.hasKey("build")) {
                Topaz.log("WARNING setup_package.json: missing 'version.build' property");
            }

            var debug = injson.debug == null ? false : injson.debug;

            outjson.depends = [];
            if (injson.hasKey("depends") && injson.depends.count) {
                for(var i in 0..<injson.depends.count) {
                    outjson.depends.push(injson.depends[i]);
                }
            }




            outjson.assets = [];
            if (!(injson.hasKey("assets") && injson.assets.count)) {
                Topaz.log("WARNING: setup_package.json specifies no assets!");
            }


            for(var i in 0..<injson.assets.count) {
                var asset = [:];
                asset.assetName = injson.assets[i].assetName;
                asset.assetType = injson.assets[i].assetType;


                if (debug) {
                    asset.assetPath = Topaz.Filesystem.getPathFromString(
                        mainPath,
                        injson.assets[i].assetFile
                    ).string;

                    outjson.assets.push(asset);
                    Topaz.log(injson.assets[i].assetName + " -> Added");

                } else {
                    const bufferIn = Resources.loadAsset('', injson.assets[i].assetFile, injson.assets[i].assetName);
                    if (!(bufferIn != null && bufferIn.byteCount)) {
                        return ('setup_package.json: could not open asset ' + injson.assets[i].assetFile);
                    }
                    allPackages.push(bufferIn.name);
                    Topaz.log("Processing asset " + injson.assets[i].assetName, false);
                    Topaz.log(".", false);

                    var byteCount = bufferIn.byteCount;
                    var bytes = bufferIn.bytes;
                    var partition = Math.floor(byteCount/5);

                    Topaz.log(".....", false);
                    Topaz.log(" ", false);

                    asset.assetBase64 = Topaz.toBase64(bytes);                
                    outjson.assets.push(asset);
                    Topaz.log("OK (" + Math.ceil(byteCount/1024) + "." + Math.floor(((byteCount%1024) / 1024.0)*100) + "KB)");
                }
            }


            var output = JSON.stringify(outjson);
            var outputAsset = Topaz.Resources.fetchAsset(Topaz.Resources.AssetType.data, "__ASSET__39245s$");
            var outputData = [];
            Topaz.log("Generating output buffer", false);
            var length = output.length;
            var partition = Math.floor(length / 5);
            for(var i in 0..<length) {
                if (i%partition == 0) {
                    Topaz.log(".", false);
                }
                outputData.push(output.charCodeAt(i));
            }
            outputAsset.bytes = outputData;    
            Topaz.Resources.writeAsset(outputAsset, "", packageOutName);
            Topaz.log(" written (" + Math.ceil(length/1024) + "." + Math.floor(((length%1024) / 1024.0)*100) + "KB)");
            return true;
        }

        func makePackage(path) {
            var allPackages = [];
            var output = genPackage(allPackages, path);
            //cleanup
            for(var i in 0..<allPackages.count) {
                Topaz.Resources.removeAsset(allPackages[i]);
            }
            return output;
        }
    }
    var Package;


    func init() {
        run = topaz_.topaz__run;
        pause = topaz_.topaz__pause;
        resume = topaz_.topaz__resume;
        iterate = topaz_.topaz__iterate;
        step = topaz_.topaz__step;
        draw = topaz_.topaz__draw;
        toBase64 = topaz_.topaz__to_base64;
        attachPreManager = topaz_.topaz__attach_pre_manager;
        attachPreManagerUnpausable = topaz_.topaz__attach_pre_manager_unpausable;
        attachPostManager = topaz_.topaz__attach_post_manager;
        attachPostManagerUnpausable = topaz_.topaz__attach_post_manager_unpausable;
        quit = topaz_.topaz__quit;
        wait = topaz_.topaz__wait;

        // classes 
        Vector = Vector_Definition;
        Color = Color_Definition;
        Asset = Asset_Definition;
        Data = Data_Definition;
        Sound = Sound_Definition;
        Image = Image_Definition;
        Object2D = Object2D_Definition;
        Object2D_Definition.Group = [
            "A" : 0,
            "B" : 1,
            "C" : 2,
            "D" : 3,

            "E" : 4,
            "F" : 5,
            "G" : 6,
            "H" : 7,

            "I" : 8,
            "J" : 9,
            "K" : 10,
            "L" : 11,

            "M" : 12,
            "N" : 13,
            "O" : 14,
            "P" : 15,

            "Q" : 16,
            "R" : 17,
            "S" : 18,
            "T" : 19,

            "U" : 20,
            "V" : 21,
            "W" : 22,
            "X" : 23,

            "Y" : 24,
            "Z" : 25
        ];
        Scheduler = Scheduler_Definition;
        Scheduler.Mode = [
            "Time"  : 0,
            "Frame" : 1
        ];
        Render2D = [
            "Parameter" : [
                "alphaRule" : 0,
                "depthTest" : 1,
                "etchRule" : 2,
                "textureFilterHint" : 3
            ]
        ];
        Shape2D = Shape2D_Definition;
        Text2D = Text2D_Definition;
        StateControl = StateControl_Definition;
        Automation = Automation_Definition;
        Automation_Definition.Function = [
            "none" : 0,
            "linear" : 1,
            "square" : 2,
            "cube" : 3,
            "squareRoot" : 4,
            "cubeRoot" : 5,
            "random" : 6
        ];


        Audio = Audio_Definition(); // subnamespace
        Resources = Resources_Definition();
        Input = Input_Definition();
        FontManager = FontManager_Definition();
        Package  = Package_Definition();
        Filesystem = Filesystem_Definition();

    }
}
var Topaz = Topz();


class Debug {
    // Can be used to allow global access to a local reference 
    // for debugging purposes.
    var refs;
    var pause;

    func init() {
        refs = [:];  
        pause = topaz_.pause;
    }

    // Prints detailed info for 
    // the evaluation of an expression 
    //   
    func expression(cl) {
        var result = cl();
        var out =  '| - Result:  \(result)\n';    
        
        
        if (result is Object) {
            out += "| - Methods: " + result.methods() + "\n";
            out += "| - Props {\n";

        }
        
        var props = result.properties();
        for(var i in 0..<props.count) {
            var str = '|       \(props[i]) : '; 
            str += '' + result.load(props[i]);

            out += str + '\n';
        }
        out += '|   }\n';
        Topaz.log(out);
        return out;
    }

}

var debug = Debug();
topaz_.t_['debug'] = debug;
