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
#include <stdlib.h>
#include <string.h>
#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif



struct topazAsset_t {
    topaz_t * ctx;
    topazAsset_Attributes_t attribs;
    topazString_t * name;
    topazString_t * fsPath;
    topazAsset_Type type;
    void * userdata;
};




topazAsset_t * topaz_asset_create(
    topaz_t * ctx, 
    topazAsset_Type type,
    const topazString_t * name,
    const topazAsset_Attributes_t * attribs
) {
    topazAsset_t * out = calloc(1, sizeof(topazAsset_t));
    out->attribs = *attribs;
    out->type = type;
    out->name = topaz_string_clone(name);
    out->fsPath = topaz_string_create();
    out->ctx = ctx;

    if (attribs->on_create) out->attribs.on_create(out, attribs->userData);
    return out;
}

void topaz_asset_destroy(topazAsset_t * a) {
    if (a->attribs.on_destroy) a->attribs.on_destroy(a, a->attribs.userData);
    topaz_string_destroy(a->name);    
    free(a);   
}

const topazAsset_Attributes_t * topaz_asset_get_attributes(const topazAsset_t * a) {
    return &a->attribs;
}



void topaz_asset_set_path(topazAsset_t * a, const topazString_t * str) {
    topaz_string_set(a->fsPath, str);
}

const topazString_t * topaz_asset_get_name(const topazAsset_t * a) {
    return a->name;
}

topazAsset_Type topaz_asset_get_type(const topazAsset_t * a) {
    return a->type;
}

const topazString_t * topaz_asset_get_path(const topazAsset_t * a) {
    return a->fsPath;
}

