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


#ifndef H_TOPAZDC__ASSET__INCLUDED
#define H_TOPAZDC__ASSET__INCLUDED

#include <topaz/containers/string.h>
typedef struct topaz_t topaz_t;


///
///
///    An asset represents a useful abstraction for an object 
///    that was populated from raw data, usually from a file or 
///    from memory.
///
///    On it's own, it does not do much. But, it serves as a
///    base class for more useful applications
///
///
typedef struct topazAsset_t topazAsset_t;




/// Type of asset.
///
/// Every asset has a type that gives a hint to what child class
/// it is. 
///
typedef enum topazAsset_Type topazAsset_Type; 
enum topazAsset_Type {
    /// The asset doesnt have a type. This is used for 
    /// error cases.
    ///
    topazAsset_Type_None,

    /// The asset is an image. See image.h
    ///
    topazAsset_Type_Image,

    /// The asset is an audio waveform.
    ///
    topazAsset_Type_Sound,

    /// The asset defines how a 3D mesh should be expressed.
    ///
    topazAsset_Type_Material,

    /// The asset is raw bytes / text.
    /// When assets are first loaded from an external source, 
    /// they will be of this type. They can then by changed into 
    /// other asset types using topaz_resources_asset_load()
    ///
    topazAsset_Type_Data,

    /// The asset is a 3D mesh
    ///
    topazAsset_Type_Mesh,
    
    /// The asset is a bundle of other assets.
    ///
    topazAsset_Type_Bundle,
    
    topazAsset_Type_Count,    
};






/// Callback for assets.
///
typedef void (*topaz_asset_callback)(
    /// Asset in question
    topazAsset_t * asset,

    /// generic data associated with the asset 
    void * userData
);



typedef struct topazAsset_Attributes_t topazAsset_Attributes_t;
/// Serves as the interface for asset objects
///
struct topazAsset_Attributes_t {
    
    /// Called when the asset is finalized.
    ///
    topaz_asset_callback on_create;

    /// Called when the asset is destroyed
    ///
    topaz_asset_callback on_destroy;


    /// User-provided data.
    ///
    void * userData;
};




/// Creates a new asset.
///
/// (No script mapping)
///
topazAsset_t * topaz_asset_create(
    /// Topaz context.
    topaz_t * context, 

    /// Type of the asset.
    topazAsset_Type type, 

    /// Name of the asset.
    const topazString_t * name, 

    /// Attributes of the asset.
    const topazAsset_Attributes_t * attribs
);

/// Destroys an asset.
///
/// (No script mapping)
///
void topaz_asset_destroy(
    /// The asset to destroy.
    topazAsset_t * asset
);

/// Returns the attributes that the asset was 
/// created with.
///
/// (No script mapping)
///
const topazAsset_Attributes_t * topaz_asset_get_attributes(
    /// The asset to retrieve attributes from.    
    const topazAsset_t * asset
);

/// Gets the name of the asset. This is immutable.
///
const topazString_t * topaz_asset_get_name(
    /// The asset in question.
    const topazAsset_t * asset
);

/// Gets the type of the asset. This is immutable.
///
topazAsset_Type topaz_asset_get_type(
    /// The asset in question.
    const topazAsset_t * asset
);


#endif


