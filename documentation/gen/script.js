topaz.import('symbolTable.js');
topaz.import('gendoc.js');


const origPath = topaz.resources.path;
topaz.resources.path = '../../include/topaz/'
topaz.resources.queryAssetPaths();
topaz.log(topaz.resources.assetPaths);



// c hints 
symbolTable.setTypeHint(');',              symbolTable.type.FUNCTION);
symbolTable.setTypeHint('typedef struct ', symbolTable.type.CLASS);
symbolTable.setTypeHint('enum',            symbolTable.type.ENUMERATOR);
symbolTable.setTypeHint(' {',              symbolTable.type.OPEN_STRUCTURE);


symbolTable.setSymbolExtractor(/(\S+)\s*\(/, symbolTable.type.FUNCTION); 
symbolTable.setSymbolExtractor(/struct\s(.+)\s/, symbolTable.type.OPEN_STRUCTURE); 
symbolTable.setSymbolExtractor(/struct\s(.+)\s/, symbolTable.type.CLASS); 
symbolTable.setSymbolExtractor(/enum\s(.+)\s/, symbolTable.type.ENUMERATOR); 

//





var data2doc = function(filename) {
    var text = topaz.resources.loadAsset('txt', filename, filename).string;

    text = text.replace(/\r/g, '');
    const lines = text.split('\n');


    var currentlyWithinDoc = false;
    var docline = false;
    var doclineContent;
    
    var currentName;
    var currentDesc = '';

    var treeStack = [];

    
    for(var i = 0; i < lines.length; ++i) {
        docline = lines[i].startsWith('///');
        doclineContent = lines[i];
        doclineContent = doclineContent.replace('///', '');

        if (currentlyWithinDoc) {
            if (docline) {
                currentDesc += '\n' + doclineContent;
            } else {
                const type = symbolTable.guessType(doclineContent);
                const symbolName = symbolTable.getSymbolName(type, doclineContent);


                symbolTable.add(
                    filename,
                    symbolName,
                    type,
                    treeStack.length ? treeStack[treeStack.length-1] : '',
                    currentDesc
                );


                if (treeStack.length == 0) {
                    treeStack.push(symbolName);
                }


                currentlyWithinDoc = false;
            }
        } else {
            if (docline) {
                // new doc started
                currentDesc = '';
                currentDesc += doclineContent;
                currentlyWithinDoc = true;       
            }
        }
    }
}


const files = topaz.resources.assetPaths;
for(var i = 0; i < files.length; ++i) {
    topaz.log(data2doc(files[i]));
}


topaz.resources.path = origPath;

for(var n = 0; n < files.length; ++n) {
    const symbols = symbolTable.getFileEntities(files[n]);
    if (!symbols.length) continue;

    const doc = gendoc.createDoc();
    for(var i = 0; i < symbols.length; ++i) {
        var block = '';
        switch(symbols[i].type) {
          case symbolTable.type.FUNCTION:
            block = doc.createElement(
                symbols[i].name + '()',    
                'h3'
            );

            block += doc.createElement(
                symbols[i].desc,
                'pre'
            );
            break;


          case symbolTable.type.CLASS:
            block = doc.createElement(
                symbols[i].name + '',    
                'h1'
            );

            block += doc.createElement(
                symbols[i].desc + '',    
                'pre'
            );

            break;


          case symbolTable.type.ENUMERATOR:
            block = doc.createElement(
                symbols[i].name + ' (enum) ',    
                'h3'
            );

            block += doc.createElement(
                symbols[i].desc,    
                'pre'
            );


            break;


          case symbolTable.type.OPEN_STRUCTURE:
            block = doc.createElement(
                symbols[i].name + ' (structure) ',    
                'div'
            );

            block += doc.createElement(
                symbols[i].desc,    
                'div'
            );

            break;


        }

        doc.addContent(doc.createElement(block, 'div')+doc.createElement('', 'br'));
    }

    doc.write(symbols[0].name+'.html');
    topaz.log(symbols[0].name+'.html');
}






