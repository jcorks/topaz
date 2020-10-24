#include <topaz/containers/string.h>

#include <string.h>

int test__string_simple() {
    topazString_t * str = topaz_string_create_from_c_str("Hello!");
    
    if (strcmp("Hello!", topaz_string_get_c_str(str))) 
        // Why: The string is just "Hello!" and apparently its not. Back to the drawing board!
        return 1;

    topaz_string_concat_printf(str, "%s", " How are you?");

    if (strcmp("Hello! How are you?", topaz_string_get_c_str(str))) 
        // Why: The concatenation failed.
        return 2;


    if (strcmp("Hello!", topaz_string_get_c_str(topaz_string_get_substr(str, 0, 5))))
        // Why: the get_substr operation failed or produced unexpected results
        return 3;

    topaz_string_set(str, topaz_string_get_substr(str, 0, 6));



    if (topaz_string_get_length(str) != strlen("Hello!") ||
        strcmp("Hello!", topaz_string_get_c_str(str)))
        // Why: the C string representation is not correct
        return 4;

    
    
    topaz_string_clear(str);
    if (topaz_string_get_length(str) != 0) 
        // Why: clear should remove the strings contents entirely.
        return 5;
    topaz_string_concat_printf(str, "%d%d%d%d", 1, 2, 3, 4);

    if (strcmp(topaz_string_get_c_str(str), "1234"))
        // Why: concat_printf should follow C printf rules.
        return 6;
    
    
    topaz_string_destroy(str);

    return 0;
}


int test__string_advanced() {
    topazString_t * str = topaz_string_create_from_c_str("In a declaration whose storage-class specifier is typedef, each declarator defines an identifier to be a typedef name that denotes the type specified for the identifier in the way described in 6.7.5. Any array size expressions associated with variable length array declarators are evaluated each time the declaration of the typedef name is reached in the order of execution. A typedef declaration does not introduce a new type, only a synonym for the type so specified. That is, in the following declarations: typedef T type_ident; type_ident D; type_ident is defined as a typedef name with the type specified by the declaration specifiers in T (known as T ), and the identifier in D has the type ‘‘derived-declarator- type-list T ’’ where the derived-declarator-type-list is specified by the declarators of D. A typedef name shares the same name space as other identifiers declared in ordinary             declarators.     ");
    static char * words[] = {
        "In",
        "a",
        "declaration",
        "whose",
        "storage-class",
        "specifier",
        "is",
        "typedef,",
        "each",
        "declarator",
        "defines",
        "an",
        "identifier",
        "to",
        "be",
        "a",
        "typedef",
        "name",
        "that",
        "denotes",
        "the",
        "type",
        "specified",
        "for",
        "the",
        "identifier",
        "in",
        "the",
        "way",
        "described",
        "in",
        "6.7.5.",
        "Any",
        "array",
        "size",
        "expressions",
        "associated",
        "with",
        "variable",
        "length",
        "array",
        "declarators",
        "are",
        "evaluated",
        "each",
        "time",
        "the",
        "declaration",
        "of",
        "the",
        "typedef",
        "name",
        "is",
        "reached",
        "in",
        "the",
        "order",
        "of",
        "execution.",
        "A",
        "typedef",
        "declaration",
        "does",
        "not",
        "introduce",
        "a",
        "new",
        "type,",
        "only",
        "a",
        "synonym",
        "for",
        "the",
        "type",
        "so",
        "specified.",
        "That",
        "is,",
        "in",
        "the",
        "following",
        "declarations:",
        "typedef",
        "T",
        "type_ident;",
        "type_ident",
        "D;",
        "type_ident",
        "is",
        "defined",
        "as",
        "a",
        "typedef",
        "name",
        "with",
        "the",
        "type",
        "specified",
        "by",
        "the",
        "declaration",
        "specifiers",
        "in",
        "T",
        "(known",
        "as",
        "T",
        "),",
        "and",
        "the",
        "identifier",
        "in",
        "D",
        "has",
        "the",
        "type",
        "‘‘derived-declarator-",
        "type-list",
        "T",
        "’’",
        "where",
        "the",
        "derived-declarator-type-list",
        "is",
        "specified",
        "by",
        "the",
        "declarators",
        "of",
        "D.",
        "A",
        "typedef",
        "name",
        "shares",
        "the",
        "same",
        "name",
        "space",
        "as",
        "other",
        "identifiers",
        "declared",
        "in",
        "ordinary",
        "declarators."
    };
    uint32_t tokenIter = 0;
    

    topazString_t * strcopy = topaz_string_clone(str);

    if (topaz_string_get_length(str) != topaz_string_get_length(strcopy) ||
        !topaz_string_test_eq(str, strcopy)) 
        // Why: The string should have been cloned entirely
        return 1;

    topaz_string_clear(strcopy);
    topaz_string_concat_printf(strcopy, " \n\t\r");
    
    const topazString_t * iter;
    for( iter = topaz_string_chain_start(str, strcopy); 
        !topaz_string_chain_is_end(str);
         iter = topaz_string_chain_proceed(str)) {

        if (strcmp(topaz_string_get_c_str(iter), words[tokenIter++])) {
            // Why: a token was out of order / missing / incorrect
            return 2;
        }
    }     
    // Why: the tokenized list ended prematurely!
    if (tokenIter != 145) return 3; 



    topaz_string_destroy(str);
    topaz_string_destroy(strcopy);   
    return 0;
}
