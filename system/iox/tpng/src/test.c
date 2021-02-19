#include "tpng.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
int main() {
    FILE * f = fopen("test.png", "rb");
    
    uint32_t count = 0;
    uint8_t data;
    while(fread(&data, 1, 1, f)) count++;
    fseek(f, SEEK_SET, 0);
    

    uint8_t * rawData = malloc(count);
    uint8_t * iter = rawData;
    while(fread(&data, 1, 1, f)) {*iter = data; iter++;}
    
    
    fclose(f);
    
    uint32_t w, h;
    uint8_t * rgba = tpng_get_rgba(
        rawData,
        count,
        
        &w, 
        &h
    );
    
    
    printf(" Image:\n");
    int x, y;
    int where = 0;
    for(y = 0; y < h; ++y) {
        printf("|");
        for(x = 0; x < w; ++x) {
            printf("{%d,%d,%d,%d} ",
                rgba[where],
                rgba[where+1],
                rgba[where+2],
                rgba[where+3]
            );
            where+=4;
        }
        printf("|\n");
    }

    free(rawData);
    free(rgba);
    return 0;
    
}
