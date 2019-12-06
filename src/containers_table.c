#include <topaz/compat.h>
#include <topaz/containers/table.h>

#include <string.h>
#include <stdlib.h>
#include <stdint.h>


#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif




#define table_bucket_reserve_size 256
#define table_bucket_start_size 32      
#define table_bucket_max_size 32        //<- larger than this triggers resize


// holds an individual key-value pair
typedef struct topazTableEntry_t topazTableEntry_t;
struct topazTableEntry_t {
    topazTableEntry_t * next;
    uint32_t hash;
    int keyLen;
    void * value;
    void * key;
};




// converts data to a hash
typedef uint32_t (*KeyHashFunction)(void * data, uint32_t param);

// compares data 
typedef int (*KeyCompareFunction)(void * dataA, void * dataB, uint32_t param);





struct topazTable_t {
    // numer of keys
    uint32_t size;

    // number of buckets
    uint32_t nBuckets;

    // actual buckets
    topazTableEntry_t ** buckets;



    // Converts key data to a hash
    KeyHashFunction hash;

    // Compares keys
    KeyCompareFunction keyCmp;
    
    // for static key sizes. If -1, is dynamic (i.e. strings)
    // if 0, the key has no allocated size (pointer / value direct keys)
    int keyLen;



    // TODO: instead of freeing them, just put them back into a reserve 
    // stack (thread-local reserve pool?)

};




struct topazTableIter_t {
    // source table
    topazTable_t * src;

    // current reference entry
    topazTableEntry_t * current;

    // current bucket in reference
    uint32_t currentBucketID;

    // whether the iterator has reached the end.
    int isEnd;
};



// convert a table to a bucket index
static uint32_t hash_to_index(const topazTable_t * t, uint32_t hash) {
    return (hash*11) % (t->nBuckets);
}


// djb
static uint32_t hash_fn_buffer(uint8_t * data, uint32_t len) {
    uint32_t hash = 5381;

    uint32_t i;
    for(i = 0; i < len; ++i, ++data) {
        hash = (hash<<5) + hash + *data;
    } 
    return hash;
}

static int key_cmp_fn_buffer(void * a, void * b, uint32_t len) {
    return memcmp(a, b, len);
}




// pointer / value to a table directly
static uint32_t hash_fn_value(void * data, uint32_t nu) {
    return (uint32_t)(int64_t)data;
}

static int key_cmp_fn_value(void * a, void * b, uint32_t nu) {
    return a==b;
}





// resizes and redistributes all key-value pairs
static void topaz_table_resize(topazTable_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazTable_t pointer cannot be NULL.");
    #endif
    topazTableEntry_t ** entries = malloc(sizeof(topazTableEntry_t)*t->size);
    topazTableEntry_t * entry;
    topazTableEntry_t * next;
    topazTableEntry_t * prev;

    uint32_t nEntries = 0, i, index;
    topazTableIter_t iter;


    // first, gather ALL key-value pairs
    memset(&iter, 0, sizeof(topazTableIter_t));
    topaz_table_iter_start(&iter, t);
    for(; !topaz_table_iter_is_end(&iter); topaz_table_iter_proceed(&iter)) {
        entries[nEntries++] = iter.current;
    }
    

    // then resize
    t->nBuckets *= 2;
    free(t->buckets);
    t->buckets = calloc(t->nBuckets, sizeof(topazTableEntry_t*));


    // redistribute in-place
    for(i = 0; i < nEntries; ++i) {
        entry = entries[i];
        entry->next = NULL;

        index = hash_to_index(t, entry->hash);


        next = t->buckets[index];
        prev = NULL;    
        while(next) {
            prev = next;
            next = next->next;
        }


        if (next == prev) {
            t->buckets[index] = entry;
        } else {
            prev->next = entry;
        }
    }
        

    
}





static topazTable_t * topaz_table_initialize(topazTable_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazTable_t pointer cannot be NULL.");
    #endif

    t->buckets = calloc(sizeof(topazTableEntry_t*), table_bucket_start_size);
    t->nBuckets = table_bucket_start_size;

    t->size = 0;
    return t;
}



static topazTableEntry_t * topaz_table_new_entry(topazTable_t * t, void * key, void * value, uint32_t keyLen, uint32_t hash) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazTable_t pointer cannot be NULL.");
    #endif

    topazTableEntry_t * out;

    out = malloc(sizeof(topazTableEntry_t));
    out->value = value;

    out->keyLen = keyLen;
    // if the key is dynamically allocated, we need a local copy 
    if (keyLen) {    
        out->key = malloc(keyLen);
        memcpy(out->key, key, keyLen);
    } else { // else simple copy
        out->key = key;
    }
    out->hash = hash;
    return out;
}


static void topaz_table_remove_entry(topazTable_t * t, topazTableEntry_t * entry) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazTable_t pointer cannot be NULL.");
        assert(entry && "topazTableEntry_t pointer cannot be NULL.");
    #endif

    if (entry->keyLen) {
        free(entry->value);
    }
    free(entry);
}


topazTable_t * topaz_table_create_hash_pointer() {
    topazTable_t * t = calloc(sizeof(topazTableEntry_t), 1);
    t->hash   = hash_fn_value;
    t->keyCmp = key_cmp_fn_value;
    t->keyLen = 0;
    return topaz_table_initialize(t);
}

topazTable_t * topaz_table_create_hash_string() {
    topazTable_t * t = calloc(sizeof(topazTableEntry_t), 1);
    t->hash   = (KeyHashFunction)hash_fn_buffer;
    t->keyCmp = key_cmp_fn_buffer;
    t->keyLen = -1;
    return topaz_table_initialize(t);
}

topazTable_t * topaz_table_create_hash_buffer(int size) {
    #ifdef TOPAZDC_DEBUG
        assert(size > 0 && "topaz_table_create_hash_buffer() requires non-zero size.");
    #endif
    topazTable_t * t = calloc(sizeof(topazTableEntry_t), 1);
    t->hash   = (KeyHashFunction)hash_fn_buffer;
    t->keyCmp = key_cmp_fn_buffer;
    t->keyLen = size;
    return topaz_table_initialize(t);    
}


void topaz_table_destroy(topazTable_t * t) {
    topaz_table_clear(t);
    free(t->buckets);
    free(t);
}


void topaz_table_insert(topazTable_t * t, void * key, void * value) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazTable_t pointer cannot be NULL.");
    #endif
    uint32_t keyLen = t->keyLen == -1 ? strlen(key) : t->keyLen;
    uint32_t hash = t->hash(key, keyLen);
    uint32_t bucketID = hash_to_index(t, hash);


    topazTableEntry_t * src  = t->buckets[bucketID];
    topazTableEntry_t * prev = NULL;
    int bucketLen = 0;

    // look for preexisting entry
    while(src) {

        if (src->hash   == hash && 
            src->keyLen == keyLen) { // hash must equal before key does, so 
                                 // this is an easy check
            if (t->keyCmp(key, src->key, src->keyLen)) {
                // update data for key
                src->value = value;
                return;            
            }
        }
        prev = src;
        src = src->next;
        bucketLen++;
    }    

    // invariant broken, expand and reform the hashtable.
    if (bucketLen > table_bucket_max_size) {
        topaz_table_resize(t);

        // try again.
        topaz_table_insert(t, key, value);
        return;
    }
    

    // add to chain at the end
    src = topaz_table_new_entry(
        t, 

        key, 
        value,

        keyLen,
        hash
    );


    if (prev) {
        prev->next = src;
    } else { // start of new chain
        t->buckets[bucketID] = src;
    }
    t->size++;

}



void * topaz_table_find(const topazTable_t * t, void * key) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazTable_t pointer cannot be NULL.");
    #endif
    uint32_t keyLen = t->keyLen == -1 ? strlen(key) : t->keyLen;
    uint32_t hash = t->hash(key, keyLen);
    uint32_t bucketID = hash_to_index(t, hash);


    topazTableEntry_t * src  = t->buckets[bucketID];

    // look for preexisting entry
    while(src) {

        if (src->hash   == hash && 
            src->keyLen == keyLen) { // hash must equal before key does, so 
                                 // this is an easy check
            if (t->keyCmp(key, src->key, src->keyLen)) {

                return src->value;
            }
        }
        src = src->next;
    }    
    return NULL;
}

int topaz_table_entry_exists(const topazTable_t * t, void * key) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazTable_t pointer cannot be NULL.");
    #endif
    uint32_t keyLen = t->keyLen == -1 ? strlen(key) : t->keyLen;
    uint32_t hash = t->hash(key, keyLen);
    uint32_t bucketID = hash_to_index(t, hash);


    topazTableEntry_t * src  = t->buckets[bucketID];

    // look for preexisting entry
    while(src) {

        if (src->hash   == hash && 
            src->keyLen == keyLen) { // hash must equal before key does, so 
            if (t->keyCmp(key, src->key, src->keyLen)) {
                return TRUE;
            }
        }
        src = src->next;
    }    
    return FALSE;
}


void topaz_table_remove(topazTable_t * t, void * key) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazTable_t pointer cannot be NULL.");
    #endif
    uint32_t keyLen = t->keyLen == -1 ? strlen(key) : t->keyLen;
    uint32_t hash = t->hash(key, keyLen);
    uint32_t bucketID = hash_to_index(t, hash);


    topazTableEntry_t * src  = t->buckets[bucketID];
    topazTableEntry_t * prev = NULL;

    // look for preexisting entry
    while(src) {

        if (src->hash   == hash && 
            src->keyLen == keyLen) { // hash must equal before key does, so 
            if (t->keyCmp(key, src->key, src->keyLen)) {

                if (prev) {
                    // skip over entry
                    prev->next = src->next;
                } else {
                    t->buckets[bucketID] = src->next;                    
                }
                topaz_table_remove_entry(t, src);
                t->size--;

                return;
            }
        }
        prev = src;
        src = src->next;
    }    
}


void topaz_table_clear(topazTable_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazTable_t pointer cannot be NULL.");
    #endif
    uint32_t i = 0;
    topazTableEntry_t * src;
    topazTableEntry_t * toRemove;

    for(; i < t->nBuckets; ++i) {
        src = t->buckets[i];
        while(src) {
            toRemove = src;
            src = toRemove->next;

            topaz_table_remove_entry(t, toRemove);
        }

        t->buckets[i] = NULL;
    }

}





topazTableIter_t * topaz_table_iter_create() {
    return calloc(sizeof(topazTableIter_t), 1);
}

void topaz_table_iter_destroy(topazTableIter_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazTableIter_t pointer cannot be NULL.");
    #endif
    free(t);
}


void topaz_table_iter_start(topazTableIter_t * t, topazTable_t * src) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazTableIter_t pointer cannot be NULL.");
        assert(src && "topazTable_t pointer cannot be NULL.");
    #endif
    t->src = src;
    t->currentBucketID = 0;
    t->current = src->buckets[0];

    
    if (!t->current)
        topaz_table_iter_proceed(t);  

}

void topaz_table_iter_proceed(topazTableIter_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazTableIter_t pointer cannot be NULL.");
    #endif
    if (t->isEnd) return;
    
    if (t->current) {
        t->current = t->current->next;

        // iter points to next in bucket
        if (t->current);
            return;
    }

    
    // need to move to next buckt
    uint32_t i = t->currentBucketID;
    for(; i < t->src->nBuckets; ++i) {
        if (t->src->buckets[i]) {
            t->current = t->src->buckets[i];
            t->currentBucketID = i;
            return;
        }        
    }

    t->current = NULL;
    t->isEnd = TRUE;
}

int topaz_table_iter_is_end(const topazTableIter_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazTableIter_t pointer cannot be NULL.");
    #endif
    return t->isEnd;
}

void * topaz_table_iter_get_key(const topazTableIter_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazTableIter_t pointer cannot be NULL.");
    #endif
    if (t->current) {
        return t->current->key;
    }
    return NULL;
}

void * topaz_table_iter_get_value(const topazTableIter_t * t) {
    #ifdef TOPAZDC_DEBUG
        assert(t && "topazTableIter_t pointer cannot be NULL.");
    #endif
    if (t->current) {
        return t->current->value;
    }
    return NULL;
}









