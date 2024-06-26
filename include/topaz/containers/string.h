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

#include <stdint.h>

///
/// Holds an array of characters, usually meant to convey text.
///
typedef struct topazString_t topazString_t;


/// Creates a new, empty string. 
///
topazString_t * topaz_string_create();

/// Creates a new string initialized with the contents of the given C-string 
///
topazString_t * topaz_string_create_from_c_str(
    /// The format string.
    const char * fmt, 

    /// The variable args following the format.
    ... /*varargs*/
);

/// Creates a new string as a copy of the given string 
///
topazString_t * topaz_string_clone(
    /// The string to clone.
    const topazString_t * str
);

/// Creates a new base64-encoded string from a raw byte buffer.
/// This then can be used with topaz_string_decode_base64() to retrieve 
/// a raw byte buffer once more.
topazString_t * topaz_string_base64_from_bytes(
    /// The raw byte buffer to encode into a base64 string.
    const uint8_t * buffer,
    /// The length of the buffer in bytes.
    uint32_t size
);

/// Returns and allocates a new byte buffer 
/// and its size iff the given string is 
/// correctly formatted as base64.
/// If the string is not correctly encoded,
/// NULL is returned and the size set to 0.
uint8_t * topaz_string_base64_to_bytes(
    /// The string encoded into base64.
    const topazString_t * string,
    /// The size of the buffer returned.
    uint32_t * size
);




/// Destroys and frees a topaz string 
///
void topaz_string_destroy(
    /// The string to destroy.
    topazString_t * str
);





/// Returns a temporary string built from the given cstring 
/// It is meant as a convenience function, but it has the following 
/// restrictions:
///     - This must only be used on the main thread. It is not thread-safe
///     - The reference fizzles after subsequent calls to this function. 
///       The string must only be used for quick operations. 
///
/// If your use case does not adhere to these, you should 
/// allocate a new string instead.
const topazString_t * topaz_string_temporary_from_c_str(
    /// The cstring to cast as a topaz string.
    const char * cstr
);


/// A shorter form of topaz_string_temporary_from_c_str().
///
#define TOPAZ_STR_CAST(__s__) topaz_string_temporary_from_c_str(__s__)


/// Sets the contents of the string A to the contents of string B
///
void topaz_string_set(
    /// The string to add to.
    topazString_t * A, 

    /// The string to copy from.
    const topazString_t * B
);

/// Resets the contents of the string.
///
void topaz_string_clear(
    /// The string to clear.
    topazString_t * str
);


/// Adds the given C printf-formatted string and accompanying arguments 
/// to the given string.
///
void topaz_string_concat_printf(
    /// The string to add to.
    topazString_t * str, 

    /// The format of the incoming variable arguments.
    const char * format, 

    /// The variable arguments, indicated by format.
    ... /*varargs*/
);


/// Adds the given string B to the end of the given string A.
///
void topaz_string_concat(
    /// The string to add to.
    topazString_t * str, 

    /// The string to copy from.
    const topazString_t * src
);


/// Returns a read-only copy of a portion of the given string 
/// from and to denote character indices. The substring is valid until 
/// the next call to this function with the same input string.
const topazString_t * topaz_string_get_substr(
    /// The string to read from.
    const topazString_t * str, 

    /// The start index.
    uint32_t from, 

    /// The end index.
    uint32_t to
);



/// Gets a read-only pointer to a c-string representation of the 
/// string under utf8 encoding. The string is valid until a 
/// string operation on str.
///
const char * topaz_string_get_c_str(
    /// The string to query.
    const topazString_t * str
);

/// Gets the number of characters within the string.
///
uint32_t topaz_string_get_length(
    /// The string to query.
    const topazString_t * str
);

/// Gets the character within the string that the given 
/// 0-indexed position. If the position is invalid, 0 is returned.
///
uint32_t topaz_string_get_char(
    /// The string to query.
    const topazString_t *, 

    /// The position index within the string.
    uint32_t position
);

/// Sets the character within the string at the given 
/// 0-indexed position. If an invalid position, no action is taken
///
void topaz_string_set_char(
    /// The string to modify.
    topazString_t *, 

    /// The position within the string to modify
    uint32_t position, 

    /// The new value within the string.
    uint32_t value
);

/// Adds a character to the end of the string
///
void topaz_string_append_char(
    /// The string to append to.
    topazString_t * string, 

    /// The value of the character to add to the string.
    int value
);


/// Gets the byte length of the data representation 
/// of this string. Depending on the context, this could 
/// match the length of the string, or it could be wider.
///
uint32_t topaz_string_get_byte_length(
    /// the string to query.
    const topazString_t * str
);

/// Gets the byte data pointer for this strings. Its length is equal to 
/// topaz_string_get_byte_length()
///
void * topaz_string_get_byte_data(
    /// the string to query.
    const topazString_t * str
);


/// Returns whether substr is found within the given string 
///
int topaz_string_test_contains(
    /// The string to search through.
    const topazString_t * str, 

    /// The string to search for.
    const topazString_t * substr
);

/// Returns wither 2 strings are equivalent 
///
int topaz_string_test_eq(
    /// The string to test equivalence.
    const topazString_t * str, 

    /// The other string to test.
    const topazString_t * other
);

/// Compares the 2 strings in a sort-ready fashion:
/// Returns < 0 if a alphabetically comes before b
/// Returns > 0 if a alphabetically comes after b
/// Returns = 0 if a and b are equivalent
int topaz_string_topaz_compare(
    /// The first string to compare.
    const topazString_t * a, 

    /// The second string to compare.
    const topazString_t * b
);





/// Chain functions can be used to 
/// work on strings in a token-like fashion.
/// Each token is referred to as a "link" in the chain.
/// Resets the chain state of the string. Using topaz_string_chain_current()
/// will return the first token according to the delimiters given.
/// The first link is returned. If no such link exists, an empty 
/// link is returned.
///
const topazString_t * topaz_string_chain_start(
    /// the string to start the chaining.
    topazString_t * t, 
    
    /// The delimiters 
    const topazString_t * delimiters
);

/// Returns the current link in the chain.
/// If the end has been reached, this is an empty string.
///
const topazString_t * topaz_string_chain_current(
    /// The string to query.
    topazString_t * t
);

/// Returns whether the last link in the chain has been reached.
///
int topaz_string_chain_is_end(
    /// The string to query.
    const topazString_t * t
);

/// Goes to the next token in the chain and returns that token.
/// The new token is returned.
/// 
const topazString_t * topaz_string_chain_proceed(
    /// The string to proceed in its chain.
    topazString_t * t
);

/// Releases the memory required to keep a cached C-String.
/// Specifically useful if the string was used to store 
/// data or long information.
void topaz_string_clear_c_str(topazString_t * t);

#endif

