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
#include <topaz/camera.h>
#include <topaz/entity.h>


#include <stdlib.h>
#include <string.h>

typedef struct {
    topazRenderer_Buffer_t * mv;
    topazRenderer_Buffer_t * proj;
    topazCamera_Type type;

    topazRenderer_Framebuffer_t * fb;
    topazRenderer_Framebuffer_t * fbAux;
} TopazCamera;



static void camera__on_step(topazEntity_t * );


topazEntity_t * topaz_camera_create(topaz_t * t) {
    TopazCamera * c = calloc(1, sizeof(TopazCamera));
    topazRenderer_t * r = topaz_context_get_backend_renderer(t);
    c->mv = topaz_renderer_buffer_reate()


}