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
#include <topaz/assets/image.h>
#include <topaz/containers/string.h>
#include <topaz/containers/array.h>
#include <topaz/topaz.h>
#include <topaz/backends/renderer.h>
#include <topaz/modules/graphics.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif

// magic number thats checked to verify all assets
#ifdef TOPAZDC_DEBUG
static char * MAGIC_ID__IMAGE = "t0p4zd4t4";
#endif



typedef struct {
    #ifdef TOPAZDC_DEBUG
    char * MAGIC_ID;
    #endif

    topazArray_t * data;
} TopazAssetData;


static void data__destroy(topazAsset_t * a, void * userData) {
    TopazAssetData * d = userData;
    topaz_array_destroy(d->data);
    free(d);
}

static TopazAssetData * data__retrieve(const topazAsset_t * a) {
    TopazAssetData * ret = topaz_asset_get_attributes(a)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(ret->MAGIC_ID == MAGIC_ID__IMAGE);
    #endif
    return ret;
}



topazAsset_t * topaz_data_create(topaz_t * t, const topazString_t * name) {

    TopazAssetData * img = calloc(1, sizeof(TopazAssetData));
    #ifdef TOPAZDC_DEBUG
        img->MAGIC_ID = MAGIC_ID__IMAGE;
    #endif
    img->data = topaz_array_create(sizeof(uint8_t));


    topazAsset_Attributes_t attribs = {};
    attribs.on_destroy = data__destroy;
    attribs.userData = img;

    return topaz_asset_create(
        t,
        topazAsset_Type_Data,
        name, 
        &attribs
    );
}


topazAsset_t * topaz_data_create_empty(topaz_t * t) {
    return topaz_data_create(
        t,
        TOPAZ_STR_CAST("")
    );
}



void topaz_data_set_from_bytes(topazAsset_t * a, const topazArray_t * dataIn) {
    TopazAssetData * d = data__retrieve(a);
    topaz_array_set_size(d->data, 0);

    uint64_t numBytes = topaz_array_get_size(dataIn) * topaz_array_get_type_size(dataIn);
    topaz_array_set_size_exact(d->data, numBytes);
    memcpy(
        topaz_array_get_data(d->data),
        topaz_array_get_data(dataIn),
        numBytes 
    );
}

void topaz_data_set_from_string(topazAsset_t * a, const topazString_t * str) {
    TopazAssetData * d = data__retrieve(a);
    topaz_array_set_size(d->data, 0);

    const char * cstr = topaz_string_get_c_str(str);
    uint64_t len = strlen(cstr);
    topaz_array_set_size_exact(d->data, len+1);
    memcpy(
        topaz_array_get_data(d->data),
        cstr,
        len+1
    );
    // can / will take a lot of data, so clear it.
    topaz_string_clear_c_str(str);
}

void topaz_data_set_from_base64(topazAsset_t * a, const topazString_t * str) {
    uint32_t size;
    uint8_t * copy = topaz_string_base64_to_bytes(str, &size);
    topaz_data_set_from_bytes(a, TOPAZ_ARRAY_CAST(copy, uint8_t, size));
    free(copy);
}


const topazArray_t * topaz_data_get_as_bytes(topazAsset_t * a) {
    TopazAssetData * d = data__retrieve(a);
    return d->data;
}

topazString_t * topaz_data_get_as_string(topazAsset_t * a) {
    TopazAssetData * d = data__retrieve(a);
    topazArray_t * clone = topaz_array_clone(d->data);
    uint8_t end = 0;
    topaz_array_push(clone, end);

    topazString_t * str = topaz_string_create_from_c_str("%s", topaz_array_get_data(clone));
    topaz_array_destroy(clone);
    return str;
}

topazString_t * topaz_data_get_as_base64(topazAsset_t * a) {
    TopazAssetData * d = data__retrieve(a);
    return topaz_string_base64_from_bytes(
        topaz_array_get_data(d->data),
        topaz_array_get_size(d->data)
    );
}


