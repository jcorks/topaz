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
#include <topaz/containers/array.h>
#include <topaz/containers/table.h>
#include <topaz/color.h>

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif


static topazTable_t * dictionary = NULL;


static void define(const char * nameSrc, const char * clr) {
    topazColor_t * c = calloc(1, sizeof(topazColor_t));
    *c = topaz_color_from_string(TOPAZ_STR_CAST(clr));

    // maintain case-insensitivity 
    char * name = strdup(nameSrc);
    uint32_t i;
    uint32_t len = strlen(name);
    for(i = 0; i < len; ++i) {
        name[i] = tolower(name[i]);
    }

    topaz_table_insert(
        dictionary,
        TOPAZ_STR_CAST(name),
        c
    );
    free(name);
}

static void load_default_colors() {
    dictionary = topaz_table_create_hash_topaz_string();
    define("Alice Blue",     ("#f0f8ff"));
    define("Antique white",  ("#faebd7"));
    define("Aqua",           ("#00ffff"));
    define("Aquamarine",     ("#7fffd4"));
    define("Azure",          ("#f0ffff"));


    define("Beige",          ("#f5f5dc"));
    define("Bisque",         ("#FFe4c4"));
    define("Black",          ("#000000"));
    define("Blanched almond",("#ffebcd"));
    define("Blue",           ("#0000ff"));
    define("Blue Violet",    ("#8a2be2"));
    define("Brown",          ("#a52a2a"));
    define("Burlywood",      ("#deb887"));


    define("Cadet blue",     ("#5F9EA0"));
    define("Chartreuse",     ("#7fff00"));
    define("Chocolate",      ("#d2691E"));
    define("Coral",          ("#FF7F50"));
    define("Cornflower",     ("#6495ed"));
    define("Cornsilk",       ("#fff8dc"));
    define("Crimson",        ("#dc143c"));
    define("Cyan",           ("#00ffff"));


    define("Dark Blue",      ("#00008b"));
    define("Dark Cyan",      ("#008b8b"));
    define("Dark Goldenrod", ("#B8860b"));
    define("Dark Gray",      ("#a9a9a9"));
    define("Dark Green",     ("#006400"));
    define("Dark Khaki",     ("#BdB76B"));
    define("Dark Magenta",   ("#8B008B"));
    define("Dark Olive Green",("#556b2f"));
    define("Dark Orange",    ("#FF8c00"));
    define("Dark Orchid",    ("#9932cc"));
    define("Dark Red",       ("#8B0000"));
    define("Dark Salmon",    ("#e9967a"));
    define("Dark Sea Green", ("#8FBc8f"));
    define("Dark Slate Blue",("#483d8b"));
    define("Dark Slate Gray",("#2f4f4f"));
    define("Dark Turquoise", ("#00ced1"));
    define("Dark Violet",    ("#9400d3"));
    define("Deep Pink",      ("#ff1493"));
    define("Deep Sky Blue",  ("#00bfff"));
    define("Dim Gray",       ("#696969"));
    define("Dodger Blue",    ("#1e90ff"));


    define("Firebrick",      ("#822222"));
    define("Floral White",   ("#FFFAF0"));
    define("Forest green",   ("#228b22"));
    define("Fuchsia",        ("#ff00ff"));



    define("Gainsboro",      ("#dcdcdc"));
    define("Ghost white",    ("#f8f8ff"));
    define("Gold",           ("#FFd700"));
    define("Goldenrod",      ("#daa520"));
    define("Gray",           ("#bebebe"));
    define("Web Gray",       ("#808080"));
    define("Green",          ("#00FF00"));
    define("Web Green",      ("#008000"));
    define("Green Yellow",   ("#adff2f"));


    define("Honeydew",       ("#f0fff0"));
    define("Hot Pink",       ("#ff69bf"));

    define("Indian Red",     ("#cd5c5c"));
    define("Indigo",         ("#4b0082"));
    define("Ivory",          ("#fffff0"));

    define("Khaki",          ("#f0e68c"));

    define("Lavender",       ("#e6e6fa"));
    define("Lavender Blush", ("#fff0f5"));
    define("Lawn Green",     ("#7cfc00"));
    define("Lemon Chiffon",  ("#fffacd"));
    define("Light Blue",     ("#add8e6"));
    define("Light Coral",    ("#f08080"));
    define("Light Cyan",     ("#eoffff"));
    define("Light Goldenrod",("#fafad2"));
    define("Light Gray",     ("#d3d3d3"));
    define("Light Green",    ("#90ee90"));
    define("Light Pink",     ("#ffb6c1"));
    define("Light Salmon",   ("#ffa07a"));
    define("Light Sea Green",("#20b2aa"));
    define("Light Sky Blue", ("#87cefa"));
    define("Light Slate Gray",("#778899"));
    define("Light Steel Blue",("#b0c6de"));
    define("Light Yellow",   ("#ffffe0"));
    define("Lime",           ("#00FF00"));
    define("Lime Green",     ("#3dcd32"));
    define("Linen",          ("#faf0e6"));



    define("Mangenta",       ("#FF00FF"));
    define("Maroon",         ("#bo3060"));
    define("Web Maroon",     ("#7f0000"));
    define("Medium Aquamarine", ("#66cdaa"));
    define("Medium Blue",    ("#0000cd"));
    define("Medium Orchid",  ("#ba55d3"));
    define("Medium Purple",  ("#9370d8"));
    define("Medium Sea Green", ("#3cb371"));
    define("Medium Slate Blue",("#7b68ee"));
    define("Medium Spring Green",("#00fa9a"));
    define("Medium Turquoise", ("#48d1cc"));
    define("Medium Violet Red",("#c71585"));
    define("Midnight Blue",  ("#191970"));
    define("Mint Cream",     ("#F5fffa"));
    define("Misty Rose",     ("#ffe4e1"));
    define("Moccasin",       ("#ffe4b5"));


    define("Navajo White",   ("#ffdead"));
    define("Navy Blue",      ("#000080"));


    define("Old Lace",       ("#fdf5e6"));
    define("Olive",          ("#808000"));
    define("Olive Drab",     ("#6b8e23"));
    define("Orange",         ("#FFa500"));
    define("Orange Red",     ("#ff4500"));
    define("Orchid",         ("#da70d6"));


    define("Pale Goldenrod", ("#eee8aa"));
    define("Pale Green",     ("#98fb98"));
    define("Pale Turquoise", ("#afeeee"));
    define("Pale Violet Red",("#db7093"));
    define("Papaya Whip",    ("#ffefd5"));
    define("Peach Puff",     ("#ffdab9"));
    define("Peru",           ("#cd853f"));
    define("Pink",           ("#ffc0cb"));
    define("Plum",           ("#ddaodd"));
    define("Powder Blue",    ("#boe0e6"));
    define("Purple",         ("#a020f0"));
    define("Web Purple",     ("#7f007f"));

    define("Rebecca Purple", ("#663399"));
    define("Red",            ("#FF0000"));
    define("Rosy Brown",     ("#BC8F8F"));
    define("Royal Blue",     ("#4169E1"));


    define("Saddle Brown",   ("#8B4513"));
    define("Salmon",         ("#FA8072"));
    define("Sandy Brown",    ("#F4A460"));
    define("Sea Green",      ("#2e8b57"));
    define("Seashell",       ("#fff5ee"));
    define("Sienna",         ("#A0522d"));
    define("Silver",         ("#c0c0c0"));
    define("Sky Blue",       ("#87ceeb"));
    define("Slate Blue",     ("#6A5ACD"));
    define("Slate Gray",     ("#708090"));
    define("Snow",           ("#FFFAFA"));
    define("Spring Green",   ("#00FF7f"));
    define("Steel blue",     ("#4682b4"));

    define("Tan",            ("#D2B48c"));
    define("Teal",           ("#008080"));
    define("Thistle",        ("#D8BFD8"));
    define("Tomato",         ("#FF6347"));
    define("Turquoise",      ("#40E0D0"));
    define("Violet",         ("#ee82ee"));
    define("Wheat",          ("#F5DEB3"));
    define("White",          ("#ffffff"));
    define("White Smoke",    ("#f5f5f5"));
    define("Yellow",         ("#ffff00"));
    define("Yellow Green",   ("#9acd32"));
}

topazColor_t topaz_color_from_string(const topazString_t * name) {
    if (!dictionary) {
        dictionary = (void*)0x1;
        load_default_colors();
    }    

    topazColor_t out;

    // case insensitive
    topazString_t * str = topaz_string_clone(name);
    uint32_t i;
    uint32_t len = topaz_string_get_length(str);
    for(i = 0; i < len; ++i) {
        topaz_string_set_char(
            str,
            i,
            tolower(
                topaz_string_get_char(str, i)
            )
        );
    }

   
    // empty color
    out.a = 0xff;
    out.r = 0;
    out.g = 0;
    out.b = 0;

    // convert hex
    if ((topaz_string_get_char(str, 0) == '#' && (len == 7 || len == 9))) {
        const topazString_t * hex = topaz_string_get_substr(str, 1, len-1);
        uint8_t cl = 0;
        uint8_t val = 0;
        int i;
        len = topaz_string_get_length(hex);
        for(i = 0; i < len; ++i) {
            val = tolower(topaz_string_get_char(hex, i));
            val = (val >= '0' && val <= '9' ? val - '0' : (val - 'a' + 10));
            cl += val*pow(16, i%2==0?1:0);

            switch(i) {
              case 1: out.r = cl; cl = 0; break;
              case 3: out.g = cl; cl = 0; break;
              case 5: out.b = cl; cl = 0; break;
              case 7: out.a = cl; cl = 0; break;
              default:;
            }
        }
    } else {
        topazColor_t * c = topaz_table_find(dictionary, str);
        if (c) {
            out = *c;
        }
    }
    topaz_string_destroy(str);
    return out;
}


int topaz_color_to_rgba_int(topazColor_t c) {
    return *((int*)&c);
}

#define COLOR_CLAMP(__T__) (__T__ < 0.f ? 0.f : (__T__ > 1.f ? 1.f : __T__))

topazColor_t topaz_color_from_amt(float r, float g, float b, float a) {
    topazColor_t out;
    out.r = COLOR_CLAMP(r);
    out.g = COLOR_CLAMP(g);
    out.b = COLOR_CLAMP(b);
    out.a = COLOR_CLAMP(a);
    return out;
}


topazColor_t topaz_color_from_int(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    topazColor_t out;
    out.r = r;
    out.g = g;
    out.b = b;
    out.a = a;
    return out;
}

topazString_t * topaz_color_to_string(topazColor_t c) {
    char cstr[30];
    sprintf(cstr, "#%02x%02x%02x%02x",
        c.r,
        c.g,
        c.b,
        c.a 
    );
    return topaz_string_create_from_c_str(cstr);
}



