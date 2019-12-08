#include "test.h"
#include <stdio.h>

int test__array_simple();
int test__array_advanced();

int test__table_simple();
int test__table_advanced();

int test__string_simple();
int test__string_advanced();

int main() {
    topazTest_t * driver = topaz_test_create();
    
    ////////////////////
    //// Add tests here

        topaz_test_add(driver, "Array  - Simple  ",   test__array_simple);
        topaz_test_add(driver, "Array  - Advanced",   test__array_advanced);
        topaz_test_add(driver, "Table  - Simple  ",   test__table_simple);
        topaz_test_add(driver, "Table  - Advanced",   test__table_advanced);
        topaz_test_add(driver, "String - Simple  ",     test__string_simple);
        topaz_test_add(driver, "String - Advanced",   test__string_advanced);


    //// ^^^^^^^^^^^^^^^
    //////////////////// 

    int result = topaz_test_run(driver);
    topaz_test_destroy(driver);
    if (result) {
        printf("A test has failed.\n");
    }
    return result;
}
