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
#include <topaz/version.h>
#include <topaz/system.h>







static intptr_t api_nothing(){return 0;}

void topaz_system_consoleDisplay_noConsoleDisplay__backend(
    topazSystem_t *            system, 
    topazSystem_Backend_t *    backend, 
    topazConsoleDisplayAPI_t * api
) {
    topaz_system_backend_bind(
        backend,
        // name
        TOPAZ_STR_CAST("NoConsoleDisplay"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2020"),

        // desc 
        TOPAZ_STR_CAST("Placeholder for a console display. Not ideal for debugging!"),


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

    api->console_display_create    = (void *                  (*)           (topazConsoleDisplay_t *, topaz_t *)) api_nothing;
    api->console_display_destroy   = (void                    (*)          (topazConsoleDisplay_t *, void *)) api_nothing;
    api->console_display_add_line  = (void                    (*)         (topazConsoleDisplay_t *, void *, const topazString_t *, const topazColor_t * reqColor)) api_nothing;
    api->console_display_clear     = (void                    (*)            (topazConsoleDisplay_t *, void *)) api_nothing;
    api->console_display_update    = (void                    (*)           (topazConsoleDisplay_t *, void *)) api_nothing;

}







