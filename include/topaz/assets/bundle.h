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


#ifndef H_TOPAZDC__BUNDLE__INCLUDED
#define H_TOPAZDC__BUNDLE__INCLUDED

#include <topaz/asset.h>
typedef struct topazArray_t topazArray_t;

/// Asset that holds a collection of other 
typedef struct topazBundle_t topazBundle_t;




/// Creates a new data asset object
/// Use asset_destroy to release.
///
topazAsset_t * topaz_bundle_create_empty(
    topaz_t *, 
    const topazString_t * bundleName,
    int versionMajor,
    int versionMinor,
    int versionMicro,
    const topazString_t * description,
    const topazString_t * author,
    const topazString_t ** dependsName,
    const topazString_t ** dependsVersion, // MAJOR.MINOR
    uint32_t dependsCount
    
);

topazAsset_t * topaz_bundle_create_from_data(
    topaz_t *, 
    const uint8_t * data,
    uint32_t size
);


void topaz_bundle_get_version(
    topazAsset_t * bundle,
    int * major,
    int * minor,
    int * micro
);

const topazString_t * topaz_bundle_get_description(
    topazAsset_t * bundle
);

const topazString_t * topaz_bundle_get_author(
    topazAsset_t * bundle
);


const topazArray_t * topaz_bundle_get_depends_names(
    topazAsset_t * bundle
);

const topazArray_t * topaz_bundle_get_depends_version_strings(
    topazAsset_t * bundle
);




/// Adds a new asset to the bundle.
///
void topaz_bundle_add_item(
    /// The bundle to modify
    topazAsset_t * bundle,
    
    /// The name of the of the asset within the bundle.
    const topazString_t * name,
    
    /// The extension to use to decode the data when unpacking.
    const topazString_t * extension,
    
    ///
    const topazAsset_t * dataAsset
);


/// Removes all the bundle's items.
void topaz_bundle_clear(
    topazAsset_t * bundle
);



/// Retrieves the raw byte state of the bundle, representing the 
/// state of all of its assets.
const uint8_t * topaz_bundle_get_packed_data(
    topazAsset_t * bundle,
    uint32_t * size
);



/// Unpacks all the items in the bundle. For 
/// each item, a new asset will be created within 
/// resources. Each asset will be prefixed with 
/// this bundle asset's name followed by "."
/// and the name of the sub-asset.
/// For example, a bundle with the name 
/// "Package" that contains an asset named 
/// "Item" could be fetched under the name "Package.Item"
/// Any assets that are, themselves, bundled will also be 
/// unpacked.
int topaz_bundle_unpack_all(
    topazAsset_t * bundle
);

/// Unpacks and loads the next nItems assets within 
/// the bundle. If nItems is greater than the number of 
/// unpacked assets, the remaining assets are unpacked.
/// Returns 1 if more items are in the bundle and 0 otherwise.
int topaz_bundle_unpack_continue(
    topazAsset_t * bundle,
    int nItems
);



#endif
