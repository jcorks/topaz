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


#ifndef H_TOPAZDC__HASH__INCLUDED
#define H_TOPAZDC__HASH__INCLUDED

/*

    Hash
    -----

    Dynamically resizing container


*/
typedef struct topazHash_t topazHash_t;


topazHash_t * topaz_hash_create_type_string();
topazHash_t * topaz_hash_create_type_buffer(uint32_t n);
topazHash_t * topaz_hash_create_type_pointer();

void topaz_hash_insert(topazHash_t *, void * key, void * value);

void * topaz_hash_find(topazHash_t *, void * key);

void topaz_hash_remove(topazHash_t *, void *);

void topaz_hash_clear(topazHash_t *);


/*

    HashIter
    --------

    Helper class for iterating through hash tables


*/
typedef struct topazHashIter_t topazHashIter_t;
topazHashIter_t * topaz_hash_iter_create();
void topaz_hash_iter_start(topazHashIter_t *, topazHash_t *);
void topaz_hash_iter_next(topazHashIter_t *);
int topaz_hash_iter_end(const topazHashIter_t *);

void * topaz_hash_iter_get_key(const topazHashIter_t *);
void * topaz_hash_iter_get_value(const topazHashIter_t *);