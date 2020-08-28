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

#include <topaz/compat.h>
#include <topaz/containers/array.h>
#include <topaz/material.h>
#include <topaz/backends/renderer.h>
#include <topaz/modules/graphics.h>
#include <topaz/topaz.h>
#include <topaz/assets/image.h>
#include <stdlib.h>
#include <string.h>


static topazMaterial_ProgramData_t default_program_data = {
    {1.f, 0.f, 1.f, 1.f},
    {1.f, 0.f, 1.f, 1.f},
    {1.f, 0.f, 1.f, 1.f},
    {
        0.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 0.f,

        0.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 0.f,

        0.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 0.f
    }
};



struct topazMaterial_t {
    topaz_t * ctx;
    topazMaterial_ProgramData_t dataSrc;

    topazRenderer_Buffer_t * materialData;
    topazRenderer_Program_t * program;
    topazArray_t * textureSlots;
    topazArray_t * textureObjects;
    topazRenderer_Framebuffer_t * samplebuffer;
};


topazMaterial_t * topaz_material_create(topaz_t * t) {
    topazMaterial_t * out = calloc(1, sizeof(topazMaterial_t));
    topazRenderer_t * r = topaz_graphics_get_renderer(topaz_context_get_graphics(t));
    out->dataSrc = default_program_data;
    out->program = topaz_renderer_program_get_preset(
        r, 
        topazRenderer_PresetProgram_Basic
    );
    
    out->materialData = topaz_renderer_buffer_create(r, (float*)&default_program_data, 32+12);
    out->textureSlots = topaz_array_create(sizeof(int));
    out->textureObjects = topaz_array_create(sizeof(topazRenderer_Texture_t *));
    out->samplebuffer = NULL;
    return out;
}


topazMaterial_t * topaz_material_clone(const topazMaterial_t * m) {
    topazMaterial_t * out = calloc(1, sizeof(topazMaterial_t));
    out->ctx = m->ctx;
    out->dataSrc = m->dataSrc;
    out->textureSlots = topaz_array_clone(m->textureSlots);
    out->textureObjects = topaz_array_clone(m->textureObjects);
    out->materialData = topaz_renderer_buffer_create(
        topaz_graphics_get_renderer(
            topaz_context_get_graphics(m->ctx)
        ),
        (float*)&out->dataSrc,
        44
    );
    out->samplebuffer = m->samplebuffer;
    out->program = m->program;
    return out;
}

void topaz_material_destroy(topazMaterial_t * m) {
    topaz_renderer_buffer_destroy(m->materialData);
    topaz_array_destroy(m->textureSlots);
    topaz_array_destroy(m->textureObjects);
    free(m);
}




const topazMaterial_ProgramData_t * topaz_material_get_program_data(const topazMaterial_t * m) {
    return &m->dataSrc;
}

void topaz_material_set_program_data(topazMaterial_t * m, const topazMaterial_ProgramData_t * data) {
    m->dataSrc = *data;
    topaz_renderer_buffer_update(
        m->materialData, 
        (float*)&m->dataSrc,
        0,
        44
    );
}

void topaz_material_set_program(topazMaterial_t * m, topazRenderer_Program_t * p) {
    m->program = p;
}

void topaz_material_set_texture(topazMaterial_t * m, int index, const topazImage_Frame_t * f) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(m->textureSlots);
    for(i = 0; i < len; ++i) {
        if (topaz_array_at(m->textureSlots, int, i) == index) {
            topaz_array_at(m->textureObjects, topazRenderer_Texture_t *, i) = topaz_image_frame_get_texture(f);
            return;
        }
    }

    topaz_array_push(m->textureSlots, index);
    topaz_array_push(m->textureObjects, f);
}

void topaz_material_clear_textures(topazMaterial_t * m) {
    topaz_array_set_size(m->textureSlots, 0);
    topaz_array_set_size(m->textureObjects, 0);
}




void topaz_material_set_framebuffer_source(topazMaterial_t * m, topazRenderer_Framebuffer_t * fb) {
    m->samplebuffer = fb;
}

void topaz_material_update_3d(topazMaterial_t * m, topazRenderer_3D_t * d3) {
    d3->samplebuffer = m->samplebuffer;
    d3->textureObjects = m->textureObjects;
    d3->textureSlots = m->textureSlots;
    d3->program = m->program;
    d3->materialData = m->materialData;
}




