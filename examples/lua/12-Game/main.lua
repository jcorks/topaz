Game = {}
Topaz.Lua.import('parameters.lua');
Topaz.Lua.import('manager.lua');
Topaz.Lua.import('block.lua');
Topaz.Lua.import('piece.lua');


Game.Parameters.Manager = Game.createManager();
Topaz.ViewManager.getDefault():getViewport():attach(Game.Parameters.Manager);


