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


#ifndef H_TOPAZDC__THREAD__INCLUDED
#define H_TOPAZDC__THREAD__INCLUDED

#include <stdint.h>

/*

    Thread
    -----
    
    A simplified threading primitive.


*/
typedef struct topazThread_t topazThread_t;


/// Creates a new thread with the given main function
///
topazThread_t * topaz_thread_create(int (*main)());

/// Returns the return status of the thread.
/// If the the thread is still running, 
/// -1 is returned.
///
int topaz_thread_status(const topazThread_t *);

/// Attempts to halt the given thread.
///
void topaz_thread_halt(const topazThread_t *);

/// Destroys the thread. In the cause that the thread is still running,
/// this stops the thread.
///
topaz_thread_destroy(topazThread_t *);



/// Updates thread-local data with the given name.
///
void topaz_thread_local_storage_update(const char * id, void *);

/// Returns thread-local data associated with the given name.
/// 
void * topaz_thread_local_storage(const char * id);



#endif
