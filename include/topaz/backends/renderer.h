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

#include <topaz/backends/api/time_api.h>
#include <topaz/backends/backend.h>



#define MINIMUM_LIGHT_COUNT             32
#define MINIMUM_TEXTURE_BINDING_COUNT   32

/*

    Renderer
    -----
    An abstraction for millisecond resolution timing.
    This short set of utilities allows for timing 

*/

typedef struct topazRender_t topazRender_t;




/// Creates a new time object
///
topazRenderer_t * topaz_renderer_create(topazBackend_t *, topazRendererAPI_t);


/// Destroys and cleans up a time API
///
void topaz_renderer_destroy(topazRenderer_t *);





/// Retrieves the backend for this time object.
///
topazBackend_t * topaz_renderer_get_backend(topazRenderer_t *);


/// Returns the API for this time.
///
topazRendererAPI_t topaz_renderer_get_api(topazRenderer_t *);





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
/// Returns the textureIndex. Passing a nullptr as the rgbaTextureData
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


// Returns a string denoting the name of the shading language supported.
// If shaders are not supported, an empty string is returned.
std::string ProgramGetLanguage() = 0;

// Compiles and enacts a shader for use with Static Rendering. If the shader fails
// to compile or is invalid in some way, the id returned is invalid. See StaticState.
// In any case, log will be populated with information on the building process status.
ProgramID ProgramAdd(const std::string & vertexSrc, const std::string & fragSrc, std::string & log) = 0;

// Returns the id referring to a builtin shader.
ProgramID ProgramGetBuiltIn(BuiltInShaderMode) = 0;






/*

    RendererLight
    -----
    TODO: change to RendererProgramData as a generic 
    buffer, which is more accurate and usable.

*/


// Creates a new light. When the light is first created, the state of the light
// is undefined. (It is expected that you will update the lights attributes
// before drawing);
LightID AddLight(LightType) = 0;

// As the default, the attributes requred are
//  Position (3-components)
//  Color (3-components)
//  Intensity (1 component)
// Thus, the renderer expects the array passed to have at least 7 components.

void UpdateLightAttributes(LightID, float *) = 0;


// Enables or diables the light by default, once added, the light is enabled by default
void EnableLight(LightID, bool doIt) = 0;

// Removes all resources associated with the light ID.
void RemoveLight(LightID) = 0;

// returns the maximum number of lights supported by the renderer.
int MaxEnabledLights() = 0;

// returns the number of lightx currently in use.
int NumLights() = 0;








/*
    RendererState2D
    ----
    // The general use case for 2D geometry is we will draw many 
    // small objects with unique tranform properties. Thus, a stati crendering 
    // approach is less favorable as that would lead to more frequent draws.
    // Render2DVertices, using user-maintained global vertices, is a more 
    // performant option for drawing specifically 2D vertices.

*/

// Struct representing a dynamic vertex state.
// Dynamic vertices do not support lighting.
struct Vertex2D {
    Vertex2D(){}
    Vertex2D(float x_, float y_,
                  float r_, float g_, float b_, float a_,
                  float tex, float tx, float ty)
        : x(x_), y(y_),
          r(r_), g(g_), b(b_), a(a_), useTex(tex), texX(tx), texY(ty) {}
    Vertex2D(float x_, float y_,
                  float r_, float g_, float b_, float a_)
        : x(x_), y(y_), 
          r(r_), g(g_), b(b_), a(a_), useTex(-1) {}

    Vertex2D(float x_, float y_,
                  float tex, float tx, float ty)
        : x(x_), y(y_),
          r(1.f), g(1.f), b(1.f), a(1.f),
          useTex(tex), texX(tx), texY(ty) {}
    float x, y;                           // vertex position
    float r, g, b, a; // color, scale from 0.f to 1.f (red, green, blue, and alpha)
    float texX, texY;                     // texture coordinates (0, 0 is topleft)
    float useTex;                         // if not used, set to -1, else float form of texture id
    float object;                         // the transform reference object
};

struct Render2DStaticParameters {
    float contextWidth;
    float contextHeight;
    EtchRule etchRule;        
    float * contextTransform;

    bool operator==(const Render2DStaticParameters & other) {
        return contextWidth == other.contextWidth &&
               contextHeight == other.contextHeight &&
               contextTransform[0] == other.contextTransform[0] &&
               contextTransform[1] == other.contextTransform[1] &&
               contextTransform[2] == other.contextTransform[2] &&
               contextTransform[3] == other.contextTransform[3] &&

               contextTransform[4] == other.contextTransform[4] &&
               contextTransform[5] == other.contextTransform[5] &&
               contextTransform[6] == other.contextTransform[6] &&
               contextTransform[7] == other.contextTransform[7] &&

               contextTransform[8] == other.contextTransform[8] &&
               contextTransform[9] == other.contextTransform[9] &&
               contextTransform[10] == other.contextTransform[10] &&
               contextTransform[11] == other.contextTransform[11] &&

               contextTransform[12] == other.contextTransform[12] &&
               contextTransform[13] == other.contextTransform[13] &&
               contextTransform[14] == other.contextTransform[14] &&
               contextTransform[15] == other.contextTransform[15];
    }

};

struct Render2DObjectParameters {
    
    // transform
    float data[16];
    
};


void Queue2DVertices(
    const uint32_t * objects,
    uint32_t count
) = 0;

// Adds a new 2D object and returns its ID.
uint32_t Add2DObject() = 0;

// Removes a 2D object of the given ID
void Remove2DObject(uint32_t) = 0;

// Adds a new vertex 
uint32_t Add2DVertex() = 0;

void Remove2DVertex(uint32_t object) = 0;

void Set2DVertex(uint32_t vertex, Vertex2D) = 0;

Vertex2D Get2DVertex(uint32_t vertex) = 0;

void Set2DObjectParameters(uint32_t object, Render2DObjectParameters) = 0;





/*

    RendererState3D

*/
struct StaticState {
    
    // Vertices points to a renderbuffer containing all the vertex dat  a pertinent to the RenderObject.
    // Each vertex consists of:
    //      3-components for position,
    //      3-components for the normal.
    //      2-copmonents for UVs (texture coordinates)
    //      4-components for user-defined data. Meant to be unitlized with custom programs.
    topazRenderer_Buffer_t * vertices;
    
    // Specifies how to render the vertices
    ProgramID program;

    // material matrix , 44-components containing
    // lighting material and arbitrary data setup.
    // The layout is as follows:
    topazRenderer_Buffer_t * materialData;  

    // local transform matrix (scale + rotation), 32-component
    // Normal matrix (inverse transpose of modelData)
    topazRenderer_Buffer_t * modelData;  

    // the source framebuffer optionally accessible 
    // during rendering. If the samplebuffer is null,
    // the no source framebuffer will be made available
    Framebuffer * samplebuffer;


    // specifies the textures to be used. Each pair is a slot referred to by 
    // each program and the ID of the texture.
    std::vector<std::pair<int, int>> * textures;


    /* Static index list */
    // Index lists are used to build the geometry of the StaticState.
    // Each index refer to the index of the vertex to be used to build the triangle.
    // every 3 vertices, a triangle is formed. If a multiple of three is not used,
    // the behavior is implementation defined.
    std::vector<uint32_t> * indices;
    


};



class Renderer : public Backend {
  public:




    // For use with StaticState. See StaticState.h
    struct StaticVertex {
        StaticVertex(){}
        StaticVertex(float x_, float y_, float z_) :
            x(x_), y(y_), z(z_),
            normalX(0), normalY(0), normalZ(0),
            texX(0), texY(0),
            userDefinedData{0, 0, 0, 0}
        {}

        StaticVertex(
            float x_, float y_, float z_,
            float normalX_, float normalY_, float normalZ_,
            float texX_, float texY_,
            float userdata0,
            float userdata1,
            float userdata2,
            float userdata3
        ) :
            x(x_), y(y_), z(z_),
            normalX(normalX_), normalY(normalY_), normalZ(normalZ_),
            texX(texX_), texY(texY_),
            userDefinedData{
                userdata0,
                userdata1,
                userdata2,
                userdata3
            }
        {}


        float x, y, z;
        float normalX, normalY, normalZ;
        float texX, texY;
        float userDefinedData[4];
    };


    











    void Render2DVertices(const Render2DStaticParameters &) = 0;

    // Clears all requests queued before the last RenderDynamicQueue
    void Clear2DQueue() = 0;






    // Unlike Dynamic requests, RenderStatic render execution times are entirely up
    // to the renderer and backend. The only guarantee is that the object will render
    // on the committed display. The benefit of this mode is utilizing advanced shaders
    // and the ability to utilize specialized renderer storage, which will almost always
    // yield better performance for static rendering over dynamic rendering. Vertices
    // here are transformed by the renderer.
    void RenderStatic(StaticState *) = 0;

    // Resets the renderer's render space to an initial state.
    void ClearRenderedData() = 0;

    
    // Resets a specific part of the renderer's render space. This is often useful for 
    // complex scenes.
    void Reset(DataLayer) = 0;

    // Returns the transformation matrix buffer IDs for static rendering.
    // All RenderStatic positional vertices are multiplied by the Viewing and
    // projection matrices respectively. Once determined by the renderer,
    // these ID's values will not change per instance.
    topazRenderer_Buffer_t * GetStaticViewingMatrixID() = 0;
    topazRenderer_Buffer_t * GetStaticProjectionMatrixID() = 0;







    // Returns the maximum number of textures that can be referred to
    // by a StaticObject when rendering.
    int MaxSimultaneousTextures() = 0;


    // sets/ gets the stretching filter to be used during the next request resolution
    void SetTextureFilter(TexFilter) = 0;
    TexFilter GetTextureFilter() = 0;




















    /* Display management */

    // Modifes how all following drawn geometry is handled.
    void SetDrawingMode (
        Polygon,
        DepthTest,
        AlphaRule
    ) = 0;

    // Retrieves how all the geometry will be drawn
    void GetDrawingMode (
        Polygon * polygon,
        DepthTest * dimension,
        AlphaRule * alhaRule
    ) = 0;


    // Ensures that the all rendering operations are reflected in the attached target.
    void Sync() {};

    // The passed framebuffer becomes the destination for all future renderings
    // until a different valid framebuffer is given. If the Type of the given
    // Framebuffer is not one of the types from SupportedFramebuffers, no
    // action is taken. If nullptr is passed, rendering will have no effect.
    void AttachTarget(Framebuffer *) = 0;


    // Returns the current target for renderings. The default is
    // nullptr.
    Dynacoe::Framebuffer * GetTarget() = 0;


    // Returns the framebuffer types that this renderer supports.
    // Should the framebuffer not match one of the given types, the framebuffer
    // attachment will fail
    std::vector<Dynacoe::Framebuffer::Type> SupportedFramebuffers() = 0;
};



