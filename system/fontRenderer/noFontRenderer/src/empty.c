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
#include <string.h>
#include <stdlib.h>

typedef struct {
    char data[6*8];
} GlyphVisual; 



static GlyphVisual nullGlyph = {
    "#.#.#."
    ".#.#.#"
    "#.#.#."
    ".#.#.#"
    "#.#.#."
    ".#.#.#"
    "      "
    "      "
};


static GlyphVisual glyphs[] = {
    {}, {}, {}, {}, {}, {}, {}, {},
    {}, {}, {}, {}, {}, {}, {}, {},
    {}, {}, {}, {}, {}, {}, {}, {},
    {}, {}, {}, {}, {}, {}, {}, {},

    // 33
    {
        "  #   "
        "  #.  "
        "  #.  "
        "  #.  "
        "      "
        "  #.  "
        "      "
        "      "
    },

    // 34
    {
        "      "
        " ## ##"
        " #. #."
        "      "
        "      "
        "      "
        "      "
        "      "
    },
    
    // 35
    {
        "      "
        "  # # "
        " #####"
        "  #.#."
        " #####"
        "  #.#."
        "      "
        "      "
    },

    // 36
    {
        "  #   "
        "-#####"
        "#.#.  "
        "#####."
        "  #. #"
        " #####"
        "  #   "
        "      "
    },


    // 37
    {
        "      "
        " #    "
        "  ..##"
        "####. "
        "      "
        "    # "
        "      "
        "      "
    },

    // 38
    {
        "   #  "
        "   ## "
        "  #.  "
        " #### "
        "  #.  "
        "   ## "
        "   #  "
        "   #  "
    },

    // 39
    {
        "      "
        "  ##  "
        "  #.  "
        "  #   "
        "      "
        "      "
        "      "
        "      "
    },

    // 39
    {
        "      "
        "  ##  "
        "  #.  "
        "  #   "
        "      "
        "      "
        "      "
        "      "
    },

    // 40
    {
        "  #   "
        " #.   "
        "#.    "
        "#     "
        "#     "
        "#.    "
        " #.   "
        "  #   "
    },


    // 41
    {
        "   #  "
        "   .# "
        "    .#"
        "     #"
        "     #"
        "    .#"
        "   .# "
        "   #  "
    },


    // 42
    {
        "   #.#"
        "   .#."
        "   #.#"
        "      "
        "      "
        "      "
        "      "
        "      "
    },


    // 43
    {
        "      "
        "   #  "
        "   #. "
        " #####"
        "   #. "
        "   #  "
        "      "
        "      "
    },


    // 44
    {
        "      "
        "      "
        "      "
        "      "
        "      "
        " #.   "
        " #    "
        "      "
    },

    // 45
    {
        "      "
        "      "
        "      "
        " #### "
        "      "
        "      "
        "      "
        "      "
    },


    // 46
    {
        "      "
        "      "
        "      "
        "      "
        " ##   "
        " ##   "
        "      "
        "      "
    },


    // 47
    {
        "    # "
        "   #. "
        "  .#  "
        "  #   "
        " .#   "
        " #    "
        "      "
        "      "
    },




    // 48
    {
        " ####."
        "#.  ##"
        "#. #.#"
        "#.#. #"
        "##.  #"
        " ####."
        "      "
        "      "
    },

    // 49
    {
        "  ##  "
        "   #. "
        "   #. "
        "   #. "
        "   #. "
        " #### "
        "      "
        "      "
    },

    // 50
    {
        "  ###."
        " #.  #"
        "    #."
        "   #. "
        "  #.  "
        " #####"
        "      "
        "      "
    },

    // 51
    {
        "  ###."
        " #. .#"
        "     #"
        "  ####"
        "    .#"
        " ####."
        "      "
        "      "
    },

    // 52
    {
        "   ## "
        "  #.#."
        " #. #."
        "######"
        "    #."
        "    #."
        "      "
        "      "
    },

    // 53
    {
        " #####"
        " #.   "
        " #.   "
        "  ### "
        "#.  #."
        ".####."
        "      "
        "      "
    },


    // 54
    {
        "   ###"
        " .#.  "
        " #.   "
        "#####."
        "#.   #"
        ".####."
        "      "
        "      "
    },


    // 55
    {
        "######"
        "    #."
        "   #. "
        "  #.  "
        " #.   "
        " #    "
        "      "
        "      "
    },

    // 56
    {
        " ####."
        "#.   #"
        "#.   #"
        "######"
        "#.  .#"
        " ####."
        "      "
        "      "
    },

    // 57
    {
        " ####."
        "#.   #"
        "#.. .#"
        " #####"
        "     #"
        " ####."
        "      "
        "      "
    },

    // 58
    {
        "      "
        "      "
        " #    "
        "      "
        " #    "
        "      "
        "      "
        "      "
    },


    // 59
    {
        "      "
        "      "
        " #    "
        "      "
        "      "
        " #.   "
        " #    "
        "      "
    },

    // 60
    {
        "      "
        "  #   "
        " #.   "
        "#.    "
        " #.   "
        "  #   "
        "      "
        "      "
    },

    // 61
    {
        "      "
        "      "
        "##### "
        "      "
        "##### "
        "      "
        "      "
        "      "
    },



    // 62
    {
        "      "
        "   #. "
        "    #."
        "     #"
        "    #."
        "   #. "
        "      "
        "      "
    },

    // 63
    {
        " ##.  "
        "#. #. "
        "   #. "
        "  #.  "
        "   #  "
        "    .#"
        "    #."
        "      "
    },

    // 64
    {
        " #### "
        "#.  .#"
        "#  ###"
        "#  # #"
        "#  # #"
        "#  .##"
        "#.    "
        " #### "
    },

    // 65
    {
        "  ##  "
        "  ##  "
        " #. # "
        " ####."
        "#.  .#"
        "#    #"
        "      "
        "      "
    },

    // 66
    {
        "#####"
        "##  .#"
        "##   #"
        "######"
        "##  .#"
        "#####."
        "      "
        "      "
    },

    // 67
    {
        " #####"
        "#.    "
        "#.    "
        "#     "
        "##    "
        ".#####"
        "      "
        "      "
    },

    // 68
    {
        "#####"
        "##  .#"
        "##   #"
        "##   #"
        "##  .#"
        "#####."
        "      "
        "      "
    },


    // 69
    {
        " #####"
        "##.   "
        "##    "
        "#####."
        "##.   "
        "######"
        "      "
        "      "
    },


    // 70
    {
        " #####"
        "##    "
        "##    "
        "##### "
        "##    "
        "##    "
        "      "
        "      "
    },


    // 71
    {
        " #####"
        "##.   "
        "##    "
        "##  ##"
        "##  .#"
        "#####."
        "      "
        "      "
    },

    // 72
    {
        "##   #"
        "##   #"
        "##. .#"
        "######"
        "##. .#"
        "##   #"
        "      "
        "      "
    },

    // 73
    {
        "######"
        "  ##. "
        "  ##. "
        "  ##. "
        "  ##. "
        "######"
        "      "
        "      "
    },

    // 74
    {
        "    ##"
        "    ##"
        "    ##"
        "    ##"
        "    #."
        "####. "
        "      "
        "      "
    },

    // 75
    {
        "##   #"
        "##  #."
        "##.#. "
        "####  "
        "##. #."
        "##   #"
        "      "
        "      "
    },

    //76
    {
        "##    "
        "##    "
        "##    "
        "##    "
        "##.   "
        "######"
        "      "
        "      "
    },

    //77
    {
        "##   #"
        "###.##"
        "##.#.#"
        "##.# #"
        "##.# #"
        "## # #"
        "      "
        "      "
    },

    //78
    {
        "##   #"
        "###  #"
        "##.# #"
        "##.# #"
        "##. ##"
        "##  ##"
        "      "
        "      "
    },


    //79
    {
        " ####."
        "#.   #"
        "#    #"
        "#    #"
        "#.   #"
        " ####."
        "      "
        "      "
    },


    //80
    {
        "######"
        "##.  #"
        "##   #"
        "##  .#"
        "#####."
        "##.   "
        "      "
        "      "
    },

    //81
    {
        " ####."
        "#.   #"
        "#    #"
        "#  #.#"
        "#. .#."
        " ###.#"
        "      "
        "      "
    },

    //82
    {
        "##### "
        "##. # "
        "##  # "
        "## #. "
        "###.#"
        "##  .#"
        "      "
        "      "
    },

    //83
    {
        " ####."
        "#.   #"
        " #.   "
        "   #. "
        "#.  ##"
        " ####."
        "      "
        "      "
    },


    //84
    {
        "######"
        "  ##. "
        "  ##. "
        "  ##. "
        "  ##. "
        "  ##  "
        "      "
        "      "
    },    

    //85
    {
        "##   #"
        "##   #"
        "##   #"
        "##   #"
        "##.  #"
        " ####."
        "      "
        "      "
    },

    //86
    {
        "##   #"
        "##  .#"
        "##  #."
        "##  # "
        "##.#. "
        "###.  "
        "      "
        "      "
    },

    //87
    {
        "##   #"
        "##   #"
        "## # #"
        "## # #"
        "##.#.#"
        " ####."
        "      "
        "      "
    },

    //88
    {
        "##   #"
        "##   #"
        "##. .#"
        " ####."
        "##. .#"
        "##   #"
        "      "
        "      "
    },    


    //89
    {
        "##   #"
        "##.  #"
        "##. .#"
        " #### "
        "  ##. "
        "  ##  "
        "      "
        "      "
    }, 

    //90
    {
        "######"
        "    ##"
        "   ##."
        "  ##. "
        " ##.  "
        "######"
        "      "
        "      "
    },   

    //91
    {
        "###   "
        "#     "
        "#     "
        "#     "
        "#     "
        "#     "
        "###   "
        "      "
    },



    //92
    {
        "#.    "
        " #    "
        " #.   "
        " .#   "
        "  #.  "
        "   #  "
        "   #. "
        "      "
    },

    //93
    {
        "   ###"
        "     #"
        "     #"
        "     #"
        "     #"
        "     #"
        "   ###"
        "      "
    },

    
    // 94
    {
        "   #  "
        "  #.# "
        " #. .#"
        "      "
        "      "
        "      "
        "      "
        "      "
    },

    // 95
    {
        "      "
        "      "
        "      "
        "      "
        "      "
        "      "
        "######"
        "      "
    },

    //96
    {
        "   #. "
        "    # "
        "      "
        "      "
        "      "
        "      "
        "      "
        "      "
    },
    
    //97
    {
        "  ##  "
        "  ##  "
        " #. # "
        " ####."
        "#.  .#"
        "#    #"
        "      "
        "      "
    },

    //98
    {
        "#####"
        "##  .#"
        "##   #"
        "######"
        "##  .#"
        "#####."
        "      "
        "      "
    },

    //99
    {
        " #####"
        "#.    "
        "#.    "
        "#     "
        "##    "
        ".#####"
        "      "
        "      "
    },

    //100
    {
        "#####"
        "##  .#"
        "##   #"
        "##   #"
        "##  .#"
        "#####."
        "      "
        "      "
    },


    //101
    {
        " #####"
        "##.   "
        "##    "
        "#####."
        "##.   "
        "######"
        "      "
        "      "
    },


    //102
    {
        " #####"
        "##    "
        "##    "
        "##### "
        "##    "
        "##    "
        "      "
        "      "
    },


    //103
    {
        " #####"
        "##.   "
        "##    "
        "##  ##"
        "##  .#"
        "#####."
        "      "
        "      "
    },

    //104
    {
        "##   #"
        "##   #"
        "##. .#"
        "######"
        "##. .#"
        "##   #"
        "      "
        "      "
    },

    //105
    {
        "######"
        "  ##. "
        "  ##. "
        "  ##. "
        "  ##. "
        "######"
        "      "
        "      "
    },

    //106
    {
        "    ##"
        "    ##"
        "    ##"
        "    ##"
        "    #."
        "####. "
        "      "
        "      "
    },

    //107
    {
        "##   #"
        "##  #."
        "##.#. "
        "####  "
        "##. #."
        "##   #"
        "      "
        "      "
    },

    //108
    {
        "##    "
        "##    "
        "##    "
        "##    "
        "##.   "
        "######"
        "      "
        "      "
    },

    //109
    {
        "##   #"
        "###.##"
        "##.#.#"
        "##.# #"
        "##.# #"
        "## # #"
        "      "
        "      "
    },

    //110
    {
        "##   #"
        "###  #"
        "##.# #"
        "##.# #"
        "##. ##"
        "##  ##"
        "      "
        "      "
    },


    //111
    {
        " ####."
        "#.   #"
        "#    #"
        "#    #"
        "#.   #"
        " ####."
        "      "
        "      "
    },


    //112
    {
        "######"
        "##.  #"
        "##   #"
        "##  .#"
        "#####."
        "##.   "
        "      "
        "      "
    },

    //113
    {
        " ####."
        "#.   #"
        "#    #"
        "#  #.#"
        "#. .#."
        " ###.#"
        "      "
        "      "
    },

    //114
    {
        "##### "
        "##. # "
        "##  # "
        "## #. "
        "###.#"
        "##  .#"
        "      "
        "      "
    },

    //115
    {
        " ####."
        "#.   #"
        " #.   "
        "   #. "
        "#.  ##"
        " ####."
        "      "
        "      "
    },


    //116
    {
        "######"
        "  ##. "
        "  ##. "
        "  ##. "
        "  ##. "
        "  ##  "
        "      "
        "      "
    },    

    //117
    {
        "##   #"
        "##   #"
        "##   #"
        "##   #"
        "##.  #"
        " ####."
        "      "
        "      "
    },

    //118
    {
        "##   #"
        "##  .#"
        "##  #."
        "##  # "
        "##.#. "
        "###.  "
        "      "
        "      "
    },

    //119
    {
        "##   #"
        "##   #"
        "## # #"
        "## # #"
        "##.#.#"
        " ####."
        "      "
        "      "
    },

    //120
    {
        "##   #"
        "##   #"
        "##. .#"
        " ####."
        "##. .#"
        "##   #"
        "      "
        "      "
    },    


    //121
    {
        "##   #"
        "##.  #"
        "##. .#"
        " #### "
        "  ##. "
        "  ##  "
        "      "
        "      "
    }, 

    //122
    {
        "######"
        "    ##"
        "   ##."
        "  ##. "
        " ##.  "
        "######"
        "      "
        "      "
    },       
    
    //123
    {
        "   ## "
        "  #.  "
        "  #.  "
        " ##   "
        "#.    "
        " ##   "
        "  #.  "
        "   ## "
    },

    //124
    {
        "  #   "
        "  #   "
        "  #   "
        "  #   "
        "  #   "
        "  #   "
        "  #   "
        "  #   "

    },

    //125
    {
        " ##.  "
        "   #  "
        "   #  "
        "   ## "
        "    .#"
        "   ## "
        "   #  "
        " ##.  "
    },

    //126
    {
        "      "
        "      "
        " #  # "
        "# ##. "
        "      "
        "      "
        "      "
        "      "
    },
    {}    
  
};


static uint8_t * visual_to_texture(int charcode) {
    GlyphVisual * g;
    if (charcode < 0 || charcode >= (sizeof(glyphs) / sizeof(GlyphVisual))) {
        g = &nullGlyph;
    } else {
        g = glyphs+charcode;
    }

    uint8_t * data = malloc(sizeof(uint32_t)*6*8);
    int i;  
    for(i = 0; i < 6*8; ++i, data+=4) {
        switch(g->data[i]) {
          case '#':
            data[0] = 255;
            data[1] = 255;
            data[2] = 255;
            data[3] = 255;
            break;
          case '.':
            data[0] = 64;
            data[1] = 64;
            data[2] = 64;
            data[3] = 255;
            break;
            
          default:
            data[0] = 0;
            data[1] = 0;
            data[2] = 0;
            data[3] = 0;
            break;
        }
    }
    
    return data;
}



uint8_t * no_font_render(
    /// renderer instance 
    ///
    topazFontRenderer_t * r, 

    /// Renderer userdata
    ///
    void * nu,

    /// Character
    ///
    int charcode, 

    /// Size request for the character in pixels.
    ///
    int sizeRequest,

    /// The output size of the RGBA pixel buffer.
    uint32_t * w,

    /// The output 
    uint32_t * h
) {
    *w = 6;
    *h = 8;
    return visual_to_texture(charcode);

}



void no_query_spacing(
        /// renderer instance 
        ///
        topazFontRenderer_t * r, 

        /// Renderer userdata
        ///
        void * nu,

        /// The spacing information to populate.
        /// The xNextOrigin and yNextOrigin should be 
        /// zero'd when starting a new string of characters,
        /// as the output xNextOrigin and yNextOrigin are relative
        /// to the initial character.
        ///
        topazFontRenderer_Spacing_t * spacing,

        /// Requested size of the font in pixels.
        ///
        int sizePixels, 

        /// The previous character in the string. If none,
        /// should be 0.
        ///
        int prevchar,

        /// The current character to request spacing info for.
        ///
        int thischar,

        /// The previous character in the string. If none,
        /// should be 0.
        ///
        int nextchar

) {
    float aspectRatio = (6.0 / 8.0);
    spacing->width = sizePixels;
    spacing->height = sizePixels*aspectRatio;
    spacing->xOffset = 0;
    spacing->yOffset = 0;
    if (thischar == '\n') {
        spacing->xNextOrigin = 0;
        spacing->yNextOrigin += spacing->height + sizePixels / 6.0;
    } else {
        spacing->xNextOrigin += sizePixels + sizePixels / 6.0;
    }
}


static intptr_t api_nothing(){return 0;}


void topaz_system_fontRenderer_noFontRenderer__backend(
    topazSystem_t *          system, 
    topazSystem_Backend_t *  backend, 
    topazFontRendererAPI_t * api
) {
    topaz_system_backend_bind(
        backend,
        // name
        TOPAZ_STR_CAST("NoFontRenderer"),

        // version 
        TOPAZ_STR_CAST("1.0"),

        // author
        TOPAZ_STR_CAST("Johnathan Corkery, 2020"),

        // desc 
        TOPAZ_STR_CAST("Placeholder font renderer. Produces really low-res glyphs that are prebaked in memory by hand. Made with love <3"),



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

    api->font_renderer_create = (void * (*) (topazFontRenderer_t *, topaz_t *))api_nothing;
    api->font_renderer_destroy = (void   (*) (topazFontRenderer_t *, void *)) api_nothing;

    api->font_renderer_render = no_font_render;
    api->font_renderer_query_spacing = no_query_spacing;

}








