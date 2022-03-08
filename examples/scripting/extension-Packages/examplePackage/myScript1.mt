@:Topaz = import(module:'Topaz');

// Auto-run scripts are run after all the package's assets are loaded in to topaz,
// so importing other scripts within the package that are not auto-run is okay.
//
@:ExampleScript2 = import(module:'ExampleScript2');
// Notice how we're referring to the ASSET NAME and not the physical on-disk name.
// Once embedded into a package, the asset name is the way you refer to ALL package 
// assets



// This function isnt run, but is instead just evidence that 
// once a package is loaded, its scripts can be auto run.
return :: {
    // Use data from myScript2.js to show the success of the import.
    Topaz.log(message:'Hello from myScript1.mt!: ');
    @:result = ExampleScript2();
    Topaz.log(message:'Result from ExampleScript2:' + result);


    // Access the embedded image and report its width/height.
    @:image = Topaz.Resources.fetchAsset(name:'ExampleImage');
    Topaz.log(message:'Embedded image: (' + image.width + ', ' + image.height + ')');
};
