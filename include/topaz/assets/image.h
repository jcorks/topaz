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


#ifndef H_TOPAZDC__IMAGE__INCLUDED
#define H_TOPAZDC__IMAGE__INCLUDED

#include <topaz/asset.h>
typedef struct topazArray_t topazArray_t;
typedef struct topazRenderer_Texture_t topazRenderer_Texture_t;
typedef struct topazAsset_t topazAsset_t;

///    Hold image information. An image within topaz can be multiple frames
///    which can be used a variety of ways (animation, sprite map, etc.)
typedef struct topazImage_t topazImage_t;




/// An individual frame of information that holds pixel data.
/// Images contain 1 or more frames.
typedef struct topazImage_Frame_t topazImage_Frame_t;



/// Creates a new image object
/// Use asset_destroy to release.
///
topazAsset_t * topaz_image_create(
    topaz_t *, 
    const topazString_t *
);

/// Creates a new image object, but without 
/// any loading behavior. This allows for creating custom 
/// images 
topazAsset_t * topaz_image_create_empty(topaz_t *);

/// Creates a new frame after the existing frames.
/// The new frame is returned for convenience.
///
topazImage_Frame_t * topaz_image_add_frame(topazAsset_t *);

/// Returns the index'th frame within the image.
/// If the index is out of bounds, index%topaz_iamge_get_frame_count() is used.
///
topazImage_Frame_t * topaz_image_get_frame(topazAsset_t *, uint32_t index);

/// Removes the index'th frame from the image. If none exists
/// no action is taken.
///
void topaz_image_remove_frame(topazAsset_t *, uint32_t);

/// Returns the number of frames within the image.
///
uint32_t topaz_image_get_frame_count(const topazAsset_t *);

/// Returns the width of the image
///
int topaz_image_get_width(const topazAsset_t *);

/// Returns the height of the image
///
int topaz_image_get_height(const topazAsset_t *);

/// Resets the width/height of the image.
/// Afterwhich, the contents of all frames are undefined
///
void topaz_image_resize(
    /// The image to modify.
    topazAsset_t * image,
    /// The new width of the image.
    int width,
    /// The new height of the image.
    int height
);


/// Different events for textures.
typedef enum topazImage_TextureEvent topazImage_TextureEvent;

enum topazImage_TextureEvent {

    /// This callback is called BEFORE texture is removed.
    topazImage_TextureEvent_Removed,

    /// This callback is valled when refetching the frame's texture 
    /// will yield a different texture object.
    topazImage_TextureEvent_Changed



};

/// Callback set with topaz_image_add_texture_event_notify().
typedef void (*topaz_image_texture_event_callback)(
    /// The image whose texture is getting removed.
    topazAsset_t * image,
    /// The frame of the texture getting removed.
    int frame,
    /// The event for the texture,
    topazImage_TextureEvent event,
    /// The associated data.
    void * data
);

/// Adds a callback to be run when the internal topazTexture_t of 
/// a frame is removed and should no longer be used. 
/// This can happen for actions such as:
/// - image resizing
/// - frame removal
/// - texture replacement
///
/// This is mostly used within topaz itself to make sure that 
/// things using topazImage_t instances can efficiently know 
/// when to stop using a texture and when to refetch a texture.
///
/// A unique ID to the notification is returned. This can 
/// be used to remove the callback.
uint32_t topaz_image_add_texture_event_notify(
    /// The image to return.
    topazAsset_t * image,

    /// The callback to be called when the image resizes.
    topaz_image_texture_event_callback callback,

    /// The data to associate with the callback.
    void * data
);


/// Removes a texture event notify callback.
///
void topaz_image_remove_texture_event_notify(
    /// The image to remove a notify from.
    topazAsset_t * image,

    /// The callback to remove.
    uint32_t callbackID
    
);






/// Creates an image frame from a pre-existing texture.
/// Every frame has its own texture, so a new texture 
/// is created as a clone of the given one.
///
void topaz_image_frame_set_from_texture(
    /// Frame to modify.
    topazImage_Frame_t *,
    /// Texture to clone from. 
    topazRenderer_Texture_t *
);


/// Sets the RGBA pixel data within the image according 
/// to the width and height of the frame. This assumes that 
/// the given buffer is at least width*height*4 bytes large.
///
void topaz_image_frame_set_data(
    /// Frame to modify.
    topazImage_Frame_t *,
    /// Texture data to read from and copy.
    const uint8_t * rgbdaData
);



/// Gets the texture handle for the frame.
///
topazRenderer_Texture_t * topaz_image_frame_get_texture(const topazImage_Frame_t *);


/// Returns a new array with rgba data of the image frame.
///
topazArray_t * topaz_image_frame_get_rgba_data(const topazImage_Frame_t *);


#endif
