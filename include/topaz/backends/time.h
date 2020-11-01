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


#ifndef H_TOPAZDC__TIME__INCLUDED
#define H_TOPAZDC__TIME__INCLUDED

#include <topaz/backends/api/time_api.h>
#include <topaz/system.h>



/*

    Time
    -----
    An abstraction for millisecond resolution timing.
    This short set of utilities allows for timing 

*/

typedef struct topazTime_t topazTime_t;






/// Creates a new time object
///
topazTime_t * topaz_time_create(topaz_t *, topazSystem_Backend_t *, topazTimeAPI_t);


/// Destroys and cleans up a time API
///
void topaz_time_destroy(topazTime_t *);





/// Retrieves the backend for this time object.
///
topazSystem_Backend_t * topaz_time_get_backend(topazTime_t *);


/// Returns the API for this time.
///
topazTimeAPI_t topaz_time_get_api(topazTime_t *);





/// Designates the main thread to sleep for the given 
/// number of milliseconds.
///
void topaz_time_sleep_ms(topazTime_t * t, uint64_t ms);


/// Returns the number of milliseconds that have passed since 
/// the creation of this time instance.
///
uint64_t topaz_time_ms_since_startup(topazTime_t *);



#endif
