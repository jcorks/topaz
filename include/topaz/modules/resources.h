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





/// Creates a new asset. 
/// If the name already exists, NULL is returned.
///
/// If name is NULL, a unique name is auto-generated
topazAsset_t * topaz_resources_create_asset(
    topazResources_t * res,
    /// The unique name of the asset.
    const topazString_t * name,

    /// The type of the asset to create.
    topazAsset_Type type
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
/// Data asset, meaning it is just a collection of 
/// bytes. The asset can then be re-interpreted
/// as another kind of asset using topaz_resources_convert_asset()
///
/// If an asset with the name already exists or the 
/// filesystem cannot load the item specified, NULL is 
/// returned, and no other change is made.
topazAsset_t * topaz_resources_create_data_asset_from_path(
    topazResources_t * res,
    /// The path to the asset.
    const topazString_t * path,
    /// The unique name to give to the asset.
    const topazString_t * name
);




/// Converts an existing data asset
/// "fileType" is the extension of the file used to determine which IO Translator 
/// to use to interpret data.
///
/// Using
///
topazAsset_t * topaz_resources_convert_asset(
    /// The resource instance to convert with.
    topazResources_t * res,
    /// The extension / filetype to attempt to convert as
    const topazString_t * fileType,
    // the asset to convert
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

    /// The asset to remove.
    topazAsset_t * asset
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







// Bundles.
//
// Bundles are special assets. Bundles have no public API 
// and can only be passed around as topazAsset_t *. They 
// can be used to store and pass around collections of 
// assets together, including other bundles.
//
// To pack a bundle:
// - Create data assets for each asset you want to pack. 
//   do not convert them! Keep them as data assets.
//
// - Call topaz_resources_pack_bundle(), which returns an 
//   data asset representing a bundle. Save this for later storage.
//
//
// 
// To unpack a bundle: 
// - Create a data asset using the bytes from asset created 
//   by topaz_resources_pack_bundle() at some point.
//
// - Call topaz_resources_convert_asset() to convert it to 
//   a bundle (using the extension "bundle")
//
// - Call topaz_resources_unpack_bundle(), which will 
//   create and convert assets for you, periodicalling calling 
//   a user callback (onNewItem) to notify usercode of 
//   each unpacked item. Note that the bundle name is 
//   NOT the asset name, but the bundle name. These are 
//   distinctly different, as bundle names are decided by 
//   the bundle itself.
//
// NOTE: bundles can also specify dependencies, meaning that 
// bundles can require that certain bundles exist within 
// the topaz resource's instance. Dependencies specifically 
// require bundle assets themselves to be loaded; this means 
// not only a data asset representing the bundle, but also 
// the data converted (using extension "bundle") into a 
// real bundle.
//
// The reason for this is that the dependencies are specified 
// using bundle names rather than asset names. This means that,
// to even find which bundle dependencies are loaded, the 
// dependencies must be actual bundles, not just loose data.
#define TOPAZ_RESOURCES_BUNDLE_VERSION_ANY -1


// Creates a new data asset containing the byte state 
// of a resource bundle. Bundles contain raw byte states 
// of other assets in their data forms so that, when unpacking 
// these data states are available for immediate use under a 
// "namespace".
//
// 
topazAsset_t * topaz_resources_pack_bundle(
    topazResources_t * res,

    // Name of the output asset that contains the bundle 
    // state.
    const topazString_t * assetName,

    // The name of the bundle. This uniquely identifies the 
    // bundle across multiple instances and is separate from an 
    // asset name. This is the name used to identify as a dependency
    // and also for unpacking.
    const topazString_t * bundleName,

    // Major version of the bundle.
    int versionMajor,

    // Minor version of the bundle.
    int versionMinor,

    // Micro version of the bundle.
    int versionMicro,


    // Text description of the bundle.
    const topazString_t * description,

    // Text data to identify author(s) of the bundle. 
    const topazString_t * author,

    // number of dependencies that this package 
    // requires when loading. If these bundles are not 
    // loaded when unpacking, the unpacking process
    // will not be successful.
    uint32_t dependsCount,

    // The names of the dependencies.
    const topazString_t ** dependsName,

    // The required minor versions of the dependencies.
    const int * dependsMinor, // MAJOR.MINOR

    // The required major versions of the dependenices.
    const int * dependsMajor, // MAJOR.MINOR

    const topazString_t ** assetNames,
    const topazString_t ** assetExtensions
    uint32_t assetCount,
);


// Unpacks all resources within a bundle and creates+converts 
// all assets within them. If the bundle has dependencies, those are 
// unpacked first. If an item is unable to read / converted either within 
// the main package or a dependency OR if a dependency is missing OR 
// if a dependency does not meet criteria, FALSE is returned.
//
// Because resources can span many different assets, a user function may 
// be run on every new asset that is loaded. This can help give 
// user feedback for large assets / bundles.
int topaz_resources_unpack_bundle(
    topazResources_t * res,
    const topazString_t * bundleName, 
    int min_minorVersionRequired,
    int min_majorVersionRequired,
    void * userdata,
    void (*onNewItem)(topazResources_t * res, void * userData)
);


#endif
