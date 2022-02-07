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
///
/// The general flow is this:
/// - Using raw data (loaded from the filesystem, a different source, etc) make 
///   a new asset. This is a pure "data" asset 
/// - Translate the asset into something else, such as an image, or audio
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





/// Creates a new asset. Assets
/// are, by default, Data assets, which are just 
/// byte blocks. To convert to something else, use 
/// topaz_resource_load_asset.
/// If the name already exists, NULL is returned.
///
/// If name is NULL, a unique name is auto-generated
topazAsset_t * topaz_resources_create_asset(
    topazResources_t * res,
    /// The unique name of the asset.
    const topazString_t * name
);

/// Returns the existing asset referred to by the given name.
/// If there is none, NULL is returned.
topazAsset_t * topaz_resources_fetch_asset(
    /// The resource instance to fetch with/
    topazResources_t * res,
    /// The unique name of the asset.
    const topazString_t * name
);



/// Creates an asset, but looks in the 
/// filesystem for an item at the given path.
/// If found, it reads the bytes and stores them 
/// within a new asset.
///
/// After this process, the asset will be loaded as a 
/// data asset, meaning it is just a collection of 
/// bytes. The asset can then be re-interpreted
/// as another kind of asset using topaz_resources_load_asset()
///
/// If an asset with the name already exists or the 
/// filesystem cannot load the item specified, NULL is 
/// returned, and no other change is made.
topazAsset_t * topaz_resources_create_asset_from_path(
    topazResources_t * res,
    /// The path to the asset.
    const topazString_t * path,
    /// The unique name to give to the asset.
    const topazString_t * name
);

/// Equivalent to topaz_resources_create_asset_from_path, 
/// except instead of reading from the filsystem, 
/// raw byte data is read instead.
///
topazAsset_t * topaz_resources_create_asset_from_bytes(
    /// The resource instance to load with.
    topazResources_t * res,
    /// The raw data for the asset. Read as if contiguous bytes.
    const topazArray_t * data,
    /// The unique name to give to the asset.
    const topazString_t * name
);

/// Equivalent to topaz_resources_create_asset_from_path, except instead of reading
/// from the filesystem, raw byte data is generated from an encoded
/// base64 string.
///
topazAsset_t * topaz_resources_create_asset_from_base64(
    /// The resource instance to load with.
    topazResources_t * res,
    /// The raw data for the asset. Read as a base64 string.
    const topazString_t * data,
    /// The unique name to give to the asset.
    const topazString_t * name
);




/// Converts an existing data asset
/// "fileType" is the extension of the file used to determine which IO Translator 
/// to use to interpret data.
///
/// Using
///
topazAsset_t * topaz_resources_load_asset(
    /// The resource instance to load with.
    topazResources_t * res,
    /// The extension / filetype to attempt load as
    const topazString_t * fileType,
    // the asset to load
    topazAsset_t *
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

/// Removes the asset by name.
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
