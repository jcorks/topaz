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
typedef struct topazAsset_t topazAsset_t;
typedef struct topaz_t topaz_t;



///
/// 3D object defined by triangle primitives.
///
/// A mesh contains 2 components: the mesh itself and a series of objects.
/// The Mesh itself provides the base data: vertex positions, normals,
/// UVs, you name it. A MeshObject contains the actual application of that source data, expressing what
/// vertices to use, the material to render the vertices with, etc.
/// Using the Mesh - MeshObject reloationship aptly can allow for flexible
/// control over performance vs. memory cost.
///
/// An object is a collection of faces 
/// specified with indices to vertices. For mesh objects, these will be 
/// indices in groups of 3.
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
topazAsset_t * topaz_mesh_create(
    topaz_t *, 
    const topazString_t *
);





/// Returns the number of vertices that the mesh contains.
///
void topaz_mesh_set_vertex_count(
    /// The mesh to modify
    topazAsset_t * mesh, 
    /// The new vertex count
    uint32_t newCount
);

/// Sets the raw vertices for the mesh. This also sets the vertex count.
/// The array type is topazRenderer_3D_Vertex_t
///
void topaz_mesh_define_vertices(
    /// The mesh to modify.
    topazAsset_t * mesh, 

    /// The new vertices to be copied into the mesh.
    const topazArray_t * vertices
);


/// Gets data from a specific vertex. If the 
/// vertex doesnt exist, a placeholder value is returned.
/// This value is valid until the next call of the function 
/// with the same mesh.
///
const float * topaz_mesh_get_vertex(
    /// The mesh to query.
    topazAsset_t * mesh, 

    /// Which vertex attribute to retrieve.
    topazMesh_VertexAttribute attribute, 

    /// The vertex index.
    uint32_t index
);

/// Gets data from a specific vertex. If the 
/// vertex doesnt exist, nothing happens
///
void topaz_mesh_set_vertex(
    /// The mesh to modify.
    topazAsset_t * mesh,

    /// The attribute to modify
    topazMesh_VertexAttribute attribute, 

    /// The vertex index to modify.
    uint32_t index,

    /// The raw data to copy.
    const float * data
);

/// Returns the number of vertex within this mesh.
///
uint32_t topaz_mesh_get_vertex_count(
    /// The mesh to query.
    topazAsset_t * mesh
);







/// Adds a new object.  This function creates a new instance
/// and its index is returned. This index can be used with topaz_mesh_get_object 
/// to modify and read from.
///
uint32_t topaz_mesh_add_object(
    /// The mesh to add an object to.
    topazAsset_t * mesh
);

/// Gets the array of face indices for the corresponding index.
///
topazArray_t * topaz_mesh_get_object(
    /// The mesh to retrieve an object from.
    topazAsset_t * mesh, 

    /// The object index.
    uint32_t index
);

/// Removes the object specified. No action is taken if the index 
/// doesn't exist.
///
void topaz_mesh_remove_object(
    /// The mesh to remove an object from.
    topazAsset_t * mesh, 

    /// The index of the object to remove.
    uint32_t index
);

/// Gets the number of objects that the mesh object holds.
///
uint32_t topaz_mesh_get_object_count(
    /// The mesh to query.
    topazAsset_t * mesh
);



void topaz_mesh_query(
    topazAsset_t * a, 
    topazRenderer_Buffer_t ** v,
    topazArray_t ** ind
);





#endif
