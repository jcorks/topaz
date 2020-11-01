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


#include "backend.h"
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

#include <topaz/compat.h>




typedef struct {
    struct timespec time;
    size_t beginTicks;
} PosixTimeData;



static void * topaz_time_posix__create(topazTime_t * api, topaz_t * ctx) {
    PosixTimeData * tData = calloc(1, sizeof(PosixTimeData));
    
    clock_gettime(CLOCK_REALTIME, &tData->time);
    tData->beginTicks = (tData->time.tv_nsec / 1000000.0) + (tData->time.tv_sec *1000.0);

    return tData;
}






static void topaz_time_posix__destroy(topazTimeAPI_t * t, void * userData) {
    free(userData);
}

static void topaz_time_posix__sleep_ms(topazTimeAPI_t * t, void * userData, uint64_t ms) {
    usleep(ms * 1000);
}

static uint64_t topaz_time_posix__ms_since_startup(topazTimeAPI_t * t, void * userData) {
    PosixTimeData * tData = userData;
    clock_gettime(CLOCK_REALTIME, &tData->time);
    return ((tData->time.tv_nsec / 1000000.0) + (tData->time.tv_sec *1000.0)) - tData->beginTicks;
}








void topaz_system_time_posix__backend(
    topazSystem_t *          system, 
    topazSystem_Backend_t *  backend, 
    topazTimeAPI_t * api
) {
    topaz_system_backend_bind(
        backend,
        // name
        TOPAZ_STR_CAST("Posix Time"),

        // version 
        TOPAZ_STR_CAST("0.1"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2019"),

        // desc 
        TOPAZ_STR_CAST("Basic time backend for POSIX-compliant systems."),





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
    api->time_create  = topaz_time_posix__create;
    api->time_destroy = topaz_time_posix__destroy;
    api->time_sleep_ms = topaz_time_posix__sleep_ms;    
    api->time_ms_since_startup = topaz_time_posix__ms_since_startup;
}







