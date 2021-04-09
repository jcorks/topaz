// Auto-run scripts are run after all the package's assets are loaded in to topaz,
// so importing other scripts within the package that are not auto-run is okay.
//
#include "ExampleScript2"
// Notice how we're referring to the ASSET NAME and not the physical on-disk name.
// Once embedded into a package, the asset name is the way you refer to ALL package 
// assets



// This function isnt run, but is instead just evidence that 
// once a package is loaded, its scripts can be auto run.
func script1Function() {
    // Use data from myScript2.js to show the success of the import.
    Topaz.log("Hello from myScript1.g!: " + script2data);

    // Access the embedded image and report its width/height.
    var asset = Topaz.Resources.loadAsset('png', '', 'ExampleImage');
    Topaz.log('Embedded image: (' + asset.width + ', ' + asset.height + ')');
}
