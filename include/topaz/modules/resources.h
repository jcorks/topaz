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


#ifndef H_TOPAZDC__RESOURCES__INCLUDED
#define H_TOPAZDC__RESOURCES__INCLUDED


#include <topaz/asset.h>
typedef struct topaz_t topaz_t;
typedef struct topazIOX_t topazIOX_t;


/// Manages assets and acts as a repository for them.
///
/// Many projects require thats some external data be 
/// loaded in during the running of the program. The Resources 
/// module provides a means to accomplish this.
/// Using standardized pathing and built-in IO translators,
/// this module will translate data for you.
///
/// Often in projects, the same asset will be used in multiple parts within 
/// the program. This module will resuse assets already loaded to provide a quick,
/// transparent way to quickly access assets.
typedef struct topazResources_t topazResources_t;



/// Creates a new resources instance. This is normally not necessary to call, as 
/// topaz_t has a default resources instance that it generates for you. 
/// See <a href="./topaz_t.html#topaz_context_get_resources">topaz_context_get_resources()</a>
///
topazResources_t * topaz_resources_create(
    /// The topaz context.
    topaz_t * context
);

/// Destroys and frees a resources instance
///
void topaz_resources_destroy(
    /// The resource instance to destroy.
    topazResources_t * res
);




/// Sets the resource path.
/// The resource path is the root from which 
/// assets may be loaded by path.
/// The default path is system dependent, but is usually 
/// the directory of the binary.
/// If successful, returns TRUE.
///
int topaz_resources_set_path(
    /// The resource instance to modify.
    topazResources_t * res, 

    /// The system path. The path itself is system dependent based on the 
    /// associated filesystem.
    const topazString_t * path
);

/// Returns the current resource path.
///
const topazString_t * topaz_resources_get_path(
    /// The resource instance to query.
    const topazResources_t * res
);

/// Queries the filesystem for all assets within the asset directory.
///
void topaz_resources_query_asset_paths(
    /// The resource instance to query.
    topazResources_t * res
);

/// Returns a read-only list of filesystem paths of resources within the current path
/// 
const topazArray_t * topaz_resources_get_asset_paths(topazResources_t *);



/// Convenience function that attempts to load a new asset 
/// from the filesystem directly. This is recommended for small assets that are 
/// effectively instanteous. If a more robust solution is needed,
/// consider topaz_resources_fetch_asset() + topaz_resources_get_translator
///
/// "name" is used as the name of the asset. For functions that refer to 
/// asset names to uniquely identify an asset such as topaz_resources_fetch_asset().
///
/// "path" is used as the filesystem path to read the asset from. This assumes the 
/// current path is topaz_resources_get_path() and is guaranteed to behave as if 
/// from topaz_filesys_read(). 
///
/// "fileType" is the extension of the file used to determine with IO Translator 
/// to use to interpret data.
///
/// Like with topaz_resources_fetch_asset, if an asset of the given name 
/// already exists, the preloaded asset is returned. If not, 
/// then a new asset is created whos name will match the path given.
/// Then, the data is attempted to be loaded from disk. The name given is 
/// first checked to see if its a partial path relative to 
/// the resources's resource path (topaz_resources_set_path()). Then 
/// its interpreted as a full path.
/// If the data could not be inerpreted as the given type, or the source 
/// data is unavailable, NULL is returned.
///
topazAsset_t * topaz_resources_load_asset(
    /// The resource instance to load with.
    topazResources_t * res,
    /// The extension / filetype to attempt to read with.
    const topazString_t * fileType,
    /// The path to the asset.
    const topazString_t * path,
    /// The unique name to give to the asset.
    const topazString_t * name
);

/// Equivalent to topaz_resources_load_asset, except instead of reading 
/// from the filsystem, raw byte data is read instead.
///
topazAsset_t * topaz_resources_load_asset_data(
    /// The resource instance to load with.
    topazResources_t * res,
    /// The extension / filetype to attempt to read with.
    const topazString_t * fileType,
    /// The raw data for the asset. Read as if contiguous bytes.
    const topazArray_t * data,
    /// The unique name to give to the asset.
    const topazString_t * name
);



/// Writes an asset to the filesystem at the outputpath 
/// given relative to the resources path.
///
int topaz_resources_write_asset(
    /// The resource instance to write with.
    topazResources_t * res,

    /// The asset to write.
    topazAsset_t * asset,

    /// The type to write the asset as.
    const topazString_t * fileType,

    /// The output path to write to. This is system-dependent based on the filesystem backend.
    const topazString_t * outputPath
);


/// Either creates a new asset if the given name doesn't exist, or 
/// returns an existing asset.
/// In the case that a new asset needs to be created, 
/// if the fileType is not able to be interpreted, then 
/// NULL is returned. This is only checked if the name doesn't exist.
/// In the case that a new asset is created, the returned asset 
/// can then be used directly to load data. See asset.h
/// 
topazAsset_t * topaz_resources_fetch_asset(
    /// The resource instance to fetch with/
    topazResources_t * res,
    /// The asset type.
    topazAsset_Type type,
    /// The unique name of the asset.
    const topazString_t * name
);

/// Fetches a translator for the given filetype. The translator can be used for 
/// fine control over timing and loading of assets. 
/// See iox.h
///
topazIOX_t * topaz_resources_get_translator(
    /// The resource instance to query.
    topazResources_t * res,

    /// The type of the file to request.
    const topazString_t * fileExtension
);


void topaz_resources_remove_asset(
    /// The resource instance to remove from.
    topazResources_t * res,

    /// The name of the asset to search and remove.
    const topazString_t * name
);

/// Returns whether a particular extension is supported.
///
int topaz_resources_is_extension_supported(
    /// The resource instance to query.
    const topazResources_t * res, 

    /// The name of the file extension / type to request.
    const topazString_t * name
);


/// Adds a new possible extension to be read.
/// The ioxBackendName is the name of the backend registered with 
/// the topaz system instance (topaz_context_get_system()). To add a 
/// new extension, a new backend handler will have to be made. See system.h.
///
void topaz_resources_add_translator(
    /// The resource instance to modify
    topazResources_t * res, 

    /// The IO translater registered with the topaz context system.
    const topazString_t * ioxBackendName    
);



#endif
