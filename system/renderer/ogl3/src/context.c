#include "context.h"
#include <stdlib.h>
#include <stdio.h>

struct topazGL3_t {
    topazGL3_FB_t * fb;

    int lastDepthTest;
    int lastAlphaRule;
    int lastEtchRule;
    int lastTextureFilter;
    
    topazGL3_Program_t * defaultProgram;
};

topazGL3_t * topaz_gl3_create() {
    topazGL3_t * out = calloc(1, sizeof(topazGL3_t));
    out->fb = 0;

    out->lastAlphaRule = -1;
    out->lastDepthTest = -1;
    out->lastEtchRule = -1;
    out->lastTextureFilter = -1;


    return out;
}

void topaz_gl3_destroy(topazGL3_t * e) {
    free(e);
}

topazGL3_2D_t * topaz_gl3_create_2d(topazGL3_t * e) {
    return topaz_gl3_2d_create();
}

topazGL3_FB_t * topaz_gl3_get_target(topazGL3_t * e) {
    return e->fb;
}

void topaz_gl3_set_target(topazGL3_t * e, topazGL3_FB_t * fb) {
    // flush?
    e->fb = fb;
}

int topaz_gl3_start(topazGL3_t * e) {
    TOPAZ_GLES_FN_IN;
    if (!e->fb) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);TOPAZ_GLES_CALL_CHECK;
        return 0;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, topaz_gl3_fb_get_handle(e->fb)[0]);TOPAZ_GLES_CALL_CHECK;
    glViewport(
        0, 0,
        topaz_gl3_fb_get_width(e->fb),
        topaz_gl3_fb_get_height(e->fb)
    );
    return 1;
}

// releases the framebuffer (unbind)
void topaz_gl3_end(topazGL3_t * e) {
    TOPAZ_GLES_FN_IN;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);TOPAZ_GLES_CALL_CHECK;

}

void topaz_gl3_sync(topazGL3_t * e) {
    glFinish();
}

void topaz_gl3_commit_process_attribs(
    topazGL3_t * p, 
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


}



topazGL3_Program_t * topaz_gl3_get_default_program(topazGL3_t * es) {
    if (!es->defaultProgram) {
        topazString_t * str = topaz_string_create();
        topazGL3_Program_t * out = topaz_gl3_program_create(
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


const char * topaz_gles_error_to_string(int e) {
    switch(e) {
      case 0x0500: return "GL_INVALID_ENUM";
      case 0x0501: return "GL_INVALID_VALUE";
      case 0x0502: return "GL_INVALID_OPERATION";
      case 0x0503: return "GL_STACK_OVERFLOW";
      case 0x0504: return "GL_STACK_UNDERFLOW";
      case 0x0505: return "GL_INVALID_ENUM";
    }
    static char unknerr[256];
    sprintf(unknerr, "UNKNOWN ERROR (%d)", e);
    return unknerr;
}
