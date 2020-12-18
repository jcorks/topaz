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


#ifndef H_TOPAZDC__MESH__INCLUDED
#define H_TOPAZDC__MESH__INCLUDED


#include <stdint.h>
#include <topaz/backends/renderer.h>
typedef struct topaz_t topaz_t;



///
///    Mesh
///    -----
///
///    3D object defined by triangle primitives.
///    A Mesh contains 2 components: the Mesh itself and a series of MeshObject s.
///    The Mesh itself provides the base data: vertex positions, normals,
///    UVs, you name it. A MeshObject contains the actual application of that source data, expressing what
///    vertices to use, the material to render the vertices with, etc.
///    Using the Mesh - MeshObject reloationship aptly can allow for flexible
///    control over performance vs. memory cost.
///
///
///
///
typedef struct topazMesh_t topazMesh_t;



/// The type of vertex attribute.
///
typedef enum topazMesh_VertexAttribute topazMesh_VertexAttribute;
enum topazMesh_VertexAttribute {
    ///  Position of the vertex. 3 components: xyz.
    ///
    topazMesh_VertexAttribute_Position, 


    /// The normal vector of the vertex. 3 components: xyz.
    ///
    topazMesh_VertexAttribute_Normal,   

    /// The texture cooridates of the vertex. 2 components: xy.
    ///
    topazMesh_VertexAttribute_UV,       

    /// User-defined data. 4 components: xyz.
    ///
    topazMesh_VertexAttribute_UserData  

};



/// Creates a new, empty mesh.
///
topazMesh_t * topaz_mesh_create(topaz_t *);

/// Creates a new mesh instance of a mesh as an exact copy.
/// This clones the vertex data as well.
///
topazMesh_t * topaz_mesh_clone(const topazMesh_t *);

/// Creates a nice instance of a mesh, but shares the vertex data 
/// with the source instance. Any edits to the vertex data will 
/// be reflected in all meshes that share the same vertex data.
///
topazMesh_t * topaz_mesh_clone_shared(const topazMesh_t *);

/// Returns source mesh instance that the vertex data belongs to
/// If the mesh was created in a non-shared way, the instance returned 
/// will match the object itself.
///
topazMesh_t * topaz_mesh_get_shared_source(topazMesh_t *);



/////////////// Vertex modification


/// Returns the number of vertices that the mesh contains.
///
void topaz_mesh_set_vertex_count(topazMesh_t *, uint32_t);

/// Sets the raw vertices for the mesh. This also sets the vertex count.
/// The array type is topazRenderer_3D_Vertex_t
///
void topaz_mesh_define_vertices(topazMesh_t *, const topazArray_t *);


/// Gets data from a specific vertex. If the 
/// vertex doesnt exist, a placeholder value is returned.
/// This value is valid until the next call of the function 
/// with the same mesh.
///
const float * topaz_mesh_get_vertex(
    const topazMesh_t *, 
    topazMesh_VertexAttribute, 
    uint32_t index
);

/// Gets data from a specific vertex. If the 
/// vertex doesnt exist, nothing happens
///
void topaz_mesh_set_vertex(
    topazMesh_t *,
    topazMesh_VertexAttribute, 
    uint32_t index,
    const float *
);

/// Returns the number of vertex within this mesh.
///
uint32_t topaz_mesh_get_vertex_count(const topazMesh_t *);



//////////// Object control

/// An object is a collection of faces 
/// specified with indices to vertices. For mesh objects, these will be 
/// indices in groups of 3.



/// Adds a new object.  This function creates a new instance
/// and its index is returned. This index can be used with topaz_mesh_get_object 
/// to modify and read from.
///
uint32_t topaz_mesh_add_object(topazMesh_t *);

/// Gets the array of face indices for the corresponding index.
///
topazArray_t * topaz_mesh_get_object(topazMesh_t *, uint32_t index);

/// Removes the object specified. No action is taken if the index 
/// doesn't exist.
///
void topaz_mesh_remove_object(topazMesh_t *, uint32_t);

/// Gets the number of objects that the mesh object holds.
///
uint32_t topaz_mesh_get_object_count(const topazMesh_t *);

/// Returns the raw buffer for all vertices. See renderer.h
///
topazRenderer_Buffer_t * topaz_mesh_get_vertex_data(topazMesh_t *);



/// Creates a new basic cube mesh.
///
topazMesh_t * topaz_mesh_create_cube(topaz_t *);

/// Creates a new basic square mesh.
///
topazMesh_t * topaz_mesh_create_square(topaz_t *);




#endif
