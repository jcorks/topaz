#include "test.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEST_COUNT 4096*2

typedef struct {
    char * label;
    int (*fn)();
} topazTestEntry_t;


struct topazTest_t {
    topazTestEntry_t tests[MAX_TEST_COUNT];
    uint32_t count;
};


topazTest_t * topaz_test_create() {
    return calloc(1, sizeof(topazTest_t));
}

void topaz_test_destroy(topazTest_t * t) {
    uint32_t i;
    for(i = 0; i < t->count; ++i) {
        free(t->tests[i].label);
    }
    free(t);
}

void topaz_test_add(topazTest_t * t, const char * label, int (*fn)()) {
    topazTestEntry_t * entry = t->tests+t->count++;
    entry->label = strdup(label);
    entry->fn = fn;
}

int topaz_test_run(topazTest_t * t) {
    printf("libtopaz test driver (%d tests registered)\n", (int)t->count);

    uint32_t i;
    for(i = 0; i < t->count; ++i) {
        printf("Performing test %s (%d of %d): ", t->tests[i].label, (int)i+1, (int)t->count);
        int result = t->tests[i].fn();
        if (result == 0) {
            printf("Pass.\n");
        } else {
            printf("Failed. (%d)\n", result);
            return result;
        }
        fflush(stdout);
    }
    return 0;
}

