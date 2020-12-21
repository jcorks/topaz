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
symbolTable.setTypeHint(',',               symbolTable.type.ENUM_VALUE);
symbolTable.setTypeHint(';',               symbolTable.type.OPEN_STRUCTURE_MEMBER);


symbolTable.setSymbolExtractor(/(\S+)\s*\(/, symbolTable.type.FUNCTION); 
symbolTable.setSymbolExtractor(/struct\s(.+)\s/, symbolTable.type.OPEN_STRUCTURE); 
symbolTable.setSymbolExtractor(/struct\s(.+)\s/, symbolTable.type.CLASS); 
symbolTable.setSymbolExtractor(/enum\s(\S+)\s/, symbolTable.type.ENUMERATOR); 
symbolTable.setSymbolExtractor(/(\S+)\s*\S*,/, symbolTable.type.ENUM_VALUE); 
symbolTable.setSymbolExtractor(/\s*(.+)\s*;/, symbolTable.type.OPEN_STRUCTURE_MEMBER); 
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
    var startsSpace = false;
    var lastStartsSpace = false;
    var symbolName = undefined;
    for(var i = 0; i < lines.length; ++i) {
        // TODO this is literally the worst
        doclineContent = lines[i];
        const templsp = startsSpace;
        startsSpace = doclineContent.startsWith(' ');
        if (startsSpace)
            doclineContent = doclineContent.replace(/\s+/, '');

        docline = doclineContent.startsWith('///');
        doclineContent = doclineContent.replace('///', '');
        if (!docline)
            startsSpace = templsp;
                


        if (currentlyWithinDoc) {
            if (docline) {
                currentDesc += '\n' + doclineContent;
            } else {
                const type = symbolTable.guessType(doclineContent);
                symbolName = symbolTable.getSymbolName(type, doclineContent);


                symbolTable.add(
                    filename,
                    symbolName,
                    type,
                    treeStack.length ? treeStack[treeStack.length-1] : '',
                    currentDesc
                );

                if (!treeStack.length) {
                    treeStack.push(symbolName);
                }
                currentlyWithinDoc = false;
            }
        } else {
            if (docline) {
                lastStartsSpace = templsp;

                // entered sub-context
                if (symbolName && !lastStartsSpace && startsSpace) {
                    treeStack.push(symbolName);    
                    lastStartsSpace = true;                
                }

                if (treeStack.length > 1 && !startsSpace && lastStartsSpace) {
                    treeStack.splice(treeStack.length-1, 1);
                }

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
                'h3',
                'id="'+symbols[i].name+'"'
            );

            block += doc.createElement(
                symbols[i].desc,
                'pre'
            );



            break;


          case symbolTable.type.CLASS:
            block = doc.createElement(
                symbols[i].name + '',    
                'h1',
                'id="'+symbols[i].name+'"'
            );

            block += doc.createElement(
                symbols[i].desc + '',    
                'pre'
            );
            // list all quick symbols
            doc.addContent('<div>\n');
            for(var m = 0; m < symbols[i].children.length; ++m) {
                block += doc.createElement(
                    symbols[i].children[m],
                    'div'
                );
                    
            }
            doc.addContent('</div>\n');
            break;


          case symbolTable.type.ENUMERATOR:
            block = doc.createElement(
                symbols[i].name + ' (enum) ',    
                'h3',
                'id="'+symbols[i].name+'"'
            );

            block += doc.createElement(
                symbols[i].desc,    
                'pre'
            );

            block += "<table>\n";
            for(var m = 0; m < symbols[i].children.length; ++m) {
                const value = symbolTable.getSymbol(symbols[i].children[m]);
                block += "<tr>\n";
                block += doc.createElement(symbols[i].children[m], 'th');
                block += doc.createElement(symbolTable.getSymbol(symbols[i].children[m]).desc, 'th');
                block += "</tr>\n";
            }
            block += "</table>\n";




            break;

            case symbolTable.type.ENUM_VALUE:    
                break;
          case symbolTable.type.OPEN_STRUCTURE:
            block = doc.createElement(
                symbols[i].name + ' (structure) ',    
                'div',
                'id="'+symbols[i].name+'"'
            );

            block += doc.createElement(
                symbols[i].desc,    
                'div'
            );

            break;

          case symbolTable.type.OPEN_STRUCTURE_MEMBER:
            block = doc.createElement(
                symbols[i].name + ' (structure member) ',    
                'div',
                'id="'+symbols[i].name+'"'
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






