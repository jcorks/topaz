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


#ifndef H_TOPAZDC__TABLE__INCLUDED
#define H_TOPAZDC__TABLE__INCLUDED

/*

    Table
    -----

    Dynamically resizing container


*/
typedef struct topazTable_t topazTable_t;


topazTable_t * topaz_table_create_hash_string();
topazTable_t * topaz_table_create_hash_buffer(int n);
topazTable_t * topaz_table_create_hash_pointer();


// copy of the key is made: value copy if a pointer, 
// shallow copy if string or buffer
void topaz_table_insert(topazTable_t *, void * key, void * value);


void * topaz_table_find(const topazTable_t *, void * key);


int topaz_table_entry_exists(const topazTable_t *, void * key);

void topaz_table_remove(topazTable_t *, void *);

void topaz_table_clear(topazTable_t *);


/*

    TableIter
    --------

    Helper class for iterating through hash tables


*/
typedef struct topazTableIter_t topazTableIter_t;
topazTableIter_t * topaz_table_iter_create();
void topaz_table_iter_start(topazTableIter_t *, topazTable_t *);
void topaz_table_iter_next(topazTableIter_t *);
int topaz_table_iter_end(const topazTableIter_t *);

void * topaz_table_iter_get_key(const topazTableIter_t *);
void * topaz_table_iter_get_value(const topazTableIter_t *);
