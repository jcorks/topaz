return ::(o) {
    @:value = o;
    @already = {}
    @pspace ::(level) {
        @str = '';
        for(0,level)::{
            str = str + '  ';
        }
        return str;
    }
    @helper ::(obj, level) {
        @poself = helper;

        return match(obj->type) {
            (String) : '(type => String): \'' + obj + '\'',
            (Number) : '(type => Number): '+obj,
            (Boolean): '(type => Boolean): '+obj,
            (Function): '(type => Function) ::',
            (Empty)  : '<empty>',
            (Type): '(type => Type): ' + obj,
            default: ::<={
                when(already[obj] == true) '(type => '+obj->type+'): [already printed]';
                already[obj] = true;

                @output = '(type => '+obj->type+'): {';

                @multi = false;
                foreach(obj)::(key, val) {                        
                    output = output + (if (multi) ',\n' else '\n'); 
                    output = output + pspace(level:level+1)+(String(from:key))+' : '+poself(obj:val, level:level+1);
                    multi = true;
                }
                output = output + pspace(level:level) + (if (multi) '\n' + pspace(level:level)+'}' else '}');
                return output;                
            }
        }
    }
    return pspace(level:1) + helper(obj:value, level:1);
}
