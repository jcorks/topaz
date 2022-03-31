@:Topaz = import(module:'Topaz');

// This example shows how to use bundles, which are pre-made packages of files.
// 
// Due to the nature of bundles requiring multiple stages,
// the example is split into multiple sections. This script 
// handles creating the bundles. After creation, example 
// of usage can be found in ./usage-example/.



// createBundle uses the package.json file within the sourcePath 
// to determine how to create the bundle. The resultant file is written 
// to the path within packageOut. This is a binary file containing 
// all of the assets and info specified within the package.json.
//
// If something goes wrong, an error will be thrown.
Topaz.Resources.createBundle(
    sourcePath:"./bundles/assertion/",
    packageOut:"./usage-example/bundles/assertion_bundle"
);

Topaz.Resources.createBundle(
    sourcePath:"./bundles/circle/",
    packageOut:"./usage-example/bundles/circle_bundle"
);

Topaz.Resources.createBundle(
    sourcePath:"./bundles/rng-color/",
    packageOut:"./usage-example/bundles/rngcolor_bundle"
);




Topaz.log(message: 'Created bundles');
Topaz.quit();
