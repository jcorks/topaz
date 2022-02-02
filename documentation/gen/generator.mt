@:Topaz       = import(module:'Topaz');
@:SymbolTable = import(module:'symbolTable.mt');
@:JSON        = import(module:'Matte.Core.JSON');

@:generateDocumentation = ::(
    langHint,
    sourcePath,
    outputPath,
    resourcePath
) {
    @symbolTable = SymbolTable.new();
    @:writeTextFile ::(str, filename) {
        @:asset = Topaz.Resources.fetchAsset(type:Topaz.Asset.TYPE.DATA, name:filename);
                        
        @bytes = [];
        for(in:[0, String.length(of:str)], do:::(i) {
            bytes[i] = String.charCodeAt(string:str, index:i);
        });

        asset.bytes = bytes;
        Topaz.Resources.writeAsset(asset:asset, extension:'txt', path:filename);
    };

    @gendoc = ::<={
        @:header = '
<!doctype html>

<html lang="en">
<head>
  <meta charset="utf-8">

  <title>topaz Documentation</title>
  <meta name="description" content="documentation page">
  <meta name="author" content="https://github.com/jcorks/topaz/">

  <link rel="stylesheet" href="'+resourcePath+'style.css?v=1.0">

</head>

<body>
';
        @:footer = '
</body>
<script src="searchindex_db.js"></script>
<script src="doclinks_db.js"></script>
<script src="'+resourcePath+'docext.js"></script>
</html>
';

        @content = ''+header;
        @level = 0;
        
        return {
            createDoc : ::() {
                return {

                    // creates an element string
                    createElement : ::(content, tag, attribs) {
                        return '<'+tag+(if(attribs != empty)' '+attribs else '')+'>\n'+content+'\n</'+tag+'>\n';
                    },

                    // adds text content
                    addContent : ::(text) {
                        content = content + text;
                    },

                    write : ::(filename) {
                        content = content+footer;
                        writeTextFile(str:content, filename:filename);
                        content = ''+header;
                    }
                };
            }

        };
    };


    @:origPath = Topaz.Resources.path;
    Topaz.Resources.path = Topaz.Filesystem.getPathFromString(string:sourcePath).string;



    // c hints
    match(langHint) {
      ('C'):::<={
        symbolTable.setTypeHint(hint:'DOCPAGE',         type:symbolTable.type.DOCPAGE);
        symbolTable.setTypeHint(hint:'#define',         type:symbolTable.type.MACRO);
        symbolTable.setTypeHint(hint:'typedef struct ', type:symbolTable.type.CLASS);
        symbolTable.setTypeHint(hint:'enum',            type:symbolTable.type.ENUMERATOR);
        symbolTable.setTypeHint(hint:'(*',         type:symbolTable.type.FUNCTION_POINTER);
        symbolTable.setTypeHint(hint:'(',               type:symbolTable.type.FUNCTION);
        symbolTable.setTypeHint(hint:' {',              type:symbolTable.type.OPEN_STRUCTURE);
        symbolTable.setTypeHint(hint:' ',                type:symbolTable.type.VARIABLE);
        symbolTable.setTypeHint(hint:'_',              type:symbolTable.type.ENUM_VALUE);

        symbolTable.setSymbolExtractor(format:'DOCPAGE[%]',type: symbolTable.type.DOCPAGE); 
        symbolTable.setSymbolExtractor(format:'[%] [%](', type:symbolTable.type.FUNCTION, removeKeys:['*', 'const '], groupIndex:1); 
        symbolTable.setSymbolExtractor(format:'struct [%] ', type:symbolTable.type.OPEN_STRUCTURE); 
        symbolTable.setSymbolExtractor(format:'struct [%] ', type:symbolTable.type.CLASS); 
        symbolTable.setSymbolExtractor(format:'enum [%] ', type:symbolTable.type.ENUMERATOR); 
        symbolTable.setSymbolExtractor(format:'[%]\n', type:symbolTable.type.ENUM_VALUE, removeKeys:[',']); 
        symbolTable.setSymbolExtractor(format:'[%] [%]\n', type:symbolTable.type.VARIABLE, removeKeys:['*', 'const ', ';'], groupIndex:1); 
        symbolTable.setSymbolExtractor(format:'*[%])', type:symbolTable.type.FUNCTION_POINTER); 
        symbolTable.setSymbolExtractor(format:'#define [%] ', type:symbolTable.type.MACRO);

        symbolTable.setReturnObjectExtractor(format:'[%] [%](', type:symbolTable.type.FUNCTION, removeKeys:['const ', '*']);
        symbolTable.setReturnObjectExtractor(format:'[%] [%]\n', type:symbolTable.type.VARIABLE, removeKeys:['*', 'const ', ';']);
        symbolTable.setReturnObjectExtractor(format:'typedef[%](', type:symbolTable.type.FUNCTION_POINTER, removeKeys:['*', 'const ']);

        symbolTable.setObjectExtractor(format:'[%] [%]\n', removeKeys:['const', '*']);
      },
      
      ('None'):::<={
        symbolTable.setTypeHint(hint:'docpage',         type:symbolTable.type.DOCPAGE);
        symbolTable.setTypeHint(hint:'#define',         type:symbolTable.type.MACRO);
        symbolTable.setTypeHint(hint:'class',           type:symbolTable.type.CLASS);
        symbolTable.setTypeHint(hint:'module',          type:symbolTable.type.MODULE);
        symbolTable.setTypeHint(hint:'enum',            type:symbolTable.type.ENUMERATOR);
        symbolTable.setTypeHint(hint:'eval',            type:symbolTable.type.ENUM_VALUE);
        symbolTable.setTypeHint(hint:'func object',     type:symbolTable.type.FUNCTION_POINTER);
        symbolTable.setTypeHint(hint:'staticfn',        type:symbolTable.type.STATIC_FUNCTION);
        symbolTable.setTypeHint(hint:'structure',       type:symbolTable.type.OPEN_STRUCTURE);
        symbolTable.setTypeHint(hint:'singleton',       type:symbolTable.type.SINGLETON);
        symbolTable.setTypeHint(hint:'@',               type:symbolTable.type.VARIABLE);
        symbolTable.setTypeHint(hint:'func',            type:symbolTable.type.FUNCTION);
        symbolTable.setTypeHint(hint:'inherits',        type:symbolTable.type.INHERITS);
        symbolTable.setTypeHint(hint:'link',            type:symbolTable.type.LINK);

        symbolTable.setSymbolExtractor(format:'docpage[%]', type:symbolTable.type.DOCPAGE); 
        symbolTable.setSymbolExtractor(format:'func[%]->', type:symbolTable.type.FUNCTION); 
        symbolTable.setSymbolExtractor(format:'staticfn[%]->', type:symbolTable.type.STATIC_FUNCTION); 
        symbolTable.setSymbolExtractor(format:'structure[%]', type:symbolTable.type.OPEN_STRUCTURE); 
        symbolTable.setSymbolExtractor(format:'class[%]', type:symbolTable.type.CLASS); 
        symbolTable.setSymbolExtractor(format:'module[%]', type:symbolTable.type.MODULE); 
        symbolTable.setSymbolExtractor(format:'singleton[%]', type:symbolTable.type.SINGLETON); 
        symbolTable.setSymbolExtractor(format:'enum[%]', type:symbolTable.type.ENUMERATOR); 
        symbolTable.setSymbolExtractor(format:'eval[%]', type:symbolTable.type.ENUM_VALUE); 
        symbolTable.setSymbolExtractor(format:'@[%] [%]', type:symbolTable.type.VARIABLE, groupIndex:1); 
        symbolTable.setSymbolExtractor(format:'func object [%]', type:symbolTable.type.FUNCTION_POINTER); 
        symbolTable.setSymbolExtractor(format:'#define [%]', type:symbolTable.type.MACRO);
        symbolTable.setSymbolExtractor(format:'link[%]', type:symbolTable.type.LINK);
        symbolTable.setSymbolExtractor(format:'[%] ', type:symbolTable.type.INHERITS);

        symbolTable.setReturnObjectExtractor(format:'->[%]', type:symbolTable.type.FUNCTION);
        symbolTable.setReturnObjectExtractor(format:'->[%]', type:symbolTable.type.STATIC_FUNCTION);
        symbolTable.setReturnObjectExtractor(format:'@[%] ', type:symbolTable.type.VARIABLE);
        symbolTable.setReturnObjectExtractor(format:'[%] ', type:symbolTable.type.FUNCTION_POINTER);
        symbolTable.setReturnObjectExtractor(format:'inherits[%]', type:symbolTable.type.INHERITS);
        symbolTable.setReturnObjectExtractor(format:'link[%]', type:symbolTable.type.LINK);


        symbolTable.setObjectExtractor(format:' [%]');
      }
    };
    //




    @:doc = gendoc.createDoc();
    @navlistContent = '';
    
    @:removeLeadingSpace ::(str) {
        loop(func:::{
            when(str == empty) false;
            @fc = String.charAt(string:str, index:0);
            when(fc == ' ' || fc == '\r' || fc == '\n' || fc == '\t') ::<= {
                str = String.substr(string:str, from:1, to:String.length(of:str)-1);
                return true;
            };
            return false;
        });    
        return if (str == empty) '' else str;
    };

    @data2doc = ::(filename) {
        @text = Topaz.Resources.loadAsset(extension:'txt', path:filename, name:filename).string;

        text = String.replace(string:text, key:'\r', with:'');
        @:lines = String.split(string:text, token:'\n');


        @currentlyWithinDoc = false;
        @docline = false;
        @doclineContent;
        
        @currentName;
        @currentDesc = '';

        @treeStack = [];
        @startsSpace = false;
        @lastStartsSpace = false;
        @symbolName = empty;
        for(in:[0, Object.length(of:lines)], do:::(i) {
            // TODO this is literally the worst
            doclineContent = lines[i]+'\n';
            @:templsp = startsSpace;
            startsSpace = String.search(string:doclineContent, key:' ') == 0;
            if (startsSpace)
                doclineContent = removeLeadingSpace(str:doclineContent);
                //doclineContent = String.replace(string:doclineContent, keys:[' ', '\r', '\n', '\t'], with:'');

            docline = String.search(string:doclineContent, key:'///') == 0;
            doclineContent = String.replace(string:doclineContent, key:'///', with:'');
            if (!docline)
                startsSpace = templsp;
                    


            if (currentlyWithinDoc) ::<={
                if (docline) ::<={
                    currentDesc = currentDesc + doclineContent;
                } else ::<={
                    @:type = symbolTable.guessType(textLine:doclineContent);
                    symbolName = symbolTable.getSymbolName(type:type, line:doclineContent);
                    @:returnObject = symbolTable.getReturnObject(type:type, line:doclineContent);
                    if (symbolTable.getSymbolSpecific(symbolName:symbolName, file:filename) == empty) ::<={
                        symbolTable.add(
                            file:filename,
                            name:symbolName,
                            type:type,
                            parent:if (Object.length(of:treeStack) > 0) treeStack[Object.length(of:treeStack)-1] else '',
                            description:currentDesc,
                            returnObject:returnObject,
                            sourceLine:doclineContent
                        );

                    };
                    if (Object.length(of:treeStack) == 0) 
                        Object.push(object:treeStack, value:symbolName);
                    
                    currentlyWithinDoc = false;
                };
            } else ::<={
                if (docline) ::<={
                    lastStartsSpace = templsp;

                    // entered sub-context
                    if (symbolName != empty && !lastStartsSpace && startsSpace) ::<={
                        Object.push(object:treeStack, value:symbolName);
                        lastStartsSpace = true;                
                    };

                    if (Object.length(of:treeStack) > 1 && !startsSpace && lastStartsSpace) ::<={
                        Object.removeKey(from:treeStack, key:Object.length(of:treeStack)-1);
                    };

                    // new doc started
                    currentDesc = '';
                    currentDesc = currentDesc + doclineContent;
                    currentlyWithinDoc = true;       
                };
            };
        });
    };

    @:files = ::<={
        @pobj = Topaz.Filesystem.getPathFromString(string:Topaz.Resources.path);
        @out = [];
        @visited = {};
        @stk = [];
        Object.push(object:stk, value:pobj);
        

        loop(func:::{
            @:len = Object.length(of:stk);

            when(len == 0) false;            
            @cur = stk[len-1];
            Object.pop(object:stk);

            when (visited[cur.string]) true;
            visited[cur.string] = true;        
            
            @:chil = cur.children;
            if (Object.length(of:chil)) ::<={
                for(in:[0, Object.length(of:chil)], do:::(i) {
                    Object.push(object:stk, value:chil[i]); 
                });
            } else ::<={
                Object.push(object:out, value:cur.string);             
            };
            
            
            return true;
        });
        return out;
    };
    for(in:[0, Object.length(of:files)], do:::(i) {
        data2doc(filename:files[i]);
    });

    Topaz.Resources.path = outputPath;

    // replaces the objectstring with a link element IF 
    // it contains a reference name. THe link generated should
    // lead a user to the location of that symbol.
    @linkObject ::(doc, objectString) {
        @symbol = symbolTable.getSymbolFromObject(objectString:objectString);
        when(symbol != '') ::<={
            @symbolRef = symbolTable.getSymbol(symbolName:symbol);
            when (symbolRef == empty) objectString;

            // Get the parent symbol
            @parent = symbolRef;
            loop(func:::{
                when(parent.parent == '') false;
                parent = symbolTable.getSymbol(symbolName:parent.parent);    
                return true;            
            });

            return doc.createElement(
                content:objectString,
                tag:'a',
                attribs:'href="'+parent.name+'.html#'+symbol+'"'
            );
        };

        return objectString;
    };


    // next generate search index 
    @searchIndex = {};
    for(in:[0, Object.length(of:files)], do:::(n) {
        @:symbols = symbolTable.getFileEntities(file:files[n]);
        when(Object.length(of:symbols) == 0) empty;



        @mainClass = '';
        listen(to:::{
            for(in:[0, Object.length(of:symbols)], do:::(i) {
                if (symbols[i].type == symbolTable.type.CLASS ||
                    symbols[i].type == symbolTable.type.SINGLETON ||
                    symbols[i].type == symbolTable.type.MODULE) ::<={
                    mainClass = symbols[i].name;
                    send();
                };
            });
        });
        
        // add the searchable symbols here. Skip doc pages, they should not be searchable
        for(in:[0, Object.length(of:symbols)], do:::(i) {            
            when (symbols[i].type == symbolTable.type.DOCPAGE) empty;
            @t = searchIndex[symbols[i].name]; 
            if (t == empty) ::<={
                t = {};
                searchIndex[symbols[i].name] = t;
            };

            t[files[n]] = {
                'mainClass'   : mainClass
            };
        });
    });

    // returns a file link to the given symbolName
    // assumes that the search index is populated.
    @:generateLink ::(file, symbolName) {
        @:t = searchIndex[symbolName];
        when (t == empty) symbolName + '.html';
        when (symbolName == t[file].mainClass) symbolName+'.html';
        return t[file].mainClass+'.html#'+symbolName;
    };


    // table links
    for(in:[0, Object.length(of:files)], do:::(n) {
        @:symbols = symbolTable.getFileEntities(file:files[n]);
        when (Object.length(of:symbols) == 0) empty;

        for(in:[0, Object.length(of:symbols)], do:::(i) {
            @:entry = searchIndex[symbols[i].name];
            if (entry) ::<={
                @:link = generateLink(
                    file:files[n], 
                    symbolName:symbols[i].name
                );
                entry[files[n]]['link'] = link;
            };
        });
    });
    


    // create database for searching in a page
    ::<={
        @searchIndexText = 'var searchIndex = ' + JSON.encode(object:searchIndex);
        @:filename = "searchindex_db.js";

        @:asset = Topaz.Resources.fetchAsset(type:Topaz.Asset.TYPE.DATA, name:filename);
        @bytes = [];
        for(in:[0, String.length(of:searchIndexText)], do:::(i) {
            bytes[i] = String.charCodeAt(string:searchIndexText, index:i);
        });
        asset.bytes = bytes;
        Topaz.Resources.writeAsset(
            asset:asset, 
            extension:'txt', 
            path:filename
        );
    };


    // Generate banner links from doc pages
    @docpages = [];
    for(in:[0, Object.length(of:files)], do:::(n) {

        @:symbols = symbolTable.getFileEntities(file:files[n]);
        when (Object.length(of:symbols) == 0) empty;

        for(in:[0, Object.length(of:symbols)], do:::(i) {
            if (symbols[i].type == symbolTable.type.DOCPAGE)
                Object.push(
                    object:docpages, 
                    value:symbols[i].name
                );
        });
    });

    writeTextFile(str:"const doclinks = "+JSON.encode(object:docpages), filename:"doclinks_db.js");



    for(in:[0, Object.length(of:files)], do:::(n) {
        @content = '';
        @mainClass = '';
        @:symbols = symbolTable.getFileEntities(file:files[n]);
        when (Object.length(of:symbols) == 0) empty;


        for(in:[0, Object.length(of:symbols)], do:::(i) {

            @block = '';
            @typename = '';
            Topaz.log(message:symbols[i].name+'('+symbolTable.typeToString(type:symbols[i].type)+')  - generating');
            match(symbols[i].type) {
              (symbolTable.type.FUNCTION,
               symbolTable.type.FUNCTION_POINTER,
               symbolTable.type.STATIC_FUNCTION) :::<={
                typename = match(symbols[i].type) {
                    (symbolTable.type.FUNCTION): 'Function',
                    (symbolTable.type.FUNCTION_POINTER): 'FunctionPointer',
                    (symbolTable.type.STATIC_FUNCTION): 'StaticFunction'
                };
                block = doc.createElement(
                    content:linkObject(doc:doc, objectString:symbols[i].returns),
                    tag:'h5',
                    attribs:'class="FunctionReturnValue"'
                );
                
                // write out all arguments within a table for better formatting.
                if (Object.length(of:symbols[i].children) > 0) ::<={
                    block = block+doc.createElement(
                        content: if(symbols[i].type == symbolTable.type.FUNCTION_POINTER)  
                                '(*' + symbols[i].name + ')('
                            else
                                symbols[i].name + '(' 
                            ,    
                        tag:'h3',
                        attribs:'id="'+symbols[i].name+'"'
                    );
                    block = block+doc.createElement(
                        content:::<={
                            @out = '';
                            for(in:[0, Object.length(of:symbols[i].children)], do:::(m) {
                                out = out+doc.createElement(
                                    content: ::<={
                                        @row = '';
                                        row = row+doc.createElement(
                                            content:linkObject(doc:doc, objectString:symbolTable.getChildSymbol(symbol:symbols[i], childIndex:m).returns),
                                            tag:'th',
                                            attribs:'class="FunctionArgumentType"'
                                        );
                                        row = row+doc.createElement(
                                            content:symbols[i].children[m],
                                            tag:'th',
                                            attribs:'class="FunctionArgumentName"'
                                        );
                                        return row;    
                                    },
                                    tag:'tr'
                                );
                            });
                            return out;
                        },
                        tag:'table'
                    );
                    block = block+doc.createElement(
                        content:')',    
                        tag:'h3'
                    );
                } else ::<={
                    // no arguments? iconic empty parenth!
                    block = block + doc.createElement(
                        content: if(symbols[i].type == symbolTable.type.FUNCTION_POINTER)
                                '(*' + symbols[i].name + ')()'
                             else
                                symbols[i].name + '()' 
                        ,   
                        tag:'h3',
                        attribs:'id="'+symbols[i].name+'"'
                    );
                };
                block = block + doc.createElement(
                    content:symbols[i].desc,
                    tag:'pre'
                );
                if (Object.length(of:symbols[i].children)) ::<={
                    block = block + doc.createElement(
                        content:"<b>Arguments:</b></br></br>",
                        tag:'div'
                    );

                    for(in:[0, Object.length(of:symbols[i].children)], do:::(m) {
                        block = block + 
                            doc.createElement(
                                content:symbols[i].children[m],
                                tag:'div'
                            ) +
                            doc.createElement(
                                content:symbolTable.getChildSymbol(
                                    symbol:symbols[i], 
                                    childIndex:m
                                ).desc,
                                
                                tag:'pre'
                            );
                    });
                };
                
              },

              (symbolTable.type.DOCPAGE):::<={
                typename = 'Docpage';
                if (mainClass == '') mainClass = symbols[i].name;
                block = doc.createElement(
                    content:symbols[i].name + '',    
                    tag:'h1',
                    attribs:'id="'+symbols[i].name+'"'
                );
                block = block + doc.createElement(
                    content:symbols[i].desc + '',    
                    tag:'pre'
                );

                for(in:[0, Object.length(of:symbols[i].children)], do:::(m) {
                    @child = symbolTable.getChildSymbol(symbol:symbols[i], childIndex:m);
                    if (child.type == symbolTable.type.LINK) ::<= {
                        block = block+doc.createElement(
                            content: linkObject(doc:doc, objectString:child.returns),
                            tag:'i'
                        );                
                    };
                });

              },




              (symbolTable.type.CLASS,
               symbolTable.type.SINGLETON,
               symbolTable.type.MODULE):::<={
                
                typename = match(symbols[i].type) {
                    (symbolTable.type.CLASS): 'Class',
                    (symbolTable.type.SINGLETON): 'Singleton',
                    (symbolTable.type.MODULE): 'Module'
                };

                if (mainClass == '') mainClass = symbols[i].name;
                block = doc.createElement(
                    content:symbols[i].name + '',    
                    tag:'h1',
                    attribs:'id="'+symbols[i].name+'"'
                );
                for(in:[0, Object.length(of:symbols[i].children)], do:::(m) {
                    @child = symbolTable.getChildSymbol(symbol:symbols[i], childIndex:m);
                    if (child.type == symbolTable.type.INHERITS) ::<= {
                        block = block+doc.createElement(
                            content:'- inherits from: ' + linkObject(doc:doc, objectString:child.returns),
                            tag:'i'
                        );                
                    };
                });

                

                block = block + doc.createElement(
                    content:symbols[i].desc + '',    
                    tag:'pre'
                );
                



                @hasFunctions = false;
                @funcList = '';

                
                @mem = '';
                // list all quick symbols
                for(in:[0, Object.length(of:symbols[i].children)], do:::(m) {
                    if (symbolTable.getChildSymbol(symbol:symbols[i], childIndex:m).type == symbolTable.type.FUNCTION) ::<={
                        mem = mem+ 
                        doc.createElement(
                            content:doc.createElement(
                                content:symbols[i].children[m] + "()",
                                tag:'a',
                                attribs:'href="'+generateLink(file:files[n], symbolName:symbols[i].children[m])+'"'
                            ),
                            tag:'div'
                        );
                        hasFunctions = true;
                    };
                });

                @hasStFunctions = false;
                @funcStList = '';

                
                @memst = '';
                // list all quick symbols
                for(in:[0, Object.length(of:symbols[i].children)], do:::(m) {
                    if (symbolTable.getChildSymbol(symbol:symbols[i], childIndex:m).type == symbolTable.type.STATIC_FUNCTION) ::<={
                        memst = memst+ 
                        doc.createElement(
                            content:doc.createElement(
                                content:symbols[i].children[m] + "()",
                                tag:'a',
                                attribs:'href="'+generateLink(file:files[n], symbolName:symbols[i].children[m])+'"'
                            ),
                            tag:'div'
                        );
                        hasStFunctions = true;
                    };
                });


                if (hasFunctions)
                    funcList = funcList + doc.createElement(
                        content:"</br><b>Member Functions:</b></br></br>",
                        tag:'div'
                    );
                
                if (hasStFunctions)
                    funcList = funcList + doc.createElement(
                        content:"</br><b>Static Functions:</b></br></br>",
                        tag:'div'
                    );

                funcList = funcList + doc.createElement(
                    content:mem + memst,
                    tag:'div',
                    attribs:'class="PublicSymbols"'
                );
                block = block + funcList;



                @hasEnums = false;
                @enumList = '';
                enumList = enumList + doc.createElement(
                    content:"</br><b>Enumerators:</b></br></br>",
                    tag:'div'
                );
                
                mem = '';
                // list all quick symbols
                for(in:[0, Object.length(of:symbols[i].children)], do:::(m) {
                    if (symbolTable.getChildSymbol(symbol:symbols[i], childIndex:m).type == symbolTable.type.ENUMERATOR) ::<={
                        mem = mem +
                        doc.createElement(
                            content:doc.createElement(
                                content:symbols[i].children[m],
                                tag:'a',
                                attribs:'href="'+generateLink(file:files[n], symbolName:symbols[i].children[m])+'"'
                            ),
                            tag:'div'
                        );
                        hasEnums = true;
                    };
                });
                enumList = enumList + doc.createElement(
                    content:mem,
                    tag:'div',
                    attribs:'class="PublicSymbols"'
                );
                if (hasEnums)
                    block = block + enumList;
                


                @publicMemList = '';
                @hasPublicMems = false;
                publicMemList = publicMemList + doc.createElement(
                    content:"</br></br><b>Public Variables:</b></br></br>",
                    tag:'div'
                );
                // list all quick symbols
                mem = '';
                for(in:[0, Object.length(of:symbols[i].children)], do:::(m) {
                    if (symbolTable.getChildSymbol(symbol:symbols[i], childIndex:m).type == symbolTable.type.VARIABLE) ::<={
                        // for structs treated as classes
                        mem = mem+ 
                        doc.createElement(
                            content:doc.createElement(
                                content:linkObject(
                                    doc:doc, 
                                    objectString:symbolTable.getChildSymbol(symbol:symbols[i], childIndex:m).returns
                                ),
                                tag:"span",
                                attribs:'class="VariableType"'
                            ) + 
                            
                            
                            doc.createElement(
                                content:' ' + symbols[i].children[m],
                                tag:"span",
                                attribs:'class="VariableName"'
                            ),
                            tag:'div'
                        ) +
                        doc.createElement(
                            content:symbolTable.getChildSymbol(symbol:symbols[i], childIndex:m).desc,
                            tag:'pre'
                        );
                        hasPublicMems = true;
                    }; 
                });
                
                publicMemList = publicMemList + doc.createElement(
                    content:mem,
                    tag:'div',
                    attribs:'class="PublicMembers"'
                );

                
                if (hasPublicMems) 
                    block = block + publicMemList;
                


                
              },


              (symbolTable.type.ENUMERATOR):::<={
                typename = 'Enumerator';
                block = doc.createElement(
                    content:symbols[i].name,    
                    tag:'h3',
                    attribs:'id="'+symbols[i].name+'"'
                );

                block = block + doc.createElement(
                    content:symbols[i].desc,    
                    tag:'pre'
                );

                block = block + "<table>\n";
                for(in:[0, Object.length(of:symbols[i].children)], do:::(m) {
                    @:value = symbolTable.getSymbol(symbolName:symbols[i].children[m]);
                    block = String.combine(strings:[
                        block,
                        "<tr>\n",
                        doc.createElement(content:symbols[i].children[m], tag:'th'),
                        doc.createElement(content:symbolTable.getChildSymbol(symbol:symbols[i], childIndex:m).desc, tag:'td'),
                        "</tr>\n"
                    ]);
                });
                block = block + "</table>\n";

              },

              (symbolTable.type.ENUM_VALUE): empty,
              (symbolTable.type.OPEN_STRUCTURE):::<={
                typename = 'OpenStructure';
                block = doc.createElement(
                    content:symbols[i].name,    
                    tag:'h3',
                    attribs:'id="'+symbols[i].name+'"'
                );

                block = block + doc.createElement(
                    content:symbols[i].desc,    
                    tag:'pre'
                );

                block = block + doc.createElement(
                    content:"<b>Members:</b></br></br>",
                    tag:'div'
                );
                @mem = '';
                for(in:[0, Object.length(of:symbols[i].children)], do:::(m) {
                    mem = mem+ 
                    doc.createElement(
                        content:doc.createElement(
                            content:linkObject(
                                doc:doc, 
                                objectString:symbolTable.getChildSymbol(symbol:symbols[i], childIndex:m).returns
                            ),
                            tag:"span",
                            attribs:'class="VariableType"'
                        ) + 
                        
                        
                        doc.createElement(
                            content:' ' + symbols[i].children[m],
                            tag:"span",
                            attribs:'class="VariableName"'
                        ),
                        tag:'div'
                    ) +
                    doc.createElement(
                        content:symbolTable.getChildSymbol(symbol:symbols[i], childIndex:m).desc,
                        tag:'pre'
                    );
                });
                
                block = block + doc.createElement(
                    content:mem,
                    tag:'div',
                    attribs:'class="PublicMembers"'
                );

              },

              (symbolTable.type.MACRO):::<={
                typename = 'Macro';
                block = block + doc.createElement(
                    content:symbols[i].name,    
                    tag:'h3',
                    attribs:'id="'+symbols[i].name+'"'
                );

                block = block + doc.createElement(
                    content:symbols[i].desc,    
                    tag:'pre'
                );
                  
              }


            };

            if (typename != '') ::<={
                content = content + doc.createElement(
                    content:doc.createElement(
                        content:'('+typename+')</br>',
                        tag:'div',
                        attribs:'class="typetag"'
                    )+
                    
                    doc.createElement(
                        content:block, 
                        tag:'div'
                    ) + "</br>",

                    tag:'div',
                    attribs:'class="'+typename+'"'
                );
            };
        });


        @navsearch = doc.createElement(
            content:doc.createElement(
                content:'',
                tag:'input',
                attribs:
                    'class="navsearch-input"'+
                    'id="searchbar"'+
                    'type="search"'+
                    'onchange="docext_on_input()"' +
                    'oninput="docext_on_input()"'
            ),
            tag:'div'
        );




        doc.addContent(
            text:doc.createElement(
                content:doc.createElement(
                    content:'',
                    tag:'img',
                    attribs:'src="'+resourcePath+'logoreduced.png"'+
                            'class="navlogo"'              
                ) + doc.createElement(
                    content:'',
                    tag:'div',
                    attribs:'class="navbar"'+
                            'id="navbar"'
                ),
                tag:'div',
                attribs:'class="navarea"'           
            ) + doc.createElement(
                content:doc.createElement(
                    content:navsearch + doc.createElement(
                        content:'',
                        tag:'div',
                        attribs:'id="searchresults"'
                    ),
                    tag:'div',
                    attribs:'class="navlist"'
                ) + 
                doc.createElement(
                    content:content,
                    tag:'div',
                    attribs:'class="symbollist"'  
                ),
                tag:'div'
            ) + doc.createElement(
                content: 'Generated by Topaz v'+Topaz.versionMajor+'.'+Topaz.versionMinor+'.'+Topaz.versionMicro +'. Johnathan Corkery, 2022. <a href="https://github.com/jcorks/topaz">https://github.com/jcorks/topaz</a>',
                tag: 'i',
                attribs:'class="symbollist credits"'  
            )
        );
        if (mainClass != '') ::<={
            doc.write(filename:generateLink(file:files[n], symbolName:mainClass));
            Topaz.log(message: generateLink(file:files[n], symbolName:mainClass));
        };
    });
    Topaz.Resources.path = origPath;
};


return generateDocumentation;


