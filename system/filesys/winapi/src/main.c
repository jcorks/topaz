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









#include <stdlib.h>
#include <stdint.h>
#include <topaz/compat.h>
#include <string.h>
#include <windows.h>
#include <direct.h> // _getcwd
#include <stdio.h>
#include "backend.h"

typedef struct {
    topazArray_t * dirObjects;
    topazArray_t * dirIsFile;
    topazString_t * currentPath;
} WINAPIFilesysData;



static void * topaz_filesys_winapi__create(topazFilesys_t * fsys, topaz_t * ctx) {
    WINAPIFilesysData * tData = calloc(1, sizeof(WINAPIFilesysData));
    tData->dirObjects = topaz_array_create(sizeof(topazString_t*));
    tData->dirIsFile = topaz_array_create(sizeof(int));
    char * path = malloc(PATH_MAX+1);
    _getcwd(path, PATH_MAX);
    tData->currentPath = topaz_string_create_from_c_str(path);
    free(path);
    return tData;
}






static void topaz_filesys_winapi__destroy(topazFilesys_t * fsys, void * userData) {
    WINAPIFilesysData * fs = userData;
    topaz_array_destroy(fs->dirObjects);
    topaz_array_destroy(fs->dirIsFile);
    topaz_string_destroy(fs->currentPath);

    free(userData);
}


static void topaz_filesys_winapi_populate_objects(WINAPIFilesysData * fs) {
	// clear out old directory objects

    uint32_t i;
    for(i = 0; i < topaz_array_get_size(fs->dirObjects); ++i) {
        topaz_string_destroy(topaz_array_at(fs->dirObjects, topazString_t *, i));
    }
    topaz_array_set_size(fs->dirObjects, 0);
    topaz_array_set_size(fs->dirIsFile, 0);

	// gather new path objects
    HANDLE dObj;
    WIN32_FIND_DATA ls;

    dObj = FindFirstFile(topaz_string_get_c_str(fs->currentPath), &ls);
    if (dObj == INVALID_HANDLE_VALUE) {
        return;
    }


    
    topazString_t * str;
    topazString_t * pathFull = topaz_string_create();
    int isFile;

    do {
        if (!strcmp(ls.cFileName, ".") ||
            !strcmp(ls.cFileName, "..")) continue;


        str = topaz_string_create_from_c_str(ls.cFileName);
        topaz_string_clear(pathFull);
        topaz_string_concat(pathFull, fs->currentPath);
        topaz_string_concat(pathFull, str);

        if (ls.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  // directory detected
            isFile = FALSE;    
        else
            isFile = TRUE;

        topaz_array_push(fs->dirObjects, str);
        topaz_array_push(fs->dirIsFile, isFile);

    } while (FindNextFile(dObj, &ls));

    FindClose(dObj);
    topaz_string_destroy(pathFull);    

	// dump file names in base vector;
    
}


static int topaz_filesys_winapi__set_path(topazFilesys_t * fsys, void * userData, const topazString_t * str) {
    WINAPIFilesysData * fs = userData;
    
    int pathMax = MAX_PATH;
    char pathTemp[pathMax+1];
  	if (!GetFullPathNameA(topaz_string_get_c_str(str), MAX_PATH, pathTemp, NULL)) 
        return FALSE;



    
    topaz_string_set(fs->currentPath, str);
    topaz_filesys_winapi_populate_objects(fs);

	return TRUE;
}

static int topaz_filesys_winapi__go_to_child(topazFilesys_t * fsys, void * userData, const topazString_t * str) {
    WINAPIFilesysData * fs = userData;
    topazString_t * child = topaz_string_clone(fs->currentPath);
    topaz_string_concat_printf(child, "%c", '/');
    topaz_string_concat(child, str); 
    int result = topaz_filesys_winapi__set_path(fsys, userData, child);
    topaz_string_destroy(child);
    return result;
}

static int topaz_filesys_winapi__go_to_parent(topazFilesys_t * fsys, void * userData) {
    WINAPIFilesysData * fs = userData;
    const char * iter = topaz_string_get_c_str(fs->currentPath);
	int index = topaz_string_get_length(fs->currentPath) - 1;
    
    // TODO: this is a bug carried over from the previous implementation. Should be fixed.
	while (iter[index] != '\\') index--;
    
    const topazString_t * part = topaz_string_get_substr(fs->currentPath, 0, index);
	int ret = chdir(topaz_string_get_c_str(part));
    return ret;
}

static const topazString_t * topaz_filesys_winapi__get_path(topazFilesys_t * fsys, void * userData) {
    WINAPIFilesysData * fs = userData;
    return fs->currentPath;    
}

static int topaz_filesys_winapi__create_node(topazFilesys_t * fsys, void * userData, const topazString_t * dir) {
    WINAPIFilesysData * fs = userData;

    topazString_t * newPath = topaz_string_clone(fs->currentPath);
    topaz_string_concat_printf(newPath, "%c", '/');
    topaz_string_concat(newPath, dir);
	int result = CreateDirectoryA(topaz_string_get_c_str(newPath), NULL);    
    topaz_string_destroy(newPath);
    return result;
}


static topazRbuffer_t * topaz_filesys_winapi__read(topazFilesys_t * fsys, void * userData, const topazString_t * dir) {
    WINAPIFilesysData * fs = userData;
    topazString_t * fullPath = topaz_string_clone(fs->currentPath);
    topaz_string_concat_printf(fullPath, "%c", '/');
    topaz_string_concat(fullPath, dir);
    
    FILE * f = fopen(topaz_string_get_c_str(fullPath), "rb");
    topazRbuffer_t * buffer = topaz_rbuffer_create();
    if (f) {
        topazArray_t * fullData = topaz_array_create(sizeof(uint8_t));

        // read in chunks
        #define chunk_size_read (2048*48)    
        uint8_t chunk[chunk_size_read];
        uint32_t chunkRead;

        while((chunkRead = fread(chunk, 1, chunk_size_read, f))) {
            topaz_array_push_n(fullData, chunk, chunkRead);
        }

        topaz_rbuffer_open(
            buffer, 
            topaz_array_get_data(fullData),
            topaz_array_get_size(fullData)
        );
         
        topaz_array_destroy(fullData);
        fclose(f);

    }
    topaz_string_destroy(fullPath);
    return buffer;
}

static int topaz_filesys_winapi__write(
    topazFilesys_t * fsys, void * userData, 
    const topazString_t * fname, 
    const topazWbuffer_t * data) 
{
    WINAPIFilesysData * fs = userData;
    topazString_t * fullPath = topaz_string_clone(fs->currentPath);
    topaz_string_concat_printf(fullPath, "%c", '/');
    topaz_string_concat(fullPath, fname);
    
    const topazArray_t * arr = topaz_wbuffer_get_data(data);
    FILE * f = fopen("wb", topaz_string_get_c_str(fullPath));
    if (f) {
        fwrite(
            topaz_array_get_data(arr),
            1,
            topaz_array_get_size(arr),
            f
        );
        fclose(f);
    }

    topaz_string_destroy(fullPath);
    return f!=NULL;
}


static const topazArray_t * topaz_filesys_winapi__query(topazFilesys_t * fsys, void * userData) {
    WINAPIFilesysData * fs = userData;
    return fs->dirObjects;
}

static int topaz_filesys_winapi__is_node(topazFilesys_t * fsys, void * userData, const topazString_t * t) {
    WINAPIFilesysData * fs = userData;
    uint32_t i;
    for(i = 0; i < topaz_array_get_size(fs->dirObjects); ++i) {
        if (topaz_string_test_eq(
                topaz_array_at(fs->dirObjects, topazString_t*, i),
                t
            )) {
            return !topaz_array_at(fs->dirIsFile, int, i);
        }
    }
    return FALSE;
}

static int topaz_filesys_winapi__is_child(topazFilesys_t * fsys, void * userData, const topazString_t * t) {
    WINAPIFilesysData * fs = userData;
    uint32_t i;
    for(i = 0; i < topaz_array_get_size(fs->dirObjects); ++i) {
        if (topaz_string_test_eq(
                topaz_array_at(fs->dirObjects, topazString_t*, i),
                t
            )) {
            return TRUE;
        }
    }
    return FALSE;
}
const topazArray_t * topaz_filesys_winapi__split_path(topazFilesys_t * fsys, void * userData, const topazString_t * path) {
    char separator;
    separator = '\\';

    static topazArray_t * arr = 0;
    if (!arr) arr = topaz_array_create(sizeof(topazString_t *));    
    uint32_t len = topaz_array_get_size(arr);
    uint32_t i;
    for(i = 0; i < len; ++i) {
        topaz_string_destroy(topaz_array_at(arr, topazString_t *, i));
    }
    topaz_array_set_size(arr, 0);


    topazString_t * str = topaz_string_create();
    len = topaz_string_get_length(path);
    for(i = 0; i < len; ++i) {
        int c = topaz_string_get_char(path, i);
        if (c == separator) {
            if (topaz_string_get_length(str)) {
                topaz_array_push(arr, str);
                str = topaz_string_create();
            }
        } else {
            topaz_string_concat_printf(str, "%c", c);
        }
    }

    if (topaz_string_get_length(str)) {
        topaz_array_push(arr, str);
        str = topaz_string_create();
    }


    return arr;
}



void topaz_system_filesys_winapi__backend(
    topazSystem_t *         system, 
    topazSystem_Backend_t * backend, 
    topazFilesysAPI_t *     api
) {
    topaz_system_backend_bind(
        backend,
        // name
        TOPAZ_STR_CAST("WINAPI Filesystem"),

        // version 
        TOPAZ_STR_CAST("0.1"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2019"),

        // desc 
        TOPAZ_STR_CAST("Basic filesystem backend for windows systems."),




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


    // Map object API functions to real ones
    api->filesys_create  = topaz_filesys_winapi__create;
    api->filesys_destroy = topaz_filesys_winapi__destroy;

    api->filesys_set_path = topaz_filesys_winapi__set_path;
    api->filesys_go_to_child = topaz_filesys_winapi__go_to_child;
    api->filesys_go_to_parent = topaz_filesys_winapi__go_to_parent;
    api->filesys_get_path = topaz_filesys_winapi__get_path;
    api->filesys_create_node = topaz_filesys_winapi__create_node;
    api->filesys_read = topaz_filesys_winapi__read;
    api->filesys_write = topaz_filesys_winapi__write;
    api->filesys_query = topaz_filesys_winapi__query;
    api->filesys_is_node = topaz_filesys_winapi__is_node;
    api->filesys_is_child = topaz_filesys_winapi__is_child;
    api->filesys_split_path = topaz_filesys_winapi__split_path;


}






