debugger;
topaz.import('symbolTable.js');

topaz.resources.path = '../../includes/topaz/'
topaz.resources.queryAssetPaths();
topaz.log(topaz.resources.assetPaths);



// c hints 
symbolTable.setTypeHint(');',              symbolTable.type.FUNCTION);
symbolTable.setTypeHint('typedef struct ', symbolTable.type.CLASS);
symbolTable.setTypeHint(' {',              symbolTable.type.OPEN_STRUCTURE);
symbolTable.setTypeHint('enum',            symbolTable.type.ENUMERATOR);


symbolTable.setSymbolExtractor(/\s(.+)\s*\{/, symbolTable.type.FUNCTION); 
symbolTable.setSymbolExtractor(/struct\s(.+)\s/, symbolTable.type.OPEN_STRUCTURE); 
symbolTable.setSymbolExtractor(/struct\s(.+)\s/, symbolTable.type.CLASS); 
symbolTable.setSymbolExtractor(/enum\s(.+)\s/, symbolTable.type.ENUMERATOR); 

//





var data2doc = function(filename) {
    var text = topaz.resources.loadAsset('txt', filename, filename).string;

    text.replace('\r', '');
    const lines = text.split('\n');


    var currentlyWithinDoc = false;
    var docline = false;
    var doclineContent;

    var currentName;
    var currentDesc;
    
    for(var i = 0; i < lines.length; ++i) {
        debugger;      
        docline = lines[i].startsWith('///');
        doclineContent = lines[i].split('///')[0];

        if (currentlyWithinDoc) {
            if (docline) {
                currentDesc += doclineContent;
            } else {
                const type = symbolTable.guessType(doclineContent);

                symbolTable.add(
                    filename,
                    symbolTable.extractName(type, doclineContent),
                    type,
                    '', // TODO
                    currentDesc
                );
                currentlyWithinDoc = false;
            }
        } else {
            if (docline) {
                // new doc started
                currentDesc = doclineContent;
            }
        }
    }
}


const files = topaz.resources.assetPaths;
for(var i = 0; i < files.length; ++i) {
    topaz.log(data2doc(files[i]));
}
