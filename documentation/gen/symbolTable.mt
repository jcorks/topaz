@:class = import(module:'Matte.Core.Class');
@:Topaz = import(module:'Topaz');
return class(
    name: 'SymbolTable',
    define:::(this) {
        

        @hints = [];
        @symbols = {};    
        @files = {};
        @symbolExtractors = {};
        @returnExtractors = {};
        @objectExtractor;

        @:types = {
            NONE : -1,
            FUNCTION : 0,
            CLASS : 1,
            ENUMERATOR : 2,
            OPEN_STRUCTURE : 3,
            LANGUAGE_TYPE : 4,
            ENUM_VALUE : 5,
            VARIABLE : 6, 
            FUNCTION_POINTER : 7,
            MACRO : 8,
            DOCPAGE : 9,
            SINGLETON : 10,
            INHERITS : 11,
            MODULE : 12,
            LINK : 13,
            STATIC_FUNCTION : 14
        };

        @:typeToString = ::(type) {
            return match(type) {
              (types.NONE):            "None",
              (types.FUNCTION):        "Function",
              (types.CLASS):           "Class",
              (types.SINGLETON):       "Singleton",
              (types.ENUMERATOR):      "Enumerator",
              (types.OPEN_STRUCTURE):  "Structure",
              (types.LANGUAGE_TYPE):   "BuiltIn",
              (types.ENUM_VALUE):      "EnumValue",
              (types.VARIABLE):        "Variable/Parameter",
              (types.FUNCTION_POINTER):"Function Pointer",
              (types.MACRO):           "Macro",
              (types.DOCPAGE):         "Documentation Page",
              (types.INHERITS):        "Inherits",
              (types.STATIC_FUNCTION): "Static Function"
            };
        };
        
        // registers a new symbol
        @:newRef = ::(name) {
            return {
                'desc'       : '',
                'children'   : [],     // order matters for remaking document
                'parameters' : [],     // {name, desc, typeSymbol}
                'parent'     : '',     // parent name
                'childrenRefs' : [],
                'type'       : types.NONE
            };        
        };
        

        // removed extra characters from a symbol name
        @:clean = ::(name) {
            when(name == empty) empty;
            return String.replace(
                string:name,
                keys:[
                    '(', ')', '[', ']',
                    ' ', '\t', '\n', '\r'
                ],
                with:''
            );
        };

        // trailing and leading space
        @:strip ::(str) {
            loop(func:::{
                when(str == empty) false;
                @fc = String.charAt(string:str, index:0);
                when(fc == ' ' || fc == '\r' || fc == '\n' || fc == '\t') ::<= {
                    str = String.substr(string:str, from:1, to:String.length(of:str)-1);
                    return true;
                };
                return false;
            });    

            loop(func:::{
                when(str == empty) false;
                @fc = String.charAt(string:str, index:String.length(of:str)-1);
                when(fc == ' ' || fc == '\r' || fc == '\n' || fc == '\t') ::<= {
                    str = String.substr(string:str, from:0, to:String.length(of:str)-2);
                    return true;
                };
                return false;
            });    
            return if (str == empty) '' else str;
        };

        this.interface = {
            type : {get::{return types;}},

            typeToString : typeToString,

            // adds a new symbol
            add : ::(
                file,         // source file
                name,         // symbol name 
                type,         // type of symbol
                parent,       // parent symbol name
                description,  // description of symbol
                returnObject, // what the object returns
                sourceLine    // original line from which it was extracted
            ) {        
                if (type != types.INHERITS && name == '') error(detail:'Empty name cannot be added.');
                @:nameCleaned   = clean(name:name);
                @:parentCleaned = clean(name:parent);
                @symbolB       = symbols[nameCleaned];
                @parentSymbolB = symbols[parentCleaned];
                

                if (symbolB == empty) ::<= {
                    symbols[nameCleaned] = [];
                    symbolB = symbols[nameCleaned];
                };

                if (parentSymbolB == empty) ::<={
                    symbols[parentCleaned] = [];
                    parentSymbolB = symbols[parentCleaned];
                };

                @symbol = symbolB[file];
                @parentSymbol = parentSymbolB[file];

                if (symbol == empty)       symbol       = newRef(name:nameCleaned);
                if (parentSymbol == empty) parentSymbol = newRef(name:parentCleaned);



                symbol.name = nameCleaned;
                symbol.desc = description;    
                symbol.type = type;
                symbol.returns = returnObject;
                Object.push(object:parentSymbol.children, value:nameCleaned);
                Object.push(object:parentSymbol.childrenRefs, value:symbol);
                symbol.file = file;
                symbol.parent = parentCleaned;
                symbol.source = sourceLine;

                if (symbol.type != types.VARIABLE && symbol.type != types.INHERITS && symbol.type != types.LINK) 
                    symbolB[file] = symbol;

                if (parentSymbol == empty)
                    parentSymbolB[file] = parentSymbol;

                if (files[file] == empty) 
                    files[file] = [];
                
                if (symbol.type != types.VARIABLE && symbol.type != types.INHERITS && symbol.type != types.LINK) 
                    Object.push(object:files[file], value:nameCleaned);
            },




            // Sets a plain text hint for a certain type
            setTypeHint : ::(
                hint,
                type,
                exclude
            ) {
            
                Object.push(
                    object:hints,
                    value:{
                        'hint' : hint, 
                        'type' : type, 
                        'excludes' : exclude
                    }
                );
            },


            // Returns a type 
            guessType ::(
                textLine            
            ) {
                when (textLine == '') types.NONE;
                textLine = strip(str:textLine);
                return listen(to:::{
                    for(in:[0, Object.length(of:hints)], do:::(i) {
                        if (String.contains(string:textLine, key:hints[i].hint)) ::<= {
                            if (hints[i].excludes == empty || !String.contains(string:textLine, key:hints[i].excludes)) 
                                send(message:hints[i].type);
                        };
                    });
                    return types.NONE;        
                });
            },

            // sets regex to extract symbol names from different types.
            setSymbolExtractor ::(
                format,
                type,
                removeKeys,
                groupIndex
            ) {
                if (groupIndex == empty) groupIndex = 0;
                symbolExtractors[type] = {format:format, removeKeys:removeKeys, groupIndex:groupIndex};
            },

            // sets a return / self object extractor
            // In this context, an "Object" is the language-specific item 
            // that may contain an actual symbol that could be referenced within 
            // the documentation tree.
            setReturnObjectExtractor ::(
                format,
                type,
                removeKeys,
                groupIndex
            ) {
                if (groupIndex == empty) groupIndex = 0;
                returnExtractors[type] = {format:format, removeKeys:removeKeys, groupIndex:groupIndex};
            },

            // Sets a regex to extract symbols from objects, such as return or parameter object strings.
            // the first group returned is any symbol found. 
            setObjectExtractor : ::(
                format,
                removeKeys,
                groupIndex
            ) {
                if (groupIndex == empty) groupIndex = 0;            
                objectExtractor = {format:format, removeKeys:removeKeys, groupIndex:groupIndex};

            },

            // Returns a symbol name if the object string refers to one.
            // Sometimes, valid objects will NOT return 
            // symbols (such as the case with built-in types).
            getSymbolFromObject : ::(
                objectString
            ) {
                when (objectExtractor == empty) '';
                if (objectExtractor.removeKeys) ::<={
                    objectString = String.replace(
                        string:objectString,
                        keys : objectExtractor.removeKeys,
                        with: ''
                    );
                };
                @:result = objectString;
                //@result = clean(name:String.scan(value:objectString, format:objectExtractor.format)[objectExtractor.groupIndex]);
                when (result == '' ) '';
                when (symbols[result] != empty) result;
                return '';
            },


            // extracts a symbol name from a line.
            getSymbolName : ::(
                type,
                line
            ) {
                when(type == types.NONE) '';
                when(symbolExtractors[type] == empty) '';
                @:b = symbolExtractors[type];

                if (b.removeKeys) ::<={
                    line = String.replace(
                        string:line,
                        keys : b.removeKeys,
                        with: ''
                    );
                };
    
                @result = clean(name:String.scan(value:line, format:b.format)[b.groupIndex]);

                when(result == empty) '';
                return result;
            },

            getReturnObject : ::(
                type,
                line
            ) {
                when (type == types.NONE) '';
                when (returnExtractors[type] == empty) '';
                @:b = returnExtractors[type];
                
                if (b.removeKeys) ::<={
                    line = String.replace(
                        string:line,
                        keys : b.removeKeys,
                        with: ''
                    );
                };


                @retobj = clean(name:String.scan(value:line, format:b.format)[b.groupIndex]);
                when(retobj == empty) '';
                return retobj;
            },

            getReport : ::(file) {
                @text = 'Within ' + file + '...\n\n';
                @:names = files[file];
                when(names == empty) '';
                for(in:[0, Object.length(of:names)], do:::(i) {
                    @:symbol = symbols[names[i]][file];
                    text += 'Name:        ' + names[i] + '\n';
                    text += 'Type:        ' + typeToString(type:symbol.type) + '\n';
                    text += 'Description: ' + symbol.desc + '\n';
                    text += 'parent:      ' + symbol.parent + '\n\n';
                });
                return text;
            },

            getSymbol : ::(symbolName) {
                @b = symbols[symbolName];
                when(b == empty) empty;
                
                return b[Object.keys(of:b)[0]];
            },
            getSymbolSpecific : ::(symbolName, file) {
                @b = symbols[symbolName];
                when(b == empty) empty;
                return b[file];
            },

            getChildSymbol : ::(symbol, childIndex) {
                return symbol.childrenRefs[childIndex];
            },

            getFileEntities : ::(file) {
                @out = [];
                @:names = files[file];
                when(names == empty) out;
                for(in:[0, Object.length(of:names)], do:::(i) {
                    @:symbol = symbols[names[i]][file];
                    @:obj = symbol;
                    obj.name = names[i];
                    out[i] = obj;
                });
                return out;
            }
        };
    }
);






