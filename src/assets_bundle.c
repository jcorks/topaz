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
#include <topaz/assets/bundle.h>
#include <topaz/assets/data.h>
#include <topaz/containers/string.h>
#include <topaz/containers/array.h>
#include <topaz/topaz.h>
#include <topaz/backends/renderer.h>
#include <topaz/modules/resources.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif

// magic number thats checked to verify all assets
#ifdef TOPAZDC_DEBUG
static char * MAGIC_ID__BUNDLE = "t0p4zbundl3";
#endif



typedef struct {
    #ifdef TOPAZDC_DEBUG
    char * MAGIC_ID;
    #endif
    topaz_t * ctx;
    topazAsset_t * src;
    topazString_t * description;
    topazString_t * author;
    topazArray_t * dependsNames;
    topazArray_t * dependsVersions;    
    int major;
    int minor;
    int micro;
    uint32_t size;
    uint8_t * data;
    topazArray_t * subBundles;
    
} TopazAssetBundle;






static void bundle__destroy(topazAsset_t * a, void * userData) {
    TopazAssetBundle * b = userData;
    topaz_bundle_clear(a);
    topaz_array_destroy(b->subBundles);
    free(b);
}

static void bundle__write_string(uint8_t ** data, const topazString_t * str) {
    *((uint32_t*)data) = topaz_string_get_length(str); 
    uint32_t len = topaz_string_get_length(str); data+=sizeof(uint32_t);
    uint32_t i;
    int * iter = (int*)data;
    for(i = 0; i < len; ++i, ++iter) {
        *iter = topaz_string_get_char(str, i);
    }
    *data = (uint8_t*)iter;
}

static int bundle__read_string(
    const uint8_t ** data, 
    const uint8_t * end,
    topazString_t * str 
) { 
    if (*data + sizeof(uint32_t) > end) return 0;
    uint32_t len = *(uint32_t*)data;
    data += sizeof(uint32_t);
    uint32_t i;
    for(i = 0; i < len && *data + sizeof(int) <= end; ++i) {
        topaz_string_append_char(str, *(int*)data);
        data += sizeof(int);
    }
    
    return *data <= end;
}

static void bundle__write_main_header(TopazAssetBundle * b, uint8_t * data) {
    data[0]  = 0xfe;
    data[1]  = 0x02;
    data[2]  = 'T';
    data[3]  = 'O';
    data[4]  = 'P';
    data[5]  = 'A';
    data[6]  = 'z';
    data[7]  = 0x05;
    data[8]  = 0;
    data[9]  = 1; //<-ver
    data[10] = 0x04;
    data[11] = 0x10;
    data+=12;
    
    *((int*)data) = b->major; data+=sizeof(int);
    *((int*)data) = b->minor; data+=sizeof(int);
    *((int*)data) = b->micro; data+=sizeof(int);
    uint32_t depends = topaz_array_get_size(b->dependsNames);
    *((uint32_t*)data) = depends; data+=sizeof(uint32_t);

    
    bundle__write_string(&data, topaz_asset_get_name(b->src));
    bundle__write_string(&data, b->description);
    bundle__write_string(&data, b->author);    
    uint32_t i;
    for(i = 0; i < depends; ++i) {
        bundle__write_string(&data, topaz_array_at(b->dependsNames, topazString_t *, i));
        bundle__write_string(&data, topaz_array_at(b->dependsVersions, topazString_t *, i));
    }
}





static uint32_t bundle__get_main_header_size(TopazAssetBundle * b) {
    uint32_t total = 12 + sizeof(uint32_t)*(4 + topaz_array_get_size(b->dependsNames));
    total += topaz_string_get_length(topaz_asset_get_name(b->src))*sizeof(int);
    total += topaz_string_get_length(b->description)*sizeof(int);
    total += topaz_string_get_length(b->author)*sizeof(int);
    
    uint32_t i;
    uint32_t len = topaz_array_get_size(b->dependsNames);
    for(i = 0; i < len; ++i) {
        total +=
            topaz_string_get_length(topaz_array_at(b->dependsNames,    topazString_t *, i))*sizeof(int) +
            topaz_string_get_length(topaz_array_at(b->dependsVersions, topazString_t *, i))*sizeof(int)
        ;
    }
    return total;
}


static int bundle__read_main_header(
    const uint8_t * data,
    uint32_t size,
    
    topazString_t * name,
    int * major,
    int * minor,
    int * micro,
    topazString_t * description,
    topazString_t * author,
    topazArray_t * dependsNames,
    topazArray_t * dependsVersions
    
        
) { 
    if (size < 28) return 0;
    const uint8_t *start = data;
    if(!(
        data[0] == 0xfe &&
        data[1] == 0x02 &&
        data[2] == 'T' &&
        data[3] == 'O' &&
        data[4] == 'P' &&
        data[5] == 'A' &&
        data[6] == 'z' &&
        data[7] == 0x05 &&
        data[8] == 0 &&
        data[9] == 1 && // ver 
        data[10] == 0x04 &&
        data[11] == 0x10
    )) return 0;
    
    
    data += 12;
    *major = *((int*)data); data += sizeof(int);
    *minor = *((int*)data); data += sizeof(int);
    *micro = *((int*)data); data += sizeof(int);
    uint32_t dependSize = *((uint32_t*)data); data += sizeof(uint32_t);
    const uint8_t * end = start + size;

    if (!bundle__read_string(&data, end, name)) return 0;
    if (!bundle__read_string(&data, end, description)) return 0;
    if (!bundle__read_string(&data, end, author)) return 0;

    uint32_t i;
    for(i = 0; i < dependSize; ++i) {
        topazString_t * dname = topaz_string_create();
        topazString_t * dversion = topaz_string_create();
        topaz_array_push(dependsNames, dname);
        topaz_array_push(dependsVersions, dversion);
        if (!bundle__read_string(&data, end, dname)) return 0;
        if (!bundle__read_string(&data, end, dversion)) return 0;
    }
    return 1;
}

static TopazAssetBundle * bundle__retrieve(const topazAsset_t * a) {
    TopazAssetBundle * ret = topaz_asset_get_attributes(a)->userData;
    #ifdef TOPAZDC_DEBUG
        assert(ret->MAGIC_ID == MAGIC_ID__BUNDLE);
    #endif
    return ret;
}

static uint32_t bundle__get_entry_size(
    const topazString_t * name,
    const topazString_t * ext,
    uint32_t size
) {
    return size + (topaz_string_get_length(name) + topaz_string_get_length(ext))*sizeof(int) +
           sizeof(uint32_t)*4;
}


static void bundle__write_entry(
    uint8_t * dest,
    const topazString_t * name,
    const topazString_t * ext,
    const uint8_t * src,
    uint32_t srcSize
) {
    uint32_t header[3];
    header[0] = topaz_string_get_length(name);
    header[1] = topaz_string_get_length(ext);
    header[2] = srcSize;
    memcpy(dest, header, sizeof(uint32_t)*3);
    dest += sizeof(uint32_t)*3;

    uint32_t totalSize = sizeof(uint32_t)*4 + (header[0] + header[1])*sizeof(int) + header[2];

    int * strname = (int*)dest;
    uint32_t i;
    uint32_t len = header[0];
    for(i = 0; i < len; ++i, ++strname) {
        strname[0] = topaz_string_get_char(name, i);
    }

    len = header[1];
    for(i = 0; i < len; ++i, ++strname) {
        strname[0] = topaz_string_get_char(ext, i);
    }
    
    memcpy(strname, src, srcSize);
    dest = (uint8_t*)strname;
    dest += srcSize;
    memcpy(dest, &totalSize, sizeof(uint32_t));
}



static uint8_t * bundle__chomp_item(
    const uint8_t * data,
    uint32_t * size,
    topazString_t ** name,
    topazString_t ** ext,
    uint32_t * outputSize
) {
    uint32_t totalSize = data[*size - sizeof(uint32_t)];
    if (totalSize >= *size) return NULL;    
    data = data+(*size-totalSize);
    
    uint32_t header[3];
    memcpy(header, data, sizeof(uint32_t)*3);
    data += sizeof(uint32_t)*3;
    if ((header[0] + header[1])*sizeof(int) + header[2] + sizeof(uint32_t)*4 != totalSize) return NULL;
    
    *name = topaz_string_create();
    int * strname = (int*)data;
    uint32_t i;
    for(i = 0; i < header[0]; ++i) {
        topaz_string_set_char(*name, i, strname[i]);
    }
    
    *ext = topaz_string_create();
    for(i = 0; i < header[1]; ++i) {
        topaz_string_set_char(*ext, i, strname[i]);
    }
    data = (uint8_t*)strname;
    uint8_t * output = malloc(header[2]);
    memcpy(output, data, header[2]);
    *outputSize = header[2];
    
    *size -= totalSize;
    return output;
}




/// Creates a new data asset object
/// Use asset_destroy to release.
///
topazAsset_t * topaz_bundle_create_empty(
    topaz_t * t, 
    const topazString_t * name,
    int versionMajor,
    int versionMinor,
    int versionMicro,
    const topazString_t * description,
    const topazString_t * author,
    const topazString_t ** dependsNames,
    const topazString_t ** dependsVersions, // MAJOR.MINOR
    uint32_t dependsCount
    
) {


    TopazAssetBundle * img = calloc(1, sizeof(TopazAssetBundle));
    #ifdef TOPAZDC_DEBUG
        img->MAGIC_ID = MAGIC_ID__BUNDLE;
    #endif
    img->data = NULL;
    img->ctx = t;
    img->subBundles = topaz_array_create(sizeof(topazAsset_t *));
    img->major = versionMajor;
    img->minor = versionMinor;
    img->micro = versionMicro;
    img->description = topaz_string_clone(description);   
    img->author = topaz_string_clone(author);
    img->dependsNames = topaz_array_create(sizeof(topazString_t*));
    img->dependsVersions = topaz_array_create(sizeof(topazString_t*));
    
    uint32_t i;
    for(i = 0; i < dependsCount; ++i) {
        topazString_t * t = topaz_string_clone(dependsNames[i]);
        topaz_array_push(img->dependsNames, t);
        t = topaz_string_clone(dependsVersions[i]);
        topaz_array_push(img->dependsVersions, t);
    }
    
    
    topazAsset_Attributes_t attribs = {};
    attribs.on_destroy = bundle__destroy;
    attribs.userData = img;

    return (img->src = topaz_asset_create(
        t,
        topazAsset_Type_Bundle,
        name, 
        &attribs
    ));
}


topazAsset_t * topaz_bundle_create_from_data(
    topaz_t * ctx,
    const uint8_t * data,
    uint32_t byteCount
) {
    
    
    TopazAssetBundle * img = calloc(1, sizeof(TopazAssetBundle));
    #ifdef TOPAZDC_DEBUG
        img->MAGIC_ID = MAGIC_ID__BUNDLE;
    #endif
    img->data = NULL;
    img->ctx = ctx;
    img->subBundles = topaz_array_create(sizeof(topazAsset_t *));
    img->description = topaz_string_create();   
    img->author = topaz_string_create();
    img->dependsNames = topaz_array_create(sizeof(topazString_t*));
    img->dependsVersions = topaz_array_create(sizeof(topazString_t*));
    topazString_t * name = topaz_string_create();

    topazAsset_Attributes_t attribs = {};
    attribs.on_destroy = bundle__destroy;
    attribs.userData = img;

    img->src = topaz_asset_create(
        ctx,
        topazAsset_Type_Bundle,
        name, 
        &attribs
    );

    if (!bundle__read_main_header(
        data,
        byteCount,
        name,
        &img->major,
        &img->minor,
        &img->micro,
        img->description,
        img->author,
        img->dependsNames,   
        img->dependsVersions
    )) {
        topaz_asset_destroy(img->src);
        return NULL;
    }
    
    img->size = 0;
    img->data = malloc(byteCount);
    memcpy(img->data, data, byteCount);
    img->size = byteCount;

    return img->src;
}



void topaz_bundle_get_version(
    topazAsset_t * a,
    int * major,
    int * minor,
    int * micro
) {
    TopazAssetBundle * b = bundle__retrieve(a);
    *major = b->major;
    *minor = b->minor;
    *micro = b->micro;
}

const topazString_t * topaz_bundle_get_description(
    topazAsset_t * a
) {
    TopazAssetBundle * b = bundle__retrieve(a);
    return b->description;
}

const topazString_t * topaz_bundle_get_author(
    topazAsset_t * a
) {
    TopazAssetBundle * b = bundle__retrieve(a);
    return b->author;
}


const topazArray_t * topaz_bundle_get_depends_names(
    topazAsset_t * a
) {
    TopazAssetBundle * b = bundle__retrieve(a);
    return b->dependsNames;
}

const topazArray_t * topaz_bundle_get_depends_version_strings(
    topazAsset_t * a
) {
    TopazAssetBundle * b = bundle__retrieve(a);
    return b->dependsVersions;
}




/// Adds a new asset to the bundle.
///
void topaz_bundle_add_item(
    /// The bundle to modify
    topazAsset_t * a,
    
    /// The name of the of the asset within the bundle.
    const topazString_t * name,
    
    /// The extension to use to decode the data when unpacking.
    const topazString_t * extension,
    
    
    /// Raw data of the asset.
    const topazAsset_t * dataAsset  
) {
    const topazArray_t * bytes = topaz_data_get_as_bytes((topazAsset_t*)dataAsset);
    TopazAssetBundle * b = bundle__retrieve(a);
    uint32_t entrySize = bundle__get_entry_size(name, extension, topaz_array_get_size(bytes));
    uint32_t oldSize = b->size;
    if (!b->size) {
        b->size = bundle__get_main_header_size(b)+entrySize;
        b->data = malloc(b->size);
        bundle__write_main_header(b, b->data);
    } else {
        b->size += entrySize;
        b->data = realloc(b->data, b->size);
    }
    
    bundle__write_entry(
        b->data+oldSize, 
        name, 
        extension, 
        topaz_array_get_data(bytes), 
        topaz_array_get_size(bytes)
    );
}


/// Removes all the bundle's items.
void topaz_bundle_clear(
    topazAsset_t * a
) {
    TopazAssetBundle * b = bundle__retrieve(a);
    topaz_array_set_size(b->subBundles, 0);
    free(b->data);
    b->data = NULL;
    b->size = 0;
}


/// Retrieves the raw byte state of the bundle, representing the 
/// state of all of its assets.
const uint8_t * topaz_bundle_get_packed_data(
    topazAsset_t * a,
    uint32_t * size
) {
    TopazAssetBundle * b = bundle__retrieve(a);
    *size = b->size;
    return b->data;
}


/// Unpacks all the items in the bundle. For 
/// each item, a new asset will be created within 
/// resources. Each asset will be prefixed with 
/// this bundle asset's name followed by "."
/// and the name of the sub-asset.
/// For example, a bundle with the name 
/// "Package" that contains an asset named 
/// "Item" could be fetched under the name "Package.Item"
/// Any assets that are, themselves, bundled will also be 
/// unpacked.
int topaz_bundle_unpack_all(
    topazAsset_t * a
) {
    while(topaz_bundle_unpack_continue(a, 1));
    TopazAssetBundle * b = bundle__retrieve(a);
    return b->size == 0;
}

int topaz_bundle_unpack_continue(
    topazAsset_t * a,
    int nItems
) {
    TopazAssetBundle * b = bundle__retrieve(a);
    topazResources_t * res = topaz_context_get_resources(b->ctx);
    int n;
    for(n = 0; n < nItems; ++n) {
        uint32_t subb = topaz_array_get_size(b->subBundles);
        if (subb) {
            topazAsset_t * as = topaz_array_at(b->subBundles, topazAsset_t *, topaz_array_get_size(b->subBundles)-1);
            if (!topaz_bundle_unpack_continue(as, nItems)) {
                topaz_array_remove(b->subBundles, topaz_array_get_size(b->subBundles)-1);
            }
            return 1;
        }
        if (!b->size) return 0;
        // start from end!
        topazString_t * name = NULL;
        topazString_t * ext = NULL;
        uint32_t len = 0;
        uint8_t * data = bundle__chomp_item(
            b->data,
            &b->size,
            &name,
            &ext,
            &len
        );
        
        if (!(len && data)) {
            if (name) topaz_string_destroy(name);
            if (ext) topaz_string_destroy(ext);
            free(data);
            return 0;
        }
        topazString_t * oldName = name;
        name = topaz_string_create_from_c_str("%s.%s",
            topaz_string_get_c_str(topaz_asset_get_name(a)),
            topaz_string_get_c_str(oldName)
        );
        topaz_string_destroy(name);
        
        topazAsset_t * asset = topaz_resources_create_asset_from_bytes(
            res,
            TOPAZ_ARRAY_CAST(data, uint8_t, len),
            name
        );
        topaz_string_destroy(name);

        if (!asset) {
            if (ext) topaz_string_destroy(ext);
            free(data);        
            return 0;
        }
        free(data);
        
        
        if (topaz_string_test_eq(ext, TOPAZ_STR_CAST("bundle"))) {
            // convert to bundle
            const topazArray_t * srcData = topaz_data_get_as_bytes(asset);
            topazAsset_t * bundle = topaz_bundle_create_from_data(
                b->ctx, 
                topaz_array_get_data(srcData),
                topaz_array_get_size(srcData)
            );
            if (!bundle) return 0;

            topaz_array_push(b->subBundles, bundle);
        } else {
            topaz_resources_load_asset(
                res, 
                ext,
                asset
            );      
        }  
        topaz_string_destroy(ext);
    }
    
    return b->size != 0;
    
}




