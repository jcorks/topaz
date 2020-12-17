
var symbolTable = (function(){
    

    var hints = [];
    var symbols = {};    
    var files = {};
    var symbolExtractors = {};
    const types = {
        NONE : -1,
        FUNCTION : 0,
        CLASS : 1,
        ENUMERATOR : 2,
        OPEN_STRUCTURE : 3,
        LANGUAGE_TYPE : 4
    }

    
    // registers a new symbol
    const newRef = function(name) {
        symbols[name] = {
            'desc'       : '',
            'children'   : [],     // order matters for remaking document
            'parameters' : [],     // {name, desc, typeSymbol}
            'parent'     : '',     // parent name
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

        // adds a new symbol
        add : function(
            file,        // source file
            name,        // symbol name 
            type,        // type of symbol
            parent,      // parent symbol name
            description  // description of symbol
        ) {
            const nameCleaned   = clean(name);
            const parentCleaned = clean(parent);

            if (!symbols[nameCleaned])   newRef(nameCleaned);
            if (!symbols[parentCleaned]) newRef(parentCleaned);


            symbols[nameCleaned].desc = description;    
            symbols[nameCleaned].type = type;
            symbols[parentCleaned].children.push(nameCleaned);

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
                topaz.log('addParam(): added params to a symbol that doesnt exist');
            }            

            symbols[symbolCleaned].parameters.push({
                'name'       : name,
                'desc'       : description,
                'typeSymbol' : typeCleaned
            });
        },


        // Sets a plain text hint for a certain type
        setTypeHint : function(
            type,
            hint
        ) {
            hints.push({'hint' : hint, 'type' : type});
        },


        // Returns a type 
        guessType : function(
            textLine            
        ) {
            for(var i = 0; i < hints.length; ++i) {
                if (textLine.includes(hints[i].hint)) {
                    return hints[i].type;
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

        // extracts a symbol name from a line.
        getSymbolName : function(
            type,
            line
        ) {
            if (type == types.NONE) return '';
            if (!symbolExtractors[type]) return '';
            
            return line.match(symbolExtractors[type])[1];
        }
    }
})();






