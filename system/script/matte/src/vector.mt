return ::<={
    @:topaz_vector__create = getExternalFunction(name:'topaz_vector__create');
    @:topaz_vector__set_xyz = getExternalFunction(name:'topaz_vector__set_xyz');
    @:topaz_vector__get_x = getExternalFunction(name:'topaz_vector__get_x');
    @:topaz_vector__get_y = getExternalFunction(name:'topaz_vector__get_y');
    @:topaz_vector__get_z = getExternalFunction(name:'topaz_vector__get_z');
    @:topaz_vector__get_distance = getExternalFunction(name:'topaz_vector__get_distance');
    @:topaz_vector__normalize = getExternalFunction(name:'topaz_vector__normalize');
    @:topaz_vector__cross = getExternalFunction(name:'topaz_vector__cross');
    @:topaz_vector__floor = getExternalFunction(name:'topaz_vector__floor');
    @:topaz_vector__rotation_x_diff = getExternalFunction(name:'topaz_vector__rotation_x_diff');
    @:topaz_vector__rotation_x_diff_relative = getExternalFunction(name:'topaz_vector__rotation_x_diff_relative');
    @:topaz_vector__rotation_y_diff = getExternalFunction(name:'topaz_vector__rotation_y_diff');
    @:topaz_vector__rotation_y_diff_relative = getExternalFunction(name:'topaz_vector__rotation_y_diff_relative');
    @:topaz_vector__rotation_z_diff = getExternalFunction(name:'topaz_vector__rotation_z_diff');
    @:topaz_vector__rotation_z_diff_relative = getExternalFunction(name:'topaz_vector__rotation_z_diff_relative');
    @:topaz_vector__rotation_x = getExternalFunction(name:'topaz_vector__rotation_x');
    @:topaz_vector__rotation_y = getExternalFunction(name:'topaz_vector__rotation_y');
    @:topaz_vector__rotation_z = getExternalFunction(name:'topaz_vector__rotation_z');
    @:topaz_vector__rotate_x = getExternalFunction(name:'topaz_vector__rotate_x');
    @:topaz_vector__rotate_y = getExternalFunction(name:'topaz_vector__rotate_y');
    @:topaz_vector__rotate_z = getExternalFunction(name:'topaz_vector__rotate_z');
    @:topaz_vector__get_length = getExternalFunction(name:'topaz_vector__get_length');

    @:statepush = ::(v) {
        if (v.native == empty) ::<={
            if (v.x == empty || introspect.isNaN(value:v.x)) v.x = 0;
            if (v.y == empty || introspect.isNaN(value:v.y)) v.y = 0;
            if (v.z == empty || introspect.isNaN(value:v.z)) v.z = 0;
            v.native = topaz_vector__create(a:v.x, b:v.y, c:v.z);
        } else ::<={
            topaz_vector__set_xyz(a:v.native, b:v.x, c:v.y, d:v.z);
        };
    };

    @:statepull = ::(v) {
        v.x = topaz_vector__get_x(a:v.native);
        v.y = topaz_vector__get_y(a:v.native);
        v.z = topaz_vector__get_z(a:v.native);
    };

    return {
        statepush : statepush,
        statepull : statepull,
        fromnative ::(native) {
            @:out = {native:native};
            statepull(v:out);
            return out;
        },
        getDistance ::(from, to){ 
            statepush(v:from); statepush(v:to);
            return topaz_vector__get_distance(a:from.native, b:to.native);
        },
        
        normalize ::(value){ 
            statepush(v:value);
            topaz_vector__normalize(a:value.native);   
            statepull(v:value);
        },
        
        cross ::(a, b){ 
            statepush(v:a);
            statepush(v:b);
            @out = {native:topaz_vector__cross(a:a.native, b:b.native)};
            statepull(v:out);
            return out;
        },

        floor ::(value){ 
            statepush(v:value);
            topaz_vector__floor(a:value.native);
            statepull(v:value);
        },
        
        rotationXDiff ::(from, to){ 
            statepush(v:from);
            statepush(v:to);
            return topaz_vector__rotation_x_diff(a:from.native, b:to.native);
        },
        
        rotationXDiffRelative ::(from, to){ 
            statepush(v:from);
            statepush(v:to);
            return topaz_vector__rotation_x_diff_relative(a:from.native, b:to.native);
        },
        
        rotationX ::(of) { 
            statepush(v:of);
            return topaz_vector__rotation_x(a:of.native);
        },
        
        rotationYDiff ::(from, to) { 
            statepush(v:from);
            statepush(v:to);
            return topaz_vector__rotation_y_diff(a:from.native, b:to.native);
        },
        
        rotationYDiffRelative ::(from, to){ 
            statepush(v:from);
            statepush(v:to);
            return topaz_vector__rotation_y_diff_relative(a:from.native, b:to.native);
        },
        
        rotationY : ::(of){ 
            statepush(v:of);
            return topaz_vector__rotation_y(a:of.native);
        },
        
        rotationZDiff : ::(from, to){
            statepush(v:from);
            statepush(v:to); 
            return topaz_vector__rotation_z_diff(a:from.native, b:to.native);
        },
        
        rotationZDiffRelative : ::(from, to){
            statepush(v:from);
            statepush(v:to);
            return topaz_vector__rotation_z_diff_relative(a:from.native, b:to.native);
        },
        
        rotationZ : ::(of){ 
            statepush(v:of);
            return topaz_vector__rotation_z(a:of.native);
        },
        
        rotateX : ::(vector, val){ 
            statepush(v:vector);
            topaz_vector__rotate_x(a:vector.native, b:val);
            statepull(v:vector);
        },
        
        rotateY : ::(vector, val){ 
            statepush(v:vector);
            topaz_vector__rotate_y(a:vector.native, b:val);
            statepull(v:vector);
        },
        
        rotateZ : ::(vector, val){ 
            statepush(v:vector);
            topaz_vector__rotate_y(a:vector.native, b:val);
            statepull(v:vector);
        },

        
        add : ::(a, b){ 
            statepush(v:a);
            statepush(v:b);
            return {x:a.x + b.x, y:a.y + b.y, z:a.z + b.z};
        },
        
        subtract : ::(a, b){ 
            statepush(v:a);
            statepush(v:b);
            return {x:a.x - b.x, y:a.y - b.y, z:a.z - b.z};
        },

        multiply : ::(a, b){ 
            statepush(v:a);
            statepush(v:b);
            return {x:a.x * b.x, y:a.y * b.y, z:a.z * b.z};
        },
        
        divide : ::(a, b){ 
            statepush(v:a);
            statepush(v:b);
            return {x:a.x / b.x, y:a.y / b.y, z:a.z / b.z};
        },
        
        length : ::(vector){
            statepush(v:vector);
            return topaz_vector__get_length(a:vector.native);
        }
    };
};

