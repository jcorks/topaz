#include "test.h"
#include <stdio.h>

int test__array_simple();
int test__array_advanced();



int main() {
    topazTest_t * driver = topaz_test_create();
    
    ////////////////////
    //// Add tests here

        topaz_test_add(driver, "Array - Simple  ", test__array_simple);
        topaz_test_add(driver, "Array - Advanced", test__array_advanced);


    //// ^^^^^^^^^^^^^^^
    //////////////////// 

    int result = topaz_test_run(driver);
    if (result) {
        printf("A test has failed.\n");
    }
    return result;
}