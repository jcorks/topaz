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


#ifndef H_TOPAZDC__RBUFFER__INCLUDED
#define H_TOPAZDC__RBUFFER__INCLUDED


#include <stdint.h>
#include <topaz/containers/array.h>
#include <topaz/containers/string.h>



/// The type of vertex attribute.
///
typedef enum {
    ///  Position of the vertex. 3 components: xyz.
    ///
    topazMesh_VertexAttribute_Position, 


    /// The normal vector of the vertex. 3 components: xyz.
    ///
    topazMesh_VertexAttribute_Normal,   

    /// The texture cooridates of the vertex. 2 components: xy.
    ///
    topazMesh_VertexAttribute_UV,       

    /// User-defined data. 3 components: xyz.
    ///
    topazMesh_VertexAttribute_UserData  

} topazMesh_VertexAttribute;


/*

    Mesh
    -----

    3D object defined by triangle primitives.
    A Mesh contains 2 components: the Mesh itself and a series of MeshObject s.
    The Mesh itself provides the base data: vertex positions, normals,
    UVs, you name it. A MeshObject contains the actual application of that source data, expressing what
    vertices to use, the material to render the vertices with, etc.
    Using the Mesh - MeshObject reloationship aptly can allow for flexible
    control over performance vs. memory cost.



*/
typedef struct topazMesh_t topazMesh_t;

/// Creates a new, empty input buffer 
///
topazMesh_t * topaz_mesh_create();


topazMesh_t * topaz_mesh_clone(const topazMesh_t *);


topazMesh_t * topaz_mesh_clone_shallow(const topazMesh_t *);





void topaz_mesh_set_vertex_count(topazMesh_t *, uint32_t);

/// Also sets the vertex count.
/// array type is topazRenderer_3D_Vertex_t
void topaz_mesh_define_vertices(topazMesh_t *, const topazArray_t *);



topazVector_t topaz_mesh_get_vertex(
    topazMesh_t *, 
    topazMesh_VertexAttribute, 
    uint32_t index
) {
    
}





