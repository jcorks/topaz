#include <stdio.h>
#include <stdint.h>

int main() {
    char * files_in[] = {
        "bootstrap.mt",
        "debug.mt"
    };

    char * files_out[] = {
        "bootstrap_bytes",
        "debug_bytes"
    };

    char * files_ref[] = {
        "const uint8_t bootstrap_bytes[]={",
        "const uint8_t debug_bytes[]={",
    };
    
    uint32_t n;
    for(n = 0; n < 2; ++n) {


        FILE * in = fopen(files_in[n], "rb");
        FILE * out = fopen(files_out[n], "wb");

        fprintf(out, files_ref[n]);
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
}
