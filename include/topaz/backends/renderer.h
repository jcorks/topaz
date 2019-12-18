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


#ifndef H_TOPAZDC__FILESYS__INCLUDED
#define H_TOPAZDC__FILESYS__INCLUDED




#include <topaz/backends/backend.h>



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




/// Attributes that govern how to process vertices and visuals 
/// in a general sense. This is usually independent of the processing 
/// program in the case of 3D rendering.
///
typedef struct {
    /// The primitive. See topazRenderer_Primitive
    ///
    topazRenderer_Primitive primitive,

    /// The depth test. See topazRenderer_DepthTest
    ///
    topazRenderer_DepthTest depthTest,

    /// The alpha rule. See topazRenderer_AlphaRule
    ///
    topazRenderer_AlphaRule alphaRule

    /// The texture filtering hint. See topazRenderer_TextureFilterHint
    ///
    topazRenderer_TextureFilterHint textureFilter;

} topazRenderer_ProcessAttribs;





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


/// Types of rendering lights.
typedef enum {
    topazRenderer_LightType_Point,
    topazRenderer_LightType_Directional,
    topazRenderer_LightType_Spot
} topazRenderer_LightType;




/// Struct representing a dynamic (2D) vertex state.
/// Dynamic vertices do not support lighting or 
/// custom graphics programs.
///
typedef struct {
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
} topazRenderer_2D_Vertex;



/*

    Renderer3D

*/

typedef struct {
    
    /// Vertices points to a renderbuffer containing all the vertex dat  a pertinent to the RenderObject.
    /// Each vertex consists of:
    ///      3-components for position,
    ///      3-components for the normal.
    ///      2-copmonents for UVs (texture coordinates)
    ///      4-components for user-defined data. Meant to be unitlized with custom programs.
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
    topazFramebuffer_t * samplebuffer;


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


} topazRenderer_3D;



/// Vertex for 3D rendering.
///
typedef struct  {
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

} topazRenderer_3D_Vertex;


typedef struct {
    // Returns the maximum number of textures that can be referred to
    // by a StaticObject when rendering.
    int maxSimultaneousTextures;

    // returns the maximum number of lights supported by the renderer.
    int MaxEnabledLights() = 0;

    // returns the number of lightx currently in use.
    int NumLights() = 0;

} topazRenderer_Parameters;



#include <topaz/backends/api/renderer_api.h>


/*
    RendererBuffer
    -----

*/
typedef struct topazRenderer_Buffer_t topazRenderer_Buffer_t;


/// Allocates a new data store. It is not guaranteed where this data store is kept,
/// but if you are using a backend with hardware support, it is likely in VRAM.
/// If the allocation fails in some way, the id returned will be invalid
///
topazRenderer_Buffer_t * topaz_renderer_buffer_create(topazRenderer_t *, float * data, int numElements) = 0;

/// Frees the buffer store.
///
void topaz_renderer_buffer_destroy(topazRenderer_Buffer_t *) = 0;




/// Updates numElements elements of the buffer starting at the offset'th element
/// No check is made to see if this oversteps the original buffer size. If
/// overstepping occurs, the result is undefined.
///
void topaz_renderer_buffer_update(topazRenderer_Buffer_t * float * newData, int offset, int numElements) = 0;


/// Populates outputData with the relevant data contained in the buffer. On most implementations
/// this will require expensive communication, so use with caution.
///
void topaz_renderer_buffer_read(topazRenderer_Buffer_t *, float * ouputData, int offset, int numELements) = 0;

/// Returns the number of elements of the buffer.
///
int topaz_renderer_buffer_get_size(topazRenderer_Buffer_t *) = 0;







/*
    RendererTexture
    -----
*/

typedef struct topazRenderer_Texture_t topazRenderer_Texture_t;


/// Stores texture data so that it may be accessed in the drawing structure.
/// Returns the textureIndex. Passing a NULL as the rgbaTextureData
/// will allocate space for the texture, keep its contents as undefined
///
topazRenderer_Texture_t * topaz_renderer_texture_create(topazRenderer_t *, int w, int h, const uint8_t * rgbaTextureData) = 0;

/// Flags a texture for deletion. Deletion is not guaranteed to be immediate, nor does it guarantee
/// freeing of GPU memory; however, it does guarantee the possibility of adding one or more
/// textures of its size or less.
///
void topaz_renderer_texture_destroy(topazRenderer_Texture_t *) = 0;

/// redefines the contents of an existing image without
/// deleting it and re-adding it. The image is expected to be the
/// same dimensions as the original.
///
void topaz_renderer_texture_update(topazRenderer_Texture_t *, const uint8_t * newData);



/// Populates the given array with the texture data.
/// THe input buffer should be allocated to GetTextureWidth()*GetTextureHeight()*4
/// As with input buffered data, the format is RGBA with no padding.
///
void topaz_renderer_texture_get(topazRenderer_Texture_t *, uint8_t *) = 0;


/// Retrieves the texture's width
///
int topaz_renderer_texture_get_width(topazRenderer_Texture_t *) = 0;

/// Retrieves the texture's height
///
int topaz_renderer_texture_get_height(topazRenderer_Texture_t *) = 0;




/* 

    RendererProgram
    -----

*/

typedef struct topazRenderer_Program_t topazRenderer_Program_t;


// Compiles and enacts a shader for use with Static Rendering. If the shader fails
// to compile or is invalid in some way, the id returned is invalid. See StaticState.
// In any case, log will be populated with information on the building process status.
topazRenderer_Program_t * topaz_renderer_program_create(
    topazRenderer_t *,
    const topazString_t *   vertexSrc, 
    const topazString_t *   fragSrc, 
    topazString_t *         log
);

topazRenderer_Program_t * topaz_renderer_program_destroy(topazRenderer_Program_t *);


/// Returns a string denoting the name of the shading language supported.
/// If shaders are not supported, an empty string is returned.
const topazString_t * topaz_renderer_program_get_language() = 0;

// Returns the id referring to a builtin shader.
topazRenderer_Program_t * topaz_renderer_program_get_preset(topazRenderer_PresetProgram) = 0;






/*

    RendererLight
    -----
    TODO: change to RendererProgramData as a generic 
    buffer, which is more accurate and usable.

*/

typedef struct topazRenderer_Light_t topazRenderer_Light_t;

// Creates a new light. When the light is first created, the state of the light
// is undefined. (It is expected that you will update the lights attributes
// before drawing);
topazRenderer_Light_t * topaz_renderer_light_create(topazRenderer_LightType) = 0;

// Removes all resources associated with the light ID.
void topaz_renderer_light_destroy(topazRenderer_Light_t *) = 0;

// As the default, the attributes requred are
//  Position (3-components)
//  Color (3-components)
//  Intensity (1 component)
// Thus, the renderer expects the array passed to have at least 7 components.

void topaz_renderer_light_update_attribs(LightID, float *) = 0;

// Enables or diables the light by default, once added, the light is enabled by default
void topaz_renderer_light_enable(LightID, bool doIt) = 0;










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




/// Context parameters used to give additional information 
/// when drawing a collection of 2D vertices.
///
typedef struct {
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
} topazRenderer_2D_Context;

typedef struct {
    // transform
    float data[16];
    
} topazRenderer_2D_ObjectParams;


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
int topaz_renderer_2d_add_objects(topazRenderer_2D_t *, uint32_t * output, uint32_t count) = 0;

/// Removes a collection of objects.
///
void Remove2DObject(topazRenderer_2D_t *, uint32_t * idsuint32_t count) = 0;


/// Requests that an additional set of objects 
/// be drawn when using topaz_renderer_draw_2d
///
void topaz_renderer_2d_queue_vertices(
    topazRenderer_2D_t *,
    const uint32_t * objects,
    uint32_t count
) = 0;

/// Clears all requests queued
///
void topaz_renderer_2d_clear_queue(topazRenderer_2D_t *) = 0;




/// Adds a collection of vertices
/// count is the number of objects
/// output is the produced ids and should be a buffer of size sizeof(uint32_t)*count
/// Success is returned.
///
int topaz_renderer_2d_add_vertices(topazRenderer_2D_t *, uint32_t * output, uint32_t count) = 0;


/// Removes a set of 2d vertices.
///
void topaz_renderer_2d_remove_vertices(topazRenderer_2D_t *, uint32_t * objects, uint32_t count) = 0;

/// Sets vertices 
///
void topaz_renderer_2d_set_vertices(
    topazRenderer_2D_t *, 
    uint32_t * vertices, 
    const topazRenderer_2D_Vertex *, 
    uint32_t count
);

/// Gets the data for the specified vertices.
///
topazRenderer_2D_Vertex Get2DVertices(
    topazRenderer_2D_t *, 
    const uint32_t * vertexIDs, 
    topazRenderer_2D_Vertex * output,
    uint32_t count
);


/// Sets parameters for a specific object
///
void Set2DObjectParameters(
    topazRenderer_2D_t *, 
    uint32_t object, 
    const topazRenderer_2D_ObjectParams *
) = 0;

























/// Creates a new renderer object
///
topazRenderer_t * topaz_renderer_create(topazBackend_t *, topazRendererAPI_t);


/// Destroys and cleans up a renderer
///
void topaz_renderer_destroy(topazRenderer_t *);





/// Retrieves the backend for this time object.
///
topazBackend_t * topaz_renderer_get_backend(topazRenderer_t *);


/// Returns the API for this time.
///
topazRendererAPI_t topaz_renderer_get_api(topazRenderer_t *);





/// Renders all queued objects and their vertices.
///
void topaz_renderer_draw_2d(
    topazRenderer_t *,
    topazRenderer_2D_t *,
    const topazRenderer_2D_Context *,
    const topazRenderer_ProcessAttribs *,

);



/// Unlike 2D requests, 3D render execution times are entirely up
/// to the renderer and backend. The only guarantee is that the object will render
/// on the committed display. The benefit of this mode is utilizing custom graphics programs
/// and the ability to utilize specialized renderer storage, which will almost always
/// yield better performance for static (3D) rendering over dynamic (2D) rendering. Vertices
/// here are transformed by the renderer.
void topaz_renderer_draw_3d(
    topazRenderer_t *,
    topazRenderer_3D_t *
    const topazRenderer_ProcessAttribs *
);

/// Returns the transformation matrix buffer IDs for static rendering.
/// All RenderStatic positional vertices are multiplied by the Viewing and
/// projection matrices respectively. Once determined by the renderer,
/// these ID's values will not change per instance.
topazRenderer_Buffer_t * topaz_renderer_get_3d_viewing_matrix() = 0;
topazRenderer_Buffer_t * topaz_renderer_get_3d_projection_matrix() = 0;





/// Resets the renderer's render space to an initial state.
///
void topaz_renderer_clear_data(topazRenderer_t *) = 0;

    
/// Resets a specific part of the renderer's render space. This is often useful for 
/// complex scenes.
///
void topaz_renderer_clear_layer(topazRenderer_t *, topazRenderer_DataLayer) = 0;








/// Gets logistical information about the renderer.
/// Some backends may not given meanginful information here.
///
topazRenderer_Parameters topaz_renderer_get_parameters(topazRenderer_t *);











/* Display management */



/// Ensures that the all rendering operations are reflected in the attached target.
///
void topaz_renderer_sync(topazRenderer_t *);

/// The passed framebuffer becomes the destination for all future renderings
/// until a different valid framebuffer is given. If the Type of the given
/// Framebuffer is not one of the types from SupportedFramebuffers, no
/// action is taken. If NULL is passed, rendering will have no effect.
///
void topaz_renderer_attach_target(topazRenderer_t *, Framebuffer *);


/// Returns the current target for renderings. The default is
/// NULL.
///
topazFramebuffer_t * topaz_renderer_get_target(topazRenderer_t *) = 0;


/// Returns the framebuffer types that this renderer supports.
/// Should the framebuffer not match one of the given types, the framebuffer
/// attachment will fail.
const topazArray_t * topaz_renderer_get_supported_framebuffers(topazRenderer_t *);



