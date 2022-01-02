// similar to vector, color is a "light object" where 
// the state is held in-script and no real methods are 
// bound to it. When it interacts with 
// the native context, a wrapper is added and the data syncd
return ::<={
    <@>topaz_color__create = getExternalFunction(name:'topaz_color__create');
    <@>topaz_color__set_rgba = getExternalFunction(name:'topaz_color__set_rgba');
    <@>topaz_color__get_r = getExternalFunction(name:'topaz_color__get_r');
    <@>topaz_color__get_g = getExternalFunction(name:'topaz_color__get_g');
    <@>topaz_color__get_b = getExternalFunction(name:'topaz_color__get_b');
    <@>topaz_color__get_a = getExternalFunction(name:'topaz_color__get_a');
    <@>topaz_color__to_hex_string = getExternalFunction(name:'topaz_color__to_hex_string');
    <@>topaz_color__set_from_string = getExternalFunction(name:'topaz_color__set_from_string');

    <@>refColor = topaz_color__create(a:0, b:0, c:0, d:0);

    <@>statepush = ::(v) {
        if (v.native == empty) ::<={
            if (introspect.isNaN(value:v.r)) v.r = 0;
            if (introspect.isNaN(value:v.g)) v.g = 0;
            if (introspect.isNaN(value:v.b)) v.b = 0;
            if (introspect.isNaN(value:v.a)) v.a = 0;
            v.native = topaz_color__create(a:v.r, b:v.g, c:v.b, d:v.a);
        } else ::<={
            topaz_color__set_rgba(a:v.native, b:v.r, c:v.g, d:v.b, e:v.a);
        };
    };

    <@>statepull = ::(v) {
        v.r = topaz_color__get_r(a:v.native);
        v.g = topaz_color__get_g(a:v.native);
        v.b = topaz_color__get_b(a:v.native);
        v.a = topaz_color__get_a(a:v.native);
    };

    return {
        fromnative : ::(native) {
            <@>out = {native:native};
            statepull(v:out);
            return out;
        },
        statepush : statepush,
        statepull : statepull,
        asString : ::(color) {
            statepush(v:color);
            return topaz_color__to_hex_string(a:color.native);
        },

        from : ::(string) {
            topaz_color__set_from_string(a:refColor, b:string);
            return {
                r: topaz_color__get_r(a:refColor),
                g: topaz_color__get_g(a:refColor),
                b: topaz_color__get_b(a:refColor),
                a: topaz_color__get_a(a:refColor)
            };
        }
    };
};

