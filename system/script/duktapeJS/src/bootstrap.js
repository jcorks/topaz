topaz = {
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


c = new topaz.color();
v = new topaz.vector();
