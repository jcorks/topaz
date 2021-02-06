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


    static func log(a) {
        topaz_.topaz__log(a);
    }    





    // convenience function for create XYZ vector
    static func VectorXYZ(x, y, z) {
        var out = Vector();
        out.x = x;
        out.y = y;
        out.z = z;
        return out;
    }

    // convenience function for create XY vector
    static func VectorXY(x, y) {
        var out = Vector();
        out.x = x;
        out.y = y;
        return out;
    }



    
    
    static func EntityNull() {
        var out = Entity();
        out.impl_ = topaz_.topaz_entity__null();
    }
    
    
        
    
    

}








class Vector {
    // effectively private.
    var impl_;

    func init() {
        impl_ = topaz_.topaz_vector__create();
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





class Entity {    
    var impl_;
    
    
            
    func init() {
        impl_ = topaz_.topaz_entity__create();
        impl_.t = self;

        start();

        topaz_.topaz_entity__set_on_step(impl_, onStep_wrapper);
        topaz_.topaz_entity__set_on_draw(impl_, onDraw_wrapper);


    }

    func onStep_wrapper(ref) {
        ref.t.onStep();
    }
    func onDraw_wrapper(ref) {
        ref.t.onDraw();
    }

    func onDraw(){};
    func onStep(){};

    
    func start() {
    
    }





    
    var isValid {
        get {return topaz_.topaz_entity__is_valid(impl_);}
    }
    
    func remove() {
        topaz_.topaz_entity__remove(impl_);
    }
    
    var children {
        get {
            var children = List();
            var len = topaz_.topaz_entity__get_child_count(impl_);
            var child;
            for(var i in 0...len) {
                child = Entity();
                child.impl_ = topaz_.topaz_entity__get_nth_child(impl_, i);
                children.push(child);
            }
            return children;
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
        
        
        Topaz.log(    '| - Result:  \(result)');
        
        if (result is Object) {
            Topaz.log("| - Methods: " + result.methods());
            Topaz.log("| - Props {");

        }
        
        var props = result.properties();
        for(var i in 0..<props.count) {
            var str = '|       \(props[i]) : '; 
            str += '' + result.load(props[i]);

            Topaz.log(str);
        }
        Topaz.log('|   }');
    }

    func pause() {
        topaz_.pause();
    }
}

var debug = Debug();
topaz_.t_['debug'] = debug;


