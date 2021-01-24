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


#ifndef H_TOPAZDC__TIME_API__INCLUDED
#define H_TOPAZDC__TIME_API__INCLUDED

#include <stdint.h>

typedef struct topaz_t topaz_t;
typedef struct topazTime_t topazTime_t;

/*

    TimeAPI
    -----
    
    The set of functions that define how the time abstraction should 
    behave.

    These API functions are called as underlying implementations for the symbols 
    within <topaz/backends/time.h> and provide a way for custom, possibly 
    system-dependent behavior to account for an environment in a robust way.

*/
typedef struct topazTimeAPI_t topazTimeAPI_t;


// Each function is an implementation-facing copy of 
// the user-side API for topazTime_t. See <topaz/backends/Time.h>
//
struct topazTimeAPI_t {
    void *                  (*time_create)           (topazTime_t *, topaz_t *);
    void                    (*time_destroy)          (topazTime_t *, void *);

    void                    (*time_sleep_ms)         (topazTime_t *, void *, uint64_t);
    uint64_t                (*time_ms_since_startup) (topazTime_t *, void *);


};



#endif
