#include <topaz/compat.h>
#include <topaz/matrix.h>
#include <topaz/math.h>
#include <topaz/topaz.h>
#include <topaz/asset.h>
#include <topaz/assets/mesh.h>
#include <topaz/modules/graphics.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
static char * MAGIC_ID__MESH = "t0p4zm3sh";
#endif


struct topazMesh_t {
    #ifdef TOPAZDC_DEBUG
    char * MAGIC_ID;
    #endif
    topaz_t * ctx;
    topazArray_t * objs;
    topazRenderer_Buffer_t * v;
};



#define TOPAZ_V_TO_FLOATS(__N__) ((sizeof(topazRenderer_3D_Vertex_t) / sizeof(float))*(__N__))
#define TOPAZ_FLOATS_TO_V(__N__) ((__N__) / sizeof(topazRenderer_3D_Vertex_t))




static void mesh__destroy(topazAsset_t * a, void * userData) {
    topazMesh_t * m = userData;
    if (m->v)
        topaz_renderer_buffer_destroy(m->v);

    uint32_t i;
    uint32_t len = topaz_array_get_size(m->objs);
    for(i = 0; i < len; ++i) {
        topazArray_t * arr = topaz_array_at(m->objs, topazArray_t *, i);
        topaz_array_destroy(arr);    
    }
    topaz_array_destroy(m->objs);
    free(m);


}

static topazMesh_t * mesh__retrieve(topazAsset_t * a) {
    topazMesh_t * m = topaz_asset_get_attributes(a)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(m->MAGIC_ID == MAGIC_ID__MESH);
    #endif
    return m;
}

topazAsset_t * topaz_mesh_create(topaz_t * t, const topazString_t * name) {
    topazMesh_t * out = calloc(1, sizeof(topazMesh_t));
    out->ctx = t;
    out->objs = topaz_array_create(sizeof(topazArray_t *));
    out->v = NULL;
    #ifdef TOPAZDC_DEBUG 
        out->MAGIC_ID = MAGIC_ID__MESH;
    #endif

    topazAsset_Attributes_t attribs = {};
    attribs.on_destroy = mesh__destroy;
    attribs.userData = out;

    return topaz_asset_create(
        t,
        topazAsset_Type_Mesh,
        name, 
        &attribs
    );
}





void topaz_mesh_set_vertex_count(topazAsset_t * a, uint32_t i) {
    topazMesh_t * m = mesh__retrieve(a);
    if (m->v)
        topaz_renderer_buffer_destroy(m->v);

    m->v = topaz_renderer_buffer_create(
        topaz_graphics_get_renderer(topaz_context_get_graphics(m->ctx)),
        NULL,
        TOPAZ_V_TO_FLOATS(i)
    );
}

void topaz_mesh_define_vertices(topazAsset_t * a, const topazArray_t * s) {
    topazMesh_t * m = mesh__retrieve(a);
    if (m->v)
        topaz_renderer_buffer_destroy(m->v);
    
    m->v = topaz_renderer_buffer_create(
        topaz_graphics_get_renderer(topaz_context_get_graphics(m->ctx)),
        topaz_array_get_data(s),
        TOPAZ_V_TO_FLOATS(topaz_array_get_size(s))
    );
}


/// Gets data from a specific vertex. If the 
/// vertex doesnt exist, nothing is returned.
///
topazRenderer_3D_Vertex_t topaz_mesh_get_vertex(
    topazAsset_t * a, 
    uint32_t index
) {
    topazMesh_t * m = mesh__retrieve(a);

    topazRenderer_3D_Vertex_t vtx = {};
    if (!m->v) return vtx;

    uint32_t count = TOPAZ_FLOATS_TO_V(topaz_renderer_buffer_get_size(m->v));

    if (index >= count) {
        return vtx;
    }

    uint32_t baseFloat = TOPAZ_V_TO_FLOATS(index);

    topaz_renderer_buffer_read(
        m->v,
        (float*)&vtx,
        baseFloat,
        TOPAZ_V_TO_FLOATS(1)
    );

    return vtx;
}



void topaz_mesh_set_vertex(
    topazAsset_t * a,
    uint32_t index,
    topazRenderer_3D_Vertex_t vertex
) {
    topazMesh_t * m = mesh__retrieve(a);
    if (!m->v) return;
    uint32_t count = TOPAZ_FLOATS_TO_V(topaz_renderer_buffer_get_size(m->v));
    if (index >= count) {
        return;
    }


    uint32_t baseFloat = TOPAZ_V_TO_FLOATS(index);

    topaz_renderer_buffer_update(
        m->v,
        (float*)&vertex,
        baseFloat,
        TOPAZ_V_TO_FLOATS(1)
    );
}

uint32_t topaz_mesh_get_vertex_count(topazAsset_t * a) {
    topazMesh_t * m = mesh__retrieve(a);
    if (!m->v) return 0;
    return TOPAZ_FLOATS_TO_V(topaz_renderer_buffer_get_size(m->v));
}


uint32_t topaz_mesh_add_object(topazAsset_t * a) {
    topazMesh_t * m = mesh__retrieve(a);
    topazArray_t * obj = topaz_array_create(sizeof(uint32_t));
    topaz_array_push(m->objs, obj);
    return topaz_array_get_size(m->objs)-1;
}

const topazArray_t * topaz_mesh_get_object_indices(topazAsset_t * a, uint32_t index) {
    topazMesh_t * m = mesh__retrieve(a);
    if (index >= topaz_array_get_size(m->objs)) return NULL;
    return topaz_array_at(m->objs, topazArray_t *, index);
}

void topaz_mesh_set_object_indices(topazAsset_t * a, uint32_t index, const topazArray_t * arr) {
    topazMesh_t * m = mesh__retrieve(a);
    if (index >= topaz_array_get_size(m->objs)) return;
    topazArray_t * arrOld = topaz_array_at(m->objs, topazArray_t *, index);
    topaz_array_destroy(arrOld);
    topaz_array_at(m->objs, topazArray_t *, index) = topaz_array_clone(arr);
}


void topaz_mesh_remove_object(topazAsset_t * a, uint32_t index) {
    topazMesh_t * m = mesh__retrieve(a);
    if (index >= topaz_array_get_size(m->objs)) return;
    topaz_array_remove(m->objs, index);
}

uint32_t topaz_mesh_get_object_count(topazAsset_t * a) {
    topazMesh_t * m = mesh__retrieve(a);
    return topaz_array_get_size(m->objs);
}

topazRenderer_Buffer_t * topaz_mesh_get_vertex_data(topazAsset_t * a) {
    topazMesh_t * m = mesh__retrieve(a);
    return m->v;
}

void topaz_mesh_query(
    topazAsset_t * a, 
    topazRenderer_Buffer_t ** v,
    topazArray_t ** ind) {

    topazMesh_t * m = mesh__retrieve(a);
    *v = m->v;
    *ind = m->objs;
}





/*

static topazRenderer_3D_Vertex_t cube_vertices[] = {
    // back face
    {-0.5f, -0.5f, -0.5f,    0, 0, -1.f,     0.f, 0.f,   {0.f, 0.f, 0.f, 0.f}}, 
    { 0.5f, -0.5f, -0.5f,    0, 0, -1.f,     1.f, 0.f,   {0.f, 0.f, 0.f, 0.f}},
    {-0.5f,  0.5f, -0.5f,    0, 0, -1.f,     0.f, 1.f,   {0.f, 0.f, 0.f, 0.f}},
    { 0.5f,  0.5f, -0.5f,    0, 0, -1.f,     1.f, 1.f,   {0.f, 0.f, 0.f, 0.f}},

    // front face
    {-0.5f, -0.5f, 0.5f,     0, 0,  1.f,     0.f, 0.f,   {0.f, 0.f, 0.f, 0.f}},         
    { 0.5f, -0.5f, 0.5f,     0, 0,  1.f,     1.f, 0.f,   {0.f, 0.f, 0.f, 0.f}},    
    {-0.5f,  0.5f, 0.5f,     0, 0,  1.f,     0.f, 1.f,   {0.f, 0.f, 0.f, 0.f}},
    { 0.5f,  0.5f, 0.5f,     0, 0,  1.f,     1.f, 1.f,   {0.f, 0.f, 0.f, 0.f}},

    // right face
    {0.5f, -0.5f, -0.5f,    1.f, 0, 0,      0.f, 0.f,   {0.f, 0.f, 0.f, 0.f}},         
    {0.5f,  0.5f, -0.5f,    1.f, 0, 0,      1.f, 0.f,   {0.f, 0.f, 0.f, 0.f}},
    {0.5f, -0.5f,  0.5f,    1.f, 0, 0,      0.f, 1.f,   {0.f, 0.f, 0.f, 0.f}},
    {0.5f,  0.5f,  0.5f,    1.f, 0, 0,      1.f, 1.f,   {0.f, 0.f, 0.f, 0.f}},

    // left face
    {-0.5f, -0.5f, -0.5f,    -1.f, 0, 0,     0.f, 0.f,   {0.f, 0.f, 0.f, 0.f}},
    {-0.5f,  0.5f, -0.5f,    -1.f, 0, 0,     1.f, 0.f,   {0.f, 0.f, 0.f, 0.f}},
    {-0.5f, -0.5f,  0.5f,    -1.f, 0, 0,     0.f, 1.f,   {0.f, 0.f, 0.f, 0.f}},
    {-0.5f,  0.5f,  0.5f,    -1.f, 0, 0,     1.f, 1.f,   {0.f, 0.f, 0.f, 0.f}},

    // bottom face
    {-0.5f, -0.5f, -0.5f,    0, -1.f, 0,     0.f, 0.f,   {0.f, 0.f, 0.f, 0.f}},
    { 0.5f, -0.5f, -0.5f,    0, -1.f, 0,     1.f, 0.f,   {0.f, 0.f, 0.f, 0.f}},
    {-0.5f, -0.5f,  0.5f,    0, -1.f, 0,     0.f, 1.f,   {0.f, 0.f, 0.f, 0.f}},
    { 0.5f, -0.5f,  0.5f,    0, -1.f, 0,     1.f, 1.f,   {0.f, 0.f, 0.f, 0.f}},

    // top face
    {-0.5f,  0.5f, -0.5f,    0, 1.f, 0,      0.f, 0.f,   {0.f, 0.f, 0.f, 0.f}},
    { 0.5f,  0.5f, -0.5f,    0, 1.f, 0,      1.f, 0.f,   {0.f, 0.f, 0.f, 0.f}},
    {-0.5f,  0.5f,  0.5f,    0, 1.f, 0,      0.f, 1.f,   {0.f, 0.f, 0.f, 0.f}},
    { 0.5f,  0.5f,  0.5f,    0, 1.f, 0,      1.f, 1.f,   {0.f, 0.f, 0.f, 0.f}},

};

static uint32_t cube_facelist[] = {
    0,  1,  2,      2,  3,  1,
    4,  5,  6,      6,  7,  5,
    8,  9,  10,     10, 11, 9,
    12, 13, 14,     14, 15, 13,
    16, 17, 18,     18, 19, 17,
    20, 21, 22,     22, 23, 21
};

topazMesh_t * topaz_mesh_create_cube(topaz_t * ctx) {
    topazMesh_t * m = topaz_mesh_create(ctx);
    topaz_mesh_define_vertices(
        m,
        TOPAZ_ARRAY_CAST(cube_vertices, topazRenderer_3D_Vertex_t, 24)
    );

    topaz_mesh_add_object(m);
    topaz_array_push_n(
        topaz_mesh_get_object(m, 0),
        cube_facelist,
        3*6*2
    );

    return m;
}









static topazRenderer_3D_Vertex_t square_vertices[] = {
    {-0.5f, 0.f, -0.5f,      0.f, 1.f, 0.f,      0.f, 0.f,   {0.f, 0.f, 0.f, 0.f}},
    { 0.5f, 0.f, -0.5f,      0.f, 1.f, 0.f,      1.f, 0.f,   {0.f, 0.f, 0.f, 0.f}},
    { 0.5f, 0.f,  0.5f,      0.f, 1.f, 0.f,      1.f, 1.f,   {0.f, 0.f, 0.f, 0.f}},
    {-0.5f, 0.f,  0.5f,      0.f, 1.f, 0.f,      0.f, 1.f,   {0.f, 0.f, 0.f, 0.f}}
};

static uint32_t square_facelist[] = {
    0, 1, 2,    0, 2, 3
};


topazMesh_t * topaz_mesh_create_square(topaz_t * ctx) {
    topazMesh_t * m = topaz_mesh_create(ctx);
    topaz_mesh_define_vertices(
        m,
        TOPAZ_ARRAY_CAST(square_vertices, topazRenderer_3D_Vertex_t, 4)
    );

    topaz_mesh_add_object(m);
    topaz_array_push_n(
        topaz_mesh_get_object(m, 0),
        square_facelist,
        6
    );

    return m;
}
*/

