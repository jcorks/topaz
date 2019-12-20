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


#ifndef H_TOPAZDC__RENDERER__INCLUDED
#define H_TOPAZDC__RENDERER__INCLUDED



#include <topaz/matrix.h>
#include <topaz/backends/backend.h>



#define MINIMUM_LIGHT_COUNT             32
#define MINIMUM_TEXTURE_BINDING_COUNT   32

/*

    Renderer
    -----

*/

typedef struct topazRenderer_t topazRenderer_t;



typedef struct topazFramebuffer_t topazFramebuffer_t;



/// Defines the texture filter hint. Linear will 
/// texture all subsequent textures bilinearly, giving 
/// a smoother look to textures when scaled up.
/// Hints may be ignored by the renderer
///
typedef enum {
    /// Linear filtering of textures.
    ///
    topazRenderer_TextureFilterHint_Linear,

    /// No filtering of textures.
    ///
    topazRenderer_TextureFilterHint_None,

} topazRenderer_TextureFilterHint;



/// Defines the polygon type 
///
typedef enum {
    /// Triangle primitive. The most basic and versatile.
    ///
    topazRenderer_Primitive_Triangle,

    /// Line primitive. 
    ///
    topazRenderer_Primitive_Line
} topazRenderer_Primitive;



/// Defines the depth test, meaning the rule that determines which objects are to be 
/// to be drawn in front or behind. "Less" is the standard for 3D, which allows polygons 
/// with a lower depth to be drawn in front. "NoTest" is standard for 2D, which 
/// allows graphics drawn later to be on top.
///
typedef enum {

    /// Generated framebuffer elements whose calculated depth are less
    /// will be in front.
    ///
    topazRenderer_DepthTest_Less,

    /// Generated framebuffer elements whose calculated depth are <=
    /// will be in front.
    ///
    topazRenderer_DepthTest_LEQ,

    /// Generated framebuffer elements whose calculated depth are greater
    /// will be in front.
    ///
    topazRenderer_DepthTest_Greater,

    /// Generated framebuffer elements whose calculated depth are >=
    /// will be in front.
    ///
    topazRenderer_DepthTest_GEQ,

    /// No depth test is calculated. All elements pass.
    ///
    topazRenderer_DepthTest_None

} topazRenderer_DepthTest;





/// Allows for differenct transparency effects 
///
typedef enum {
    /// allows for standard "fade" transparency. What we're all used to
    ///
    topazRenderer_AlphaRule_Allow,       


    /// Ignores transparency
    ///
    topazRenderer_AlphaRule_Opaque,      

    /// Additive blending, which adds the result color ontop of whats already 
    /// there.
    ///
    topazRenderer_AlphaRule_Translucent, 

    /// Makes the drawn objects invisible. Note that etching and depth 
    /// information may still be written.
    ///
    topazRenderer_AlphaRule_Invisible    
} topazRenderer_AlphaRule;



/// Attributes that govern how to process vertices and visuals 
/// in a general sense. This is usually independent of the processing 
/// program in the case of 3D rendering.
///
typedef struct topazRenderer_ProcessAttribs_t topazRenderer_ProcessAttribs_t;

struct topazRenderer_ProcessAttribs_t {
    /// The primitive. See topazRenderer_Primitive
    ///
    topazRenderer_Primitive primitive;

    /// The depth test. See topazRenderer_DepthTest
    ///
    topazRenderer_DepthTest depthTest;

    /// The alpha rule. See topazRenderer_AlphaRule
    ///
    topazRenderer_AlphaRule alphaRule;

    /// The texture filtering hint. See topazRenderer_TextureFilterHint
    ///
    topazRenderer_TextureFilterHint textureFilter;

};









/// Allows for etching, which will ignore rendered results if they arent in the etch.
/// Etch regions last until next frame.
///
typedef enum {
    /// Default, no etching is done
    ///
    topazRenderer_EtchRule_NoEtching,    


    /// Along with whatever is being drawn, the etch region is defined. 
    /// In the 3D case, etches are only defined in ares where the depth test passes
    ///
    topazRenderer_EtchRule_Define,   


    /// Along with whatever is being drawn, the etch region is undefined if 
    /// defined within the frame.
    ///
    topazRenderer_EtchRule_Undefine, 


    /// Only shows things that fall within the etch region
    ///
    topazRenderer_EtchRule_In,       

    /// Only shows things that fall OUTSIDE the etch region
    ///
    topazRenderer_EtchRule_Out       

} topazRenderer_EtchRule;



/// Data component of the a framebuffer.
///
typedef enum {
    /// RGBA component.
    ///
    topazRenderer_DataLayer_Color,

    /// Depth component.
    ///
    topazRenderer_DataLayer_Depth,

    /// Etch component.
    ///
    topazRenderer_DataLayer_Etch
} topazRenderer_DataLayer;




/// Parameters that define how geometry is drawn and processed in 
/// a general sense.
///
typedef struct topazRenderer_Parameters_t topazRenderer_Parameters_t;

struct topazRenderer_Parameters_t {
    // Returns the maximum number of textures that can be referred to
    // by a StaticObject when rendering.
    int maxSimultaneousTextures;

    // returns the maximum number of lights supported by the renderer.
    int maxEnabledLights;

    // returns the number of lightx currently in use.
    int numLights;

    // C string with the name of the renderer program language.
    // If blank, custom renderer programs are unsupported.
    //
    char programLanguage[256];

};


typedef struct topazRendererAPI_t topazRendererAPI_t;
#include <topaz/backends/renderer_texture.h>
#include <topaz/backends/renderer_light.h>
#include <topaz/backends/renderer_program.h>
#include <topaz/backends/renderer_buffer.h>
#include <topaz/backends/renderer_3d.h>
#include <topaz/backends/renderer_2d.h>

#include <topaz/backends/api/renderer_buffer_api.h>
#include <topaz/backends/api/renderer_light_api.h>
#include <topaz/backends/api/renderer_2d_api.h>
#include <topaz/backends/api/renderer_program_api.h>
#include <topaz/backends/api/renderer_texture_api.h>
#include <topaz/backends/api/renderer_api.h>






/// Creates a new renderer object. The renderer implementation is made up 
/// of a number of components that, together, provide complete behavior of the 
/// renderer. The indivudal components are meant to be part of the renderer as 
/// a whole even though they are separate objects.
///
topazRenderer_t * topaz_renderer_create(
    topazBackend_t *, 

    /// API for the renderer direct functions    
    ///
    topazRendererAPI_t,

    /// API for renderer buffers, which provide renderer 
    /// storage.
    ///
    topazRenderer_BufferAPI_t,    

    /// API for renderer programs, which provide a way to 
    /// generate dynamic renderer programs such as shaders.
    ///
    topazRenderer_ProgramAPI_t,    

    /// API for renderer textures, which provide image data.
    /// 
    topazRenderer_TextureAPI_t,    

    /// API for the 2d renderer, which provides operations optimized 
    /// for 2d geometry (small batch, dynamic visuals)
    ///
    topazRenderer_2DAPI_t,    

    /// API for the light renderer, which provides lighting control 
    /// for 3d scenes.
    ///
    topazRenderer_LightAPI_t
);


/// Destroys and cleans up a renderer
///
void topaz_renderer_destroy(topazRenderer_t *);





/// Retrieves the backend for this time object.
///
topazBackend_t * topaz_renderer_get_backend(topazRenderer_t *);


/// Returns the API for this time.
///
topazRendererAPI_t topaz_renderer_get_api(topazRenderer_t *);







///////////// Drawing Requests



/// Unlike 2D requests, 3D render execution times are entirely up
/// to the renderer and backend. The only guarantee is that the object will render
/// on the committed display. The benefit of this mode is utilizing custom graphics programs
/// and the ability to utilize specialized renderer storage, which will almost always
/// yield better performance for static (3D) rendering over dynamic (2D) rendering. Vertices
/// here are transformed by the renderer.
void topaz_renderer_draw_3d(
    topazRenderer_t *,
    topazRenderer_3D_t *,
    const topazRenderer_ProcessAttribs_t *
);



/// Renders all queued objects and their vertices.
///
void topaz_renderer_draw_2d(
    topazRenderer_t *,
    topazRenderer_2D_t *,
    const topazRenderer_2D_Context_t *,
    const topazRenderer_ProcessAttribs_t *

);

/// Returns the transformation matrix buffer IDs for static rendering.
/// All RenderStatic positional vertices are multiplied by the Viewing and
/// projection matrices respectively. 
const topazMatrix_t * topaz_renderer_get_3d_viewing_matrix(topazRenderer_t *);
const topazMatrix_t * topaz_renderer_get_3d_projection_matrix(topazRenderer_t *);

void topaz_renderer_set_3d_viewing_matrix(topazRenderer_t *, const topazMatrix_t *);
void topaz_renderer_set_3d_projection_matrix(topazRenderer_t *, const topazMatrix_t *);








///////////// Framebuffer


/// Resets the renderer's render space to an initial state.
///
void topaz_renderer_clear_data(topazRenderer_t *);

    
/// Resets a specific part of the renderer's render space. This is often useful for 
/// complex scenes.
///
void topaz_renderer_clear_layer(topazRenderer_t *, topazRenderer_DataLayer);

/// Gets logistical information about the renderer.
/// Some backends may not given meanginful information here.
///
topazRenderer_Parameters_t topaz_renderer_get_parameters(topazRenderer_t *);



/* Display management */



/// Ensures that the all rendering operations are reflected in the attached target.
///
void topaz_renderer_sync(topazRenderer_t *);

/// The passed framebuffer becomes the destination for all future renderings
/// until a different valid framebuffer is given. If the Type of the given
/// Framebuffer is not one of the types from SupportedFramebuffers, no
/// action is taken. If NULL is passed, rendering will have no effect.
///
void topaz_renderer_attach_target(topazRenderer_t *, topazFramebuffer_t *);


/// Returns the current target for renderings. The default is
/// NULL.
///
topazFramebuffer_t * topaz_renderer_get_target(topazRenderer_t *);


/// Returns the framebuffer types that this renderer supports.
/// Should the framebuffer not match one of the given types, the framebuffer
/// attachment will fail.
const topazArray_t * topaz_renderer_get_supported_framebuffers(topazRenderer_t *);




#endif
