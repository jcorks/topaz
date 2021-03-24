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



/// An abstraction for filsystems allows for more flexible management 
/// of runtime assets, including reading assets from remote sources,
/// multithreaded asset loading, ROM devices / files as directory sources,
/// as well as traditional / fully-fledged filesystem access.
/// 
/// 
/// 
/// The Filesystem is abstracted as a tree structure of "nodes".
/// Each node contains any number of child objects. There are 2 types of 
/// child objects: nodes, and files, each of which can be represented 
/// as a string.
/// 
/// topaz_filesys_query can be used to get all child object names from 
/// the currently set path.
///
/// If a child object is a node, its name can be used as an argument to 
/// go_to_child(). If the object is a file, can be used with the read() function 
/// to acquire its data.
///
typedef struct topazFilesys_t topazFilesys_t;






/// Creates a new filesys object. The starting path is 
/// the same upon first creating the filsys instance.
///
topazFilesys_t * topaz_filesys_create(
    /// The topaz context.
    topaz_t * context, 


    /// The backend to implement the backend's features.
    topazSystem_Backend_t * backend, 

    /// The raw API to implement the backend's features.
    topazFilesysAPI_t api
);


/// Destroys and cleans up a filesys API
///
void topaz_filesys_destroy(
    /// The filesys to destroy.
    topazFilesys_t * filesystem
);





/// Retrieves the backend for this filesys object.
///
topazSystem_Backend_t * topaz_filesys_get_backend(
    /// The filesystem to query.
    topazFilesys_t * filesystem
);


/// Returns the API for this filesys.
///
topazFilesysAPI_t topaz_filesys_get_api(
    /// The filesystem to query.
    topazFilesys_t * filesystem
);






/// Returns all the object names in the current path as 
/// an array of topazString_t *. 
///  	
const topazArray_t * topaz_filesys_query(
    /// The filesystem to query.
    topazFilesys_t * filesystem
);






/// Change to the specified path. A path is a system-defined string 
/// representation of a node location, so this will not be consistent 
/// across different systems. On most backends, this is the 
/// fully qualified path.
///
/// Returns TRUE on success.
///
int topaz_filesys_set_path(
    /// The filesystem to modify.
    topazFilesys_t * filesystem, 

    /// The new path.
    const topazString_t * newPath
);


/// Returns a read-only string containing the full path of a child item from 
/// the current directory. This is a convenient way to get the path of an item 
/// without worrying about appending implementation-specific pathing notation.
/// If an error occurs, or the item given isn't a child, an empty string is returned.
///
const topazString_t * topaz_filesys_get_child_path(
    /// The filesystem to query.
    topazFilesys_t * filesystem, 

    /// The child to query.
    const topazString_t * child
);

/// Returns the current path
///
const topazString_t * topaz_filesys_get_path(
    /// The filesystem to query.
    topazFilesys_t * filesystem
);

/// Given a path, it returns the node and file components of the path separated
/// This takes into account filesystems-specific path separators. 
/// The array (and topazString_t *) references are valid until next calling 
/// of topaz_filesys_split_path.
///
const topazArray_t * topaz_filesys_split_path(
    /// The filesystem to use.
    topazFilesys_t * filesystem,

    /// The path to split. 
    const topazString_t * path
);


/// Given an array of tokens, returns a single read-only 
/// path consistent of those tokens, valid until the next calling of this 
/// function.
const topazString_t * topaz_filesys_join_path(
    /// The filesystem to use.
    topazFilesys_t * filesystem,

    /// An array of topazString_t *. 
    const topazArray_t * pathTokens
);

/// Change to the specified child node.
/// The nodeName must match the name of a child node. On some systems 
/// partial paths may be supported, but these are non-standard.
///	
int topaz_filesys_go_to_child(
    /// The filesystem to modify.
    topazFilesys_t * filesystem,

    /// The child to change to. 
    const topazString_t * nodeName
);


/// Change directory to the parent node.
///
int topaz_filesys_go_to_parent(
    /// The filesystem to modify.
    topazFilesys_t * filesystem
);




/// Creates a node with the given name within the current path.
///
int topaz_filesys_create_node(
    /// The filesystem to create a node on.
    topazFilesys_t * filesystem, 

    /// The name of the node to make.
    const topazString_t * nodeName
);




/// Reads a file within the directory and returns a buffer 
/// containing that file's data. If an error occurs, the buffer 
/// is empty. The retrned buffer must be freed with topaz_rbuffer_destroy
///
topazRbuffer_t * topaz_filesys_read(
    /// The filesystem to read from.
    topazFilesys_t * filesystem, 

    /// The item to read.
    const topazString_t * child
);

/// Writes the given file 
///
int topaz_filesys_write(
    /// The filesystem to write to.
    topazFilesys_t * filesystem, 

    /// The name of the item to write/create.
    const topazString_t * child, 

    /// The data to write.
    const topazWbuffer_t * bufferData
);




/// returns whether the given name is a file or node.
///
int topaz_filesys_is_node(
    /// The filesystem to query.
    topazFilesys_t * filesystem,

    /// The child node to query. 
    const topazString_t * child
);

/// returns whether the given name is a child of this directory 
///
int topaz_filesys_is_child(
    /// The filesystem to query.
    topazFilesys_t * filesystem,

    /// The child node to query. 
    const topazString_t * child
);

#endif
