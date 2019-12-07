/*
Copyright (c) 2020, Johnathan Corkery. (jcorkery@umich.edu)
All rights reserved.

This file is part of the topaz project (https://github.com/jcorks/topaz)
topaz was released under the MIT License, as detailed below.



Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is furnished 
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall
be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.


*/


#ifndef H_TOPAZDC__STRING__INCLUDED
#define H_TOPAZDC__STRING__INCLUDED

/*

    String
    -----
    


*/
typedef struct topazString_t topazString_t;


/// Creates a new, empty string. 
///
topazString_t * topaz_string_create();

/// Creates a new string initialized with the contents of the given C-string 
///
topazString_t * topaz_string_create_from_c_str(const char *);

/// Creates a new string as a copy of the given string 
///
topazString_t * topaz_string_clone(const topazString_t *);

/// Destroyes and frees a topaz string 
///
void topaz_string_destroy(topazString_t *);






/// Sets the contents of the string A to the contents of string B
///
void topaz_string_set(topazString_t * A, const topazString_t * B);

/// Adds the given C printf-formatted string and accompanying arguments 
/// to the given string.
///
void topaz_string_concat_printf(topazString_t *, const char * format, ...);


/// Adds the given string B to the end of the given string A.
///
void topaz_string_concat(topazString_t *, const topazString_t *);


/// Returns a read-only copy of a portion of the given string 
/// from and to denote character indices. The substring is valid until 
/// the next call to this function with the same input string.
const topazString_t * topaz_string_get_substr(
    const topazString_t *, 
    uint32_t from, 
    uint32_t to
);



/// Gets a read-only pointer to a c-string representation of the 
/// string.
const char * topaz_string_get_c_str(const topazString_t *);

/// Gets the number of characters within the string.
///
uint32_t topaz_string_get_length(const topazString_t *);

/// Creates a temporary 
const topazString_t * topaz_string_cast_from_c_str(const char *);






/////// Chain control
/// 
/// Chain functions can be used to 
/// work on strings in a token-like fashion.


/// Resets the chain state of the string. Using topaz_string_chain_current()
/// will return the first token according to the delimiters given.
///
void topaz_string_chain_start(topazString_t * t, const topazString_t * delimiters);

/// Returns the current token in the chain.
///
const topazString_t * topaz_string_chain_current(topazString_t * t);

/// Returns whether the last link in the chain has been reached.
///
int topaz_string_chain_is_end(topazString_t * t);

/// Goes to the next token in the chain.
/// 
void topaz_string_chain_proceed(topazString_t * t);

#endif

