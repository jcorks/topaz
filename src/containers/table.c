#include <topaz/compat.h>
#include <topaz/containers/table.h>

#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif




#define table_bucket_reserve_size 256
#define table_bucket_start_size 32      
#define table_bucket_max_size 32        //<- larger than this triggers resize


// holds an individual value
typedef struct {
    topazTableEntry_t * next;
    uint32_t hash;
    void * value;
    int keyLen;
} topazTableEntry_t;




// converts data to a hash
typedef uint32_t (KeyHashFunction)(void * data, uint32_t param);

// compares data 
typedef int (KeyCompareFunction)(void * dataA, void * dataB, uint32_t param);





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







// convert a table to a bucket index
static uint32_t hash_to_index(const topazTable_t * t, uint32_t table) {
    return (table*11) % (t->nBuckets);
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
static uint32_t hash_fn_value(void * data) {
    return (uint32_t)data;
}

static int key_cmp_fn_value(void * a, void * b, uint32_t nu) {
    return a==b;
}







static topazTable_t * topaz_table_initialize(topazTable_t * t) {

    t->buckets = calloc(sizeof(topazTableEntry_t*), table_bucket_start_size);
    t->nBuckets = table_bucket_start_size;

    t->size = 0;
}



static topazTableEntry_t * topaz_table_new_entry(topazTable_t * t, void * key, void * data, uint32_t keyLen, uint32_t hash) {
    topazTableEntry_t * out;

    out = malloc(sizeof(topazTableEntry_t));
    out->data = data;

    out->dynamicKeySize = keyLen;
    // if the key is dynamically allocated, we need a local copy 
    if (keyLen) {    
        out->key = malloc(keyLen);
        memcpy(out->key, key, keyLen);
    } else { // else simple copy
        out->key = key;
    }
    out->hash = hash;
}


static topazTableEntry_t * topaz_table_remove_entry(topazTable_t * t, topazTableEntry_t * entry) {
    if (entry->dynamicKeySize) {
        free(entry->data);
    }
    free(entry);
}


topazTable_t * topaz_table_create_hash_pointer() {
    topazTable_t * t = calloc(sizeof(topazTableEntry_t), 1);
    t->hash = hash_fn_value;
    t->keyCmp = key_cmp_fn_value;
    t->keyLen = 0;
    return topaz_table_initialize(t);
}

topazTable_t * topaz_table_create_hash_string() {
    topazTable_t * t = calloc(sizeof(topazTableEntry_t), 1);
    t->hash = hash_fn_buffer;
    t->keyCmp = key_cmp_fn_buffer;
    t->keyLen = -1;
    return topaz_table_initialize(t);
}

topazTable_t * topaz_table_create_hash_buffer(int size) {
    #ifdef TOPAZDC_DEBUG
        assert(size > 0);
    #endif
    topazTable_t * t = calloc(sizeof(topazTableEntry_t), 1);
    t->hash = hash_fn_buffer;
    t->keyCmp = key_cmp_fn_buffer;
    t->keyLen = size;
    return topaz_table_initialize(t);    
}


void topaz_table_destroy(topazTable_t * t) {
    topaz_table_clear(t);
    free(t->buckets);
    free(t);
}


void topaz_table_insert(topazTable_t * t, void * key, void * data) {
    uint32_t hash = t->hash(key);
    uint32_t bucketID = hash_to_index(t, hash);


    topazTableEntry_t * src  = t->buckets[bucketID];
    topazTableEntry_t * prev = NULL;
    int bucketLen = 0;

    // look for preexisting entry
    while(src) {

        if (src->hash == hash) { // hash must equal before key does, so 
                                 // this is an easy check
            if (t->keyCmp(key, src->key, src->keyLen)) {
                // update data for key
                src->data = data;
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
        topaz_table_insert(t, key, data);
        return;
    }
    

    // add to chain at the end
    src = topaz_table_new_entry(
        t, 

        key, 
        data,

        src->keyLen == -1 ? strlen(key) : src->keyLen,
        hash
    );


    if (prev) {
        prev->next = src;
    } else { // start of new chain
        t->buckets[bucketID] = src;
    }
    t->size++;

}



void * topaz_table_find(topazTable_t * t, void * key) {
    uint32_t hash = t->hash(key);
    uint32_t bucketID = hash_to_index(t, hash);


    topazTableEntry_t * src  = t->buckets[bucketID];

    // look for preexisting entry
    while(src) {

        if (src->hash == hash) { // hash must equal before key does, so 
                                 // this is an easy check
            if (t->keyCmp(key, src->key, src->keyLen)) {
                // update data for key
                return data;            
            }
        }
        src = src->next;
    }    
    return NULL;
}

int topaz_table_entry_exists(const topazTable_t * t, void * key) {
    uint32_t hash = t->hash(key);
    uint32_t bucketID = hash_to_index(t, hash);


    topazTableEntry_t * src  = t->buckets[bucketID];

    // look for preexisting entry
    while(src) {

        if (src->hash == hash) { // hash must equal before key does, so 
                                 // this is an easy check
            if (t->keyCmp(key, src->key, src->keyLen)) {
                // update data for key
                return 1;
            }
        }
        src = src->next;
    }    
    return 0;
}


void topaz_table_remove(topazTable_t * t, void * key) {
    uint32_t hash = t->hash(key);
    uint32_t bucketID = hash_to_index(t, hash);


    topazTableEntry_t * src  = t->buckets[bucketID];
    topazTableEntry_t * prev;

    // look for preexisting entry
    while(src) {

        if (src->hash == hash) { // hash must equal before key does, so 
                                 // this is an easy check
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







