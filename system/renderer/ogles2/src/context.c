#include "context.h"
#include <stdlib.h>

struct topazES2_t {
    topazES2_FB_t * fb;
    topazES2_TexMan_t * texm;

    int lastDepthTest;
    int lastAlphaRule;
    int lastEtchRule;
    int lastTextureFilter;
    
    topazES2_Program_t * defaultProgram;
};

topazES2_t * topaz_es2_create() {
    topazES2_t * out = calloc(1, sizeof(topazES2_t));
    out->fb = 0;
    out->texm = topaz_es2_texman_create();


    out->lastAlphaRule = -1;
    out->lastDepthTest = -1;
    out->lastEtchRule = -1;
    out->lastTextureFilter = -1;
    return out;
}

void topaz_es2_destroy(topazES2_t * e) {
    topaz_es2_texman_destroy(e->texm);
    free(e);
}

topazES2_TexMan_t * topaz_es2_get_texture_manager(topazES2_t * e) {
    return e->texm;
}

topazES2_2D_t * topaz_es2_create_2d(topazES2_t * e) {
    return topaz_es2_2d_create(e->texm);
}

topazES2_FB_t * topaz_es2_get_target(topazES2_t * e) {
    return e->fb;
}

void topaz_es2_set_target(topazES2_t * e, topazES2_FB_t * fb) {
    // flush?
    e->fb = fb;
}

void topaz_es2_start(topazES2_t * e) {
    if (!e->fb) return;
    TOPAZ_GLES_FN_IN;
    glBindFramebuffer(GL_FRAMEBUFFER, topaz_es2_fb_get_handle(e->fb)[0]);TOPAZ_GLES_CALL_CHECK;
    glViewport(
        0, 0,
        topaz_es2_fb_get_width(e->fb),
        topaz_es2_fb_get_height(e->fb)
    );
}

// releases the framebuffer (unbind)
void topaz_es2_end(topazES2_t * e) {
    TOPAZ_GLES_FN_IN;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);TOPAZ_GLES_CALL_CHECK;

}

void topaz_es2_sync(topazES2_t * e) {
    glFinish();
}

const char * topaz_gles_error_to_string(int e) {
    switch(e) {
      case 0x0500: return "GL_INVALID_ENUM";
      case 0x0501: return "GL_INVALID_VALUE";
      case 0x0502: return "GL_INVALID_OPERATION";
      case 0x0503: return "GL_STACK_OVERFLOW";
      case 0x0504: return "GL_STACK_UNDERFLOW";
      case 0x0505: return "GL_INVALID_ENUM";
    }
    return "UNKNOWN ERROR";
}



void topaz_es2_commit_process_attribs(
    topazES2_t * p, 
    const topazRenderer_Attributes_t * attribs
) {

    // transparency control. Preserved until it changes
    if (p->lastAlphaRule != attribs->alphaRule) {
        p->lastAlphaRule = attribs->alphaRule;
        switch(p->lastAlphaRule) {
          case topazRenderer_AlphaRule_Allow:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); 
            break;

          case topazRenderer_AlphaRule_Opaque:
            glDisable(GL_BLEND);
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); 
            break;

          case topazRenderer_AlphaRule_Translucent:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); 
            break;

          case topazRenderer_AlphaRule_Invisible:
            glDisable(GL_BLEND);
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); 
            break;
        }
    }



    // Depth control
    if (p->lastDepthTest != attribs->depthTest) {
        p->lastDepthTest = attribs->depthTest;
        switch(p->lastDepthTest) {
          case topazRenderer_DepthTest_None:
            glDisable(GL_DEPTH_TEST);
            break;

          case topazRenderer_DepthTest_Less: 
            glEnable(GL_DEPTH_TEST); 
            glDepthFunc(GL_LESS);
            break;

          case topazRenderer_DepthTest_LEQ: 
            glEnable(GL_DEPTH_TEST); 
            glDepthFunc(GL_LEQUAL);
            break;

          case topazRenderer_DepthTest_Greater: 
            glEnable(GL_DEPTH_TEST); 
            glDepthFunc(GL_GREATER);
            break;

          case topazRenderer_DepthTest_GEQ: 
            glEnable(GL_DEPTH_TEST); 
            glDepthFunc(GL_GEQUAL);
            break;

          case topazRenderer_DepthTest_Equal: 
            glEnable(GL_DEPTH_TEST); 
            glDepthFunc(GL_EQUAL);
            break;
        }
    }

    if (p->lastEtchRule != attribs->etchRule) {
        p->lastEtchRule = attribs->etchRule;

        switch(p->lastEtchRule) {
          case topazRenderer_EtchRule_NoEtching:
            glDisable(GL_STENCIL_TEST);
            break;

          case topazRenderer_EtchRule_Define:
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(GL_ALWAYS, 1, 1);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            break;

          case topazRenderer_EtchRule_Undefine:
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(GL_ALWAYS, 0, 1);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            break;

          case topazRenderer_EtchRule_In:
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(GL_EQUAL, 1, 1);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            break;

          case topazRenderer_EtchRule_Out:
            glEnable(GL_STENCIL_TEST);
            glStencilFunc(GL_EQUAL, 0, 1);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            break;
        }

    }

    if (p->lastTextureFilter != attribs->textureFilter) {
        p->lastTextureFilter = attribs->textureFilter;
        switch(p->lastTextureFilter) {
          case topazRenderer_TextureFilterHint_Linear:
            topaz_es2_texman_set_filter(p->texm, 1);
            break;

          case topazRenderer_TextureFilterHint_None:
            topaz_es2_texman_set_filter(p->texm, 0);
            break;
        }
    }


}


topazES2_Program_t * topaz_es2_get_default_program(topazES2_t * es) {
    if (!es->defaultProgram) {
        topazString_t * str = topaz_string_create();
        topazES2_Program_t * out = topaz_es2_program_create(
            TOPAZ_STR_CAST(
                "void main() {\n"
                "   topazSetPosition(topaz_mat_projection * (topaz_mat_modelview * (vec4(topaz_vertex_position, 1.0))));\n"
                "}\n"   
            ),
            
            TOPAZ_STR_CAST(
                "void main() {\n"
                "   topazSetOutput(vec4(1, 0, 1, 1));\n"
                "}\n"               
            ),
            str
        );
        if (!out) {
            printf("FATAL ERROR: built-in fallback program failed to compile: %s", topaz_string_get_c_str(str));
            exit(99);
        }
        topaz_string_destroy(str);
        es->defaultProgram = out;
    }
    return es->defaultProgram;
}

