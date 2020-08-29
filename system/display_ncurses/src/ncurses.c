#include "backend.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <topaz/backends/renderer.h>
#include <topaz/backends/display.h>
#include <topaz/compat.h>

typedef struct {
    int w;
    int h;
    int charw;
    int charh;
} NCURSESTOPAZ;

static char printChars[0xff+1] = {
    ' ',
    '.',
    '.',
    '.',
    '.',
    '.',
    '.',
    '.',
    '.',
    '.',
    '.',
    '.',
    '.',
    '.',
    '.',
    '.',


    ',',
    ',',
    ',',
    ',',
    ',',
    ',',
    ',',
    ',',
    ',',
    ',',
    ',',
    ',',
    ',',
    ',',
    ',',
    ',',

    '_',
    '_',
    '_',
    '_',
    '_',
    '_',
    '_',
    '_',
    '_',
    '_',
    '_',
    '_',
    '_',
    '_',
    '_',
    '_',

    '-',
    '-',
    '-',
    '-',
    '-',
    '-',
    '-',
    '-',
    '-',
    '-',
    '-',
    '-',
    '-',
    '-',
    '-',
    '-',


    's',
    's',
    's',
    's',
    's',
    's',
    's',
    's',
    's',
    's',
    's',
    's',
    's',
    's',
    's',
    's',


    'r',
    'r',
    'r',
    'r',
    'r',
    'r',
    'r',
    'r',
    'r',
    'r',
    'r',
    'r',
    'r',
    'r',
    'r',
    'r',

    'g',
    'g',
    'g',
    'g',
    'g',
    'g',
    'g',
    'g',
    'g',
    'g',
    'g',
    'g',
    'g',
    'g',
    'g',
    'g',

    '>',
    '>',
    '>',
    '>',
    '>',
    '>',
    '>',
    '>',
    '>',
    '>',
    '>',
    '>',
    '>',
    '>',
    '>',
    '>',







    '$',
    '$',
    '$',
    '$',
    '$',
    '$',
    '$',
    '$',
    '$',
    '$',
    '$',
    '$',
    '$',
    '$',
    '$',
    '$',


    '%',
    '%',
    '%',
    '%',
    '%',
    '%',
    '%',
    '%',
    '%',
    '%',
    '%',
    '%',
    '%',
    '%',
    '%',
    '%',


    '#',
    '#',
    '#',
    '#',
    '#',
    '#',
    '#',
    '#',
    '#',
    '#',
    '#',
    '#',
    '#',
    '#',
    '#',
    '#',


    '&',
    '&',
    '&',
    '&',
    '&',
    '&',
    '&',
    '&',
    '&',
    '&',
    '&',
    '&',
    '&',
    '&',
    '&',







    'S',
    'S',
    'S',
    'S',
    'S',
    'S',
    'S',
    'S',
    'S',
    'S',
    'S',
    'S',
    'S',
    'S',
    'S',
    'S',



    'R',
    'R',
    'R',
    'R',
    'R',
    'R',
    'R',
    'R',
    'R',
    'R',
    'R',
    'R',
    'R',
    'R',
    'R',
    'R',



    'G',
    'G',
    'G',
    'G',
    'G',
    'G',
    'G',
    'G',
    'G',
    'G',
    'G',
    'G',
    'G',
    'G',
    'G',
    'G',

    '@',
    '@',
    '@',
    '@',
    '@',
    '@',
    '@',
    '@',
    '@',
    '@',
    '@',
    '@',
    '@',
    '@',
    '@',
    '@',
    '@',






};

#define GUESS_PIXELS_PER_CHAR__WIDTH  7
#define GUESS_PIXELS_PER_CHAR__HEIGHT 10


void ncurses_display_resize(topazDisplayAPI_t * api, int w, int h) {
    NCURSESTOPAZ * disp = api->implementationData;    
    disp->w = w;
    disp->h = w;
    disp->charw = w/GUESS_PIXELS_PER_CHAR__WIDTH;
    disp->charh = h;
    resizeterm(disp->charw, disp->charh);
    refresh();
}



void ncurses_display_create(topazDisplayAPI_t * api) {
    NCURSESTOPAZ * out = calloc(1, sizeof(NCURSESTOPAZ));
    initscr();
    //raw();
    //keypad(stdscr, TRUE); // TODO: input manager??????

    ncurses_display_resize(api, 640, 480);
    api->implementationData = out;
}
void ncurses_display_destroy(topazDisplayAPI_t * api) {
    free(api->implementationData);
}





void ncurses_display_set_position(topazDisplayAPI_t * api, int x, int y) {
    return; 
}
void ncurses_display_fullscreen(topazDisplayAPI_t * api, int doIT) {
    return;
}

void ncurses_display_hide(topazDisplayAPI_t * api, int doIT) {
    return;
}    

int ncurses_display_has_input_focus (topazDisplayAPI_t * api) {
    return TRUE;
} 

void ncurses_display_lock_client_resize(topazDisplayAPI_t * api, int h) {
    return;    
}

void ncurses_display_lock_client_position(topazDisplayAPI_t * api, int h) {
    return;
}


int  ncurses_display_get_width(topazDisplayAPI_t * api) {
    NCURSESTOPAZ * disp = api->implementationData;    
    return disp->w;
}
int  ncurses_display_get_height(topazDisplayAPI_t * api) {
    NCURSESTOPAZ * disp = api->implementationData;    
    return disp->h;
}


int  ncurses_display_get_x(topazDisplayAPI_t * api) {
    return 0;
}
int  ncurses_display_get_y(topazDisplayAPI_t * api) {
    return 0;
}

void ncurses_display_set_name(topazDisplayAPI_t * api, const topazString_t * str) {
    return;
}

void ncurses_display_add_resize_callback(topazDisplayAPI_t * api, void(*callback)(int w, int h, void *), void * resizeData) {
    return; // TODO
}
void ncurses_display_remove_resize_callback(topazDisplayAPI_t * api, void(*cb)(int w, int h, void *)) {
    return; // TODO;
}
void ncurses_display_add_close_callback(topazDisplayAPI_t * api, void(*cb)(void *), void * resizeData) {
    return; // TODO
}
void ncurses_display_remove_close_callback(topazDisplayAPI_t * api, void(*cb)(void *)) {
    return; // TODO
}


int ncurses_display_is_capable(topazDisplayAPI_t * api, topazDisplay_Capability cap) {
    return topazDisplay_Capability_CanResize; // thats it
}


void ncurses_display_update(topazDisplayAPI_t * api, topazRenderer_Framebuffer_t * fb) {
    NCURSESTOPAZ * disp = api->implementationData;    

    uint8_t * srcIter = topaz_renderer_framebuffer_get_handle(fb);
    int w = topaz_renderer_framebuffer_get_width(fb);
    int h = topaz_renderer_framebuffer_get_height(fb);

    int minw = w/GUESS_PIXELS_PER_CHAR__WIDTH  < disp->charw ? w/GUESS_PIXELS_PER_CHAR__WIDTH  : disp->charw;
    int minh = h/GUESS_PIXELS_PER_CHAR__HEIGHT < disp->charh ? h/GUESS_PIXELS_PER_CHAR__HEIGHT : disp->charh;

    int x;
    int y;
    //printf("FRAME: ____________________\n");

    for(y = 0; y < minh; ++y) {
        move(y, 0);
        uint8_t * iter = srcIter + (4*w*(y*GUESS_PIXELS_PER_CHAR__HEIGHT));
        printf("|");

        for(x = 0; x < minw; ++x) {
            addch(printChars[(uint8_t)((iter[0] + iter[1] + iter[2]) / 3.f)]); // red only?
            //printf("%c", printChars[(uint8_t)((iter[0] + iter[1] + iter[2]) / 3.f)]);
            
            iter += GUESS_PIXELS_PER_CHAR__WIDTH*4;
        }
        //printf("|\n");
    }
    //printf("       ____________________\n");


} 
const topazArray_t * ncurses_display_supported_framebuffers(topazDisplayAPI_t * api) {
    topazRenderer_Framebuffer_Handle h = topazRenderer_Framebuffer_Handle_RGBA_PixelArray;
    return TOPAZ_ARRAY_CAST(&h, int, 1);
}

topazDisplay_Handle ncurses_display_get_system_handle_type(topazDisplayAPI_t * api) {
    return topazDisplay_Handle_Unknown;
}

void * ncurses_display_get_system_handle(topazDisplayAPI_t * api) {
    return NULL;
}

topazDisplay_Event ncurses_display_get_system_event_type(topazDisplayAPI_t * api) {
    return topazDisplay_Event_Unknown;
}


void * ncurses_display_get_last_system_event(topazDisplayAPI_t * api) {
    return NULL;
}

topazArray_t * ncurses_display_get_current_clipboard(topazDisplayAPI_t * api) {
    return NULL;
}

void ncurses_display_set_current_clipboard(topazDisplayAPI_t * api, const topazArray_t * arr) {

}


topazBackend_t * topaz_system_display_ncurses__backend() {
    return topaz_backend_create(
        // name
        TOPAZ_STR_CAST("ncurses"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2020"),

        // desc 
        TOPAZ_STR_CAST("Outputs to the a terminal"),




        // On init
        NULL,

        // On init late
        NULL,

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
}


void topaz_system_display_ncurses__api(topazDisplayAPI_t * api){
    api->display_create = ncurses_display_create;
    api->display_destroy = ncurses_display_destroy;
    api->display_resize = ncurses_display_resize;
    api->display_set_position = ncurses_display_set_position;
    api->display_hide = ncurses_display_hide;
    api->display_has_input_focus = ncurses_display_has_input_focus;
    api->display_lock_client_resize = ncurses_display_lock_client_resize;
    api->display_lock_client_position = ncurses_display_lock_client_position;
    api->display_get_height = ncurses_display_get_height;
    api->display_get_width = ncurses_display_get_width;
    api->display_get_x = ncurses_display_get_x;
    api->display_get_y = ncurses_display_get_y;
    api->display_set_name = ncurses_display_set_name;
    api->display_add_resize_callback = ncurses_display_add_resize_callback;
    api->display_remove_resize_callback = ncurses_display_remove_resize_callback;
    api->display_add_close_callback = ncurses_display_add_close_callback;
    api->display_remove_close_callback = ncurses_display_remove_close_callback;
    api->display_is_capable = ncurses_display_is_capable;
    api->display_update = ncurses_display_update;
    api->display_supported_framebuffers = ncurses_display_supported_framebuffers;
    api->display_get_system_handle_type = ncurses_display_get_system_handle_type;
    api->display_get_system_handle = ncurses_display_get_system_handle;
    api->display_get_system_event_type = ncurses_display_get_system_event_type;
    api->display_get_last_system_event = ncurses_display_get_last_system_event;
    api->display_get_current_clipboard = ncurses_display_get_current_clipboard;
    api->display_set_current_clipboard = ncurses_display_set_current_clipboard;

}
