#include <topaz/containers/string.h>
#include <topaz/containers/table.h>
#include <string.h>
#include <stdint.h>
int test__table_simple() {
    topazTable_t * table = topaz_table_create_hash_c_string();


    topaz_table_insert(table, "Apple", "Fruit");
    topaz_table_insert(table, "Tableeee", "Thing");
    topaz_table_insert(table, "Math", "Subject");

    if (!topaz_table_entry_exists(table, "Apple")) return 1;
    if (!topaz_table_entry_exists(table, "Tableeee")) return 2;
    if (!topaz_table_entry_exists(table, "Math")) return 3;

    
    if (strcmp(topaz_table_find(table, "Apple"), "Fruit"))  return 4;
    if (strcmp(topaz_table_find(table, "Math"), "Subject")) return 5;
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
        if (!topaz_table_entry_exists(table, (void*)i)) return 1;
        if (topaz_table_find(table, (void*)i) != (void*)i) return 2;
    }



    for(i = 0; i < 100000; ++i) {
        if (i%3 == 0)    
            topaz_table_remove(table, (void*)i);
    }
    
    for(i = 0; i < 100000; ++i) {
        uintptr_t value = (uintptr_t)topaz_table_find(table, (void*)i);
        if (value && value%3 == 0)    
            return 3;
    }

    


    topaz_table_destroy(table);

    table = topaz_table_create_hash_topaz_string();


    topaz_table_insert(table, TOPAZ_STR_CAST("Apple"), "Fruit");
    topaz_table_insert(table, TOPAZ_STR_CAST("Tableeee"), "Thing");
    topaz_table_insert(table, TOPAZ_STR_CAST("Math"), "Subject");

    if (!topaz_table_entry_exists(table, TOPAZ_STR_CAST("Apple"))) return 1;
    if (!topaz_table_entry_exists(table, TOPAZ_STR_CAST("Tableeee"))) return 2;
    if (!topaz_table_entry_exists(table, TOPAZ_STR_CAST("Math"))) return 3;

    
    if (strcmp(topaz_table_find(table, TOPAZ_STR_CAST("Apple")), "Fruit"))  return 4;
    if (strcmp(topaz_table_find(table, TOPAZ_STR_CAST("Math")), "Subject")) return 5;
    if (strcmp(topaz_table_find(table, TOPAZ_STR_CAST("Tableeee")), "Thing"))  return 6;

    topaz_table_remove(table, TOPAZ_STR_CAST("Math"));
    
    if (topaz_table_find(table, TOPAZ_STR_CAST("Math"))) return 7;
    if (strcmp(topaz_table_find(table, TOPAZ_STR_CAST("Apple")), "Fruit"))  return 8;
    if (strcmp(topaz_table_find(table, TOPAZ_STR_CAST("Tableeee")), "Thing"))  return 9;


    topaz_table_destroy(table);

    return 0;
}
