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


#ifndef H_TOPAZDC__ARRAY__INCLUDED
#define H_TOPAZDC__ARRAY__INCLUDED


#include <stdint.h>





/// Dynamically resizing container
///
/// Standard containers in many languages, this will hold shallow data
/// and resize itself as need be. 
///
typedef struct topazArray_t topazArray_t;



/// Returns a new, empty array 
///
/// sizeofType refers to the size of the elements that the array will hold 
/// the most convenient way to do this is to use "sizeof()"
topazArray_t * topaz_array_create(
    /// Size of the object that the array should hold.
    uint32_t sizeofType
);

/// Destroys the container and buffer that it manages.
///
void topaz_array_destroy(
    /// The array to destroy.
    topazArray_t * array
);

/// Creates a temporary array whos data is managed for you.
///
const topazArray_t * topaz_array_temporary_from_static_array(
    /// The data to populate the temporary array for you.
    const void * array, 

    /// Size of the object within the input array.
    uint32_t sizeofType, 

    /// The object count that the array holds.
    uint32_t length
);

/// Convenience call for topaz_array_temperator_from_static_array() 
/// The first argument is the array pointer
/// The second argument is the type of each member within the array 
/// The third argument is the number of members in the array
#define TOPAZ_ARRAY_CAST(__D__,__T__,__L__) (topaz_array_temporary_from_static_array(__D__, sizeof(__T__), __L__))

/// Returns an empty, read-only array. 
///
const topazArray_t * topaz_array_empty();

/// Clones an entire array, returning a new array instance.
///
topazArray_t * topaz_array_clone(
    /// The source array to clone.
    const topazArray_t * array
);

/// Returns the size of the array
///
uint32_t topaz_array_get_size(
    /// The array to query.
    const topazArray_t * array
);

/// Returns the size of each element in bytes.
///
uint32_t topaz_array_get_type_size(
    /// The array to query.
    const topazArray_t * array
);

/// Adds an additional element to the array.
///
/// The value given should be an addressable value. For example a 
/// value "12" will not be able to be pushed, but if 
/// setting a variable ahead of time and adding 
/// "i = 12; topaz_array_push(array, i);" would work.
/// Example:
///
///     int i = 0;
///     topazArray_t * arr = topaz_array_create(sizeof(int));
///     topaz_array_push(arr, i);
///
#define topaz_array_push(__A__,__VAL__) (topaz_array_push_n(__A__, &__VAL__, 1))

/// Gets the value at the given index.
/// Example:
///     
///     int i = 42;
///     topazArray_t * arr = topaz_array_create(sizeof(int));
///     topaz_array_push(arr, i);
///     printf("%d", topaz_array_at(arr, 0));
///
/// The above example would print 42.
#define topaz_array_at(__A__,__T__,__I__) (((__T__*)topaz_array_get_data(__A__))[__I__])

/// Adds a contiguous set of elements to the array 
///
void topaz_array_push_n(
    /// The array to modify.
    topazArray_t * array, 

    /// An input array of data. The element array is assumed to be 
    /// aligned to the size of the topaz array's object size.
    const void * element, 

    /// The number of items within the element array.
    uint32_t count
);


/// Removes a specific member of the array
///
void topaz_array_remove(
    /// The array to modify.
    topazArray_t * array, 

    /// The index to remove.
    uint32_t index
);



/// Compares 2 elements within an array.
/// Each points to an element within an array; this is used 
/// for topaz_array_lower_bound(). Its expected that 
/// the return value returns whether A is "less" than B.
typedef int (*topaz_array_comparator) (
    /// Pointer to array element A.
    const void * a, 

    /// Pointer to array element B.
    const void * b
);

/// Returns the index that this element should be inserted into 
/// given that the rest of the array is sorted. If the element is 
/// greater than the rest of the array's elements, the index 
/// after the array is returned.
///
uint32_t topaz_array_lower_bound(
    /// The array to query.
    const topazArray_t * array, 

    /// The element in question
    ///    
    const void * element, 

    /// Returns whether the value that a points to is "less" than 
    /// the value that b points to.
    ///
    topaz_array_comparator less
);

/// Inserts the given value at the given position. If the position 
/// is above or equal to the size of the array, the new element is 
/// placed at the end.
///
#define topaz_array_insert(__A__,__I__,__V__) (topaz_array_insert_n(__A__, __I__, &(__V__), 1))



/// Inserts the given number of elements at the given index.
///
void topaz_array_insert_n(
    /// The array to insert data into.
    topazArray_t * array, 

    /// The index to insert at.
    uint32_t index, 

    /// The source data to insert into the array.
    void * element, 

    /// The number of elements to insert.
    uint32_t count
);


/// Gets a pointer to the raw data of the array
/// This pointer is guaranteed to be a contiguous memory block of the 
/// current state of the array. It is editable.
///
void * topaz_array_get_data(
    /// The array to query.
    const topazArray_t * array
);


/// Clears the contents of the array.
/// After this operation, the size of the array
/// is 0.
///
void topaz_array_clear(
    /// The array to modify.
    topazArray_t * array
);


/// Sets the size of the array
/// If the array were to be larger than it could account for,
/// this invokes an internal resize
///
void topaz_array_set_size(
    /// The array to modify.
    topazArray_t * array,

    /// The new size. 
    uint32_t size
);

/// Sets the size and allocation size of the array.
/// For large arrays with a static size, this can 
/// save on memory usage.
void topaz_array_set_size_exact(
    /// The array to modify
    topazArray_t * t, 
    /// The new size, and allocation, of the array
    uint32_t size
);




#endif
