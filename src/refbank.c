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
#include <topaz/refbank.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif


struct topazRefbank_t {
    topazTable_t * counter;

    void (*accountRemover)(topazRefbank_t *, const void *, void *);
    void * data;
};



topazRefbank_t * topaz_refbank_create() {
    topazRefbank_t * out = calloc(1, sizeof(topazRefbank_t));
    out->counter = topaz_table_create_hash_pointer();
    return out;    
}


void topaz_refbank_destroy(topazRefbank_t * t) {
    topaz_table_destroy(t->counter);
    free(t);
}



void topaz_refbank_deposit(topazRefbank_t * t, const void * account, uint32_t amount) {
    uint32_t oldBalance = (uint32_t)(uintptr_t)topaz_table_find(t->counter, account);    
    oldBalance = (oldBalance + amount < oldBalance ? UINT32_MAX : oldBalance + amount);
    topaz_table_insert(t->counter, account, (void*)(uintptr_t)oldBalance);    
}

void topaz_refbank_increment(topazRefbank_t * t, const void * account) {
    topaz_refbank_deposit(t, account, 1);
}



void topaz_refbank_withdraw(topazRefbank_t * t, const void * account, uint32_t amount) {
    uint32_t curBalance = (uint32_t)(uintptr_t)topaz_table_find(t->counter, account);
    if (curBalance) {
        curBalance = (curBalance < amount ? curBalance : amount);
        if (!curBalance) {
            if (t->accountRemover) t->accountRemover(t, account, t->data);
            topaz_table_remove(t->counter, account);            
        } else {
            topaz_table_insert(t->counter, account, (void*)(uintptr_t)curBalance);
        }
    }
}


void topaz_refbank_decrement(topazRefbank_t * t, const void * account) {
    topaz_refbank_withdraw(t, account, 1);
}


uint32_t topaz_refbank_balance(const topazRefbank_t * t, const void * account) {
    return (uint32_t)(uintptr_t)topaz_table_find(t->counter, account);
}


void topaz_refbank_remove_all(topazRefbank_t * t) {
    topazTableIter_t * iter = topaz_table_iter_create();

    if (t->accountRemover) {
        for( topaz_table_iter_start(iter, t->counter); 
            !topaz_table_iter_is_end(iter);
             topaz_table_iter_proceed(iter)) {

            if (topaz_table_iter_get_value(iter)) {
                t->accountRemover(
                    t,
                    topaz_table_iter_get_key(iter),
                    t->data
                );
            }

        }
    }

    topaz_table_clear(t->counter);
}

void topaz_refbank_set_account_remover(
    topazRefbank_t * t,
    void (*accountRemover)(topazRefbank_t * t, const void * account, void * userData), 
    void * data
) {
    t->accountRemover = accountRemover;
    t->data = data;
}
