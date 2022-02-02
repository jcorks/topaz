
@:generateDocumentation = import(module:'generator.mt');
@:Topaz = import(module:'Topaz');
Topaz.enableConsole();

generateDocumentation(
    langHint:'None', 
    sourcePath:'../src/top', 
    outputPath:'../',
    resourcePath: './'
);
generateDocumentation(
    langHint:'C',    
    sourcePath:'../../include/topaz/',
    outputPath:'../c/',
    resourcePath: '../'
);
generateDocumentation(
    langHint:'None', 
    sourcePath:'../src/scripting/', 
    outputPath:'../scripting/',
    resourcePath: '../'
);


Topaz.quit();

