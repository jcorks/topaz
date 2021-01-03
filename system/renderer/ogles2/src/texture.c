#include "texture.h"
#include "charpixel.h"
#include <stdlib.h>
#include <stdio.h>
#include <topaz/containers/array.h>

#define ATLAS_START_LEN 512

typedef struct {
    topaz_es2_texman_reset_fn fn;
    void * data;
} ResetCB;



typedef struct {
    topazES2_TexMan_t * parent;
    // index within the texture manager
    int index;
    // Gl texture object
    GLuint object;

    // framebuffer for reading
    GLuint framebufferReader;

    // width in texels
    int width;
    // height in texels
    int height;
    
    // maximum allowed length of the atlas
    int maxLength;

    // we're doing the dumb way:
    // maintain an iterator marking where we are based on maximum tex dimensions per "row".
    int iterX;
    int iterY;
   
    topazArray_t * textures;
    int needsReshuffle;
} GLTexAtlas;

struct topazES2_Texture_t {
    GLTexAtlas * src;
    int w; // width
    int h; // height
    int x; // right (x from topleft origin) 
    int y; // low (y from topleft origin);
    void * intermediateData; // used during reset events
};

struct topazES2_TexMan_t {
    GLTexAtlas * atlases[16];
    GLuint ids[16];

    // all callbacks to capture texture changed events
    // this is only really for when the textures get shuffled 
    // around from resize events.
    topazArray_t * cbs; // ResetCB;

};


// Creates an independent atlas
static GLTexAtlas * atlas_create(topazES2_TexMan_t *, int index, GLuint);

// destroys an atlas
static void atlas_destroy(GLTexAtlas *);

// returns raw rgba of a section of GL texture memory
static void atlas_read(GLTexAtlas *, int x, int y, int w, int h, uint8_t * target);

// Adds a texture to the atlas, success is returned
// this populates the x, y of the texture. If this fails, the x/y are set 
// to -1. no texel data is modified.
static int atlas_request_region(GLTexAtlas * atlas, topazES2_Texture_t * texture);

// releases the region in use by the texture
static void atlas_release_region(GLTexAtlas * atlas, topazES2_Texture_t * texture);

// Writes to the texel space
static void atlas_write(GLTexAtlas *, int x, int y, int w, int h, const uint8_t * target);

// Re-requests all texture regions within the atlas. This procs the texture changed callback 
// events. This only acts if needsReshuffle is nonzero. It resets needsReshuffle after completion
static void atlas_reset(GLTexAtlas *);

// resizes the atlas. The unfortunately requires re-downloading all textures 
// and re-uploading them.
static void atlas_resize(GLTexAtlas *, int w, int h);


topazES2_TexMan_t * topaz_es2_texman_create() {
    TOPAZ_GLES_FN_IN;
    topazES2_TexMan_t * out = calloc(1, sizeof(topazES2_TexMan_t));
    out->cbs = topaz_array_create(sizeof(ResetCB));
    glGenTextures(16, out->ids); TOPAZ_GLES_CALL_CHECK;

    return out;
}

void topaz_es2_texman_destroy(topazES2_TexMan_t * tm) {
    TOPAZ_GLES_FN_IN;
    int i;
    for(i = 0; i < 16; ++i) {
        atlas_destroy(tm->atlases[i]);
    }   
    glDeleteTextures(16, tm->ids); TOPAZ_GLES_CALL_CHECK;
    topaz_array_destroy(tm->cbs);
    free(tm);
}



const GLuint * topaz_es2_texman_gl_textures(const topazES2_TexMan_t * a) {
    return a->ids;
}

void topaz_es2_texman_set_filter(
    topazES2_TexMan_t * tm,
    int filter
) {
    TOPAZ_GLES_FN_IN;
    

    int i;
    for(i = 0; i < 16; ++i) {
        if (!tm->atlases[i]) continue;
        glBindTexture(GL_TEXTURE_2D, tm->ids[i]); TOPAZ_GLES_CALL_CHECK;
        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            filter ? GL_LINEAR : GL_NEAREST
        ); TOPAZ_GLES_CALL_CHECK;


        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            filter ? GL_LINEAR : GL_NEAREST
        ); TOPAZ_GLES_CALL_CHECK;        
    }


}


void topaz_es2_texman_print_section(
    topazES2_TexMan_t * tm,
    int x,
    int y,
    int w,
    int h,
    int index
) {
    GLTexAtlas * a = tm->atlases[index];
    uint8_t * raw = calloc(4, w*h);
    atlas_read(
        a,
        x,
        y,
        w,
        h,
        raw
    );
    uint8_t * iter = raw;
    printf("ATLAS: @ %d,%d (%dx%d)____________________+\n", x, y, w, h);
    for(y = 0; y < h; ++y) {
        printf("|");
        for(x = 0; x < w; ++x) {
            printf("%c", topaz_es2_get_char_from_pixel(iter));
            iter+=4;
        }
        printf("|\n");
    }
    printf("+_________________________________________+\n");

}


void topaz_es2_texman_add_reset_callback(
    topazES2_TexMan_t * tm,
    topaz_es2_texman_reset_fn fn,
    void * cbData
) {
    ResetCB cb;
    cb.fn = fn;
    cb.data = cbData;

    topaz_array_push(tm->cbs, cb);
}









topazES2_Texture_t * topaz_es2_texture_create(
    topazES2_TexMan_t * tm, 
    int w, 
    int h, 
    const uint8_t * rawData
) {
    topazES2_Texture_t * out = calloc(1, sizeof(topazES2_Texture_t));
    out->w = w;
    out->h = h;
    out->x = -1;
    out->y = -1;


    int i;
    for(i = 0; i < 16; ++i) {
        // create if new
        GLTexAtlas * a = tm->atlases[i];
        if (!a) {
            tm->atlases[i] = atlas_create(tm, i, tm->ids[i]);
            a = tm->atlases[i];
        }
    
        // if cant fit, move on to next atlas and try again
        if (!atlas_request_region(a, out)) {
            continue;
        }

        // update the atlas at the owned position
        atlas_write(
            a,
            out->x,
            out->y,
            out->w,
            out->h,
            rawData
        );

        topaz_array_push(a->textures, out);
        out->src = a;
        return out;

    }

    #ifdef TOPAZDC_DEBUG
        assert(!"Out of texture-space memory.\n");
    #endif

    
    free(out);
    return NULL;
}


void topaz_es2_texture_destroy(
    topazES2_Texture_t * t
) {
    uint32_t i;
    uint32_t len = topaz_array_get_size(t->src->textures);
    for(i = 0; i < len; ++i) {
        if (topaz_array_at(t->src->textures, topazES2_Texture_t *, i) == t) {
            topaz_array_remove(t->src->textures, i);
            break;
        }
    }
    atlas_release_region(t->src, t);
    free(t);
}



void topaz_es2_texture_get(
    topazES2_Texture_t * t, 
    uint8_t * data
) {
    atlas_read(
        t->src,
        t->x, t->y,
        t->w, t->h,
        data
    );
}


void topaz_es2_texture_update(
    topazES2_Texture_t * t,
    const uint8_t * data
) {
    atlas_write(
        t->src,
        t->x, t->y,
        t->w, t->h,
        data
    );
}

void topaz_es2_texture_local_coords_to_atlas(
    topazES2_Texture_t * t,
    float u,
    float v,
    float * outU,
    float * outV,
    int * atlasIndex
) {
    *outU = (t->x+(u*t->w)) / (float)t->src->width;
    *outV = (t->y+(v*t->h)) / (float)t->src->height;
    *atlasIndex = t->src->index;
}






////// static


static GLTexAtlas * atlas_create(topazES2_TexMan_t * tm, int index, GLuint texture) {
    TOPAZ_GLES_FN_IN;
    GLTexAtlas * out = calloc(1, sizeof(GLTexAtlas));
    out->index = index;
    out->parent = tm;
    out->object = texture;
    glBindTexture(GL_TEXTURE_2D, out->object); TOPAZ_GLES_CALL_CHECK;

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_NEAREST
    ); TOPAZ_GLES_CALL_CHECK;


    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_NEAREST
    ); TOPAZ_GLES_CALL_CHECK;

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &out->maxLength); 


    out->width  = 2048; // minimum max
    out->height = ATLAS_START_LEN;
    out->textures = topaz_array_create(sizeof(topazES2_Texture_t *));


    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        out->width,
        out->height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        0
    ); TOPAZ_GLES_CALL_CHECK;

    glBindTexture(GL_TEXTURE_2D, 0); TOPAZ_GLES_CALL_CHECK;


    glGenFramebuffers(1, &out->framebufferReader); TOPAZ_GLES_CALL_CHECK;

    glBindFramebuffer(GL_FRAMEBUFFER, out->framebufferReader); TOPAZ_GLES_CALL_CHECK;

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        out->object,
        0
    ); TOPAZ_GLES_CALL_CHECK;
    //glClearColor(1, 1, 1, 1);
    //glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); TOPAZ_GLES_CALL_CHECK;

    return out;
}

void atlas_destroy(GLTexAtlas * a) {
    free(a);
}


void atlas_read(GLTexAtlas * a, int x, int y, int w, int h, uint8_t * target) {
    TOPAZ_GLES_FN_IN;
    glBindFramebuffer(GL_FRAMEBUFFER, a->framebufferReader); TOPAZ_GLES_CALL_CHECK;

    int needsXshift = 0;
    int needsYshift = 0;

    if (x < 0) {
        w -= x;
        x -= x;
        needsXshift = 1;
    }

    if (y < 0) {
        h -= y;
        y -= y;
        needsYshift = 1;
    }

    if (x >= a->width || !w) {
        return;
    }

    if (y >= a->height || !h) {
        return;
    }


    glReadPixels(
        x, y,
        w, h,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        target
    );
    TOPAZ_GLES_CALL_CHECK;

    glBindFramebuffer(GL_FRAMEBUFFER, 0); TOPAZ_GLES_CALL_CHECK;

    


    // not implemented yet.
    #ifdef TOPAZDC_DEBUG
        assert(!(needsXshift || needsYshift));
    #endif
}


#define ATLAS_SPACE_BORDER 2
#define ATLAS_RESIZE_AMT 1024;
static int atlas_request_region(GLTexAtlas * a, topazES2_Texture_t * t) {

    // do we need to resize height?
    if (a->height < a->maxLength) { 
        a->height += ATLAS_RESIZE_AMT;
        if (a->height > a->maxLength)
            a->height = a->maxLength;

        atlas_resize(a, a->width, a->height);
    }



    if (a->iterX + t->w + ATLAS_SPACE_BORDER < a->width &&
        a->iterY + t->h + ATLAS_SPACE_BORDER < a->height) {
        // normal case fit
        t->x = a->iterX + ATLAS_SPACE_BORDER;
        t->y = a->iterY + ATLAS_SPACE_BORDER;

        a->iterX += t->w + ATLAS_SPACE_BORDER*2;

        return 1;

    }

    // check to see if a reset will help (this is just garbage collection)
    if (a->needsReshuffle) {
        atlas_reset(a);
        return atlas_request_region(a, t);
    }



    // the atlas is max size and cannot fit the texture.
    return 0;



    
}

void atlas_release_region(GLTexAtlas * a, topazES2_Texture_t * t) {
    a->needsReshuffle = 1;
}

void atlas_resize(GLTexAtlas * a, int newWidth, int newHeight) {
    TOPAZ_GLES_FN_IN;

    uint32_t i;
    uint32_t len = topaz_array_get_size(a->textures);
    // have to copy all the texture data to CPU memory
    for(i = 0; i < len; ++i) {
        topazES2_Texture_t * texture = topaz_array_at(a->textures, topazES2_Texture_t *, i);
        uint8_t * dataBuffer = malloc(texture->w * texture->h * 4);
        atlas_read(a, texture->x, texture->y, texture->w, texture->h, dataBuffer);
        texture->intermediateData = dataBuffer;
    }

    


    glBindTexture(GL_TEXTURE_2D, a->object); TOPAZ_GLES_CALL_CHECK;
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        a->width,
        a->height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        0
    ); TOPAZ_GLES_CALL_CHECK;
    glBindTexture(GL_TEXTURE_2D, 0); TOPAZ_GLES_CALL_CHECK;




    // re-upload to atlas, but KEEP all same regions (no resize event needed)
    for(i = 0; i < len; ++i) {
        topazES2_Texture_t * texture = topaz_array_at(a->textures, topazES2_Texture_t *, i);
        atlas_write(a, texture->x, texture->y, texture->w, texture->h, texture->intermediateData);
        free(texture->intermediateData);
        texture->intermediateData = 0;
    }


}

void atlas_reset(GLTexAtlas * a) {
    if (!a->needsReshuffle) return;

    uint32_t i, n;
    uint32_t len = topaz_array_get_size(a->textures);
    uint32_t cbCount = topaz_array_get_size(a->parent->cbs);
    // have to copy all the texture data to CPU memory
    for(i = 0; i < len; ++i) {
        topazES2_Texture_t * texture = topaz_array_at(a->textures, topazES2_Texture_t *, i);
        uint8_t * dataBuffer = malloc(texture->w * texture->h * 4);
        atlas_read(a, texture->x, texture->y, texture->w, texture->h, dataBuffer);
        texture->intermediateData = dataBuffer;
    }
    topazArray_t * cl = topaz_array_clone(a->textures);
    topaz_array_set_size(a->textures, 0);



    // re-upload to atlas starting from the beginning.

    a->iterX = 0;
    a->iterY = 0;
    for(i = 0; i < len; ++i) {
        topazES2_Texture_t * texture = topaz_array_at(cl, topazES2_Texture_t *, i);
        atlas_request_region(a, texture);
        topaz_array_push(a->textures, texture);
        atlas_write(a, texture->x, texture->y, texture->w, texture->h, texture->intermediateData);
        free(texture->intermediateData);
        texture->intermediateData = 0;

        for(n = 0; n < cbCount; ++n) {
            ResetCB * cb = &topaz_array_at(a->parent->cbs, ResetCB, n);
            cb->fn(texture, cb->data);
        }
    }

    a->needsReshuffle = 0;
}


static void atlas_write(GLTexAtlas * a, int x, int y, int w, int h, const uint8_t * target) {
    TOPAZ_GLES_FN_IN;
    glBindTexture(GL_TEXTURE_2D, a->object); TOPAZ_GLES_CALL_CHECK;

    int needsXshift = 0;
    int needsYshift = 0;

    if (x < 0) {
        w -= x;
        x -= x;
        needsXshift = 1;
    }

    if (y < 0) {
        h -= y;
        y -= y;
        needsYshift = 1;
    }

    if (x >= a->width || !w) {
        return;
    }

    if (y >= a->height || !h) {
        return;
    }

    #ifdef TOPAZDC_DEBUG
        assert(!(needsXshift || needsYshift));
    #endif

    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        x, y,
        w, h,

        GL_RGBA,
        GL_UNSIGNED_BYTE,
        target
    );TOPAZ_GLES_CALL_CHECK;

    glBindTexture(GL_TEXTURE_2D, 0); TOPAZ_GLES_CALL_CHECK;

}







