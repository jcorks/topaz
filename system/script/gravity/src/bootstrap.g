extern var topaz_;

/*
    Some maintenance notes for gravity:
    
    -   Using class definitions within class definitions as public entities
        i.e. using classes as namespaces will cause problems when referencing 
        sibling classes. So "namespacing" is only recommended for one class 
        at a time without special meta class bindings.
        
    -   "self" is the keyword for self-refrential 

    -   function object instances (closures) do NOT have default bindings even 
        if those closures come from instances i.e. "myObject.method".
        the closure needs to either be bound manually with .bind or 
        an explicit binding with .apply
        
    -   obj.load(arg) where arg is a closure WILL change the scope context to that 
        closure, so watch out.
        

*/

class Topaz {
    static func run() {
        topaz_.topaz__run();
    }    

    static func pause() {
        topaz_.topaz__pause();
    }



    static func resume() {
        topaz_.topaz__resume();
    }

    static func iterate() {
        topaz_.topaz__iterate();
    }

    static func step() {
        topaz_.topaz__step();
    }

    static func draw() {
        topaz_.topaz__draw();
    }

    static func toBase64(f) {
        return topaz_.topaz__to_base64(f);
    }

    static func fromBase64(f) {
        var out = Data();
        out.impl_ = topaz_.topaz__from_base64(f);
        return out;
    }


    static var isPaused {
        get {
            return topaz_.topaz__is_paused();
        }
    }

    static var root {
        get {
            return topaz_.topaz__get_root();
        }
        set {
            topaz_.topaz__set_root(value.impl_);
        }
    }

    static func attachPreManager(a) {
        topaz_.topaz__attach_pre_manager(a);
    }

    static func attachPreManagerUnpausable(a) {
        topaz_.topaz__attach_pre_manager_unpausable(a);
    }


    static func attachPostManager(a) {
        topaz_.topaz__attach_post_manager(a);
    }

    static func attachPostManagerUnpausable(a) {
        topaz_.topaz__attach_post_manager_unpausable(a);
    }

    static func quit() {
        topaz_.topaz__quit();
    }

    static func wait(a) {
        topaz_.topaz__wait(a);
    }

    static func getTime() {
        return topaz_.topaz__get_time();
    }

    static var versionMicro {
        get { return topaz_.topaz__get_version_micro(); }
    }

    static var versionMinor {
        get { return topaz_.topaz__get_version_minor(); }
    }

    static var versionMajor {
        get { return topaz_.topaz__get_version_major(); }
    }


    static func log(a, b) {
        if (b == null) b = true;
        topaz_.topaz__log(''+a, b);
    }    



    
    
    static func EntityNull() {
        var out = Entity();
        out.impl_ = topaz_.topaz_entity__null();
    }
    
    
        
    
    

}






class AudioPlaybackSound {
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

class Vector {
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

    var length {
        get {return topaz_.topaz_vector__get_length(impl_);}
    }    

    func distance(other) {
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
        if (other is Vector) {
            return Vector(
                x + other.x,
                y + other.y,
                z + other.z
            );
        } else {
            return Vector(
                x + other,
                y + other,
                z + other
            );
        }
    }

    func * (other) {
        if (other is Vector) {
            return Vector(
                x * other.x,
                y * other.y,
                z * other.z
            );
        } else {
            return Vector(
                x * other,
                y * other,
                z * other
            );
        }
    }

    func - (other) {
        if (other is Vector) {
            return Vector(
                x - other.x,
                y - other.y,
                z - other.z
            );
        } else {
            return Vector(
                x - other,
                y - other,
                z - other
            );
        }
    }

    func / (other) {
        if (other is Vector) {
            return Vector(
                x / other.x,
                y / other.y,
                z / other.z
            );
        } else {
            return Vector(
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








class Color {
    var impl_;
    func init(r_, g_, b_, a_) {
        impl_ = topaz_.topaz_color__create();
        if (r_.length > 0) {
            hex = r_;
        } else  {
            if (r_ != null) r = r_;
            if (g_ != null) g = g_;
            if (b_ != null) b = b_;
            if (a_ != null) a = a_;
        }
    }

    var hex {
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
        return hex;
    }


}


enum AssetType {
    None,
    Image,
    Sound,
    Model,
    Particle,
    Data,
    Actor,
    Count
};
class Asset {


    var impl_;
    var name {
        get {return topaz_.topaz_asset__get_name(impl_)}
    }

    var type {
        get {return topaz_.topaz_asset__get_type(impl_)}
    }
}





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
            var out = Vector();
            out.impl_ = topaz_.topaz_entity__get_rotation(impl_);
            return out;
        }
        
        set {
            topaz_.topaz_entity__set_rotation(impl_, value.impl_);
        }
    }


    var position {
        get { 
            var out = Vector();
            out.impl_ = topaz_.topaz_entity__get_position(impl_);
            return out;
        }
        
        set {
            topaz_.topaz_entity__set_position(impl_, value.impl_);
        }
    }
    
    var scale {
        get { 
            var out = Vector();
            out.impl_ = topaz_.topaz_entity__get_scale(impl_);
            return out;
        }
        
        set {
            topaz_.topaz_entity__set_scale(impl_, value.impl_);
        }
    }
    
    
    
    var globalPosition {
        get {
            var out = Vector();
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
        topaz_.topaz_entity__remove_component(impl_, c);
    }
}



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


class Data : Asset{
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


class Sound : Asset{
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

class Image : Asset{
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


enum Key {
    /// Not an input.
    topazNotAnInput = 0,
    /// The 0 key.
    topaz_0= 1,
    /// The 1 key.
    topaz_1= 2,
    /// The 2 key.
    topaz_2= 3, 
    /// The 3 key.
    topaz_3= 4,
    /// The 4 key.
    topaz_4= 5,
    /// The 5 key.
    topaz_5= 6,
    /// The 6 key.
    topaz_6= 7,
    /// The 7 key.
    topaz_7= 8,
    /// The 8 key.
    topaz_8= 9,
    /// The 9 key.
    topaz_9= 10,
    /// The a key.
    topaz_a= 11,
    /// The b key.
    topaz_b= 12, 
    /// The c key.
    topaz_c= 13, 
    /// The d key.
    topaz_d= 14, 
    /// The e key.
    topaz_e= 15, 
    /// The f key.
    topaz_f= 16, 
    /// The g key.
    topaz_g= 17, 
    /// The h key.
    topaz_h= 18, 
    /// The i key.
    topaz_i= 19, 
    /// The j key.
    topaz_j= 20,  
    /// The k key.
    topaz_k= 21, 
    /// The l key.
    topaz_l= 22, 
    /// The m key.
    topaz_m= 23, 
    /// The n key.
    topaz_n= 24, 
    /// The o key.
    topaz_o= 25, 
    /// The p key.
    topaz_p= 26, 
    /// The q key.
    topaz_q= 27, 
    /// The r key.
    topaz_r= 28, 
    /// The s key.
    topaz_s= 29, 
    /// The t key.
    topaz_t= 30, 
    /// The u key.
    topaz_u= 31, 
    /// The v key.
    topaz_v= 32, 
    /// The w key.
    topaz_w= 33, 
    /// The x key.
    topaz_x= 34, 
    /// The y key.
    topaz_y= 35, 
    /// The z key.
    topaz_z= 36, 
    /// The lshift key.
    topaz_lshift= 37, 
    /// The rshift key.
    topaz_rshift= 38, 
    /// The lctrl key.
    topaz_lctrl= 39,  
    /// The rctrl key.
    topaz_rctrl= 40,  
    /// The lalt key.
    topaz_lalt= 41,   
    /// The ralt key.
    topaz_ralt= 42,   
    /// The tab key.
    topaz_tab= 43,    
    /// The F1 key.
    topaz_F1= 44,     
    /// The F2 key.
    topaz_F2= 45,     
    /// The F3 key.
    topaz_F3= 46,     
    /// The F4 key.
    topaz_F4= 47,     
    /// The F5 key.
    topaz_F5= 48,     
    /// The F6 key.
    topaz_F6= 49,     
    /// The F7 key.
    topaz_F7= 50,     
    /// The F8 key.
    topaz_F8= 51,     
    /// The F9 key.
    topaz_F9= 52,     
    /// The F10 key.
    topaz_F10= 53,    
    /// The F11 key.
    topaz_F11= 54,    
    /// The F12 key.
    topaz_F12= 55,    
    /// The up key.
    topaz_up= 100,     
    /// The down key.
    topaz_down= 101,   
    /// The left key.
    topaz_left= 102,   
    /// The right key.
    topaz_right= 103,  
    /// The minus key.
    topaz_minus= 104,  
    /// The equal key.
    topaz_equal= 105,  
    /// The backspace key.
    topaz_backspace= 106,  
    /// The grave key.
    topaz_grave= 107,  
    /// The esc key.
    topaz_esc= 108,    
    /// The home key.
    topaz_home= 109,   
    /// The pageUp key.
    topaz_pageUp= 110, 
    /// The pageDown key.
    topaz_pageDown= 111,  
    /// The end key.
    topaz_end= 112,    
    /// The backslash key.
    topaz_backslash= 113, 
    /// The lbracket key.
    topaz_lbracket= 114, 
    /// The rbracket key.
    topaz_rbracket= 115, 
    /// The semicolon key.
    topaz_semicolon= 116, 
    /// The apostrophe key.
    topaz_apostrophe= 117, 
    /// The frontslash key.
    topaz_frontslash= 118, 
    /// The enter key.
    topaz_enter= 119, 
    /// The delete key.
    topaz_delete= 120, 
    /// The numpad0 key.
    topaz_numpad0= 121, 
    /// The numpad1 key.
    topaz_numpad1= 122, 
    /// The numpad2 key.
    topaz_numpad2= 123, 
    /// The numpad3 key.
    topaz_numpad3= 124, 
    /// The numpad4 key.
    topaz_numpad4= 125, 
    /// The numpad5 key.
    topaz_numpad5= 126, 
    /// The numpad6 key.
    topaz_numpad6= 127, 
    /// The numpad7 key.
    topaz_numpad7= 128, 
    /// The numpad8 key.
    topaz_numpad8= 129, 
    /// The numpad9 key.
    topaz_numpad9= 130, 
    /// The prtscr key.
    topaz_prtscr= 131, 
    /// The lsuper key.
    topaz_lsuper= 132, 
    /// The rsuper key.
    topaz_rsuper= 133, 
    /// The space key.
    topaz_space= 134,  
    /// The insert key.
    topaz_insert= 135, 
    /// The comma key.
    topaz_comma= 136, 
    /// The period key.
    topaz_period= 137 , 
    /// The world1 key.
    topaz_world1= 138, 
    /// The world2 key.
    topaz_world2= 139, 
    /// The world3 key.
    topaz_world3= 140, 
    /// The world4 key.
    topaz_world4= 141,  
    /// The world5 key.
    topaz_world5= 142,  
    /// The world6 key.
    topaz_world6= 143, 
    /// The world7 key.
    topaz_world7= 144,  
    /// The world8 key.
    topaz_world8= 145,  
    /// The world9 key.
    topaz_world9= 146,  

    /// The 0th pointer button.
    topazPointer_0= 256, 
    /// The 1st pointer button.
    topazPointer_1= 257, 
    /// The 2nd pointer button.
    topazPointer_2= 258, 

    /// The pointer x axis.
    topazPointer_X= 259, 
    /// The pointer y axis.
    topazPointer_Y= 260, 
    /// The pointer wheel axis.
    topazPointer_Wheel= 261, 

    /// The a pad input.
    topazPad_a= 300,     
    /// The b pad input.
    topazPad_b= 301,     
    /// The c pad input.
    topazPad_c= 302,     
    /// The x pad input.
    topazPad_x= 303,     
    /// The y pad input.
    topazPad_y= 304,     
    /// The r pad input.
    topazPad_r= 305,     
    /// The l pad input.
    topazPad_l= 306,     
    /// The r2 pad input.
    topazPad_r2= 307,    
    /// The l2 pad input.
    topazPad_l2= 308,    
    /// The r3 pad input.
    topazPad_r3= 309,    
    /// The l3 pad input.
    topazPad_l3= 310,    
    /// The start pad input.
    topazPad_start= 311,    
    /// The select pad input.
    topazPad_select= 312,
    /// The b13 pad input.
    topazPad_b13= 313,
    /// The b14 pad input.
    topazPad_b14= 314,
    /// The b15 pad input.
    topazPad_b15= 315,
    /// The b16 pad input.
    topazPad_b16= 316,
    /// The b17 pad input.
    topazPad_b17= 317,
    /// The b18 pad input.
    topazPad_b18= 318,
    /// The b19 pad input.
    topazPad_b19= 319,
    /// The b20 pad input.
    topazPad_b20= 320,
    /// The b21 pad input.
    topazPad_b21= 321,
    /// The b22 pad input.
    topazPad_b22= 322,
    /// The b23 pad input.
    topazPad_b23= 323,
    /// The b24 pad input.
    topazPad_b24= 324,
    /// The b25 pad input.
    topazPad_b25= 325,
    /// The b26 pad input.
    topazPad_b26= 326,
    /// The b27 pad input.
    topazPad_b27= 327,
    /// The b28 pad input.
    topazPad_b28= 328,
    /// The b29 pad input.
    topazPad_b29= 329,
    /// The b30 pad input.
    topazPad_b30= 330,
    /// The b31 pad input.
    topazPad_b31= 331,
    /// The b32 pad input.
    topazPad_b32= 332,

    /// The axisX pad input.
    topazPad_axisX= 400, 
    /// The axisY pad input.
    topazPad_axisY= 401, 
    /// The axisZ pad input.
    topazPad_axisZ= 402, 
    /// The axisX2 pad input.
    topazPad_axisX2= 403,
    /// The axisY2 pad input.
    topazPad_axisY2= 404,
    /// The axisZ2 pad input.
    topazPad_axisZ2= 405,
    /// The axisX3 pad input.
    topazPad_axisX3= 406,
    /// The axisY3 pad input.
    topazPad_axisY3= 407,
    /// The axisZ3 pad input.
    topazPad_axisZ3= 408,
    /// The axisX4 pad input.
    topazPad_axisX4= 409,
    /// The axisY4 pad input.
    topazPad_axisY4= 410,
    /// The axisZ4 pad input.
    topazPad_axisZ4= 411,
    /// The axisX5 pad input.
    topazPad_axisX5= 412,
    /// The axisY5 pad input.
    topazPad_axisY5= 413,
    /// The axisZ5 pad input.
    topazPad_axisZ5= 414,
    /// The axisX6 pad input.
    topazPad_axisX6= 415,
    /// The axisY6 pad input.
    topazPad_axisY6= 416,
    /// The axisZ6 pad input.
    topazPad_axisZ6= 417,

    /// The axisR pad input.
    topazPad_axisR= 450,   
    /// The axisL pad input.
    topazPad_axisL= 451,    
    /// The axisR2 pad input.
    topazPad_axisR2= 452,    
    /// The axisL2 pad input.
    topazPad_axisL2= 453,    
    /// The axisR3 pad input.
    topazPad_axisR3= 454,    
    /// The axisL3 pad input.
    topazPad_axisL3= 455,    
    /// The axisR4 pad input.
    topazPad_axisR4= 456,    
    /// The axisL4 pad input.
    topazPad_axisL4= 457,    

    /// The options pad input.
    topazPad_options= 511,

    /// Number of built-in inputs.
    topazInput_Count= 512
};


class Input {



    static func addKeyboardListener(l) {
        topaz_.topaz_input__add_keyboard_listener(l);
    }

    static func addPadListener(l) {
        topaz_.topaz_input__add_pad_listener(l);
    }

    static func addPointerListener(l) {
        topaz_.topaz_input__add_pointer_listener(l);
    }

    static func addMappedListener(l) {
        topaz_.topaz_input__add_mapped_listener(l);
    }



    static func getState(l) {
        return topaz_.topaz_input__get_state(l);
    }


    static func getPadState(p, i) {
        return topaz_.topaz_input__get_pad_state(p, i);
    }

    static func getMappedState(o) {
        return topaz_.topaz_input__get_mapped_state(o);
    }    

    static func getDeadzone(p, i, a) {
        return topaz_.topaz_input__set_deadzone(p, i, a);
    }

    static func queryPadCount() {
        return topaz_.topaz_input__query_pad_count();
    }

    static func queryPadID(pIndex) {
        return topaz_.topaz_input__query_pad_id(pIndex);
    }

    static func addUnicodeListener(a) {
        topaz_.topaz_input__add_unicode_listener(a);
    }

    static var mouse {
        get {
            return Vector(
                topaz_.topaz_input__mouse_x(),
                topaz_.topaz_input__mouse_y()
            );
        }
    }

    static var mouseDelta {
        get {
            return Vector(
                topaz_.topaz_input__mouse_delta_x(),
                topaz_.topaz_input__mouse_delta_y()
            );
        }
    }

    static var mouseWheel {
        get {
            return topaz_.topaz_input__mouse_wheel();
        }
    }
}




class Audio {
    static func playSound(asset, channel) {
        return AudioPlaybackSound(topaz_.topaz_audio__play_sound(asset.impl_, channel == undefined ? 0 : channel));
    }
    
    static func channelHalt(channel) {
        topaz_.topaz_audio__channel_halt(channel);
    }
    
    static func channelSetVolume(channel, volume) {
        topaz_.topaz_audio__channel_set_volume(channel, volume);
    }
    
    static func channelSetPanning(channel, panning) {
        topaz_.topaz_audio__channel_set_panning(channel, panning);
    }

}

    enum ObjectGroup {
        /// Group A.
        A,

        /// Group B.
        B,

        /// Group C.
        C,

        /// Group D.
        D,

        /// Group E.
        E,

        /// Group F.
        F,

        /// Group G.
        G,

        /// Group H.
        H,

        /// Group I.
        I,

        /// Group J.
        J,

        /// Group K.
        K,

        /// Group L.
        L,

        /// Group M.
        M,

        /// Group N.
        N,

        /// Group O.
        O,

        /// Group P.
        P,

        /// Group Q.
        Q,

        /// Group R.
        R,

        /// Group S.
        S,

        /// Group T.
        T,

        /// Group U.
        U,

        /// Group V.
        V,

        /// Group W.
        W,

        /// Group X.
        X,

        /// Group Y.
        Y,

        /// Group Z.
        Z,
    };

class Object2D : Component {




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
            var out = Vector();
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
                var vector = Vector();
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
        return 'Component::Object2D\n  velocity:' + Vector(velocityX, velocityY) + '\n  speed:' + speed + '\n  direction:' + direction + '\n  group' + group;
    }
}



class Resources {
    static var path {
        get {
            return topaz_.topaz_resources__get_path();
        }   

        set {
            topaz_.topaz_resources__set_path(value);
        }
    }


    static func loadAsset(a, b, c) {
        if (c == '') c = b;
        var impl = topaz_.topaz_resources__load_asset(a, b, c);
        var out;
        switch(topaz_.topaz_asset__get_type(impl)) {
          case AssetType.Data:
            out = Data();
            out.impl_ = impl;
            return out;

          case AssetType.Image:
            out = Image();
            out.impl_ = impl;
            return out;

          case AssetType.Sound:
            out = Sound();
            out.impl_ = impl;
            return out;

        }
        return null;
    }

    static func loadAssetData(a, b, c) {
        if (c == '') c = b;
        var impl = topaz_.topaz_resources__load_asset_data(a, b, c);
        var out;
        switch(topaz_.topaz_asset__get_type(impl)) {
          case AssetType.Data:
            out = Data();
            out.impl_ = impl;
            return out;

          case AssetType.Image:
            out = Image();
            out.impl_ = impl;
            return out;

          case AssetType.Sound:
            out = Sound();
            out.impl_ = impl;
            return out;

        }
        return null;
    }


    static func fetchAsset(a, b) {
        var impl = topaz_.topaz_resources__fetch_asset(a, b);
        var out;
        switch(topaz_.topaz_asset__get_type(impl)) {
          case AssetType.Data:
            out = Data();
            out.impl_ = impl;
            return out;

          case AssetType.Image:
            out = Image();
            out.impl_ = impl;
            return out;

          case AssetType.Sound:
            out = Sound();
            out.impl_ = impl;
            return out;

        }
        return null;
    }
    
    static func writeAsset(asset, name, ext) {
        return topaz_.topaz_resources__write_asset(asset.impl_, name, ext);
    }

    static func removeAsset(a) {
        topaz_.topaz_resources__remove_asset(a);
    }

    static func isExtensionSupported(a) {
        topaz_.topaz_resources__is_extension_supported(a);
    }    

    static func queryAssetPaths() {
        topaz_.topaz_resources__query_asset_paths();
    }

    static var assetPaths {
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

class FontManager {
    static func registerFont(a) {
        topaz_.topaz_font_manager__register_font(a);
    }

    static func preloadGlyphs(a, b, c) {
        topaz_.topaz_font_manager__preload_glyphs(a, b, c);
    }

    static func unregisterFont(a) {
        topaz_.topaz_font_manager__unregister(a);
    }
}



class FilesystemPath {
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
            return FilesystemPath(topaz_.topaz_filesystem_path__get_parent(impl_));
        }
    }
    
    var children {
        get {
            var out = [];
            var len = topaz_.topaz_filesystem_path__get_child_count(impl_);
            for(var i in 0..<len) {
                out.push(FilesystemPath(topaz_.topaz_filesystem_path__get_nth_child(impl_, i)));
            }
            return out;
        }
    }
}

enum FilesystemDefaultNode {
    Resources,
    Topaz,
    UserData
}

class Filesystem {
    static func getPath(n) {
        return FilesystemPath(topaz_.topaz_filesystem__get_path(n));
    }
    
    static func getPathFromString(pth, str) {
        if (pth) {
            return FilesystemPath(topaz_.topaz_filesystem__get_path_from_string(pth.impl_, str));
        } else {
            return FilesystemPath(topaz_.topaz_filesystem__get_path_from_string(str));        
        }
    }
    
}


enum SchedulerMode {
    Time,
    Frame,
}

class Scheduler : Component {

    

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

enum Parameter2D {
    /// The transparency rule for the 2D object.
    /// Default is topazRenderer_AlphaRule_Allow
    ///
    AlphaRule,

    /// The transparency rule for the 2D object.
    /// Default is topazRenderer_AlphaRule_Allow
    ///
    DepthTest,

    /// The currently set etch rule, which determines how the 
    /// etch rules is applied when rendering this 2d object.
    /// Default is topazRenderer_EtchRule_Out
    ///
    EtchRule,

    /// The texturing hint for the 2D object.
    /// Default is topazRenderer_TextureFilterHint_Linear
    ///
    TextureFilterHint
};

class Shape2D : Component {
    




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
            var out = Vector();
            out.impl_ = topaz_.topaz_shape2d__get_center(impl_);
            return out; 
        }

        set {
            topaz_.topaz_shape2d__set_center(impl_, value.impl_);
        }
    }

    var rotation {
        get {
            var out = Vector();
            out.impl_ = topaz_.topaz_shape2d__get_rotation(impl_);
            return out; 
        }

        set {
            topaz_.topaz_shape2d__set_rotation(impl_, value.impl_);
        }
    }
    
    var position {
        get {
            var out = Vector();
            out.impl_ = topaz_.topaz_shape2d__get_position(impl_);
            return out; 
        }

        set {
            topaz_.topaz_shape2d__set_position(impl_, value.impl_);
        }
    }

    var scale {
        get {
            var out = Vector();
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


class Text2D : Component {
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
            var out = Vector();
            out.impl_ = topaz_.topaz_text2d__get_rotation(impl_);
            return out; 
        }

        set {
            topaz_.topaz_text2d__set_rotation(impl_, value.impl_);
        }
    }
    
    var position {
        get {
            var out = Vector();
            out.impl_ = topaz_.topaz_text2d__get_position(impl_);
            return out; 
        }

        set {
            topaz_.topaz_text2d__set_position(impl_, value.impl_);
        }
    }

    var scale {
        get {
            var out = Vector();
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



class StateControl : Component {
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

enum AutomationFunction {
    None,
    Linear,
    Square,
    Cube,
    SquareRoot,
    CubeRoot,
    Random
};



class Automation : Component {

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
        var out = Vector();
        out.impl_ = topaz_.topaz_automation__vector_at(impl_, val);
        return out;
    }

    func at(val) {
        return topaz_.topaz_automation__at(impl_, val);
    }

    var vector {
        get {
            return topaz_.topaz_automation__current_vector(impl_);
        }
    }

    var value {
        get {
            return topaz_.topaz_automation__current(impl_);
        }
    }



}




class Debug {
    // Can be used to allow global access to a local reference 
    // for debugging purposes.
    var refs;

    func init() {
        refs = [:];        
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

    func pause() {
        topaz_.pause();
    }
}

var debug = Debug();
topaz_.t_['debug'] = debug;
