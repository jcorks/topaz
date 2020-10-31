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
#include <windows.h>
#include <stdio.h>






static intptr_t api_nothing(){return 0;}

typedef struct {
    topazConsoleDisplay_t * ref;
    char inputBuffer[256];
    CONSOLE_READCONSOLE_CONTROL readControl;
    HANDLE file;

} WINAPITerm;



static void update_input(topazSystem_Backend_t * b, WINAPITerm * t) {
    if (t->ref) {
        DWORD readN = 0;
        ReadConsole(
            t->file,
            t->inputBuffer,
            255,
            &readN,
            &t->readControl
        );
        t->inputBuffer[readN] = 0;

        if (readN) {
            topaz_console_display_send_input(t->ref, TOPAZ_STR_CAST(t->inputBuffer));
        }
    }
}

static void * term_init(topazConsoleDisplay_t * d, topaz_t * t) {
    topazSystem_Backend_t * backend = topaz_console_display_get_backend(d);
    WINAPITerm * term = topaz_system_backend_get_user_data(backend);
    term->ref = d;
    return term;
}

static void term_print(topazConsoleDisplay_t * d, void * data, const topazString_t * c, const topazColor_t * reqColor) {
    printf("%s\n", topaz_string_get_c_str(c));
    fflush(stdout);
}


static void term_clear(topazConsoleDisplay_t * d, void * data) {
    system("cls");
    fflush(stdout);
}



void topaz_system_consoleDisplay_winapi__backend(
    topazSystem_t *            system, 
    topazSystem_Backend_t *    backend, 
    topazConsoleDisplayAPI_t * api
) {

    WINAPITerm * term = calloc(1, sizeof(WINAPITerm));
    term->readControl.nLength = sizeof(CONSOLE_READCONSOLE_CONTROL);
    term->readControl.nInitialChars = 0;
    term->readControl.dwCtrlWakeupMask = '\r';
    term->readControl.dwControlKeyState = 0;
    term->file = CreateFile(
        "CONIN$",
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0, NULL
    );



    topaz_system_backend_bind(
        backend,
        // name
        TOPAZ_STR_CAST("winapi"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2020"),

        // desc 
        TOPAZ_STR_CAST("Simple console control for windows systems."),


        // on step 
        NULL,
        
        // on step late 
        (void (*)(topazSystem_Backend_t *, void *)) update_input, // occasionally check
        
        // on draw 
        NULL,

        // on draw late
        NULL,



        // backend callback user data
        term,


        // API version 
        TOPAZ__VERSION__MAJOR,
        TOPAZ__VERSION__MINOR,
        TOPAZ__VERSION__MICRO
    );


    api->console_display_create    = term_init;
    api->console_display_destroy   = (void                    (*)          (topazConsoleDisplay_t *, void *)) api_nothing;
    api->console_display_add_line  = term_print;
    api->console_display_clear     = term_clear;

}







