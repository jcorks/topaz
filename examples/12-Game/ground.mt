@:Topaz = import(module:'Topaz');
@:class = import(module:'Matte.Core.Class');

return class(
    inherits : [Topaz.Entity],
    define:::(this) {
        @:visual = Topaz.Shape2D.new();
        visual.lines
    }
);
