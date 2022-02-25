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
#include <assert.h>
#include <topaz/color.h>




static intptr_t api_nothing(){return 0;}

typedef struct {
    topazConsoleDisplay_t * ref;
    char inputBuffer[256];
    CONSOLE_READCONSOLE_CONTROL readControl;
    HANDLE file;
    HANDLE cout;
    topazString_t * str;

} WINAPITerm;

static void winapi_printf(HANDLE * outf, topazString_t * str) {
    DWORD len;
    WriteFile(outf, topaz_string_get_c_str(str), topaz_string_get_length(str), &len, NULL);
    FlushFileBuffers(outf);
    topaz_string_destroy(str);
}

static void update_input(topazSystem_Backend_t * b, WINAPITerm * t) {
    
    // TODO: Unicode ;w; 
    if (t->ref && t->file) {
        INPUT_RECORD input;
        DWORD nu;
        PeekConsoleInput(
            t->file,
            &input,
            1,
            &nu
        );
        int i;
        for(i = 0; i < nu; ++i) {
            ReadConsoleInput(
                t->file,
                &input,
                1,
                &nu
            );
            if (input.EventType == KEY_EVENT && input.Event.KeyEvent.bKeyDown) {
                if (input.Event.KeyEvent.uChar.AsciiChar == 13) { // return
                    winapi_printf(t->cout, topaz_string_create_from_c_str("\n"));
                    topaz_console_display_send_input(t->ref, t->str);
                    topaz_string_clear(t->str);
                } else if (input.Event.KeyEvent.uChar.AsciiChar == 8) { // backspace!
                    if (topaz_string_get_length(t->str)) {
                        topazString_t * temp = t->str;
                        t->str = topaz_string_clone(topaz_string_get_substr(t->str, 0, topaz_string_get_length(t->str)-2));               
                        topaz_string_destroy(temp);
                        winapi_printf(t->cout, topaz_string_create_from_c_str("\b \b"));
                    }
                    
                } else if (!iscntrl(input.Event.KeyEvent.uChar.AsciiChar)) {
                    topaz_string_concat_printf(t->str, "%c", input.Event.KeyEvent.uChar.AsciiChar);
                    winapi_printf(t->cout, topaz_string_create_from_c_str("%c", input.Event.KeyEvent.uChar.AsciiChar));
                } else {
                    //printf("%c(%d)", input.Event.KeyEvent.uChar.AsciiChar, input.Event.KeyEvent.uChar.AsciiChar); fflush(stdout);
                    
                }
            }
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
    WINAPITerm * term = data;
    // from standard SGR custom color specification.
    // Windows willl choose the nearest color for us
    winapi_printf(term->cout, topaz_string_create_from_c_str("\x1b[38;2;%d;%d;%dm",
        (int)(reqColor->r*255),
        (int)(reqColor->g*255),
        (int)(reqColor->b*255)
    ));
    winapi_printf(term->cout, topaz_string_clone(c));
    winapi_printf(term->cout, topaz_string_create_from_c_str("\x1b[0m"));
}

static void term_newline(topazConsoleDisplay_t * d, void * data) {
    WINAPITerm * term = data;
    winapi_printf(term->cout, topaz_string_create_from_c_str("\n"));
    winapi_printf(term->cout, topaz_string_create_from_c_str("\x1b[0m"));
}


static void term_clear(topazConsoleDisplay_t * d, void * data) {

}

static void term_enable(topazConsoleDisplay_t * d, void * data, int enable) {
    WINAPITerm * term = data;
    if (enable && !term->file) {
        FreeConsole();
        AllocConsole();
        DWORD dwMode = 0;
        

        assert(term->file != INVALID_HANDLE_VALUE);
        FILE * d;
        term->file = CreateFileA("CONIN$", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        term->cout = CreateFileA("CONOUT$", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (GetConsoleMode(term->cout, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(term->cout, dwMode);
        }

        if (GetConsoleMode(term->file, &dwMode)) {
            dwMode |= ENABLE_INSERT_MODE;
            SetConsoleMode(term->file, dwMode);
        }


    } else if (term->file && !enable) {
        FreeConsole();
        ShowWindow(GetConsoleWindow(), FALSE);
        term->file = NULL;  
    }
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
    term->str = topaz_string_create();




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
    api->console_display_add_text  = term_print;
    api->console_display_new_line  = term_newline;
    api->console_display_clear     = term_clear;
    api->console_display_enable    = term_enable;
}







