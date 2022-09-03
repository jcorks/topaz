@:Topaz = import(module:'Topaz');

// This example shows how to use bundles, which are pre-made packages of files.
// If you haven't read it, go ahead and read the "main.mt" in the directory above,
// which describes how to produce the bundle used here.
//
//
// When running, topaz scans the bundle path (usually ./bundles/),
// and loads all files within the directory as bundles. These bundles 
// are read and loaded into the topaz instance, but are not unpacked.
//
// require() accomplishes a few tasks, including unpacking them. 
// First, the bundles dependencies are checked
// and resolved first. If a bundle does not meet its requirements, the resolution 
// process is halted and an error reported. If a bundle does resolve, its assets 
// are loaded into topaz. 
// 
// Optionally, you may specify a function to run whenever a new asset is loaded
// during the require process. This includes during unpacking of dependencies 
// and sub-dependencies as well, allowing for full awareness of what a 
// bundle will load, as well as allowing the application a way to 
// update in the case of large number of unpacking items.
//
Topaz.Resources.require(
    bundle:'CircleShape', 

    onLoadItem:::(asset) {
        Topaz.Console.print(message: 'Unpacked item: ' + asset.name);
    }
);

// now that our package is loaded, we can access the bundle's
// assets. The assets are prefixed with the bundle name. 
// Bundle assets names are the ones specified in the package.json.

@:CircleShape_Entity = import(module:'CircleShape.Entity');


Topaz.defaultDisplay.root = CircleShape_Entity.new();


