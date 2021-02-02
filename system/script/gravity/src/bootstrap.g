extern var topaz_;

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
            topaz_.topaz__set_root(value);
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
}



