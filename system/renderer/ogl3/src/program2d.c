#include "program2d.h"
#include "context.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct topazGL3_Program2D_t {
    GLint program;
    GLint vertexShader;
    GLint fragmentShader;

    GLint locationVBOposition;
    GLint locationVBOuv;
    GLint locationVBOrgba;
    GLuint vao;

    GLint locationUniformMatrixGlobalTF;
    GLint locationUniformMatrixGlobalProj;
    GLint locationUniformMatrixLocal;
    GLint locationUniformSampler;
    GLint locationUniformUseTexturing;

    int lastW, lastH;
};

#include "glsl_bytes"

topazGL3_Program2D_t * topaz_gl3_p2d_create() {
    TOPAZ_GLES_FN_IN;
    topazGL3_Program2D_t * out = calloc(1, sizeof(topazGL3_Program2D_t));
    out->vertexShader   = glCreateShader(GL_VERTEX_SHADER);   TOPAZ_GLES_CALL_CHECK;
    out->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); TOPAZ_GLES_CALL_CHECK;
    out->program = glCreateProgram();TOPAZ_GLES_CALL_CHECK;
    GLint result;

    const char * source[2];
    source[0] = (char*)   vertex_shader_2d_bytes;
    source[1] = (char*) fragment_shader_2d_bytes;    
    // vertex shader
    glShaderSource(
        out->vertexShader,
        1,
        source,
        NULL
    ); TOPAZ_GLES_CALL_CHECK;
    glCompileShader(out->vertexShader);TOPAZ_GLES_CALL_CHECK;
    glGetShaderiv(out->vertexShader, GL_COMPILE_STATUS, &result);TOPAZ_GLES_CALL_CHECK;
    if (!result) {
        int logLen = 2048;
        char * log = malloc(logLen);
        glGetShaderInfoLog(
            out->vertexShader,
            logLen,
            NULL,
            log
        );TOPAZ_GLES_CALL_CHECK;
        printf("GL3 (2D renderer): Vertex shader failed to compile. Log:\n%s\n", log);
        free(log);
        exit(10);
    } 
    glAttachShader(out->program, out->vertexShader);TOPAZ_GLES_CALL_CHECK;

    // fragment shader 
    glShaderSource(
        out->fragmentShader,
        1,
        source+1,
        NULL
    );TOPAZ_GLES_CALL_CHECK;
    glCompileShader(out->fragmentShader);TOPAZ_GLES_CALL_CHECK;
    glGetShaderiv(out->fragmentShader, GL_COMPILE_STATUS, &result);TOPAZ_GLES_CALL_CHECK;
    if (!result) {
        int logLen = 2048;
        char * log = malloc(logLen);
        glGetShaderInfoLog(
            out->fragmentShader,
            logLen,
            NULL,
            log
        );TOPAZ_GLES_CALL_CHECK;
        printf("GL3 (2D renderer): Fragment shader failed to compile. Log:\n%s\n", log);
        free(log);
        exit(11);
    }
    glAttachShader(out->program, out->fragmentShader);TOPAZ_GLES_CALL_CHECK;


    glLinkProgram(out->program);TOPAZ_GLES_CALL_CHECK;
    glGetProgramiv(out->program, GL_LINK_STATUS, &result);TOPAZ_GLES_CALL_CHECK;
    if (!result) {
        int logLen = 2048;
        char * log = malloc(logLen);
        glGetProgramInfoLog(
            out->program,
            logLen,
            NULL,
            log
        );TOPAZ_GLES_CALL_CHECK;
        printf("GL3 (2D renderer): Program failed to link. Log:\n%s\n", log);
        free(log);
        exit(112);
    }


    glDeleteShader(out->vertexShader);TOPAZ_GLES_CALL_CHECK;
    glDeleteShader(out->fragmentShader);TOPAZ_GLES_CALL_CHECK;
    
    out->locationVBOposition = glGetAttribLocation(out->program, "position");TOPAZ_GLES_CALL_CHECK;
    #ifdef TOPAZDC_DEBUG
        assert(out->locationVBOposition != -1);
    #endif
    out->locationVBOuv       = glGetAttribLocation(out->program, "uv");TOPAZ_GLES_CALL_CHECK;
    #ifdef TOPAZDC_DEBUG
        assert(out->locationVBOuv != -1);
    #endif
    out->locationVBOrgba     = glGetAttribLocation(out->program, "rgba");TOPAZ_GLES_CALL_CHECK;
    #ifdef TOPAZDC_DEBUG
        assert(out->locationVBOrgba != -1);
    #endif



    out->locationUniformMatrixGlobalProj = glGetUniformLocation(out->program, "proj");TOPAZ_GLES_CALL_CHECK;
    #ifdef TOPAZDC_DEBUG
        assert(out->locationUniformMatrixGlobalProj != -1);
    #endif
    out->locationUniformMatrixGlobalTF   = glGetUniformLocation(out->program, "mv");TOPAZ_GLES_CALL_CHECK;
    #ifdef TOPAZDC_DEBUG
        assert(out->locationUniformMatrixGlobalTF != -1);
    #endif
    out->locationUniformMatrixLocal      = glGetUniformLocation(out->program, "localMat");TOPAZ_GLES_CALL_CHECK;
    #ifdef TOPAZDC_DEBUG
        assert(out->locationUniformMatrixLocal != -1);
    #endif

    out->locationUniformSampler      = glGetUniformLocation(out->program, "sampler");TOPAZ_GLES_CALL_CHECK;
    #ifdef TOPAZDC_DEBUG
        assert(out->locationUniformSampler != -1);
    #endif
    out->locationUniformUseTexturing = glGetUniformLocation(out->program, "useTexturing");TOPAZ_GLES_CALL_CHECK;
    #ifdef TOPAZDC_DEBUG
        assert(out->locationUniformUseTexturing != -1);
    #endif


    glUseProgram(out->program);TOPAZ_GLES_CALL_CHECK;
    glUniform1i(out->locationUniformSampler, 0);TOPAZ_GLES_CALL_CHECK;

    glGenVertexArrays(1, &out->vao);
    glBindVertexArray(out->vao);

    glEnableVertexAttribArray(out->locationVBOposition);TOPAZ_GLES_CALL_CHECK;
    glEnableVertexAttribArray(out->locationVBOuv);TOPAZ_GLES_CALL_CHECK;
    glEnableVertexAttribArray(out->locationVBOrgba);TOPAZ_GLES_CALL_CHECK;



    glBindVertexArray(0);
    glUseProgram(0);TOPAZ_GLES_CALL_CHECK;

    return out;
}


static void projection_orthographic(topazMatrix_t * out,
    float left, float right,
    float top, float bottom,
    float zNear, float zFar
) {
    topaz_matrix_set_identity(out);
    float * projection = out->data+0;
    projection[0] = (2)  / (right - left);
    projection[5] = (2)  / (top - bottom);
    projection[10] = (-2) / (zFar - zNear);

    projection[3]  = -(right + left) / (right - left);
    projection[7]  = -(top + bottom) / (top - bottom);
    projection[11] = -(zFar + zNear) / (zFar - zNear);

}

void topaz_gl3_p2d_render(
    topazGL3_Program2D_t * p, 
    const topazRenderer_2D_Context_t * ctx,
    const topazRenderer_Attributes_t * attribs,
    const topazGL3_Program2D_Renderable_t ** objects,
    uint32_t count
) {
    TOPAZ_GLES_FN_IN;
    glUseProgram(p->program);TOPAZ_GLES_CALL_CHECK;

    // upload projection if applicable
    if ((int)ctx->width != p->lastW ||
        (int)ctx->height != p->lastH) {

        topazMatrix_t out;
        projection_orthographic(
            &out, 
            -ctx->width/2, ctx->width/2,
            ctx->height/2, -ctx->height/2,
            1000, -1000
        );

        glUniformMatrix4fv(
            p->locationUniformMatrixGlobalProj,
            1, // count,
            1, // transpose (yes)
            (GLfloat*)&out
        );TOPAZ_GLES_CALL_CHECK;

        p->lastW = ctx->width;
        p->lastH = ctx->height;
    }


    // upload main transform always
    glUniformMatrix4fv(
        p->locationUniformMatrixGlobalTF,
        1, // count,
        1, // transpose (yes)
        (GLfloat*)ctx->transform
    );TOPAZ_GLES_CALL_CHECK;




    glActiveTexture(GL_TEXTURE0);TOPAZ_GLES_CALL_CHECK;
    glBindTexture(GL_TEXTURE_2D, 0);TOPAZ_GLES_CALL_CHECK;

    uint32_t i;
    int primitive = attribs->primitive == topazRenderer_Primitive_Triangle ? GL_TRIANGLES : GL_LINES;
    GLuint lastTexture = 0;
    const topazGL3_Program2D_Renderable_t * current;

    glBindVertexArray(p->vao);



    for(i = 0; i < count; ++i) {
        current = objects[i];
        glBindBuffer(GL_ARRAY_BUFFER, current->vbo);TOPAZ_GLES_CALL_CHECK;

        // TODO: prep this for all objects ahead of time.
        glVertexAttribPointer(p->locationVBOposition, 2, GL_FLOAT, GL_FALSE, sizeof(topazRenderer_2D_Vertex_t), 0);TOPAZ_GLES_CALL_CHECK;
        glVertexAttribPointer(p->locationVBOrgba,     4, GL_FLOAT, GL_FALSE, sizeof(topazRenderer_2D_Vertex_t), (void*)(sizeof(float)*2));TOPAZ_GLES_CALL_CHECK;
        glVertexAttribPointer(p->locationVBOuv,       2, GL_FLOAT, GL_FALSE, sizeof(topazRenderer_2D_Vertex_t), (void*)(sizeof(float)*6));TOPAZ_GLES_CALL_CHECK;


        if (lastTexture != current->texture) {       
            glBindTexture(GL_TEXTURE_2D, current->texture);TOPAZ_GLES_CALL_CHECK;
            lastTexture = current->texture;
        }

        glUniformMatrix4fv(
            p->locationUniformMatrixLocal,
            1, // count,
            1, // transpose (yes!)
            (GLfloat*)&current->localMatrix
        );TOPAZ_GLES_CALL_CHECK;

        glUniform1i(p->locationUniformUseTexturing, current->texture ? 1 : 0);TOPAZ_GLES_CALL_CHECK;


        glDrawArrays(primitive, 0, current->vboVertexCount);TOPAZ_GLES_CALL_CHECK;
    }
    

    glUseProgram(0);TOPAZ_GLES_CALL_CHECK;
    glBindBuffer(GL_ARRAY_BUFFER, 0);TOPAZ_GLES_CALL_CHECK;
    glBindVertexArray(0);

} 




