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
///    Asset
///    -----
///
///    An asset represents a useful abstraction for an object 
///    that was populated from raw data, usually from a file or 
///    from memory.
///
///    On it's own, it does not do much. But, it serves as a
///    base class for more useful classes.
///
///
typedef struct topazAsset_t topazAsset_t;




/// Type of asset.
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
    topazAsset_Type_Audio,

    /// The asset is a 3D model with assets.
    ///
    topazAsset_Type_Model,

    /// The asset is a particle specification.
    ///
    topazAsset_Type_Particle,

    /// The asset is raw bytes / text.
    ///
    topazAsset_Type_Data,

    topazAsset_Type_Actor,
    topazAsset_Type_Count,    
};







typedef void (*topaz_asset_callback)(
    topazAsset_t *,
    void * userData
);


/// Serves as the interface for asset objects
typedef struct topazAsset_Attributes_t topazAsset_Attributes_t;
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
topazAsset_t * topaz_asset_create(
    topaz_t *, 
    topazAsset_Type type,
    const topazString_t * name,
    const topazAsset_Attributes_t *
);

/// Destroys an asset.
///
void topaz_asset_destroy(topazAsset_t *);

/// Returns the attributes that the asset was 
/// created with.
///
const topazAsset_Attributes_t * topaz_asset_get_attributes(const topazAsset_t *);

/// Gets the name of the asset. This is immutable.
///
const topazString_t * topaz_asset_get_name(const topazAsset_t *);

/// Gets the type of the asset. This is immutable.
///
topazAsset_Type topaz_asset_get_type(const topazAsset_t *);


#endif


