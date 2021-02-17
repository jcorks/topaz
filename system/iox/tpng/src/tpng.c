

// standalone, simple png encoder/decoder 
// jcorkery@umich.edu, 2021


#define TPNG_PALETTE_LIMIT 256





typedef struct tpng_iter_t tpng_iter_t;

tpng_iter_t * tpng_iter_create(const uint8_t * data, uint32_t size);

void tpng_iter_destroy(tpng_iter_t *);

const void * tpng_iter_advance(tpng_iter_t *, uint32_t);










#define TPNG_BEGIN(__iter__)void*next_;tpng_iter_t * iter_=__iter__;
#define TPNG_READ(__type__)*(__type__*)((next_=tpng_iter_advance(iter_, sizeof(__type__))))
#define TPNG_READ_N(__len__)tpng_iter_advance(iter_, __len__);







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
    int w;
    int h;
    
    int bitDepth;
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



static void tpng_read_chunk(tpng_iter_t *, tpng_chunk_t * chunk); 
static void tpng_process_chunk(tpng_image_t * image, tpng_chunk_t * chunk);
static void tpng_image_init(tpng_image_t *, uint32_t rawLen);


uint8_t * tpng_get_rgba(
    const uint8_t * rawData,
    uint32_t        rawSize,

    uint32_t * w, 
    uint32_t * h
) {
    tpng_chunk_t chunk;
    tpng_image_t image;
    tpng_image_init(&image);

    tpng_iter_t * iter = tpng_iter_create(rawData, rawSize);

    
    // universal PNG header
    TPNG_BEGIN(iter);
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
        return 0;
    }    
    
    
    // next read chunks
    tpng_read_chunk(iter, &chunk); 
    tpng_process_chunk(&image, &chunk);
    
    while(strcmp(current.type, "IEND")) {
        tpng_read_chunk(iter, &chunk); 
        tpng_process_chunk(&image, &chunk);
    }
      
      
      
    // return processed image
    *w = image.w;
    *h = image.h;
    tpng_image_cleanup(&image);
    return image->rgba;
      
}














typedef struct {
    const uint8_t * data;
    uint32_t        size;
    uint32_t        iter;
    uint8_t **      errors;
    uint32_t        nerrors;
} tpng_iter_t;

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
        void * out = t->data+t->iter;        
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




static void tpng_read_chunk(tpng_iter_t * iter, tpng_chunk_t * chunk) {
    TPNG_BEGIN(iter);
    memset(chunk, 0, sizeof(tpng_chunk_t));
    chunk.length = TPNG_READ(uint32_t);
    chunk.type[0] = TPNG_READ(char);
    chunk.type[1] = TPNG_READ(char);
    chunk.type[2] = TPNG_READ(char);
    chunk.type[3] = TPNG_READ(char);
    chunk.type[5] = 0;
    chunk.data = TPNG_READ_N(chunk.length);
    chunk.data = TPNG_READ(int); // crc 
    
    if (chunk.type[0] == 0 &&
        chunk.type[1] == 0 &&
        chunk.type[2] == 0 &&
        chunk.type[3] == 0) {
        // corruption or early EOF.
        // mark with auto-end chunk
        chunk.type[0] = 'I';   
        chunk.type[1] = 'E';   
        chunk.type[2] = 'N';   
        chunk.type[3] = 'D';   
    }
}

static void tpng_image_init(tpng_image_t * image, uint32_t rawlen) {
    image->idataLength = 0;
    image->idata = malloc(rawLen);
    
}

static void tpng_process_chunk(tpng_image_t * image, tpng_chunk_t * chunk) {
    if (!strcmp(chunk.type, "IHDR")) {
        tpng_iter_t * iter = tpng_iter_create(chunk->data, chunk->length);
        TPNG_BEGIN(iter);
        image->w = TPNG_READ(int);
        image->h = TPNG_READ(int);
        
        image->bitDepth        = TPNG_READ(char);
        image->colorDepth      = TPNG_READ(char);
        image->compression     = TPNG_READ(char);
        image->filterMethod    = TPNG_READ(char);
        image->interlaceMethod = TPNG_READ(char);
        
        image->data = calloc(4, image->w*image->h);

        tpng_iter_destroy(iter);
    } else if (!strcmp(chunk.type, "PLTE")) {
        tpng_iter_t * iter = tpng_iter_create(chunk->data, chunk->length);
        TPNG_BEGIN(iter);
        
        image->nPalette = chunk->length / sizeof(tpng_palette_entry_t);
        if (image->nPalette > TPNG_PALETTE_LIMIT) image->nPalette = TPNG_PALETTE_LIMIT;
        uint32_t i;
        for(i = 0; i < image->nPalette; ++i) {
            image->palette[i].r = TPNG_PNG(uint8_t);
            image->palette[i].g = TPNG_PNG(uint8_t);
            image->palette[i].b = TPNG_PNG(uint8_t);
        }
        tpng_iter_destroy(iter);        
    } else if (!strcmp(chunk.type, "IDAT")) {
        memcpy(image->idata+image->idataLength, chunk.data, chunk.length);         
        image->idataLength += chunk.length;
    } else if (strcmp(chunk.type, "IEND")) {
        // compression mode is the current and only accepted type.
        if (image->compression != 0) return;


        // now safe to work with IDAT input
        // first: decompress (inflate)
        size_t rawUncompLen;
        void * rawUncomp = tinfl_decompress_mem_to_heap(
            image->idata, 
            image->idataLength, 
            &rawUncompLen, 
            0 // no zlib header
        );
        tpng_iter_t * iter = tpng_iter_create(rawUncomp, rawUncompLen);
        TPNG_BEGIN(iter);        

        

        // next: filter
        // each scanline contains a single byte specifying how its filtered (reordered)
        uint32_t row = 0;
        for(row = 0; row < image->h; ++row) {
            switch(TPNG_READ(uint8_t)) {
              case 0: // no filtering 
                break;              
              default:
                assert(!"TODO");
            }            
            
            // finally: get scanlines
                
            
            // save raw previous scanline            
        }
            
        
        
        


        tpng_iter_destroy(iter);        
    }
}





