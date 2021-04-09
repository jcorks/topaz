
var SymbolTable = function(){
    

    var hints = [];
    var symbols = {};    
    var files = {};
    var symbolExtractors = {};
    var returnExtractors = {};
    var objectExtractor = null;

    const types = {
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
        SINGLETON : 10
    }

    const typeToString = function(type) {
        switch(type) {
          case types.NONE:           return "None";
          case types.FUNCTION:       return "Function";
          case types.CLASS:          return "Class";
          case types.SINGLETON:      return "Singleton";
          case types.ENUMERATOR:     return "Enumerator";
          case types.OPEN_STRUCTURE: return "Structure";
          case types.LANGUAGE_TYPE:  return "BuiltIn";
          case types.ENUM_VALUE:     return "EnumValue";
          case types.VARIABLE:       return "Variable/Parameter";
          case types.FUNCTION_POINTER:      return "Function Pointer";
          case types.MACRO:          return "Macro";
          case types.DOCPAGE:        return "Documentation Page";
        }
    }
    
    // registers a new symbol
    const newRef = function(name) {
        return {
            'desc'       : '',
            'children'   : [],     // order matters for remaking document
            'parameters' : [],     // {name, desc, typeSymbol}
            'parent'     : '',     // parent name
            'childrenRefs' : [],
            'type'       : types.NONE
        };        
    }

    // removed extra characters from a symbol name
    const clean = function(name) {
        var out = name.replace(/\(\)\[\]\s/, '');
        return out;
    }

    return {
        type : types,

        typeToString : typeToString,

        // adds a new symbol
        add : function(
            file,         // source file
            name,         // symbol name 
            type,         // type of symbol
            parent,       // parent symbol name
            description,  // description of symbol
            returnObject, // what the object returns
            sourceLine    // original line from which it was extracted
        ) {
            const nameCleaned   = clean(name);
            const parentCleaned = clean(parent);

            var symbol       = symbols[nameCleaned];
            var parentSymbol = symbols[parentCleaned];

            if (!symbol)       symbol       = newRef(nameCleaned);
            if (!parentSymbol) parentSymbol = newRef(parentCleaned);



            symbol.name = nameCleaned
            symbol.desc = description;    
            symbol.type = type;
            symbol.returns = returnObject;
            parentSymbol.children.push(nameCleaned);
            parentSymbol.childrenRefs.push(symbol);
            symbol.file = file;
            symbol.parent = parentCleaned;
            symbol.source = sourceLine;

            if (symbol.type != types.VARIABLE) 
                symbols[nameCleaned] = symbol;

            if (!parentSymbol)
                symbols[parentCleaned] = parentSymbol;

            if (!files[file]) {
                files[file] = [];
            }
            if (symbol.type != types.VARIABLE) 
                files[file].push(nameCleaned);
        },


        // adds a parameter to a symbol
        addParam : function(
            symbol,
            name,
            description,
            typeSymbol
        ) {
            if (!symbol) return;

            const symbolCleaned = clean(symbol);
            const typeCleaned   = clean(typeSymbol);

            if (symbol == '') return;
            
            if (!symbols[symbolCleaned]) {
                Topaz.log('addParam(): added params to a symbol that doesnt exist');
            }            

            symbols[symbolCleaned].parameters.push({
                'name'       : name,
                'desc'       : description,
                'typeSymbol' : typeCleaned
            });
        },


        // Sets a plain text hint for a certain type
        setTypeHint : function(
            hint,
            type,
            exclude
        ) {
            hints.push({'hint' : hint, 'type' : type, 'excludes' : exclude});
        },


        // Returns a type 
        guessType : function(
            textLine            
        ) {
            if (textLine == '') return types.NONE;
            for(var i = 0; i < hints.length; ++i) {
                if (textLine.includes(hints[i].hint)) {
                    if (!hints[i].excludes || !textLine.includes(hints[i].excludes)) {
                        return hints[i].type;
                    }
                }
            }    
            return types.NONE;        
        },

        // sets regex to extract symbol names from different types.
        setSymbolExtractor : function(
            regex,
            type 
        ) {
            symbolExtractors[type] = regex;
        },

        // sets a return / self object extractor
        // In this context, an "Object" is the language-specific item 
        // that may contain an actual symbol that could be referenced within 
        // the documentation tree.
        setReturnObjectExtractor : function(
            regex,
            type 
        ) {
            returnExtractors[type] = regex;
        },

        // Sets a regex to extract symbols from objects, such as return or parameter object strings.
        // the first group returned is any symbol found. 
        setObjectExtractor : function(
            regex
        ) {
            objectExtractor = regex;

        },

        // Returns a symbol name if the object string refers to one.
        // Sometimes, valid objects will NOT return 
        // symbols (such as the case with built-in types).
        getSymbolFromObject : function(
            objectString
        ) {
            if (!objectExtractor) return ''
            var result = objectString.match(objectExtractor);
            if (!result) return '';
            if (result[1] && symbols[result[1]]) {
                return result[1]
            }
            return '';
        },


        // extracts a symbol name from a line.
        getSymbolName : function(
            type,
            line
        ) {
            if (type == types.NONE) return '';
            if (!symbolExtractors[type]) return '';

            return line.match(symbolExtractors[type])[1];
        },

        getReturnObject : function(
            type,
            line
        ) {
            if (type == types.NONE) return '';
            if (!returnExtractors[type]) return '';

            var retobj = line.match(returnExtractors[type]);
            if (retobj) return retobj[1]
            else return '';
        },

        getReport : function(file) {
            var text = 'Within ' + file + '...\n\n';
            const names = files[file];
            if (!names) return '';
            for(var i = 0; i < names.length; ++i) {
                const symbol = symbols[names[i]];
                text += 'Name:        ' + names[i] + '\n';
                text += 'Type:        ' + typeToString(symbol.type) + '\n';
                text += 'Description: ' + symbol.desc + '\n';
                text += 'parent:      ' + symbol.parent + '\n\n';
            }
            return text;
        },

        getSymbol : function(symbolName) {
            return symbols[symbolName];
        },

        getChildSymbol : function(symbol, childIndex) {
            return symbol.childrenRefs[childIndex];
        },

        getFileEntities : function(file) {
            var out = [];
            const names = files[file];
            if (!names) return out;
            for(var i = 0; i < names.length; ++i) {
                const symbol = symbols[names[i]];
                const obj = symbol;
                obj.name = names[i];
                out.push(obj);
            }
            return out;
        }
    }
};






