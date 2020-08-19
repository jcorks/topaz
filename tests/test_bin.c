#include <topaz/containers/bin.h>
#include <topaz/containers/array.h>
#include <topaz/compat.h>

#include <string.h>
#include <stdint.h>

int test__bin_simple() {
    topazBin_t * bin = topaz_bin_create();

    topazArray_t * arr =topaz_bin_get_all(bin);
    if (topaz_array_get_size(arr) != 0) {
        // Why: empty bins shouldnt have any elements
        return 1;
    }
    topaz_array_destroy(arr);
    
    char * ptr1 = "ptr1";
    char * ptr2 = "ptr2";
    char * ptr3 = "ptr3";
    char * ptr4 = "ptr4";

    uint32_t ptr1_id;
    uint32_t ptr2_id;
    uint32_t ptr3_id;
    uint32_t ptr4_id;

    ptr1_id = topaz_bin_add(bin, ptr1);
    ptr2_id = topaz_bin_add(bin, ptr2);
    ptr3_id = topaz_bin_add(bin, ptr3);
    
    if (ptr1_id == ptr2_id ||
        ptr2_id == ptr3_id ||
        ptr1_id == ptr3_id) {
        // Why: all IDs should be unique
        return 2;
    }


    if (!(
            topaz_bin_fetch(bin, ptr1_id) == ptr1 &&
            topaz_bin_fetch(bin, ptr2_id) == ptr2 &&
            topaz_bin_fetch(bin, ptr3_id) == ptr3
        )) {
        // Why: each id needs to correspond to the ptr assigned from it.
        return 3;
    }

    topaz_bin_remove(bin, ptr2_id);

    if (!(
            topaz_bin_contains(bin, ptr1_id) == 1 &&
            topaz_bin_contains(bin, ptr2_id) == 0 &&
            topaz_bin_contains(bin, ptr3_id) == 1

        )) {
        // Why: the bin contains 1 and 3, but not 2.
        return 4;
    }

    if (!(
            topaz_bin_fetch(bin, ptr1_id) == ptr1 &&
            topaz_bin_fetch(bin, ptr2_id) == NULL &&
            topaz_bin_fetch(bin, ptr3_id) == ptr3
        )) {
        // Why: 2 should be NULL, but 1 and 3 shoudl be unaffected.
        return 5;
    }

    
    arr = topaz_bin_get_all(bin);
    if (topaz_array_get_size(arr) != 2) {
        // Why: the bin only contains 2 entries at this time.
        return 6;
    }

    uint32_t i;
    for(i = 0; i < topaz_array_get_size(arr); ++i) {
        if (!(topaz_array_at(arr, void*, i) == ptr1 ||
              topaz_array_at(arr, void*, i) == ptr3)) {
            // Why: the bin should only contain both ptr1 and ptr3 
            return 7;
        }
    }

    topaz_array_destroy(arr);



    ptr4_id = topaz_bin_add(bin, ptr4);
    ptr2_id = topaz_bin_add(bin, ptr2);

    
    if (!(
            topaz_bin_fetch(bin, ptr1_id) == ptr1 &&
            topaz_bin_fetch(bin, ptr2_id) == ptr2 &&
            topaz_bin_fetch(bin, ptr3_id) == ptr3 &&
            topaz_bin_fetch(bin, ptr4_id) == ptr4
        )) {
        // Why: All 4 items should exist and be added
        return 8;
    }

    topaz_bin_destroy(bin);


    return 0;

}

int test__bin_advanced() {
    // TODO:
    return 0;
}
