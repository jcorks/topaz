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



#include <topaz/backends/backend.h>
#include <topaz/matrix.h>
#include <topaz/containers/array.h>



#define MINIMUM_LIGHT_COUNT             32
#define MINIMUM_TEXTURE_BINDING_COUNT   32

/*

    Renderer
    -----

*/

typedef struct topazRenderer_t topazRenderer_t;











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




/*
    RendererBuffer
    -----

*/
typedef struct topazRenderer_Buffer_t topazRenderer_Buffer_t;


/// Allocates a new data store. It is not guaranteed where this data store is kept,
/// but if you are using a backend with hardware support, it is likely in VRAM.
/// If the allocation fails in some way, the id returned will be invalid
///
topazRenderer_Buffer_t * topaz_renderer_buffer_create(topazRenderer_t *, float * data, int numElements);

/// Frees the buffer store.
///
void topaz_renderer_buffer_destroy(topazRenderer_Buffer_t *);




/// Updates numElements elements of the buffer starting at the offset'th element
/// No check is made to see if this oversteps the original buffer size. If
/// overstepping occurs, the result is undefined.
///
void topaz_renderer_buffer_update(topazRenderer_Buffer_t *, float * newData, int offset, int numElements);


/// Populates outputData with the relevant data contained in the buffer. On most implementations
/// this will require expensive communication, so use with caution.
///
void topaz_renderer_buffer_read(topazRenderer_Buffer_t *, float * ouputData, int offset, int numELements);

/// Returns the number of elements of the buffer.
///
int topaz_renderer_buffer_get_size(topazRenderer_Buffer_t *);











/*
    Renderer2D
    ----
    // The general use case for 2D geometry is we will draw many 
    // small objects with unique tranform properties. Thus, a static rendering 
    // approach is less favorable as that would lead to more frequent draws.
    // Render2DVertices, using user-maintained global vertices, is a more 
    // performant option for drawing specifically 2D vertices.

*/

typedef struct topazRenderer_2D_t topazRenderer_2D_t;




/// Struct representing a dynamic (2D) vertex state.
/// Dynamic vertices do not support lighting or 
/// custom graphics programs.
///
typedef struct topazRenderer_2D_Vertex_t topazRenderer_2D_Vertex_t;

struct topazRenderer_2D_Vertex_t{
    /// vertex position
    ///
    float x, y;       
    
    /// color, scale from 0.f to 1.f (red, green, blue, and alpha)             
    ///
    float r, g, b, a; 
    
    /// texture coordinates (0, 0 is topleft)
    ///
    float texX, texY;                     
    
    /// if not used, set to -1, else float form of texture id
    ///
    float useTex;                         
    
    /// the transform reference object
    ///
    float object;                         
};



/// Context parameters used to give additional information 
/// when drawing a collection of 2D vertices.
///
typedef struct topazRenderer_2D_Context_t topazRenderer_2D_Context_t;

struct topazRenderer_2D_Context_t {
    /// Output display width.
    ///
    float width;

    /// Output display height.
    ///
    float height;

    /// Etch rule when drawing. 
    ///
    topazRenderer_EtchRule etchRule;        

    /// Matrix transform. Even though the input vertices are 2D, it 
    /// is possible to get a3D produced visual if the transform calls 
    /// for the transformation.
    ///
    float * transform;
};


typedef struct topazRenderer_2D_ObjectParams_t topazRenderer_2D_ObjectParams_t;

struct topazRenderer_2D_ObjectParams_t {
    // transform
    float data[16];
    
};








/// Creates a 2D render object.
///
topazRenderer_2D_t * topaz_renderer_2d_create(topazRenderer_t *);

/// Destroys a 2D render object.
///
void topaz_renderer_2d_destroy(topazRenderer_2D_t *);

/// Adds a new 2D objects and returns its IDs.
/// count should be the number of objects to request 
/// output is a buffer of sizeof(uint32_t)*count length.
/// Success is returned.
///
int topaz_renderer_2d_add_objects(topazRenderer_2D_t *, uint32_t * output, uint32_t count);

/// Removes a collection of objects.
///
void topaz_renderer_2d_remove_objects(topazRenderer_2D_t *, uint32_t * ids, uint32_t count);


/// Requests that an additional set of objects 
/// be drawn when using topaz_renderer_draw_2d
///
void topaz_renderer_2d_queue_vertices(
    topazRenderer_2D_t *,
    const uint32_t * objects,
    uint32_t count
);

/// Clears all requests queued
///
void topaz_renderer_2d_clear_queue(topazRenderer_2D_t *);




/// Adds a collection of vertices
/// count is the number of objects
/// output is the produced ids and should be a buffer of size sizeof(uint32_t)*count
/// Success is returned.
///
int topaz_renderer_2d_add_vertices(topazRenderer_2D_t *, uint32_t * output, uint32_t count);


/// Removes a set of 2d vertices.
///
void topaz_renderer_2d_remove_vertices(topazRenderer_2D_t *, uint32_t * objects, uint32_t count);

/// Sets vertices 
///
void topaz_renderer_2d_set_vertices(
    topazRenderer_2D_t *, 
    uint32_t * vertices, 
    const topazRenderer_2D_Vertex_t *, 
    uint32_t count
);

/// Gets the data for the specified vertices.
///
void topaz_renderer_2d_get_vertices(
    topazRenderer_2D_t *, 
    const uint32_t * vertexIDs, 
    topazRenderer_2D_Vertex_t * output,
    uint32_t count
);


/// Sets parameters for a specific object
///
void topaz_renderer_2d_set_object_params(
    topazRenderer_2D_t *, 
    uint32_t object, 
    const topazRenderer_2D_ObjectParams_t *
);







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









/*

    RendererLight
    -----
    TODO: change to RendererProgramData as a generic 
    buffer, which is more accurate and usable.

*/

typedef struct topazRenderer_Light_t topazRenderer_Light_t;




/// Types of rendering lights.
typedef enum {
    topazRenderer_LightType_Point,
    topazRenderer_LightType_Directional,
    topazRenderer_LightType_Spot
} topazRenderer_LightType;



// Creates a new light. When the light is first created, the state of the light
// is undefined. (It is expected that you will update the lights attributes
// before drawing);
topazRenderer_Light_t * topaz_renderer_light_create(topazRenderer_t *, topazRenderer_LightType);

// Removes all resources associated with the light ID.
void topaz_renderer_light_destroy(topazRenderer_Light_t *);

// As the default, the attributes requred are
//  Position (3-components)
//  Color (3-components)
//  Intensity (1 component)
// Thus, the renderer expects the array passed to have at least 7 components.

void topaz_renderer_light_update_attribs(topazRenderer_Light_t *, float *);

// Enables or diables the light by default, once added, the light is enabled by default
void topaz_renderer_light_enable(topazRenderer_Light_t *, int doIt);















/* 

    RendererProgram
    -----

*/

typedef struct topazRenderer_Program_t topazRenderer_Program_t;




/// Preset programs are default contexts 
///
typedef enum {
    /// MaterialIDs for render objects should be only 4 floats
    /// Each float represents a color, all 4 corresponding to RGBA.
    /// No lighting is enabled
    ///
    topazRenderer_PresetProgram_Basic,

    /// The objects output color is determined by
    /// a phong shading-like algorithm that utilizes any Light objects
    /// and set material for the object
    ///
    topazRenderer_PresetProgram_Light,


} topazRenderer_PresetProgram;


// Compiles and enacts a shader for use with Static Rendering. If the shader fails
// to compile or is invalid in some way, the id returned is invalid. See StaticState.
// In any case, log will be populated with information on the building process status.
topazRenderer_Program_t * topaz_renderer_program_create(
    topazRenderer_t *,
    const topazString_t *   vertexSrc, 
    const topazString_t *   fragSrc, 
    topazString_t *         log
);


// Returns a new renderer program from built-in params.
topazRenderer_Program_t * topaz_renderer_program_create_preset(
    topazRenderer_t *,
    topazRenderer_PresetProgram
);


void topaz_renderer_program_destroy(topazRenderer_Program_t *);







/// Vertex for 3D rendering.
///
typedef struct topazRenderer_3D_Vertex_t topazRenderer_3D_Vertex_t;


struct topazRenderer_3D_Vertex_t {
    /// Positional 3D coordinates
    ///
    float x, y, z;

    /// Normal vector, normalized
    ///
    float normalX, normalY, normalZ;

    /// Texture UVs. [0, 1]
    ///
    float texX, texY;

    /// User-define data (for programs)
    ///
    float userDefinedData[4];

};





/*

    RendererFramebuffer
    -----
    


*/

typedef struct topazRenderer_Framebuffer_t topazRenderer_Framebuffer_t;



/// \brief Types refer to the internal class by which visual information is
/// passed to the Display.
///
typedef enum {
    topazRenderer_Framebuffer_Handle_RGBA_PixelArray, ///< (uint8_t *) RGBA-ordered pixel array with no padding, matched to the dimensions given
    topazRenderer_Framebuffer_Handle_GLFBPacket,      ///< (GLRenderTarget **)  A pointer to a pointer to
                     /// a GLRenderTarget instance.
    topazRenderer_Framebuffer_Handle_Unknown,         ///< The framebuffer's data contents are unknown and should not be relied on.
} topazRenderer_Framebuffer_Handle;




/// Creates a new framebuffer for the renderer. Framebuffers hold 
/// results of drawing operations from the renderer.
///
topazRenderer_Framebuffer_t * topaz_renderer_framebuffer_create(topazRenderer_t *);


/// Destroys a framebuffer.
///
void topaz_renderer_framebuffer_destroy(topazRenderer_Framebuffer_t *);




/// Resizes the framebuffer. Success is returned.
///
int topaz_renderer_framebuffer_resize(topazRenderer_Framebuffer_t *, int newW, int newH);


/// Returns the width of the framebuffer.
///
int topaz_renderer_framebuffer_get_width(topazRenderer_Framebuffer_t *);

/// Returns the height of the framebuffer.
///
int topaz_renderer_framebuffer_get_height(topazRenderer_Framebuffer_t *);

/// Returns what type the handle refers to
///
topazRenderer_Framebuffer_Handle topaz_renderer_framebuffer_get_handle_type(topazRenderer_Framebuffer_t *);

/// Returns the source data that reflects the framebuffer's data
/// in the context of the implemented child. For example, on an OpenGL-variant
/// backend, the handle is likely an OpenGL texture object id.
/// This allows things like topazDisplay_t to utilize the rendered data.
///
void * topaz_renderer_framebuffer_get_handle(topazRenderer_Framebuffer_t *);



/// returns a RGBF pixel reduction by setting the
/// buffer given. if this isnt possible, false is returned.
/// the buffer should be of size Width*Height*4. Note that on
/// hardware-accelerated implementations, calling this could be very costly.
/// Alpha color information is always 1.f
int topaz_renderer_framebuffer_get_raw_data(topazRenderer_Framebuffer_t *, uint8_t *);

/// Sets whether to interpret the Framebuffer's data
/// in a filtered way.
///
/// It is not defined by topaz as to exactly what this means, other than some sort of pixel
/// interpolation to make the visual data smoother than otherwise when displayed to a user.
/// This is most applicable when using the Framebuffer where
/// its natural size cannot be expressed (i.e. displaying the framebuffer
/// visual on a system window of a different size). It should also be mentioned,
/// that any module that utilizes the Framebuffer may choose to ignore the
/// the filtered hint. As such, you should expect the filtered setting
/// to be a purely cosmetic effect after all graphics processing as finished. 
/// The default is true.
void topaz_renderer_framebuffer_set_filtered_hint(topazRenderer_Framebuffer_t *, int filter);

/// \brief Returns whether to interpret the Framebuffer's data in a filtered
/// way.
///
/// See set_filtered_hint().
int topaz_renderer_framebuffer_get_filtered_hint(topazRenderer_Framebuffer_t *);









/// A 3D rendering object 
///
typedef struct topazRenderer_3D_t topazRenderer_3D_t;


struct topazRenderer_3D_t {
    
    /// Vertices points to a renderbuffer containing all the vertex dat  a pertinent to the RenderObject.
    /// Each vertex consists of:
    ///      3-components for position,
    ///      3-components for the normal.
    ///      2-copmonents for UVs (texture coordinates)
    ///      4-components for user-defined data. Meant to be utilized with custom programs.
    ///
    topazRenderer_Buffer_t * vertices;
    
    /// Specifies how to render the vertices
    topazRenderer_Program_t * program;

    /// material matrix , 44-components containing
    /// lighting material and arbitrary data setup.
    /// The layout is as follows:
    /// 4-floats for ambient (standardly)
    /// 4-floats for diffuse (standardly)
    /// 4-floats for specular (standardly);
    /// 32-floats of user data.
    ///
    topazRenderer_Buffer_t * materialData;  

    /// local transform matrix (scale + rotation), 32-component
    /// Normal matrix (inverse transpose of modelData)
    ///
    topazRenderer_Buffer_t * modelData;  

    /// the source framebuffer optionally accessible 
    /// during rendering. If the samplebuffer is null,
    /// the no source framebuffer will be made available
    ///
    topazRenderer_Framebuffer_t * samplebuffer;


    /// specifies the textures to be used. Each pair is a slot referred to by 
    /// each program and the ID of the texture.
    ///
    topazArray_t * textureSlots;
    topazArray_t * textureObjects;


    /* Static index list */
    // Index lists are used to build the geometry of the StaticState.
    // Each index refer to the index of the vertex to be used to build the triangle.
    // every 3 vertices, a triangle is formed. If a multiple of three is not used,
    // the behavior is implementation defined.
    topazArray_t * indices;    


};









#include <topaz/backends/api/renderer_api.h>






/// Creates a new renderer object. The renderer implementation is made up 
/// of a number of components that, together, provide complete behavior of the 
/// renderer. The indivudal components are meant to be part of the renderer as 
/// a whole even though they are separate objects.
///
topazRenderer_t * topaz_renderer_create(
    topazBackend_t *, 
    topazRendererAPI_t 
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
void topaz_renderer_attach_target(topazRenderer_t *, topazRenderer_Framebuffer_t *);


/// Returns the current target for renderings. The default is
/// NULL.
///
topazRenderer_Framebuffer_t * topaz_renderer_get_target(topazRenderer_t *);


/// Returns the framebuffer types that this renderer supports.
/// Should the framebuffer not match one of the given types, the framebuffer
/// attachment will fail.
const topazArray_t * topaz_renderer_get_supported_framebuffers(topazRenderer_t *);




#endif
