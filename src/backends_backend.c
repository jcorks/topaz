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
#include <topaz/backends/backend.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif


struct topazBackend_t {
    topazString_t * name;
    topazString_t * version;
    topazString_t * author;
    topazString_t * description;

    int (*on_init)      (topazBackend_t *);
    int (*on_init_late) (topazBackend_t *);
    void (*on_step)     (topazBackend_t *);
    void (*on_step_late)(topazBackend_t *);
    void (*on_draw)     (topazBackend_t *);
    void (*on_draw_late)(topazBackend_t *);

    void * userData;

    int topazMajorVersion;
    int topazMinorVersion;
    int topazMicroVersion;
};




typedef struct topazBackend_t topazBackend_t;


topazBackend_t * topaz_backend_create(
    const topazString_t * name,
    const topazString_t * version,
    const topazString_t * author,
    const topazString_t * description,



    int (*on_init)(topazBackend_t *),
    int (*on_init_late)(topazBackend_t *),
    void (*on_step)(topazBackend_t *),
    void (*on_step_late)(topazBackend_t *),
    void (*on_draw)(topazBackend_t *),
    void (*on_draw_late)(topazBackend_t *),
    void * userData,

    int topazMajorVersion,
    int topazMinorVersion,
    int topazMicroVersion

) {
    topazBackend_t * out = calloc(1, sizeof(topazBackend_t));
    out->name        = topaz_string_clone(name);
    out->version     = topaz_string_clone(version);
    out->author      = topaz_string_clone(author);
    out->description = topaz_string_clone(description);

    out->on_init = on_init;
    out->on_step = on_step;
    out->on_draw = on_draw;

    out->on_init_late = on_init_late;
    out->on_step_late = on_step_late;
    out->on_draw_late = on_draw_late;

    out->userData = userData;

    out->topazMajorVersion = topazMajorVersion;
    out->topazMinorVersion = topazMinorVersion;
    out->topazMicroVersion = topazMicroVersion;

    return out;
}






int topaz_backend_get_topaz_major_version(const topazBackend_t * t) {
    return t->topazMajorVersion;
}

int topaz_backend_get_topaz_minor_version(const topazBackend_t * t) {
    return t->topazMinorVersion;
}

int topaz_backend_get_topaz_micro_version(const topazBackend_t * t) {
    return t->topazMicroVersion;
}





const topazString_t * topaz_backend_get_name   (const topazBackend_t * t) {
    return t->name;
}

const topazString_t * topaz_backend_get_version(const topazBackend_t * t) {
    return t->version;
}

const topazString_t * topaz_backend_get_author (const topazBackend_t * t) {
    return t->author;
}

const topazString_t * topaz_backend_get_description(const topazBackend_t * t) {
    return t->description;
}





void topaz_backend_init(topazBackend_t * t) {
    t->on_init(t);
}

void topaz_backend_init_late(topazBackend_t * t) {
    t->on_init_late(t);
}

void topaz_backend_step(topazBackend_t * t) {
    t->on_step(t);
}

void topaz_backend_step_late(topazBackend_t * t) {
    t->on_step_late(t);
}


void topaz_backend_draw(topazBackend_t * t) {
    t->on_draw(t);
}

void topaz_backend_draw_late(topazBackend_t * t) {
    t->on_draw_late(t);
}





void * topaz_backend_get_user_data(topazBackend_t * t) {
    return t->userData;
}

