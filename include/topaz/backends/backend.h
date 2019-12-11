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


#ifndef H_TOPAZDC__BACKEND__INCLUDED
#define H_TOPAZDC__BACKEND__INCLUDED

/*

    Backend
    -----
    
    Contains standardized information about the backend.


*/

typedef struct topazBackend_t topazBackend_t;


topazBackend_t * topaz_backend_create(
    /// The name of the backend.
    const topazString_t * name,

    /// The version of the backend
    const topazString_t * version,

    /// The author of the backend
    const topazString_t * author,

    /// A description of the backend
    const topazString_t * description,





    /// Called when initializing the backend
    int (*on_init)(void *);

    /// Called after all on_inits are called.
    int (*on_init_late)(void *);

    /// Called when the backend is stepped    
    void (*on_step)(void *);

    /// Called after all the backends are stepped
    void (*on_step_late)(void *);

    /// Called when the backend is drawn
    void (*on_draw)(void *);

    /// Called after all the backends have drawn
    void (*on_draw_late(void *)

);


/// Returns the major version of the topaz library 
/// that this backend was built with.
///
int topaz_backend_get_topaz_major_version(const topazBackend_t *);

/// Returns the minor version of the topaz library 
/// that this backend was built with.
///
int topaz_backend_get_topaz_minor_version(const topazBackend_t *);

/// Returns the micro version of the topaz library 
/// that this backend was build with.
///
int topaz_backend_get_topaz_micro_version(const topazBackend_t *);



/// Gets the name of this backend.
///
const topazString_t * topaz_backend_get_name   (const topazBackend_t *);


/// Gets the name of this backend.
///
const topazString_t * topaz_backend_get_version(const topazBackend_t *);

/// Gets the author name 
///
const topazString_t * topaz_backend_get_author (const topazBackend_t *);

/// Gets the description 
///
const topazString_t * topaz_backend_get_author (const topazBackend_t *);




/// Initializes the backend.
///
void topaz_backend_init(topazBackend_t *);

/// Flags that initialization has finished.
///
void topaz_backend_init_late(topazBackend_t *);

/// Steps the backend
///
void topaz_backend_step(topazBackend_t *);

/// Posts that initialization has finished
///
void topaz_backend_step_late(topazBackend_t *);

/// Draws the backend
///
void topaz_backend_draw(topazBackend_t *);

/// Posts that initialization has finished
///
void topaz_backend_draw_late(topazBackend_t *);









#endif
