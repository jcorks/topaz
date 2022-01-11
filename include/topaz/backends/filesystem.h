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


#ifndef H_TOPAZDC__FILESYSTEM__INCLUDED
#define H_TOPAZDC__FILESYSTEM__INCLUDED


typedef struct topazRbuffer_t topazRbuffer_t;
typedef struct topazWbuffer_t topazWbuffer_t;


#include <topaz/backends/api/filesystem_api.h>
#include <topaz/system.h>



/// An abstraction for filsystems allows for more flexible management 
/// of runtime assets, including reading assets from remote sources,
/// multithreaded asset loading, ROM devices / files as directory sources,
/// as well as traditional / fully-fledged filesystem access.
/// 
/// 
/// 
/// The filesystem is abstracted as a tree structure of "nodes".
/// Each node contains any number of child objects. There are 2 types of 
/// child objects: nodes, and files, each of which can be represented 
/// as a string.
/// 
/// topaz_filesystem_query can be used to get all child object names from 
/// the currently set path.
///
/// If a child object is a node, its name can be used as an argument to 
/// go_to_child(). If the object is a file, can be used with the read() function 
/// to acquire its data.
///
typedef struct topazFilesystem_t topazFilesystem_t;






/// Creates a new filesystem object. The starting path is 
/// the same upon first creating the filsys instance.
///
topazFilesystem_t * topaz_filesystem_create(
    /// The topaz context.
    topaz_t * context, 


    /// The backend to implement the backend's features.
    topazSystem_Backend_t * backend, 

    /// The raw API to implement the backend's features.
    topazFilesystemAPI_t api
);


/// Destroys and cleans up a filesystem API
///
void topaz_filesystem_destroy(
    /// The filesystem to destroy.
    topazFilesystem_t * filesystem
);





/// Retrieves the backend for this filesystem object.
///
topazSystem_Backend_t * topaz_filesystem_get_backend(
    /// The filesystem to query.
    topazFilesystem_t * filesystem
);


/// Returns the API for this htem.
///
topazFilesystemAPI_t topaz_filesystem_get_api(
    /// The filesystem to query.
    topazFilesystem_t * filesystem
);



typedef struct topazFilesystem_Path_t topazFilesystem_Path_t;


/// The filesystem instance will have built in paths available 
/// for topaz applications to read from/write to. Since filesystem path 
/// objects need to come from preexisting filesystem
typedef enum topazFilesystem_DefaultNode topazFilesystem_DefaultNode;
enum topazFilesystem_DefaultNode {
    /// The filesystem location for where data for the program is preferred.
    topazFilesystem_DefaultNode_Resources,
    
    /// The filesystem location for where the topaz binary is located.
    topazFilesystem_DefaultNode_Topaz,
    
    /// Filesystem location where user data should be located as suggested by the system. 
    topazFilesystem_DefaultNode_UserData,

};


/// Gets a read-only built-in filesystem path.
const topazFilesystem_Path_t * topaz_filesystem_get_path(
    // The filesystem to query.
    topazFilesystem_t * fs,
    
    // The path to retrieve.
    topazFilesystem_DefaultNode def
);


/// Returns a new filesystem path from a string representation of a 
/// path. On systems that support partial / relative paths, they will be 
/// from the "from" parameter path. If no such path on the filesystem 
/// can be found, NULL is returned.
/// If "from" is NULL, it is assumed that the last argument is 
/// a full path on the filesystem. The last argument is a partial 
/// if and only if "from" is non-null.
const topazFilesystem_Path_t * topaz_filesystem_get_path_from_string(
    /// The filesystem to use.
    topazFilesystem_t * fs,
    
    /// The base path. if none, can be NULL
    const topazFilesystem_Path_t * from,
    
    /// The string representation of the path.
    const topazString_t * partialOrFull
);




/// Gets a read-only string representing the path.
/// For most filesystems, this is a "fully qualified" path.
const topazString_t * topaz_filesystem_path_as_string(
    /// The path to get a string form of.
    const topazFilesystem_Path_t * path
);

/// Gets a read-only string representing the name.
/// For most filesystems, this is just the file name.
const topazString_t * topaz_filesystem_path_get_name(
    /// The path to get a string form of.
    const topazFilesystem_Path_t * path
);

/// Gets the parent filesystem path of the given path. 
/// If none, NULL is returned.
const topazFilesystem_Path_t * topaz_filesystem_path_get_parent(
    /// The filesystem path.
    const topazFilesystem_Path_t * path
);

/// Gets an array of children filesystem paths.
/// If none, the array returned is empty. 
const topazArray_t * topaz_filesystem_path_get_children(
    /// The path to query.
    const topazFilesystem_Path_t * path
);

/// Returns a buffer of data represented at the given path.
/// This buffer must be freed by the caller (topaz_rbuffer_destroy).
/// If the filesystem path is invalid or the file cannot be read,
/// NULL is returned.
topazRbuffer_t * topaz_filesystem_path_read(
    /// The filesystem path.
    const topazFilesystem_Path_t * path
);


/// Writes the given buffer to a file on the filesystem.
/// Name should refer to the name of the file to write.
int topaz_filesystem_path_write(
    /// The filesystem path.
    const topazFilesystem_Path_t * path,
    /// The name of the file.
    const topazString_t * name,
    /// The buffer.
    const topazWbuffer_t * data
);


/// Normally, all created filesystem paths are kept 
/// until the filesystem instance is destroyed.
/// This function will free all filesystem paths in use. 
/// This is usually not necessary, but might be needed 
/// for more complex uses of the filesystem instance.
void topaz_filesystem_expunge(
    /// The filesystem to modify.
    topazFilesystem_t * fs
);


#endif
