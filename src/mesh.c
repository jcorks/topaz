#include <topaz/compat.h>
#include <topaz/matrix.h>
#include <topaz/math.h>
#include <topaz/topaz.h>
#include <topaz/mesh.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif


struct topazMesh_t {
    topaz_t * ctx;
    topazArray_t * objs;
    topazRenderer_Buffer_t * v;
    topazMesh_t * sharedFrom;
    float reg[4];
};

#define TOPAZ_V_TO_FLOATS(__N__) ((sizeof(topazRenderer_3D_Vertex_t) / sizeof(float))*(__N__))
#define TOPAZ_FLOATS_TO_V(__N__) ((__N__) / sizeof(topazRenderer_3D_Vertex_t))

static void topaz_mesh__get_attrib_params(topazMesh_VertexAttribute attrib, uint32_t * offset, uint32_t * length) {
    *offset = 0;
    *length = 0;
    switch(attrib) {
      case topazMesh_VertexAttribute_Position:
        *offset = 0;
        *length = 3;
        break;

      case topazMesh_VertexAttribute_Normal:
        *offset = 3;
        *length = 3;
        break;

      case topazMesh_VertexAttribute_UV:
        *offset = 6;
        *length = 2;
        break;

      case topazMesh_VertexAttribute_UserData:
        *offset = 8;
        *length = 4;
        break;


    }
}


topazMesh_t * topaz_mesh_create(topaz_t * ctx) {
    topazMesh_t * out = calloc(1, sizeof(topazMesh_t));
    out->ctx = ctx;
    out->objs = topaz_array_create(sizeof(topazArray_t *));
    out->v = NULL;
    out->sharedFrom = out;
    return out;
}

topazMesh_t * topaz_mesh_clone(const topazMesh_t * src) {
    topazMesh_t * out = topaz_mesh_create(src->ctx);

    // copy vertices;
    if (src->v) {
        int numFlt = topaz_renderer_buffer_get_size(src->v);
        float * buffer = malloc(numFlt*sizeof(float));
        topaz_renderer_buffer_read(src->v, buffer, 0, numFlt);
        out->v = topaz_renderer_buffer_create(
            topaz_context_get_backend_renderer(src->ctx),
            buffer,
            numFlt
        );
        free(buffer);
    }

    // copy objects
    uint32_t i;
    uint32_t len = topaz_array_get_size(src->objs);
    for(i = 0; i < len; ++i) {
        topazArray_t * t = topaz_array_clone(topaz_array_at(src->objs, topazArray_t *, i));
        topaz_array_push(out->objs, t);
    }
    return out;
}


topazMesh_t * topaz_mesh_clone_shared(const topazMesh_t * src) {
    topazMesh_t * out = topaz_mesh_create(src->ctx);
    out->v = src->v;
    out->sharedFrom = src->sharedFrom;


    // copy objects
    uint32_t i;
    uint32_t len = topaz_array_get_size(src->objs);
    for(i = 0; i < len; ++i) {
        topazArray_t * t = topaz_array_clone(topaz_array_at(src->objs, topazArray_t *, i));
        topaz_array_push(out->objs, t);
    }
    return out;
}


topazMesh_t * topaz_mesh_get_shared_source(topazMesh_t * m) {
    return m->sharedFrom;
}

void topaz_mesh_set_vertex_count(topazMesh_t * m, uint32_t i) {
    if (m->v)
        topaz_renderer_buffer_destroy(m->v);

    m->v = topaz_renderer_buffer_create(
        topaz_context_get_backend_renderer(m->ctx),
        NULL,
        TOPAZ_V_TO_FLOATS(i)
    );
}

void topaz_mesh_define_vertices(topazMesh_t * m, const topazArray_t * s) {
    if (m->v)
        topaz_renderer_buffer_destroy(m->v);
    
    m->v = topaz_renderer_buffer_create(
        topaz_context_get_backend_renderer(m->ctx),
        topaz_array_get_data(s),
        TOPAZ_V_TO_FLOATS(topaz_array_get_size(s))
    );
}


/// Gets data from a specific vertex. If the 
/// vertex doesnt exist, nothing is returned.
///
const float * topaz_mesh_get_vertex(
    const topazMesh_t * mSrc, 
    topazMesh_VertexAttribute attrib, 
    uint32_t index
) {
    topazMesh_t * m = (topazMesh_t *) mSrc; // shhhh its fine
    m->reg[0] = m->reg[1] = m->reg[2] = m->reg[3] = 0.f;

    if (!m->v) return m->reg;

    uint32_t count = TOPAZ_FLOATS_TO_V(topaz_renderer_buffer_get_size(m->v));
    if (index >= count) {
        return m->reg;
    }

    uint32_t baseFloat = TOPAZ_V_TO_FLOATS(index);
    uint32_t offset;
    uint32_t length;
    topaz_mesh__get_attrib_params(attrib, &offset, &length);

    topaz_renderer_buffer_read(
        m->v,
        m->reg,
        offset+baseFloat,
        length
    );

    return m->reg;
}



void topaz_mesh_set_vertex(
    topazMesh_t * m,
    topazMesh_VertexAttribute attrib, 
    uint32_t index,
    const float * data
) {
    if (!m->v) return;
    uint32_t count = TOPAZ_FLOATS_TO_V(topaz_renderer_buffer_get_size(m->v));
    if (index >= count) {
        return;
    }


    uint32_t baseFloat = TOPAZ_V_TO_FLOATS(index);
    uint32_t offset;
    uint32_t length;
    topaz_mesh__get_attrib_params(attrib, &offset, &length);

    topaz_renderer_buffer_update(
        m->v,
        data,
        offset+baseFloat,
        length
    );
}

uint32_t topaz_mesh_get_vertex_count(const topazMesh_t * m) {
    if (!m->v) return 0;
    return TOPAZ_FLOATS_TO_V(topaz_renderer_buffer_get_size(m->v));
}


uint32_t topaz_mesh_add_object(topazMesh_t * m) {
    topazArray_t * obj = topaz_array_create(sizeof(uint32_t));
    topaz_array_push(m->objs, obj);
    return topaz_array_get_size(m->objs)-1;
}

topazArray_t * topaz_mesh_get_object(topazMesh_t * m, uint32_t index) {
    if (index >= topaz_array_get_size(m->objs)) return NULL;
    return topaz_array_at(m->objs, topazArray_t *, index);
}


void topaz_mesh_remove_object(topazMesh_t * m, uint32_t index) {
    if (index >= topaz_array_get_size(m->objs)) return;
    topaz_array_remove(m->objs, index);
}

uint32_t topaz_mesh_get_object_count(const topazMesh_t * m) {
    return topaz_array_get_size(m->objs);
}

topazRenderer_Buffer_t * topaz_mesh_get_vertex_data(topazMesh_t * m) {
    return m->v;
}







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

