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


#ifndef H_TOPAZDC__FILESYS__INCLUDED
#define H_TOPAZDC__FILESYS__INCLUDED

#include <topaz/backends/api/filesys_api.h>
#include <topaz/backends/backend.h>



/*

    Filesys
    -----
    An abstraction for filsystems allows for more flexible management 
    of runtime assets, including reading assets from remote sources,
    multithreaded asset loading, ROM devices / files as directory sources,
    as well as traditional / fully-fledged filesystem access.

*/

typedef struct topazFilesys_t topazFilesys_t;






/// Creates a new filesys object
///
topazFilesys_t * topaz_filesys_create(topazBackend_t *, topazFilesysAPI_t);


/// Destroys and cleans up a filesys API
///
void topaz_filesys_destroy(topazFilesys_t *);





/// Retrieves the backend for this filesys object.
///
topazBackend_t * topaz_filesys_get_backend(topazFilesys_t *);


/// Returns the API for this filesys.
///
topazFilesysAPI_t topaz_filesys_get_api(topazFilesys_t *);






/// Change to the specified directory.
/// Returns TRUE on success.
int topaz_filesys_change_dir(topazFilesys_t *, const topazString_t *);

/// Change to the specified child directory.
/// NOTICE: you only need to specify the name of the directory
/// Ex) if you query the directory and it returns true, then
/// this function will succeed as well.
///	
int topaz_filesys_go_to_child(topazFilesys_t *, const topazString_t *);


/// Change directory to the parent directory.
///
int topaz_filesys_go_to_parent(topazFilesys_t *);

/// return the current working directory.
///
const topazString_t * topaz_filesys_get_cwd(topazFilesys_t *);

/// Creates a directory with the given name.
/// The directory is relative to the current working directory.
///
int topaz_filesys_create_dir(topazFilesys_t *, const topazString_t *);

/// Reads a file within the directory
///
topazRbuffer_t * topaz_filesys_read(topazFilesys_t *, const topazString_t *);


/// Writes the given file 
///
void topaz_filesys_write(topazFilesys_t *, const topazString_t *, const topazWbuffer_t *);


/// return all the object names in the current working directory as 
/// an array of topazString_t
///  	
const topazArray_t * topaz_filesys_query_directory(topazFilesys_t *);


/// returns whether the given name is a file or note.
///
int topaz_filesys_is_file(topazFilesys_t *, const topazString_t *);

/// returns whether the given name is a child of this directory 
///
int topaz_filesys_child_exists(topazFilesys_t *, const topazString_t *);

#endif
