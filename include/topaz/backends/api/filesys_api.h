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


#ifndef H_TOPAZDC__FILESYS_API__INCLUDED
#define H_TOPAZDC__FILESYS_API__INCLUDED

#include <topaz/containers/string.h>
#include <topaz/rbuffer.h>
#include <topaz/wbuffer.h>

typedef struct topazFilesys_t topazFilesys_t;

/*

    FilesysAPI
    -----
    
    The set of functions that define how the filesystem abstraction should 
    behave. Creating filsystem abstractions requires this API to be 
    populated.

    These API functions are called as underlying implementations for the symbols 
    within <topaz/backends/filesys.h> and provide a way for custom, possibly 
    system-dependent behavior to account for an environment in a robust way.

*/
typedef struct topazFilesysAPI_t topazFilesysAPI_t;


/// Each function is an implementation-facing copy of 
/// the user-side API for topazFilesys_t. See <topaz/backends/filesys.h>
///
struct topazFilesysAPI_t {
    void                    (*filesys_create)           (topazFilesysAPI_t *);
    void                    (*filesys_destroy)          (topazFilesysAPI_t *);


    const topazArray_t *    (*filesys_query)            (topazFilesysAPI_t *);
    int                     (*filesys_set_path)         (topazFilesysAPI_t *, const topazString_t *);
    const topazString_t *   (*filesys_get_path)         (topazFilesysAPI_t *);
    const topazArray_t *    (*filesys_split_path)       (topazFilesysAPI_t *, const topazString_t *);


    int                     (*filesys_go_to_child)      (topazFilesysAPI_t *, const topazString_t *);
    int                     (*filesys_go_to_parent)     (topazFilesysAPI_t *);
    int                     (*filesys_create_node)      (topazFilesysAPI_t *, const topazString_t *);
    topazRbuffer_t *        (*filesys_read)             (topazFilesysAPI_t *, const topazString_t *);
    int                     (*filesys_write)            (topazFilesysAPI_t *, const topazString_t *, const topazWbuffer_t *);

    int                     (*filesys_is_node)          (topazFilesysAPI_t *, const topazString_t * t);
    int                     (*filesys_is_child)         (topazFilesysAPI_t *, const topazString_t * t);



    /// User-given data. This is expected to data needed to persist
    /// throughout the lifetime of the filesys
    ///
    void * implementationData;


};



#endif
