
@:generateDocumentation = import(module:'generator.mt');
@:Topaz = import(module:'Topaz');
Topaz.enableConsole();

generateDocumentation(
    langHint:'C',    
    sourcePath:'../../include/topaz/',
    outputPath:'../c/'
);
/*generateDocumentation(
    langHint:'None', 
    sourcePath:'../src/scripting/', 
    outputPath:'../scripting/'
);*/


Topaz.quit();

