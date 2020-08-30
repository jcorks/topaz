#include <topaz/containers/string.h>
#include <topaz/containers/table.h>
#include <string.h>
#include <stdint.h>
int test__table_simple() {
    topazTable_t * table = topaz_table_create_hash_c_string();


    topaz_table_insert(table, "Apple", "Fruit");
    topaz_table_insert(table, "Tableeee", "Thing");
    topaz_table_insert(table, "Math", "Subject");

    // Why: Apple was missing!
    if (!topaz_table_entry_exists(table, "Apple")) return 1;

    // Why: Tableeee was missing!
    if (!topaz_table_entry_exists(table, "Tableeee")) return 2;

    // Why: Math was missing!
    if (!topaz_table_entry_exists(table, "Math")) return 3;

    
    // Why: Apple had the incorrect value
    if (strcmp(topaz_table_find(table, "Apple"), "Fruit"))  return 4;

    // Why: Math had the incorrect value
    if (strcmp(topaz_table_find(table, "Math"), "Subject")) return 5;

    // Why: Tableeee had the incorrect value
    if (strcmp(topaz_table_find(table, "Tableeee"), "Thing"))  return 6;


    topaz_table_destroy(table);

    return 0;

}

int test__table_advanced() {
    topazTable_t * table = topaz_table_create_hash_pointer();
    uintptr_t i = 0;
    for(i = 0; i < 100000; ++i) {
        topaz_table_insert(table, (void*)i, (void*)i);
    }

    for(i = 0; i < 100000; ++i) {
        // Why: Missing i value
        if (!topaz_table_entry_exists(table, (void*)i)) return 11;

        // Why: Incorrect table value
        if (topaz_table_find(table, (void*)i) != (void*)i) return 12;
    }



    for(i = 0; i < 100000; ++i) {
        if (i%3 == 0)    
            topaz_table_remove(table, (void*)i);
    }
    
    for(i = 0; i < 100000; ++i) {
        uintptr_t value = (uintptr_t)topaz_table_find(table, (void*)i);
        if (value && value%3 == 0)    
            // Why: all integers that were divisible by 3 were removed
            return 13;
    }

    


    topaz_table_destroy(table);

    table = topaz_table_create_hash_topaz_string();


    topaz_table_insert(table, TOPAZ_STR_CAST("Apple"), "Fruit");
    topazTableIter_t * iter = topaz_table_iter_create();
    int size = 0;
    for(topaz_table_iter_start(iter, table);
        !topaz_table_iter_is_end(iter);
        topaz_table_iter_proceed(iter)) {
        
        size++;
    }
    if (size != 1) return 10;

    topaz_table_insert(table, TOPAZ_STR_CAST("Tableeee"), "Thing");
    topaz_table_insert(table, TOPAZ_STR_CAST("Math"), "Subject");


    // Why: Apple was missing!
    if (!topaz_table_entry_exists(table, TOPAZ_STR_CAST("Apple"))) return 1;

    // Why: Tableeee was missing!
    if (!topaz_table_entry_exists(table, TOPAZ_STR_CAST("Tableeee"))) return 2;

    // Why: Math was missing!
    if (!topaz_table_entry_exists(table, TOPAZ_STR_CAST("Math"))) return 3;

    
    // Why: Apple had the incorrect value
    if (strcmp(topaz_table_find(table, TOPAZ_STR_CAST("Apple")), "Fruit"))  return 4;

    // Why: Math had the incorrect value
    if (strcmp(topaz_table_find(table, TOPAZ_STR_CAST("Math")), "Subject")) return 5;

    // Why: Tableeee had the incorrect value
    if (strcmp(topaz_table_find(table, TOPAZ_STR_CAST("Tableeee")), "Thing"))  return 6;

    topaz_table_remove(table, TOPAZ_STR_CAST("Math"));
    

    // Why: Math was removed
    if (topaz_table_find(table, TOPAZ_STR_CAST("Math"))) return 7;

    // Why: Apple key/val should be unchanged
    if (strcmp(topaz_table_find(table, TOPAZ_STR_CAST("Apple")), "Fruit"))  return 8;

    // Why: Apple key/val should be unchanged
    if (strcmp(topaz_table_find(table, TOPAZ_STR_CAST("Tableeee")), "Thing"))  return 9;


    topaz_table_destroy(table);

    return 0;
}
