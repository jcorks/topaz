topaz.import('symbolTable.js');
topaz.import('gendoc.js');


const origPath = topaz.resources.path;
topaz.resources.path = '../../include/topaz/'
topaz.resources.queryAssetPaths();
topaz.log(topaz.resources.assetPaths);



// c hints 
symbolTable.setTypeHint('#define',         symbolTable.type.MACRO);
symbolTable.setTypeHint('typedef struct ', symbolTable.type.CLASS);
symbolTable.setTypeHint('enum',            symbolTable.type.ENUMERATOR);
symbolTable.setTypeHint('typedef',         symbolTable.type.FUNCTION_POINTER);
symbolTable.setTypeHint('(',              symbolTable.type.FUNCTION, '(*');
symbolTable.setTypeHint(' {',              symbolTable.type.OPEN_STRUCTURE);
symbolTable.setTypeHint('',               symbolTable.type.VARIABLE);


symbolTable.setSymbolExtractor(/(\S+)\s*\(/, symbolTable.type.FUNCTION); 
symbolTable.setSymbolExtractor(/struct\s(.+)\s/, symbolTable.type.OPEN_STRUCTURE); 
symbolTable.setSymbolExtractor(/struct\s(.+)\s/, symbolTable.type.CLASS); 
symbolTable.setSymbolExtractor(/enum\s(\S+)\s/, symbolTable.type.ENUMERATOR); 
symbolTable.setSymbolExtractor(/(\S+)\s*\S*,/, symbolTable.type.ENUM_VALUE); 
symbolTable.setSymbolExtractor(/\s*([^\(\*\s,;]*)\s*(?:\)\(|$|\,|\;)/, symbolTable.type.VARIABLE); 
symbolTable.setSymbolExtractor(/\s*\(\*(\S+)\s*\)/, symbolTable.type.FUNCTION_POINTER); 
symbolTable.setSymbolExtractor(/\s*#define\s*(\S*)/, symbolTable.type.MACRO);

symbolTable.setReturnObjectExtractor(/^\s*(.*)\s\S*\s*\(/, symbolTable.type.FUNCTION);
symbolTable.setReturnObjectExtractor(/^\s*((?:const\s*|\s*)[^\s*]*(?:\s*\*)*)/, symbolTable.type.VARIABLE);
symbolTable.setReturnObjectExtractor(/\s*(\S*)\s*\(\*/, symbolTable.type.FUNCTION_POINTER);

symbolTable.setObjectExtractor(/\s*(?:const)?\s*(\S*)\s*/)

//




const doc = gendoc.createDoc();
var navlistContent = '';

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
                const returnObject = symbolTable.getReturnObject(type, doclineContent);

                symbolTable.add(
                    filename,
                    symbolName,
                    type,
                    treeStack.length ? treeStack[treeStack.length-1] : '',
                    currentDesc,
                    returnObject,
                    doclineContent
                );

                // add to navlist
                if (type == symbolTable.type.CLASS) {
                    navlistContent += doc.createElement(
                        doc.createElement(
                            symbolName,
                            'div',
                            'class="navlist-item"'
                        ),
                        'a',
                        'href="'+symbolName+'.html"'
                    );
                }

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

// replaces the objectstring with a link element IF 
// it contains a reference name. THe link generated should
// lead a user to the location of that symbol.
var linkObject = function(doc, objectString) {
    var symbol = symbolTable.getSymbolFromObject(objectString);
    if (symbol != '') {
        var symbolRef = symbolTable.getSymbol(symbol);

        // Get the parent symbol
        var parent = symbolRef;
        while(parent && parent.parent) {
            parent = symbolTable.getSymbol(symbolRef.parent);
        }

        return doc.createElement(
            objectString,
            'a',
            'href="'+parent.name+'.html#'+symbol+'"'
        )
    } else {
        return objectString
    }

}

//for(var n = 0; n < files.length; ++n) {

for(var n = 0; n <= 6; n++) {
    var content = '';
    const symbols = symbolTable.getFileEntities(files[n]);
    if (!symbols.length) continue;


    for(var i = 0; i < symbols.length; ++i) {
        var block = '';
        var typename = ''
        topaz.log(symbols[i].name+'('+symbolTable.typeToString(symbols[i].type)+')  - generating');
        switch(symbols[i].type) {
          case symbolTable.type.FUNCTION:
          case symbolTable.type.FUNCTION_POINTER:
            typename = symbols[i].type == symbolTable.type.FUNCTION ? 'Function' : 'FunctionPointer';
            block = doc.createElement(
                linkObject(doc, symbols[i].returns),
                'h5',
                'class="FunctionReturnValue"'
            );

            // write out all arguments within a table for better formatting.
            if (symbols[i].children.length) {
                block += doc.createElement(
                    symbols[i].type == symbolTable.type.FUNCTION ? 
                        symbols[i].name + '(' 
                    : 
                        '(*' + symbols[i].name + ')('
                    ,    
                    'h3',
                    'id="'+symbols[i].name+'"'
                );
                block += doc.createElement(
                    (function() {
                        var out = '';
                        for(var m = 0; m < symbols[i].children.length; ++m) {
                            out += doc.createElement(
                                (function(m) {
                                    var row = '';
                                    row += doc.createElement(
                                        linkObject(doc, symbolTable.getChildSymbol(symbols[i], m).returns),
                                        'th',
                                        'class="FunctionArgumentType"'
                                    );
                                    row += doc.createElement(
                                        symbols[i].children[m],
                                        'th',
                                        'class="FunctionArgumentName"'
                                    );
                                    return row;    
                                })(m),
                                'tr'
                            )
                            
                        }
                        return out;
                    })(),
                    'table'
                );
                block += doc.createElement(
                    ')',    
                    'h3'
                );
            } else {
                // no arguments? iconic empty parenth!
                block += doc.createElement(
                    symbols[i].type == symbolTable.type.FUNCTION ? 
                        symbols[i].name + '()' 
                    : 
                        '(*' + symbols[i].name + ')()'
                    ,   
                    'h3',
                    'id="'+symbols[i].name+'"'
                );
            }
            block += doc.createElement(
                symbols[i].desc,
                'pre'
            );
            if (symbols[i].children.length) {
                block += doc.createElement(
                    "<b>Arguments:</b></br></br>",
                    'div'
                );

                for(var m = 0; m < symbols[i].children.length; ++m) {
                    block += 
                    doc.createElement(
                        symbols[i].children[m],
                        'div'
                    ) +
                    doc.createElement(
                        symbolTable.getChildSymbol(symbols[i], m).desc,
                        'pre'
                    );
                }
            }
            break;


          case symbolTable.type.CLASS:
            typename = 'Class';
            block = doc.createElement(
                symbols[i].name + '',    
                'h1',
                'id="'+symbols[i].name+'"'
            );

            block += doc.createElement(
                symbols[i].desc + '',    
                'pre'
            );


            block += doc.createElement(
                "<b>Relevant elements:</b></br></br>",
                'div'
            );


            // list all quick symbols
            doc.addContent('<div>\n');
            for(var m = 0; m < symbols[i].children.length; ++m) {
                if (symbolTable.getChildSymbol(symbols[i], m).type == symbolTable.type.VARIABLE) {
                    // for structs treated as classes
                    block += 
                    doc.createElement(
                        linkObject(doc, symbolTable.getChildSymbol(symbols[i], m).returns) + ' ' + symbols[i].children[m],
                        'div'
                    ) +
                    doc.createElement(
                        symbolTable.getChildSymbol(symbols[i], m).desc,
                        'pre'
                    );
                } else {
                    block += 
                    doc.createElement(
                        doc.createElement(
                            symbols[i].children[m],
                            'a',
                            'href="'+symbols[i].name+'.html#'+symbols[i].children[m]+'"'
                        ),
                        'div'
                    );
                }
            }
            doc.addContent('</div>\n');


            
            break;


          case symbolTable.type.ENUMERATOR:
            typename = 'Enumerator';
            block = doc.createElement(
                symbols[i].name,    
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
                block += doc.createElement(symbolTable.getChildSymbol(symbols[i], m).desc, 'td');
                block += "</tr>\n";
            }
            block += "</table>\n";




            break;

            case symbolTable.type.ENUM_VALUE:    
                break;
          case symbolTable.type.OPEN_STRUCTURE:
            typename = 'OpenStructure';
            block = doc.createElement(
                symbols[i].name,    
                'h3',
                'id="'+symbols[i].name+'"'
            );

            block += doc.createElement(
                symbols[i].desc,    
                'pre'
            );

            block += doc.createElement(
                "<b>Members:</b></br></br>",
                'div'
            );

            for(var m = 0; m < symbols[i].children.length; ++m) {
                block += 
                doc.createElement(
                    linkObject(doc, symbolTable.getChildSymbol(symbols[i], m).returns) + ' ' + symbols[i].children[m],
                    'div'
                ) +
                doc.createElement(
                    symbolTable.getChildSymbol(symbols[i], m).desc,
                    'pre'
                );
            }

            break;

          case symbolTable.type.MACRO:
            typename = 'Macro';
            block += doc.createElement(
                symbols[i].name,    
                'h3',
                'id="'+symbols[i].name+'"'
            );

            block += doc.createElement(
                symbols[i].desc,    
                'pre'
            );
              
            break;


        }

        

        content += doc.createElement(
            doc.createElement(
                '('+typename+')</br>',
                'div',
                'class="typetag"'
            )+
            
            doc.createElement(
                block, 
                'div'
            ) + "</br>",

            'div',
            'class="'+typename+'"'
        );

    }


    var navsearch = doc.createElement(
        doc.createElement(
            '',
            'input',
            'class="navsearch-input"'
        ),
        'div'
    );




    doc.addContent(
        doc.createElement(
            '(navbar)',
            'div',
            'class="navbar"'
        ) + doc.createElement(
            doc.createElement(
                navsearch + navlistContent,
                'div',
                'class="navlist"'
            ) + 
            doc.createElement(
                content,
                'div',
                'class="symbollist"'  
            ),
            'div'
        )
    );

    doc.write('../'+symbols[0].name+'.html');
    topaz.log(symbols[0].name+'.html');
}






