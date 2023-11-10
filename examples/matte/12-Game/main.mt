@:Topaz   = import(module:'Topaz');
@:createManager = import(module:'manager.mt');
@:Parameters = import(module:'parameters.mt');
Parameters.Manager = createManager();
Topaz.ViewManager.getDefault().getViewport().attach(child: Parameters.Manager);


