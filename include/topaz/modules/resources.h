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



/*
    Resources
    -----
    Manages assets
*/
typedef struct topazResources_t topazResources_t;



/// Creates a new resources instance. This is normally not necessary to call, as 
/// topaz_t has a default resources instance that it generates for you. 
/// See topaz_get_resources();
///
topazResources_t * topaz_resources_create(topaz_t *);

/// Destroys and frees a resources instance
///
void topaz_resources_destroy(topazResources_t *);




/// Sets the resource path.
/// The resource path is the root from which 
/// assets may be loaded by path.
/// The default path is system dependent, but is usually 
/// the directory of the binary.
/// If successful, returns TRUE.
///
int topaz_resources_set_path(topazResources_t *, const topazString_t *);


/// Convenience function that attempts to load a new asset 
/// from disk directly. This is recommended for small assets that are 
/// effectively instanteous. If a more robust solution is needed,
/// consider topaz_resources_fetch_asset() + topaz_resources_get_decoder
///
/// Like with topaz_resources_fetch_asset, if an asset of the given name 
/// already exists, the preloaded asset is returned. If not, 
/// then a new asset is created whos name will match the path given.
/// Then, the data is attempted to be loaded from disk. The name given is 
/// first checked to see if its a partial path relative to 
/// the resources's resource path (topaz_resources_set_resource_path()). Then 
/// its interpreted as a full path.
/// If the data could not be inerpreted as the given type, or the source 
/// data is unavailable, NULL is returned.
///
topazAsset_t * topaz_resources_load_asset(
    topazResources_t *,
    const topazString_t * fileType,
    const topazString_t * name
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
    topazResources_t *,
    topazAsset_Type,
    const topazString_t * name
);

/// Fetches a decoder for the given filetype. The decoder can be used for 
/// fine control over timing and loading of assets. This 
topazDecoder_t * topaz_resources_get_decoder(
    topazResources_t *,
    const topazString_t * fileType
)


void topaz_resources_remove_asset(
    topazResources_t *,
    const topazString_t * name
);

/// Returns whether a particular extension is supported.
///
int topaz_resources_is_extension_supported(
    const topazResources_t *, 
    const topazString_t *
);


/// Adds a new possible extension to be read 
/// topazAsset_LoadingProfile_t defines the set of functions
/// to interact with a specific data format. This is used to 
/// automatically pick the type of asset that should be 
/// generated for a specific set of data. If the extensions is 
/// already added, its attributes are replaced.
///
void topaz_resources_add_extension(
    topazResources_t *, 
    const topazString_t * ext,
    topazAsset_Type,
    const topazAsset_LoadingProfile_t *,
    const topazAsset_Attributes_t *
);



#endif
