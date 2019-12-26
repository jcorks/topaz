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


#ifndef H_TOPAZDC__RENDER2D__INCLUDED
#define H_TOPAZDC__RENDER2D__INCLUDED

#include <topaz/containers/string.h>
#include <topaz/containers/array.h>

/*

    Render2D
    -----
    Base component to render a 2D object.
    

*/
typedef struct topazRender2D_t topazRender2D_t;

/// A RenderMode specifies how the aspect should be expressed.
///
typedef enum {

    /// This is the default. Colors are interpreted as you would normally expect
    /// with the alpha channle being expressed as a transparency value.
    ///
    topazRender2D_Mode_Normal, 

    /// Ignores transparency overall.        
    ///
    topazRender2D_Mode_Opaque, 

    /// Translucency in Dynacoe is a way of expressing transparency. When an Aspect is
    /// translucent, the aspect's colors are added to whatever is drawn underneath it.
    /// This is referred to in some places as additive blending.
    ///
    topazRender2D_Mode_Translucent, 

    /// If an aspect's render mode is None, it will not be drawn.
    ///
    topazRender2D_Mode_None, 

} topazRender2D_Mode;



typedef enum {

    /// Ignores etching.
    ///
    topazRender2D_EtchMode_Ignore,   

    /// Defines the etchable region, but with no visual
    ///
    topazRender2D_EtchMode_Define,   

    /// Undefines a previously defined region
    ///
    topazRender2D_EtchMode_Undefine, 

    /// Draws only on regions with the etch defined
    ///
    topazRender2D_EtchMode_In,       

    /// Draws only on regions with the etch not defined. THis is the default
    ///
    topazRender2D_EtchMode_Out,      

} topazRender2D_EtchMode;



topazRender2D_t * topaz_render2d_create()




class Render2D : public Component, public Spatial {
  public:

    /// \brief A RenderMode specifies how the aspect should be expressed.
    ///
    enum class RenderMode {
    };



    /// \brief  The visual mode for rendered vertices
    ///
    /// See Renderer.h
    RenderMode mode;


    /// \brief The etch mode for rendered vertices
    ///
    /// See Renderer.h
    EtchMode etch;


    /// \brief Sets whether drawing calculations from the node should be interpreted as absolute 
    /// pixel values on the display or should be within the hierarchy. The default is false.
    ///
    void SetAbsolute(bool);

    /// \brief Gets the absolute field. See SetAbsolute().
    ///
    bool GetAbsolute() const;

    /// \brief Returns the raw vertices compiled for the renderable object 
    ///
    /// See Renderer.h
    std::vector<Renderer::Vertex2D> GetVertices() const;    
    
    uint32_t GetObjectID() const;
    
    const std::vector<uint32_t> & GetVertexIDs() const;
    
    /// \brief Returns the polygon to be used when drawing the vertices 
    ///
    /// See Renderer.h
    Renderer::Polygon GetPolygon() const;
    ~Render2D();

  protected:
    Render2D(const std::string &);

    void SetVertices(const std::vector<Renderer::Vertex2D> &);
    void SetPolygon(Renderer::Polygon);
    void OnUpdateTransform();

  private:
     std::vector<uint32_t> vertexSrc;
     int objectID;
     bool absolute;
     Renderer::Polygon polygon;
};

#endif