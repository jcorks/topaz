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
#include <topaz/system.h>



/*

    Filesys
    -----
    An abstraction for filsystems allows for more flexible management 
    of runtime assets, including reading assets from remote sources,
    multithreaded asset loading, ROM devices / files as directory sources,
    as well as traditional / fully-fledged filesystem access.



    The Filesystem is abstracted as a tree structure of "nodes".
    Each node contains any number of child objects. There are 2 types of 
    child objects: nodes, and files, each of which can be represented 
    as a string.

    topaz_filesys_query can be used to get all child object names from 
    the currently set path.

    If a child object is a node, its name can be used as an argument to 
    go_to_child(). If the object is a file, can be used with the read() function 
    to acquire its data.

*/

typedef struct topazFilesys_t topazFilesys_t;






/// Creates a new filesys object. The starting path is 
/// the same upon first creating the filsys instance.
///
topazFilesys_t * topaz_filesys_create(topazSystem_Backend_t *, topazFilesysAPI_t);


/// Destroys and cleans up a filesys API
///
void topaz_filesys_destroy(topazFilesys_t *);





/// Retrieves the backend for this filesys object.
///
topazSystem_Backend_t * topaz_filesys_get_backend(topazFilesys_t *);


/// Returns the API for this filesys.
///
topazFilesysAPI_t topaz_filesys_get_api(topazFilesys_t *);






/// Returns all the object names in the current path as 
/// an array of topazString_t *. 
///  	
const topazArray_t * topaz_filesys_query(topazFilesys_t *);






/// Change to the specified path. A path is a system-defined string 
/// representation of a node location, so this will not be consistent 
/// across different systems. On most backends, this is the 
/// fully qualified path.
///
/// Returns TRUE on success.
///
int topaz_filesys_set_path(topazFilesys_t *, const topazString_t *);

/// Returns the current path
///
const topazString_t * topaz_filesys_get_path(topazFilesys_t *);

/// Given a path, it returns the node and file components of the path separated
/// This takes into account filesystems-specific path separators. 
/// The array (and topazString_t *) references are valid until next calling 
/// of topaz_filesys_split_path.
///
const topazArray_t * topaz_filesys_split_path(topazFilesys_t *, const topazString_t * path);


/// Change to the specified child node.
/// The nodeName must match the name of a child node. On some systems 
/// partial paths may be supported, but these are non-standard.
///	
int topaz_filesys_go_to_child(topazFilesys_t *, const topazString_t * nodeName);


/// Change directory to the parent node.
///
int topaz_filesys_go_to_parent(topazFilesys_t *);




/// Creates a node with the given name within the current path.
///
int topaz_filesys_create_node(topazFilesys_t *, const topazString_t *);




/// Reads a file within the directory and returns a buffer 
/// containing that file's data. If an error occurs, the buffer 
/// is empty. The retrned buffer must be freed with topaz_rbuffer_destroy
///
topazRbuffer_t * topaz_filesys_read(topazFilesys_t *, const topazString_t *);

/// Writes the given file 
///
int topaz_filesys_write(topazFilesys_t *, const topazString_t *, const topazWbuffer_t *);




/// returns whether the given name is a file or node.
///
int topaz_filesys_is_node(topazFilesys_t *, const topazString_t *);

/// returns whether the given name is a child of this directory 
///
int topaz_filesys_is_child(topazFilesys_t *, const topazString_t *);

#endif
