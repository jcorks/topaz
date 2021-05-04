// This example shows how to use the package made in the examplePackage directory.
// If you haven't read it, go ahead and read the "script" in the directory above,
// which describes how to produce the package used here.
//
//
// Topaz.Package.importPath() scans the given path, relative to the resource directory,
// and looks for any file with the suffix ".package.json". Any file with this suffix 
// is than read and pre-loaded into topaz. This can fail if the data does not exist or 
// is not readable in the correct format. Once all the packages are read, they are 
// then resolved.
//
// resolving accomplishes a few tasks. First, the packages dependencies are checked
// and resolved first. If a package does not meet its requirements, the resolution 
// process is halted and an error reported. If a package does resolve, its assets 
// are loaded into topaz. If a script asset is marked as "autorun", then the 
// script is run at this time. If multiple scripts are marked as autorun, then 
// the scripts are run sequentially.
// 
Topaz.Package.importPath('../examplePackage/');


// Now that the packages are loaded, we can check to see that we have 
// the package we expect. It can be helpful to have "require()" calls 
// before working with non-package code, if youre working on 
// scripts within a package, it is much cleaner to just add the dependency
// in the setup_package.json.
Topaz.Package.require('ExamplePackage', {major:1, minor:0});



// If everything worked, we should be able to run the function within 
// the packaged version of example-package/myScript1.js
script1Function();
