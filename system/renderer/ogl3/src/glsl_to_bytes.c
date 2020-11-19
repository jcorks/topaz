#include <stdio.h>
#include <stdint.h>

int main() {
    FILE * in0 = fopen("vertex_shader_2d.glsl",   "rb");
    FILE * in1 = fopen("fragment_shader_2d.glsl", "rb");

    FILE * out = fopen("glsl_bytes", "wb");
    #define CHUNK_SIZE (1024*4)
    uint32_t readSize;
    uint32_t i;
    char dataRead[CHUNK_SIZE];



    fprintf(out, "const uint8_t vertex_shader_2d_bytes[]={");
    while((readSize = fread(dataRead, 1, CHUNK_SIZE, in0))) {
        for(i = 0; i < readSize; ++i) {
            fprintf(out, "0x%02x,", dataRead[i]);
            if (i%32==0)
                fprintf(out, "\n");
        }
    }
    fprintf(out, "0x00};\n");
    fprintf(out, "const uint8_t fragment_shader_2d_bytes[]={");
    while((readSize = fread(dataRead, 1, CHUNK_SIZE, in1))) {
        for(i = 0; i < readSize; ++i) {
            fprintf(out, "0x%02x,", dataRead[i]);
            if (i%32==0)
                fprintf(out, "\n");
        }
    }
    fprintf(out, "0x00};\n");

    fclose(in0);
    fclose(in1);
    fclose(out);
}
