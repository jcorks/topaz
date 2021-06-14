#include <stdio.h>
#include <stdint.h>

int main() {
    FILE * in = fopen("bootstrap.lua", "rb");
    FILE * out = fopen("bootstrap_bytes", "wb");

    fprintf(out, "const uint8_t bootstrap_bytes[]={");
    #define CHUNK_SIZE (1024*4)
    uint32_t readSize;
    uint32_t i;
    char dataRead[CHUNK_SIZE];

    while((readSize = fread(dataRead, 1, CHUNK_SIZE, in))) {
        for(i = 0; i < readSize; ++i) {
            fprintf(out, "0x%02x,", dataRead[i]);
            if (i%32==0)
                fprintf(out, "\n");
        }
    }
    fprintf(out, "0x00};");

    fclose(in);
    fclose(out);
}
