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







// Indicates this is POSIX compliant source
// It's required for some posix features/ symbols
//
#define _POSIX_C_SOURCE 199309L
#define _XOPEN_SOURCE 500
//


#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <dirent.h>
#include <topaz/compat.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <libgen.h>
#ifdef __linux__
    #include <linux/limits.h>
#endif
#include <stdio.h>
#include "backend.h"
#include <topaz/containers/array.h>
#include <topaz/containers/string.h>












void * topaz_filesystem_posix__create(
        topazFilesystem_t * fs,
        topaz_t * ctx,
        topazString_t * defaultResourcesPath,
        topazString_t * defaultTopazPath,
        topazString_t * defaultUserDataPath
) {
    char * buffer = malloc(PATH_MAX+1);
    getcwd(buffer, PATH_MAX);
    
    topaz_string_clear(defaultResourcesPath);
    topaz_string_clear(defaultTopazPath);
    topaz_string_clear(defaultUserDataPath);

    topaz_string_concat_printf(defaultResourcesPath, "%s", buffer);
    topaz_string_concat_printf(defaultTopazPath,     "%s", buffer);
    topaz_string_concat_printf(defaultUserDataPath,  "%s", buffer);

    free(buffer);
    return NULL;
}




#define READCHUNK 4096


static void * topaz_filesystem_posix__read(
    topazFilesystem_t * fs, 
    void * nu, 
    const topazString_t * fullpath, 
    uint32_t * size
) {
    uint8_t * buffer = malloc(READCHUNK);
    uint32_t chunk;
    FILE * f = fopen(topaz_string_get_c_str(fullpath), "rb");
    *size = 0;
    if (!f) {
        return NULL;
    }

    
    while((chunk = fread(buffer, 1, READCHUNK, f))) {
        *size += chunk;
    }
    fseek(f, 0, SEEK_SET);
    uint8_t * out = malloc(*size);
    uint8_t * iter = out;
    while((chunk = fread(buffer, 1, READCHUNK, f))) {
        memcpy(iter, buffer, chunk);
        iter += chunk;
    }
    return out;
}


static int topaz_filesystem_posix__write(
    topazFilesystem_t * fs, 
    void * nu, 
    const topazString_t * fullpathBase, 
    const topazString_t * fname,
    
    // raw bytes
    const uint8_t * data, 
    
    // byte count
    uint32_t size
) {
    topazString_t * fullPath = topaz_string_clone(fullpathBase);
    topaz_string_concat(fullPath, TOPAZ_STR_CAST("/"));
    topaz_string_concat(fullPath, fname);
    
    FILE * f = fopen(topaz_string_get_c_str(fullPath), "wb");
    if (f) {
        fwrite(
            data,
            1,
            size,
            f
        );
        fclose(f);
        topaz_string_destroy(fullPath);
        return 1;
    }
    topaz_string_destroy(fullPath);
    return 0;
}


static topazString_t * topaz_filesystem_posix__path_parent(
    topazFilesystem_t * fs,
    void * nu,
    topazString_t * path
) {  
    char * cpy = strdup(topaz_string_get_c_str(path));  
    dirname(cpy);
    topazString_t * out = topaz_string_create_from_c_str(cpy);
    free(cpy);
    return out;
}

static void topaz_filesystem_posix__path_get_children(
    topazFilesystem_t * fs,
    void * nu,
    const topazString_t * path,
    topazArray_t * children
) {
    topaz_array_set_size(children, 0);


	// gather new path objects
    DIR * dObj;
    struct dirent * ls;

    if (!(dObj = opendir(topaz_string_get_c_str(path)))) {
        return;
    }


    
    topazString_t * str;
    topazString_t * fullPath;

    while ((ls = readdir(dObj))) {
        if (!strcmp(ls->d_name, ".") ||
            !strcmp(ls->d_name, "..")) continue;


        
        str = topaz_string_clone(path);
        topaz_string_concat_printf(str, "/%s", ls->d_name);        
        char * pathReal = realpath(topaz_string_get_c_str(str), NULL);

        if (pathReal) {
            fullPath = topaz_string_create_from_c_str(pathReal);                
            topaz_array_push(children, fullPath);
        }
        free(pathReal);
        topaz_string_destroy(str);
    }
    closedir(dObj);
}


static topazString_t * topaz_filesystem_posix__path_string_get_name(
    topazFilesystem_t * fs,
    void * nu,
    const topazString_t * path    
) {
    uint32_t i;
    uint32_t len = topaz_string_get_length(path);
    for(i = len - 1; i != 0; --i) {
        if (topaz_string_get_char(path, i) == '/') {
            return topaz_string_get_substr(path, i+1, len-1);
        }
    }
    return topaz_string_create();
}




static int topaz_filesystem_posix__path_validate(
    topazFilesystem_t * fs,
    void * nu,
    const topazString_t * from, // may be null. If so, assume path is full.
    topazString_t * path
) {

    char * v;
    if (from) {
        topazString_t * full = topaz_string_clone(from);
        topaz_string_concat(full, TOPAZ_STR_CAST("/"));
        topaz_string_concat(full, path);
        v = realpath(topaz_string_get_c_str(full), NULL);
        topaz_string_destroy(full);

    } else {
        v = realpath(topaz_string_get_c_str(path), NULL);
    }
    
    if (v) {
        topaz_string_clear(path);
        topaz_string_concat_printf(path, "%s", v);
        return 1;
    } else {
        return 0;        
    }
}

static intptr_t api_nothing(){return 0;}






void topaz_system_filesystem_posix__backend(
    topazSystem_t *         system, 
    topazSystem_Backend_t * backend, 
    topazFilesystemAPI_t *     api
) {
    topaz_system_backend_bind(
        backend,
        // name
        TOPAZ_STR_CAST("Posix Filesystem"),

        // version 
        TOPAZ_STR_CAST("0.1"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2019"),

        // desc 
        TOPAZ_STR_CAST("Basic filesystem backend for POSIX-compliant systems."),




        // TODO: could implement "partially" async reading using this
        // on step 
        NULL,
        
        // on step late 
        NULL,
        
        // on draw 
        NULL,

        // on draw late
        NULL,



        // backend callback user data
        NULL,


        // API version 
        TOPAZ__VERSION__MAJOR,
        TOPAZ__VERSION__MINOR,
        TOPAZ__VERSION__MICRO
    );

    api->filesystem_create = topaz_filesystem_posix__create;
    api->filesystem_destroy = (void (*)(
        topazFilesystem_t *,
        void *
    ))api_nothing;


    api->filesystem_read = topaz_filesystem_posix__read;
    api->filesystem_write = topaz_filesystem_posix__write;
    api->filesystem_path_parent = topaz_filesystem_posix__path_parent;
    api->filesystem_path_get_children = topaz_filesystem_posix__path_get_children;
    api->filesystem_path_validate = topaz_filesystem_posix__path_validate;
    api->filesystem_path_string_get_name = topaz_filesystem_posix__path_string_get_name;

}






