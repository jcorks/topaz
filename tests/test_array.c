#include <topaz/containers/array.h>

int test__array_simple() {
    topazArray_t * arr = topaz_array_create(sizeof(uint64_t));
    if (topaz_array_get_size(arr) != 0) {
        // Why: A new array should have 0 elements.
        return 1;
    }

    uint64_t val = 20;
    topaz_array_push(arr, val);

    val = 42;
    topaz_array_push(arr, val);

    val = 98;
    topaz_array_push(arr, val);

    if (topaz_array_get_size(arr) != 3) {
        // Why: There should be 3 elements within the array.
        return 2;
    }



    uint32_t i;
    for(i = 0; i < 3; ++i) {
        if (topaz_array_at(arr, uint64_t, 0) != 20 ||
            topaz_array_at(arr, uint64_t, 1) != 42 ||
            topaz_array_at(arr, uint64_t, 2) != 98) {
            // Why: arrays always push at the end and contain the 3 values listed
            return 3;
        }
    }

    topaz_array_remove(arr, 1);
    if (topaz_array_at(arr, uint64_t, 1) != 98 ||
        topaz_array_at(arr, uint64_t, 0) != 20 ||
        topaz_array_get_size(arr) != 2) {
        // Why: element "42" was removed and the array should reflect that
        return 4;
    }


    topaz_array_clear(arr);
    if (topaz_array_get_size(arr) != 0) {
        // Why: clear should remove all elements
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
            // Why: every element should be added for each i of test size in order.
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
        // Why: half of all elements were removed (all even elements)
        return 2;
    }


    for(i = 0; i < testsize/2; ++i) {
        // Why: even elements shouldnt exist since they were removed
        if (topaz_array_at(arr, uint32_t, i) % 2 == 0) {
            return 3;            
        }
    }
    topazArray_t * copy = topaz_array_clone(arr);

    for(i = 0; i < testsize/2; ++i) {
        // Why: the cloned array should also not have any even elements
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
        // Why: the second index (value 15) is the correct lower bound for this array.        
        return 5;
    }

    i = 9;
    if (topaz_array_lower_bound(copy, &i, int_cmp)!=0) {
        // Why: the 0th index (value 10) is the correct lower bound for 9 since every value is below it
        return 6;
    }
    
    i = 1221;
    if (topaz_array_lower_bound(copy, &i, int_cmp)!=topaz_array_get_size(copy)) {
        // Why: the size'th imaginary index is the correct lower bound for 1221 since every value is above it
        return 7;
    }



    i = 103;
    topaz_array_insert(copy, 5, i);

    i = 104;
    if (topaz_array_get_size(copy) != 8 ||
        topaz_array_lower_bound(copy, &i, int_cmp) != 6 ||
        topaz_array_at(copy, int, 5) != 103) {
        // Why: the array had a value added, setting it to size 8,
        //      the proper index is 6 and the 5th index is 103 since we
        //      are inserting the value /at/ index 5
        return 8;
    }
    

    topaz_array_destroy(copy);
    topaz_array_destroy(arr);

    {
        
        int arrS[] = {
            102,
            314,
            11,
            42,
            4422,
            110000
        };
        const topazArray_t * arr = TOPAZ_ARRAY_CAST(arrS, int, 6);
        if (topaz_array_get_size(arr) != 6) {
            // Why: The array should be of size 6, as thats what we 
            //      designated in the "cast"
            return 9;
        }

        if (topaz_array_at(arr, int, 2) != 11 ||
            topaz_array_at(arr, int, 5) != 110000) {

            // Why: the test values @ 2 and 5 do not correspond to what was specified.   
            return 10;
        }
    }
    return 0;
}
