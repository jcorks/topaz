
#include "tinfl.h"
#include "tpng.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// standalone, simple png encoder/decoder 
// jcorkery@umich.edu, 2021


#define TPNG_PALETTE_LIMIT 256





typedef struct tpng_iter_t tpng_iter_t;

tpng_iter_t * tpng_iter_create(const uint8_t * data, uint32_t size);

void tpng_iter_destroy(tpng_iter_t *);

const void * tpng_iter_advance(tpng_iter_t *, uint32_t);










#define TPNG_BEGIN(__iter__)const void*next_;tpng_iter_t * iter_=__iter__
#define TPNG_READ(__type__)*(__type__*)((next_=tpng_iter_advance(iter_, sizeof(__type__))))
#define TPNG_READ_N(__len__)tpng_iter_advance(iter_, __len__)







typedef struct {
    uint32_t        length;
    char            type[5]; // actually 4 bytes, nul terminated though!
    const uint8_t * data;
    int             crc; // ignored
} tpng_chunk_t;



typedef struct {
    uint8_t bytes[8];
} tpng_header_t;


typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} tpng_palette_entry_t;


typedef struct {
    int littleEndian;
    int w;
    int h;
    
    int colorDepth;
    int BPP;
    int colorType;
    
    int compression;
    int filterMethod;
    int interlaceMethod;
    
    tpng_palette_entry_t palette[TPNG_PALETTE_LIMIT];
    uint32_t nPalette;
    
    uint8_t * rgba;
    uint8_t * idata;
    uint32_t idataLength;
} tpng_image_t;



static void tpng_read_chunk(tpng_image_t *, tpng_iter_t *, tpng_chunk_t * chunk); 
static void tpng_process_chunk(tpng_image_t * image, tpng_chunk_t * chunk);
static void tpng_image_init(tpng_image_t *, uint32_t rawLen);
static void tpng_image_cleanup(tpng_image_t *);


uint8_t * tpng_get_rgba(
    const uint8_t * rawData,
    uint32_t        rawSize,

    uint32_t * w, 
    uint32_t * h
) {
    

    tpng_chunk_t chunk;
    tpng_image_t image;
    tpng_image_init(&image, rawSize);


    tpng_iter_t * iter = tpng_iter_create(rawData, rawSize);
    TPNG_BEGIN(iter);
    
    // universal PNG header
    {
        tpng_header_t header = TPNG_READ(tpng_header_t);
        if (header.bytes[0] != 137 ||
            header.bytes[1] != 80 ||
            header.bytes[2] != 78 ||
            header.bytes[3] != 71 ||
            
            header.bytes[4] != 13 ||
            header.bytes[5] != 10 ||
            header.bytes[6] != 26 ||
            header.bytes[7] != 10
        ) {
            // not a PNG!
            *w = 0;
            *h = 0;
            tpng_image_cleanup(&image);
            tpng_iter_destroy(iter);
            return 0;
        }    
    }

    
    // next read chunks
    tpng_read_chunk(&image, iter, &chunk); 
    tpng_process_chunk(&image, &chunk);
    
    while(strcmp(chunk.type, "IEND")) {
        tpng_read_chunk(&image, iter, &chunk); 
        tpng_process_chunk(&image, &chunk);
    }
      
      
      
    // return processed image
    *w = image.w;
    *h = image.h;
    tpng_image_cleanup(&image);
    return image.rgba;
      
}














struct tpng_iter_t {
    const uint8_t * data;
    uint32_t        size;
    uint32_t        iter;
    uint8_t **      errors;
    uint32_t        nerrors;
};

tpng_iter_t * tpng_iter_create(const uint8_t * data, uint32_t size) {
    tpng_iter_t * out = calloc(1, sizeof(tpng_iter_t));
    out->data = data;
    out->size = size;
    out->iter = 0;
    
    out->nerrors = 0;
    out->errors = NULL;
}

void tpng_iter_destroy(tpng_iter_t * t) { 
    uint32_t i;
    for(i = 0; i < t->nerrors; ++i) {
        free(t->errors[i]);
    }
    free(t);
}

const void * tpng_iter_advance(tpng_iter_t * t, uint32_t size) {
    if (!size) return NULL;
    if (t->iter + size <= t->size) {
        const void * out = t->data+t->iter;     
        t->iter+=size;   
        return out;
    } else {
        // no partial.
        void * newError = calloc(1, size);
        if (t->nerrors) {
            void * newErrorArray = malloc(sizeof(void*)*(t->nerrors+1));
            memcpy(newErrorArray, t->errors, sizeof(void*)*t->nerrors);
        }
        t->errors[t->nerrors] = newError;
        t->nerrors++;
        return newError;
    }
}


static int tpng_correct_byte_order(tpng_image_t * img, int i) {
    if (img->littleEndian) {
        char * data = &i;
        char t;
        t = data[0]; data[0] = data[3]; data[3] = t;
        t = data[1]; data[1] = data[2]; data[2] = t;
    }
    return i;
}

static uint32_t tpng_correct_byte_order_unsigned(tpng_image_t * img, uint32_t i) {
    if (img->littleEndian) {
        char * data = &i;
        char t;
        t = data[0]; data[0] = data[3]; data[3] = t;
        t = data[1]; data[1] = data[2]; data[2] = t;
    }
    return i;
}



static void tpng_read_chunk(tpng_image_t * image, tpng_iter_t * iter, tpng_chunk_t * chunk) {
    TPNG_BEGIN(iter);
    memset(chunk, 0, sizeof(tpng_chunk_t));
    chunk->length = tpng_correct_byte_order_unsigned(image, TPNG_READ(uint32_t));
    chunk->type[0] = TPNG_READ(char);
    chunk->type[1] = TPNG_READ(char);
    chunk->type[2] = TPNG_READ(char);
    chunk->type[3] = TPNG_READ(char);
    chunk->type[4] = 0;
    chunk->data = TPNG_READ_N(chunk->length);
    chunk->crc  = tpng_correct_byte_order(image, TPNG_READ(int)); // crc 
    
    if (chunk->type[0] == 0 &&
        chunk->type[1] == 0 &&
        chunk->type[2] == 0 &&
        chunk->type[3] == 0) {
        // corruption or early EOF.
        // mark with auto-end chunk
        chunk->type[0] = 'I';   
        chunk->type[1] = 'E';   
        chunk->type[2] = 'N';   
        chunk->type[3] = 'D';   
    }
}

static void tpng_image_init(tpng_image_t * image, uint32_t rawlen) {
    image->idataLength = 0;
    image->idata = malloc(rawlen);
    image->rgba = 0;
    image->littleEndian = 1;
    image->littleEndian = *((char*)(&image->littleEndian));
}


static void tpng_expand_row(tpng_image_t * image, const uint8_t * row, uint8_t * expanded) {
    uint32_t i;
    uint32_t bitCount = image->colorDepth*image->w;
    int iter;
    int palette;
    switch(image->colorType) {
      // grayscale!
      case 0:
        switch(image->colorDepth) {
          case 1:
            for(i = 0; i < bitCount; ++i) {
                iter = (i/8)*4;
                expanded[iter] = row[i/8]  & 1 << i%8; 
                expanded[iter+1] = expanded[iter];
                expanded[iter+2] = expanded[iter];
                expanded[iter+3] = 255;
            }
            break;
          case 2:
            for(i = 0; i < bitCount; i+=2) {
                iter = (i/4)*4;
                expanded[iter] = row[i/4] & 3 << (i%4); 
                expanded[iter+1] = expanded[iter];
                expanded[iter+2] = expanded[iter];
                expanded[iter+3] = 255;
            }
            break;
          case 4:
            for(i = 0; i < bitCount; i+=4) {
                iter = (i/2)*4;
                expanded[(i/2)*4] = row[i/2] & 15 << i; 
                expanded[iter+1] = expanded[iter];
                expanded[iter+2] = expanded[iter];
                expanded[iter+3] = 255;
            }
            break;
          case 8:
            for(i = 0; i < bitCount; i+=8) {
                expanded[i*4] = row[i]; 
                expanded[iter+1] = expanded[iter];
                expanded[iter+2] = expanded[iter];
                expanded[iter+3] = 255;
            }
            break;
          case 16:
            for(i = 0; i < bitCount; i+=16) {
                expanded[i*4]   = (row[i*2]+row[i*2+1])/2; 
                expanded[iter+1] = expanded[iter];
                expanded[iter+2] = expanded[iter];
                expanded[iter+3] = 255;

            }
            break;
          default:;
        }       
        break;


        
        
        
       // plain RGB!
       case 2:
        switch(image->colorDepth) {
          case 8:
            for(i = 0; i < image->w; ++i) {
                expanded[i*4  ] = row[i*3  ]; 
                expanded[i*4+1] = row[i*3+1]; 
                expanded[i*4+2] = row[i*3+2]; 
            }
            break;
          case 16:
            for(i = 0; i < image->w; ++i) {
                expanded[i*4  ] = (row[i*6  ] + row[i*6+1]) / 2; 
                expanded[i*4+1] = (row[i*6+2] + row[i*6+3]) / 2; 
                expanded[i*4+2] = (row[i*6+4] + row[i*6+5]) / 2; 
            }
            break;
          default:;  
        }
        break;  
            
      // palette!
      case 3:
        switch(image->colorDepth) {
          case 1:
            for(i = 0; i < bitCount; ++i) {
                iter = (i/8)*4;
                palette = row[i/8]  & 1 << i%8;
                expanded[iter]   = image->palette[palette].r; 
                expanded[iter+1] = image->palette[palette].g; 
                expanded[iter+2] = image->palette[palette].b; 
                expanded[iter+3] = 255;
            }
            break;
          case 2:
            for(i = 0; i < bitCount; i+=2) {
                iter = (i/4)*4;
                palette = row[i/4] & 3 << i%4;
                expanded[iter]   = image->palette[palette].r; 
                expanded[iter+1] = image->palette[palette].g; 
                expanded[iter+2] = image->palette[palette].b; 
                expanded[iter+3] = 255;
            }
            break;
          case 4:
            for(i = 0; i < bitCount; i+=4) {
                iter = (i/2)*4;
                palette = row[i/2] & 15 << i; 
                expanded[iter]   = image->palette[palette].r; 
                expanded[iter+1] = image->palette[palette].g; 
                expanded[iter+2] = image->palette[palette].b; 
                expanded[iter+3] = 255;
            }
            break;
          case 8:
            for(i = 0; i < bitCount; i+=8) {
                iter = i;
                palette = row[i];
                expanded[iter]   = image->palette[palette].r; 
                expanded[iter+1] = image->palette[palette].g; 
                expanded[iter+2] = image->palette[palette].b; 
                expanded[iter+3] = 255;
            }
            break;
          default:;
        }       
        break;
            
            
            
            
            
      // grayscale + alpha!
      case 4:
        switch(image->colorDepth) {
          case 8:
            for(i = 0; i < image->w; ++i) {
                expanded[i*4]   = row[i*2]; 
                expanded[i*4+1] = row[i*2]; 
                expanded[i*4+2] = row[i*2]; 
                expanded[i*4+3] = row[i*2+1]; 

            }

          case 16:
            for(i = 0; i < image->w; ++i) {
                expanded[i*4]   = (row[i*4  ]+row[i*4+1])/2; 
                expanded[i*4+1] = expanded[i*4+0];
                expanded[i*4+2] = expanded[i*4+1];
                expanded[i*4+3] = (row[i*4+2]+row[i*4+3])/2; 
            }
            break;
        }
        break;           

      // RGBA!
      case 6:
        switch(image->colorDepth) {
          case 8:
            for(i = 0; i < image->w; ++i) {
                expanded[i*4  ] = row[i*4  ]; 
                expanded[i*4+1] = row[i*4+1]; 
                expanded[i*4+2] = row[i*4+2]; 
                expanded[i*4+3] = row[i*4+3]; 
            }
            break;
          case 16:
            for(i = 0; i < image->w; ++i) {
                expanded[i*4  ] = (row[i*8  ] + row[i*8+1]) / 2; 
                expanded[i*4+1] = (row[i*8+2] + row[i*8+3]) / 2; 
                expanded[i*4+2] = (row[i*8+4] + row[i*8+5]) / 2; 
                expanded[i*4+3] = (row[i*8+6] + row[i*8+7]) / 2; 
            }
            break;
          default:;  
        }
        break;

      default:;
    }
}



static void tpng_process_chunk(tpng_image_t * image, tpng_chunk_t * chunk) {
    if (!strcmp(chunk->type, "IHDR")) {
        tpng_iter_t * iter = tpng_iter_create(chunk->data, chunk->length);
        TPNG_BEGIN(iter);
        image->w = tpng_correct_byte_order(image, TPNG_READ(int));
        image->h = tpng_correct_byte_order(image, TPNG_READ(int));
        
        image->colorDepth      = TPNG_READ(char);
        image->BPP             = image->colorDepth < 8 ? 1 : image->colorDepth / 8;
        image->colorType       = TPNG_READ(char);
        image->compression     = TPNG_READ(char);
        image->filterMethod    = TPNG_READ(char);
        image->interlaceMethod = TPNG_READ(char);
        
        image->rgba = calloc(4, image->w*image->h);

        tpng_iter_destroy(iter);
    } else if (!strcmp(chunk->type, "PLTE")) {
        tpng_iter_t * iter = tpng_iter_create(chunk->data, chunk->length);
        TPNG_BEGIN(iter);
        
        image->nPalette = chunk->length / sizeof(tpng_palette_entry_t);
        if (image->nPalette > TPNG_PALETTE_LIMIT) image->nPalette = TPNG_PALETTE_LIMIT;
        uint32_t i;
        for(i = 0; i < image->nPalette; ++i) {
            image->palette[i].r = TPNG_READ(uint8_t);
            image->palette[i].g = TPNG_READ(uint8_t);
            image->palette[i].b = TPNG_READ(uint8_t);
        }
        tpng_iter_destroy(iter);        
    } else if (!strcmp(chunk->type, "IDAT")) {
        memcpy(image->idata+image->idataLength, chunk->data, chunk->length);         
        image->idataLength += chunk->length;
    } else if (!strcmp(chunk->type, "IEND")) {
        // compression mode is the current and only accepted type.
        if (image->compression != 0) return;


        // now safe to work with IDAT input
        // first: decompress (inflate)
        size_t rawUncompLen;
        void * rawUncomp = tinfl_decompress_mem_to_heap(
            image->idata, 
            image->idataLength, 
            &rawUncompLen, 
            1 // actually contains zlib header
        );
        tpng_iter_t * iter = tpng_iter_create(rawUncomp, rawUncompLen);
        TPNG_BEGIN(iter);        

        

        // next: filter
        // each scanline contains a single byte specifying how its filtered (reordered)
        uint32_t row = 0;
        uint32_t rowBytes = (image->colorDepth * image->w)/8;

        // row bytes of the above row, filter byte discarded
        uint8_t * prevRow = calloc(1, rowBytes);
        // row bytes of the current row, filter byte discarded
        uint8_t * thisRow = calloc(1, rowBytes);

        // Expanded raw row, where each RGBA pixel is given 
        // the raw value within 
        uint8_t * rowExpanded = calloc(4, image->w);
        
        for(row = 0; row < image->h; ++row) {
            int filter = TPNG_READ(uint8_t);
            memcpy(thisRow, TPNG_READ_N(rowBytes), rowBytes);

            switch(filter) {
              case 0: // no filtering 
                break;              
              default:
                assert(!"TODO");
            }            
            

            // finally: get scanlines from data
            tpng_expand_row(image, thisRow, rowExpanded);
            
            memcpy(
                image->rgba + row*image->w*4, 
                rowExpanded,
                4 * (image->w)
            );
                
            // save raw previous scanline            
            memcpy(prevRow, thisRow, rowBytes);
        }
            
        tpng_iter_destroy(iter);        
    }
}



static void tpng_image_cleanup(tpng_image_t * image) {
    free(image->idata);
}




