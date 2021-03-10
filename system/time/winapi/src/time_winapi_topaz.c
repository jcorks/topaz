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





#include "backend.h"
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <windows.h>
#include <topaz/compat.h>




typedef struct {
    double beginTicks;
    LARGE_INTEGER freq;
} WINAPITimeData;



static void * topaz_time_winapi__create(topazTime_t * api, topaz_t * ctx) {
    WINAPITimeData * tData = calloc(1, sizeof(WINAPITimeData));
    QueryPerformanceFrequency(&tData->freq);
    LARGE_INTEGER p;
    QueryPerformanceCounter(&p);
    tData->beginTicks = (p.QuadPart / (double)(tData->freq.QuadPart)) * 1000;
    return tData;
}






static void topaz_time_winapi__destroy(topazTime_t * t, void * userData) {
    free(userData);
}


static double topaz_time_winapi__ms_since_startup(topazTime_t * t, void * userData) {
    WINAPITimeData * tData = userData;
    LARGE_INTEGER p;
    QueryPerformanceCounter(&p);
    return (p.QuadPart / (double)(tData->freq.QuadPart)) * 1000 - tData->beginTicks;
}

static void topaz_time_winapi__sleep_ms(topazTime_t * t, void * userData, double ms) {
    // TODO: remove use of this. This is "recommended" by windows for the multimedia timer 
    // to change, essentially, the minimum sleep time allowable to be around 1 ms if available.
    // Under most circumstances, the system sleep interval without this may be 1-15ms
    // which is not useable for multimedia. timeBeginPeriod, prior to windows 10 2004,
    // affected the ENTIRE system, which is why timeEndPeriod() is required to "undo" 
    // this change. This, the only safe way to deal with this is to constantly enable and disable it
    timeBeginPeriod(1);
        Sleep(ms);
    timeEndPeriod(1);
}







void topaz_system_time_winapi__backend(
    topazSystem_t *          system, 
    topazSystem_Backend_t *  backend, 
    topazTimeAPI_t * api
) {

    topaz_system_backend_bind(
        backend,
        // name
        TOPAZ_STR_CAST("WINAPI Time"),

        // version 
        TOPAZ_STR_CAST("0.1"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2021"),

        // desc 
        TOPAZ_STR_CAST("Basic time backend for windows systems."),





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
    api->time_create  = topaz_time_winapi__create;
    api->time_destroy = topaz_time_winapi__destroy;
    api->time_sleep_ms = topaz_time_winapi__sleep_ms;    
    api->time_ms_since_startup = topaz_time_winapi__ms_since_startup;
}







