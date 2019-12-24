#include <topaz/containers/array.h>

int test__array_simple() {
    topazArray_t * arr = topaz_array_create(sizeof(uint64_t));
    if (topaz_array_get_size(arr) != 0) {
        return 1;
    }

    uint64_t val = 20;
    topaz_array_push(arr, val);

    val = 42;
    topaz_array_push(arr, val);

    val = 98;
    topaz_array_push(arr, val);


    if (topaz_array_get_size(arr) != 3) {
        return 2;
    }



    uint32_t i;
    for(i = 0; i < 3; ++i) {
        if (topaz_array_at(arr, uint64_t, 0) != 20 ||
            topaz_array_at(arr, uint64_t, 1) != 42 ||
            topaz_array_at(arr, uint64_t, 2) != 98) {
            return 3;
        }
    }

    topaz_array_remove(arr, 1);
    if (topaz_array_at(arr, uint64_t, 1) != 98 ||
        topaz_array_at(arr, uint64_t, 0) != 20 ||
        topaz_array_get_size(arr) != 2) {
        return 4;
    }


    topaz_array_clear(arr);
    if (topaz_array_get_size(arr) != 0) {
        return 5;
    }

    topaz_array_destroy(arr);
    return 0;
}

static int int_cmp(const void * a, const void * b) {
    return *(int*)a < *(int*)b;
}

int test__array_advanced() {
    uint32_t i = 0;
    topazArray_t * arr = topaz_array_create(sizeof(uint32_t));
    #define testsize 10000
    for(; i < testsize; ++i) {
        topaz_array_push(arr, i);
    }

    for(i = 0; i < testsize; ++i) {
        if (topaz_array_at(arr, uint32_t, i) != i) {
            return 1;
        }
    }

    for(i = 0; i < topaz_array_get_size(arr); ++i) {
        if (topaz_array_at(arr, uint32_t, i) % 2 == 0) {
            topaz_array_remove(arr, i);
            i--;
        }
    }

    if (topaz_array_get_size(arr) != testsize/2) {
        return 2;
    }


    for(i = 0; i < testsize/2; ++i) {
        if (topaz_array_at(arr, uint32_t, i) % 2 == 0) {
            return 3;            
        }
    }
    topazArray_t * copy = topaz_array_clone(arr);

    for(i = 0; i < testsize/2; ++i) {
        if (topaz_array_at(copy, uint32_t, i) % 2 == 0) {
            return 4;            
        }
    }


    topaz_array_set_size(copy, 0);
    i = 10; topaz_array_push(copy, i);
    i = 15; topaz_array_push(copy, i);
    i = 100; topaz_array_push(copy, i);
    i = 101; topaz_array_push(copy, i);
    i = 101; topaz_array_push(copy, i);
    i = 121; topaz_array_push(copy, i);
    i = 1210; topaz_array_push(copy, i);
    
    i = 40;
    if (topaz_array_lower_bound(copy, &i, int_cmp)!=2) {
        return 5;
    }

    i = 9;
    if (topaz_array_lower_bound(copy, &i, int_cmp)!=0) {
        return 6;
    }
    
    i = 1221;
    if (topaz_array_lower_bound(copy, &i, int_cmp)!=topaz_array_get_size(copy)) {
        return 7;
    }



    i = 103;
    topaz_array_insert(copy, 5, i);

    i = 104;
    if (topaz_array_get_size(copy) != 8 ||
        topaz_array_lower_bound(copy, &i, int_cmp) != 6 ||
        topaz_array_at(copy, int, 5) != 103) {
        return 8;
    }
    

    topaz_array_destroy(copy);
    topaz_array_destroy(arr);

    return 0;
}
