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



#include <topaz/system.h>
#include <topaz/matrix.h>
#include <topaz/containers/array.h>



#define MINIMUM_LIGHT_COUNT             32
#define MINIMUM_TEXTURE_BINDING_COUNT   32


///
/// Low-level renderer abstraction. Using this interface,
/// applications can create rendered visuals with accelerated pathways.
/// The most common use is to create visuals that Displays can present
/// the users quickly and repeatedly. Its possible to also use renderers
/// to create visuals without displays (though usually, this is more 
/// slow)
///
/// The renderer is split up into smaller components:
/// - Textures:     holds image information, has its own objects.
/// - Buffers:      holds generic information for the rest of the renderer. Usually 
///                 its stored on the renderering device when applicable.
/// - Renderer2D:   Handles components optimized for 2D rendering.
/// - Lights:       Light specifications for 3D rendering.
/// - Programs:     Programmable pipeline interface if the renderer supports it.
/// - Framebuffers: Accelerated targets for rendering operations.
///
typedef struct topazRenderer_t topazRenderer_t;











/// Defines the texture filter hint. Linear will 
/// texture all subsequent textures bilinearly, giving 
/// a smoother look to textures when scaled up.
/// Hints may be ignored by the renderer
///
typedef enum topazRenderer_TextureFilterHint topazRenderer_TextureFilterHint;
enum topazRenderer_TextureFilterHint {
    /// Linear filtering of textures.
    ///
    topazRenderer_TextureFilterHint_Linear,

    /// No filtering of textures.
    ///
    topazRenderer_TextureFilterHint_None,

};



/// Defines the polygon type 
///
typedef enum topazRenderer_Primitive topazRenderer_Primitive;
enum topazRenderer_Primitive {
    /// Triangle primitive. The most basic and versatile.
    ///
    topazRenderer_Primitive_Triangle,

    /// Line primitive. 
    ///
    topazRenderer_Primitive_Line
};




/// Defines the depth test, meaning the rule that determines which objects are to be 
/// to be drawn in front or behind. "Less" is the standard for 3D, which allows polygons 
/// with a lower depth to be drawn in front. "NoTest" is standard for 2D, which 
/// allows graphics drawn later to be on top.
///
typedef enum topazRenderer_DepthTest topazRenderer_DepthTest;
enum topazRenderer_DepthTest {

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

    /// Generated framebuffer elements whole calculated depth is equal to 
    /// whats in the depth buffer
    topazRenderer_DepthTest_Equal,


    /// No depth test is calculated. All elements pass.
    ///
    topazRenderer_DepthTest_None

};





/// Allows for differenct transparency effects 
///
typedef enum topazRenderer_AlphaRule topazRenderer_AlphaRule;
enum topazRenderer_AlphaRule {
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
};



/// Allows for etching, which will ignore rendered results if they arent in the etch.
/// Etch regions last until next frame by default.
///
typedef enum topazRenderer_EtchRule topazRenderer_EtchRule;
enum topazRenderer_EtchRule {
    /// Default, no etching is done
    ///
    topazRenderer_EtchRule_NoEtching,    


    /// Along with whatever is being drawn, the etch region is defined. 
    /// Etches are only defined in ares where the depth test passes
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

};


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

    /// Etch rule when drawing. 
    ///
    topazRenderer_EtchRule  etchRule;        

    /// The texture filtering hint. See topazRenderer_TextureFilterHint
    ///
    topazRenderer_TextureFilterHint textureFilter;

};










/// Data component of the a framebuffer.
///
typedef enum topazRenderer_DataLayer topazRenderer_DataLayer;
enum topazRenderer_DataLayer {
    /// RGBA component.
    ///
    topazRenderer_DataLayer_Color = 1,

    /// Depth component.
    ///
    topazRenderer_DataLayer_Depth = 2,

    /// Etch component.
    ///
    topazRenderer_DataLayer_Etch = 4,

    /// All components.
    ///
    topazRenderer_DataLayer_All = 7
};




typedef struct topazRenderer_Parameters_t topazRenderer_Parameters_t;
/// Parameters that define how geometry is drawn and processed in 
/// a general sense.
///
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





/// Textures hold visual information expressed in pixels.
/// All textures are encoded RGBA (red-green-blue-alpha) 
/// where each component is 8-bits of saturation information.
///
typedef struct topazRenderer_Texture_t topazRenderer_Texture_t;


/// Stores texture data so that it may be accessed in the drawing structure.
/// Returns the textureIndex. Passing a NULL as the rgbaTextureData
/// will allocate space for the texture, keep its contents as undefined
///
topazRenderer_Texture_t * topaz_renderer_texture_create(
    /// The renderer to use.
    topazRenderer_t * renderer, 

    /// Width of the new texture.
    int w, 

    /// Height of the new texture.
    int h, 

    /// The RGBA source data. Optional.
    const uint8_t * rgbaTextureData
);

/// Flags a texture for deletion. Deletion is not guaranteed to be immediate, nor does it guarantee
/// freeing of GPU memory; however, it does guarantee the possibility of adding one or more
/// textures of its size or less.
///
void topaz_renderer_texture_destroy(
    /// The texture object to destroy.
    topazRenderer_Texture_t * tex
);

/// redefines the contents of an existing image without
/// deleting it and re-adding it. The image is expected to be the
/// same dimensions as the original.
///
void topaz_renderer_texture_update(
    /// The texture object to update.
    topazRenderer_Texture_t * tex, 

    /// The new RGBA data whose size matches the existing texture.
    const uint8_t * newData
);



/// Populates the given array with the texture data.
/// THe input buffer should be allocated to GetTextureWidth()*GetTextureHeight()*4
/// As with input buffered data, the format is RGBA with no padding.
///
void topaz_renderer_texture_get(
    /// The texture to query.
    topazRenderer_Texture_t * tex, 
    /// The buffer to populate with RGBA information.
    uint8_t * buffer
);


/// Retrieves the texture's width
///
int topaz_renderer_texture_get_width(
    /// The texture to query.
    topazRenderer_Texture_t * tex
);

/// Retrieves the texture's height
///
int topaz_renderer_texture_get_height(
    /// The texture to query.
    topazRenderer_Texture_t * tex
);





/// Generically holds data, usually on accelerated
/// mediums so that they are ready for rendering operations.
/// 
typedef struct topazRenderer_Buffer_t topazRenderer_Buffer_t;


/// Allocates a new data store. It is not guaranteed where this data store is kept,
/// but if you are using a backend with hardware support, it is likely in VRAM.
/// If the allocation fails in some way, the id returned will be invalid
///
topazRenderer_Buffer_t * topaz_renderer_buffer_create(
    /// The renderer to use.
    topazRenderer_t * renderer,

    /// The initial data of the buffer. 
    float * data, 

    /// The number of floating values.
    uint32_t numElements
);

/// Frees the buffer store.
///
void topaz_renderer_buffer_destroy(
    /// The buffer to destroy.
    topazRenderer_Buffer_t * buffer
);




/// Updates numElements elements of the buffer starting at the offset'th element
/// No check is made to see if this oversteps the original buffer size. If
/// overstepping occurs, the result is undefined.
///
void topaz_renderer_buffer_update(
    /// The buffer to update.
    topazRenderer_Buffer_t * buffer,
    /// The incoming data. 
    const float * newData,
    /// The offset to start editing the buffer. 
    uint32_t offset, 
    /// The number of float values to upload.
    uint32_t numElements
);


/// Populates outputData with the relevant data contained in the buffer. On most implementations
/// this will require expensive communication, so use with caution.
///
void topaz_renderer_buffer_read(
    /// The buffer to read.
    topazRenderer_Buffer_t * buffer,

    /// The buffer to populate with data. 
    float * ouputData, 

    /// The offset of the buffer to start reading.
    uint32_t offset, 

    /// the number of elements to read.
    uint32_t numElements
);

/// Returns the number of elements of the buffer.
///
int topaz_renderer_buffer_get_size(
    /// The buffer to query.
    topazRenderer_Buffer_t * buffer
);











/// The general use case for 2D geometry is we will draw many 
/// small objects with unique tranform properties. Thus, a static rendering 
/// approach is less favorable as that would lead to more frequent draws.
/// Render2DVertices, using user-maintained global vertices, is a more 
/// performant option for drawing specifically 2D vertices.
typedef struct topazRenderer_2D_t topazRenderer_2D_t;




typedef struct topazRenderer_2D_Vertex_t topazRenderer_2D_Vertex_t;

/// Struct representing a dynamic (2D) vertex state.
/// Dynamic vertices do not support lighting or 
/// custom graphics programs.
///
struct topazRenderer_2D_Vertex_t{
    /// The vertex x position.
    float x;    
    /// The vertex y position.
    float y;       
    
    /// Color, scale from 0.f to 1.f: red.
    float r;
    /// Color, scale from 0.f to 1.f: green.
    float g; 
    /// Color, scale from 0.f to 1.f: blue.
    float b; 
    /// Color, scale from 0.f to 1.f: alpha.
    float a;
    
    /// Texture coordinates (UVs), x position. (0, 0 is topleft)
    float texX;
    /// Texture coordinates (UVs), y position. (0, 0 is topleft)
    float texY;       
};



typedef struct topazRenderer_2D_Context_t topazRenderer_2D_Context_t;
/// Context parameters used to give additional information 
/// when drawing a collection of 2D vertices.
///
struct topazRenderer_2D_Context_t {
    /// Output display width.
    ///
    float width;

    /// Output display height.
    ///
    float height;

    /// Matrix transform. Even though the input vertices are 2D, it 
    /// is possible to get a3D produced visual if the transform calls 
    /// for the transformation.
    ///
    topazMatrix_t * transform;
};









/// Creates a 2D render object.
///
topazRenderer_2D_t * topaz_renderer_2d_create(
    /// The source renderer
    topazRenderer_t * renderer
);

/// Destroys a 2D render object.
///
void topaz_renderer_2d_destroy(
    /// The 2D renderer to destroy.
    topazRenderer_2D_t * r2d
);

/// Adds a new 2D objects and returns its IDs.
/// count should be the number of objects to request 
/// output is a buffer of sizeof(uint32_t)*count length.
/// Success is returned.
///
int topaz_renderer_2d_add_objects(
    /// The 2D renderer to modify.
    topazRenderer_2D_t * r2d,
    /// The created objects. This buffer will be populated with object IDs. 
    uint32_t * output, 
    /// The number of objects to create.
    uint32_t count
);

/// Removes a collection of objects.
///
void topaz_renderer_2d_remove_objects(
    /// The 2D renderer to remove objects from.
    topazRenderer_2D_t * r2d, 
    /// The objects to remove.
    uint32_t * ids, 
    /// The number of objects to read and remove.
    uint32_t count
);


/// Requests that an additional set of objects 
/// be drawn when using topaz_renderer_draw_2d
///
void topaz_renderer_2d_queue_objects(
    /// The 2D renderer to queue objects to.
    topazRenderer_2D_t * r2d,
    /// The obejcts to queue.
    const uint32_t * objects,
    /// The number of objects to queue.
    uint32_t count
);

/// Clears all requests queued
///
void topaz_renderer_2d_clear_queue(
    /// The 2D renderer to clear.
    topazRenderer_2D_t * r2d
);



/// Attaches a buffer to the object that provides vertex data.
/// The buffer is assumed to be laid out in data as if it were 
/// contiguous topazRenderer_Vertex_2D_t objects.
///
void topaz_renderer_2d_set_object_vertices(
    /// The owner of the object
    topazRenderer_2D_t * r2d, 

    /// The ID of the object to modify
    uint32_t object, 

    /// The buffer containing vertex info.
    topazRenderer_Buffer_t * buffer
);


/// Sets parameters for a specific object
///
void topaz_renderer_2d_set_object_transform(
    /// The owner of the object
    topazRenderer_2D_t * r2d,

    /// The object to modify.
    uint32_t object,

    /// Transform to use.
    const topazMatrix_t * transform
);

void topaz_renderer_2d_set_object_texture(
    /// The owner of the object
    topazRenderer_2D_t * r2d, 

    /// The object to modify.
    uint32_t object, 

    /// The texture to 
    topazRenderer_Texture_t * texture


);

/// Gets the source renderer context that made 
/// this renderer2D instance 
///
topazRenderer_t * topaz_renderer_2d_get_context(
    /// The 2D renderer to query.
    topazRenderer_2D_t * r2d
);












/// TODO: change to RendererProgramData as a generic 
/// buffer, which is more accurate and usable.
///
typedef struct topazRenderer_Light_t topazRenderer_Light_t;




/// Types of rendering lights.
///
typedef enum topazRenderer_LightType topazRenderer_LightType;
enum topazRenderer_LightType {
    /// The light is a point-light. It has a single position in 3D space.
    topazRenderer_LightType_Point,

    /// The light is applied in a direction universally. This is useful for 
    /// things like sunlight
    topazRenderer_LightType_Directional
};



// Creates a new light. When the light is first created, the state of the light
// is undefined. (It is expected that you will update the lights attributes
// before drawing);
topazRenderer_Light_t * topaz_renderer_light_create(
    /// The renderer to create the light for.
    topazRenderer_t * renderer, 

    /// The type of the light.
    topazRenderer_LightType type
);

// Removes all resources associated with the light ID.
void topaz_renderer_light_destroy(
    /// The light to remove.
    topazRenderer_Light_t * light
);

// As the default, the attributes requred are
//  Position (3-components)
//  Color (3-components)
//  Intensity (1 component)
// Thus, the renderer expects the array passed to have at least 7 components.
void topaz_renderer_light_update_attribs(
    /// The light to modify.
    topazRenderer_Light_t * light, 

    /// The data to copy to the light.
    float * data
);

// Enables or diables the light by default, once added, the light is enabled by default
void topaz_renderer_light_enable(
    /// The light to enable.
    topazRenderer_Light_t * light, 

    /// The state of the light.
    int doIt
);


















/// Renderer programs allow for a bridge to 
/// platform-dependent programmable graphics.
/// Because they are managed by the platform, the 
/// language is backend-dependent for 
typedef struct topazRenderer_Program_t topazRenderer_Program_t;




typedef enum topazRenderer_PresetProgram topazRenderer_PresetProgram;
/// Preset programs are default contexts.
///
enum topazRenderer_PresetProgram {
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
};

// TODO: this should be changed to something more abstract. I.e. program_add_stage()
// which could add a vertex / fragment processing stage which are supported by 
// backends optionally.
/// Compiles and enacts a shader for use with Static Rendering. If the shader fails
/// to compile or is invalid in some way, the id returned is invalid. See StaticState.
/// In any case, log will be populated with information on the building process status.
topazRenderer_Program_t * topaz_renderer_program_create(
    /// The renderer to create the program with
    topazRenderer_t * renderer,

    /// Source for the custom vertex shader stage.
    const topazString_t * vertexSrc, 

    /// Source for the custom fragment shader stage.
    const topazString_t * fragSrc, 
    
    /// Output log. May be modified in case of error.
    topazString_t * log
);


// Returns a standard renderer program from built-in params.
topazRenderer_Program_t * topaz_renderer_program_get_preset(
    /// The renderer to query.
    topazRenderer_t * renderer,

    /// The preset program to retrieve.
    topazRenderer_PresetProgram preset
);


void topaz_renderer_program_destroy(
    /// The program to destroy.
    topazRenderer_Program_t * program
);







typedef struct topazRenderer_3D_Vertex_t topazRenderer_3D_Vertex_t;

/// Vertex for 3D rendering.
///
struct topazRenderer_3D_Vertex_t {
    /// Positional 3D coordinates: x
    float x;
    /// Positional 3D coordinates: x
    float y; 
    /// Positional 3D coordinates: z
    float z;

    /// Normal vector, normalized: x
    float normalX; 
    /// Normal vector, normalized: y
    float normalY; 
    /// Normal vector, normalized: z
    float normalZ;

    /// Texture UVs: x / u [0, 1]
    float texX;
    /// Texture UVs: y / v [0, 1]
    float texY;

    /// User-define data (for programs)
    float userDefinedData[4];

};






/// Holds the results of rendering operations
typedef struct topazRenderer_Framebuffer_t topazRenderer_Framebuffer_t;



/// Types refer to the internal class by which visual information is
/// passed to the Display.
///
typedef enum topazRenderer_Framebuffer_Handle topazRenderer_Framebuffer_Handle;
enum topazRenderer_Framebuffer_Handle {
    /// (uint8_t *) RGBA-ordered pixel array with no padding, matched to the dimensions given
    topazRenderer_Framebuffer_Handle_RGBA_PixelArray, 
    /// (GLuint[2])  A pointer to a pointer to an array of to GLuints, the first representing the framebuffer object, the next representing the texture attached to that target.
    topazRenderer_Framebuffer_Handle_GLFBPacket,      
    /// The framebuffer's data contents are unknown and should not be relied on.
    topazRenderer_Framebuffer_Handle_Unknown,         
};




/// Creates a new framebuffer for the renderer. Framebuffers hold 
/// results of drawing operations from the renderer.
///
topazRenderer_Framebuffer_t * topaz_renderer_framebuffer_create(
    /// The renderer to create the framebuffer for.
    topazRenderer_t * renderer
);


/// Destroys a framebuffer.
///
void topaz_renderer_framebuffer_destroy(
    /// The framebuffer to destroy.
    topazRenderer_Framebuffer_t * fb
);




/// Resizes the framebuffer. Success is returned.
///
int topaz_renderer_framebuffer_resize(
    /// The framebuffer to resize.
    topazRenderer_Framebuffer_t * fb, 
    /// The new width of the framebuffer.
    int newW, 
    /// The new height of the framebuffer.
    int newH
);


/// Returns the width of the framebuffer.
///
int topaz_renderer_framebuffer_get_width(
    /// The framebuffer to query.
    topazRenderer_Framebuffer_t * fb
);

/// Returns the height of the framebuffer.
///
int topaz_renderer_framebuffer_get_height(
    /// The framebuffer to query.
    topazRenderer_Framebuffer_t * fb
);


/// Returns what type the handle refers to
///
topazRenderer_Framebuffer_Handle topaz_renderer_framebuffer_get_handle_type(
    /// The framebuffer to query.
    topazRenderer_Framebuffer_t * fb
);


/// Returns the source data that reflects the framebuffer's data
/// in the context of the implemented child. For example, on an OpenGL-variant
/// backend, the handle is likely an OpenGL texture object id.
/// This allows things like topazDisplay_t to utilize the rendered data.
///
void * topaz_renderer_framebuffer_get_handle(
    /// The framebuffer to query.
    topazRenderer_Framebuffer_t * fb
);




/// returns a RGBF pixel reduction by setting the
/// buffer given. if this isnt possible, false is returned.
/// the buffer should be of size Width*Height*4. Note that on
/// hardware-accelerated implementations, calling this could be very costly.
/// Alpha color information is always 1.f
int topaz_renderer_framebuffer_get_raw_data(
    /// The framebuffer to query.
    topazRenderer_Framebuffer_t * fb, 
    /// The buffer to populate with the framebuffer's current contents.
    uint8_t * buffer
);

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
/// The default is true. This does not affect the retrieval of pixels from 
/// topaz_renderer_framebuffer_get_raw_data().
void topaz_renderer_framebuffer_set_filtered_hint(
    /// The framebuffer to modify.
    topazRenderer_Framebuffer_t * fb, 

    /// whether to filter.
    int filter
);

/// \brief Returns whether to interpret the Framebuffer's data in a filtered
/// way.
///
/// See set_filtered_hint().
int topaz_renderer_framebuffer_get_filtered_hint(
    /// The framebuffer to query.
    topazRenderer_Framebuffer_t * fb
);










typedef struct topazRenderer_3D_t topazRenderer_3D_t;
/// A 3D rendering object 
///
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
    /// each program.
    topazArray_t * textureSlots;

    //// The texture objects to use. Each index has a corresponding
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
    /// The backend to use to create the renderer.
    topazSystem_Backend_t * backend, 

    /// The API for the renderer.
    topazRendererAPI_t api
);


/// Destroys and cleans up a renderer
///
void topaz_renderer_destroy(
    /// The renderer to destroy.
    topazRenderer_t * renderer
);





/// Retrieves the backend for this time object.
///
topazSystem_Backend_t * topaz_renderer_get_backend(
    /// The renderer to query.
    topazRenderer_t * renderer
);



/// Returns the API for this time.
///
topazRendererAPI_t topaz_renderer_get_api(
    /// The renderer to query.
    topazRenderer_t * renderer
);





/// Unlike 2D requests, 3D render execution times are entirely up
/// to the renderer and backend. The only guarantee is that the object will render
/// on the committed display. The benefit of this mode is utilizing custom graphics programs
/// and the ability to utilize specialized renderer storage, which will almost always
/// yield better performance for static (3D) rendering over dynamic (2D) rendering. Vertices
/// here are transformed by the renderer.
void topaz_renderer_draw_3d(
    /// The renderer to use.
    topazRenderer_t * renderer,

    /// The 3D instance to render.
    topazRenderer_3D_t * inst,

    /// The processing attributes for rendering the object.
    const topazRenderer_ProcessAttribs_t * attribs
);



/// Renders all queued objects and their vertices.
///
void topaz_renderer_draw_2d(
    /// The renderer to use.
    topazRenderer_t * renderer,

    /// The 2D renderer instance to use. This will have 
    /// queued data to render.
    topazRenderer_2D_t * inst,

    /// The context for the 2D instance.
    const topazRenderer_2D_Context_t * context,

    /// Further processing details for rendering the 2D instance.
    const topazRenderer_ProcessAttribs_t * attribs

);


/// Sets the 3D viewing matrix buffer. The buffer should be 
/// a 4x4 matrix representing the translation, scale, etc of the view.
/// Row-major matrices are expected.
///
void topaz_renderer_set_3d_viewing_matrix(
    /// The renderer to modify.
    topazRenderer_t * renderer, 

    /// The viewing matrix to use.
    topazRenderer_Buffer_t * transform
);

/// Sets the 3D projection matrix buffer. The buffer should be 
/// a 4x4 matrix representing this matrix.
/// Row-major matrices are expected.
///
void topaz_renderer_set_3d_projection_matrix(
    /// The renderer to modify.
    topazRenderer_t * renderer, 

    /// The viewing matrix to use.
    topazRenderer_Buffer_t * transform
);



/// Returns the 3D viewing matrix buffer currently set. 
/// The default is "none".
///
topazRenderer_Buffer_t * topaz_renderer_get_3d_viewing_matrix(
    /// The renderer to query.
    topazRenderer_t * renderer
);

/// Returns the 3D projection matrix.
/// The default is "none".
topazRenderer_Buffer_t * topaz_renderer_get_3d_projection_matrix(
    /// The renderer to query.
    topazRenderer_t * renderer
);







    
/// Resets a specific part of the renderer's render space. This is often useful for 
/// complex scenes.
/// The individual datalayers may be optionally ORd 
///
void topaz_renderer_clear_layer(
    /// The renderer to clear.
    topazRenderer_t * renderer, 

    /// The layer of the attached framebuffer to clear.
    topazRenderer_DataLayer layer
);

/// Gets logistical information about the renderer.
/// Some backends may not given meanginful information here.
///
topazRenderer_Parameters_t topaz_renderer_get_parameters(
    /// The renderer to query.
    topazRenderer_t * renderer
);









/// Ensures that the all rendering operations are reflected in the attached target.
///
void topaz_renderer_sync(
    /// The renderer to sync.
    topazRenderer_t * renderer
);

/// The passed framebuffer becomes the destination for all future renderings
/// until a different valid framebuffer is given. If NULL is passed, rendering 
//// will have no effect.
///
void topaz_renderer_attach_target(
    topazRenderer_t * renderer, 
    topazRenderer_Framebuffer_t * fb
);


/// Returns the current target for renderings. The default is
/// NULL.
///
topazRenderer_Framebuffer_t * topaz_renderer_get_target(
    /// the renderer to query.
    topazRenderer_t * renderer
);






#endif
