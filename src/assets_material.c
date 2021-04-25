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
#include <topaz/asset.h>
#include <topaz/containers/array.h>
#include <topaz/containers/table.h>
#include <topaz/assets/material.h>
#include <topaz/backends/renderer.h>
#include <topaz/modules/graphics.h>
#include <topaz/modules/console.h>
#include <topaz/topaz.h>
#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
static char * MAGIC_ID__MATERIAL = "t0p4zm4t3r14l";
#endif



static topazMaterial_ProgramData_t default_program_data = {
    {1.f, 0.f, 1.f, 1.f},
    {1.f, 0.f, 1.f, 1.f},
    {1.f, 0.f, 1.f, 1.f},
    {
        0.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 0.f,
    }
};



struct topazMaterial_t {
    #ifdef TOPAZDC_DEBUG
    char * MAGIC_ID;
    #endif
    topaz_t * ctx;
    topazMaterial_ProgramData_t dataSrc;

    topazRenderer_Buffer_t * materialData;
    topazRenderer_Program_t * program;
    topazTable_t * vertex;
    topazTable_t * fragment;
    int needsCompilation;
    int ownsProgram;
};



static void material__destroy(topazAsset_t * a, void * userData) {
    topazMaterial_t * mat = userData;
    topaz_material_clear_sources(a);
    topaz_table_destroy(mat->vertex);
    topaz_table_destroy(mat->fragment);
    topaz_renderer_buffer_destroy(mat->materialData);
    if (mat->ownsProgram)
        topaz_renderer_program_destroy(mat->program);
    free(mat);
}

topazAsset_t * topaz_material_create(topaz_t * t, const topazString_t * name) {
    topazMaterial_t * out = calloc(1, sizeof(topazMaterial_t));
    topazRenderer_t * r = topaz_graphics_get_renderer(topaz_context_get_graphics(t));
    out->dataSrc = default_program_data;
    out->program = topaz_renderer_program_get_fallback(r);
    out->vertex   = topaz_table_create_hash_topaz_string();
    out->fragment = topaz_table_create_hash_topaz_string();    
    out->materialData = topaz_renderer_buffer_create(r, (float*)&default_program_data, 16+12);
    out->ctx = t;


    topazAsset_Attributes_t attribs = {};
    attribs.on_destroy = material__destroy;
    attribs.userData = out;

    return topaz_asset_create(
        t,
        topazAsset_Type_Material,
        name, 
        &attribs
    );
}





const topazMaterial_ProgramData_t * topaz_material_get_program_data(topazAsset_t * a) {
    topazMaterial_t * m = topaz_asset_get_attributes(a)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(m->MAGIC_ID == MAGIC_ID__MATERIAL);
    #endif
    return &m->dataSrc;
}

void topaz_material_set_program_data(topazAsset_t * a, const topazMaterial_ProgramData_t * data) {
    topazMaterial_t * m = topaz_asset_get_attributes(a)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(m->MAGIC_ID == MAGIC_ID__MATERIAL);
    #endif

    m->dataSrc = *data;
    topaz_renderer_buffer_update(
        m->materialData, 
        (float*)&m->dataSrc,
        0,
        28
    );
}


void topaz_material_update_3d(topazAsset_t * a, topazRenderer_3D_t * d3) {
    topazMaterial_t * m = topaz_asset_get_attributes(a)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(m->MAGIC_ID == MAGIC_ID__MATERIAL);
    #endif

    if (m->needsCompilation) {
        topazRenderer_t * r = topaz_graphics_get_renderer(topaz_context_get_graphics(m->ctx));
        // first, see if compilation is even possible.
        topazRenderer_Parameters_t params = topaz_renderer_get_parameters(r);
        topazString_t * lang = topaz_string_create_from_c_str(params.programLanguage);


        // we are using a language that has material source data.
        // attempt to recompile        
        if (topaz_table_entry_exists(m->vertex, lang)) {
            // remove existing one
            if (m->ownsProgram) 
                topaz_renderer_program_destroy(m->program);
                
                
            topazString_t * log = topaz_string_create();
            m->program = topaz_renderer_program_create(
                r,
                topaz_table_find(m->vertex, lang),
                topaz_table_find(m->fragment, lang),
                log                
            );
            
            // program failed to compile. Report and continue.
            if (!m->program) {
                
                m->program = topaz_renderer_program_get_fallback(r);
                topazConsole_t * console = topaz_context_get_console(m->ctx);
                topazString_t * message = topaz_string_create();
                topaz_string_concat_printf(
                    message,
                    "topazMaterial_t: failure to compile renderer program (language: %s):", 
                    topaz_string_get_c_str(lang)
                );
                topaz_string_concat(message, log);
                topaz_console_print_message(console, message, topazConsole_MessageType_Error);
                topaz_string_destroy(message);
            } else {
                m->ownsProgram = TRUE;
            }
            topaz_string_destroy(log);
        }
        
        topaz_string_destroy(lang);
        m->needsCompilation = FALSE;
    }

    d3->program = m->program;
    d3->material = m->materialData;
}




const topazTable_t * topaz_material_get_vertex_source_table(
    topazAsset_t * a
) {
    topazMaterial_t * m = topaz_asset_get_attributes(a)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(m->MAGIC_ID == MAGIC_ID__MATERIAL);
    #endif
    return m->vertex;
}

const topazTable_t * topaz_material_get_fragment_source_table(
    topazAsset_t * a
) {
    topazMaterial_t * m = topaz_asset_get_attributes(a)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(m->MAGIC_ID == MAGIC_ID__MATERIAL);
    #endif
    return m->vertex;
}

/// Removes all shader program sources from the material.
void topaz_material_clear_sources(
    /// The material to modify.
    topazAsset_t * a
) {
    topazMaterial_t * m = topaz_asset_get_attributes(a)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(m->MAGIC_ID == MAGIC_ID__MATERIAL);
    #endif

    topazTableIter_t * iter = topaz_table_iter_create();
    for(topaz_table_iter_start(iter, m->vertex);
        !topaz_table_iter_is_end(iter);
        topaz_table_iter_proceed(iter)) {
        topaz_string_destroy(topaz_table_iter_get_value(iter));
        topaz_string_destroy(topaz_table_find(m->fragment, topaz_table_iter_get_key(iter)));   
    }
    topaz_table_iter_destroy(iter);
}


void topaz_material_set_program_source(
    /// The material to modify
    topazAsset_t * a, 

    /// The language that this source pertains to.
    const topazString_t * language,

    /// The vertex shader source.    
    const topazString_t * vertexSource,
    
    /// The fragment shader source.
    const topazString_t * fragmentSource
) {
    topazMaterial_t * m = topaz_asset_get_attributes(a)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(m->MAGIC_ID == MAGIC_ID__MATERIAL);
    #endif

    topaz_table_insert(
        m->vertex,
        language,
        topaz_string_clone(vertexSource)
    );

    topaz_table_insert(
        m->fragment,
        language,
        topaz_string_clone(fragmentSource)
    );
    m->needsCompilation = TRUE;

}


