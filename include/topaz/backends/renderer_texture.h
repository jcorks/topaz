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


#ifndef H_TOPAZDC__RENDERER_TEXTURE__INCLUDED
#define H_TOPAZDC__RENDERER_TEXTURE__INCLUDED




#include <topaz/backends/renderer.h>


/*
    RendererTexture
    -----
*/

typedef struct topazRenderer_Texture_t topazRenderer_Texture_t;


/// Stores texture data so that it may be accessed in the drawing structure.
/// Returns the textureIndex. Passing a NULL as the rgbaTextureData
/// will allocate space for the texture, keep its contents as undefined
///
topazRenderer_Texture_t * topaz_renderer_texture_create(topazRenderer_t *, int w, int h, const uint8_t * rgbaTextureData);

/// Flags a texture for deletion. Deletion is not guaranteed to be immediate, nor does it guarantee
/// freeing of GPU memory; however, it does guarantee the possibility of adding one or more
/// textures of its size or less.
///
void topaz_renderer_texture_destroy(topazRenderer_Texture_t *);

/// redefines the contents of an existing image without
/// deleting it and re-adding it. The image is expected to be the
/// same dimensions as the original.
///
void topaz_renderer_texture_update(topazRenderer_Texture_t *, const uint8_t * newData);



/// Populates the given array with the texture data.
/// THe input buffer should be allocated to GetTextureWidth()*GetTextureHeight()*4
/// As with input buffered data, the format is RGBA with no padding.
///
void topaz_renderer_texture_get(topazRenderer_Texture_t *, uint8_t *);


/// Retrieves the texture's width
///
int topaz_renderer_texture_get_width(topazRenderer_Texture_t *);

/// Retrieves the texture's height
///
int topaz_renderer_texture_get_height(topazRenderer_Texture_t *);







#endif
