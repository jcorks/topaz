#include "program.h"
#include "context.h"
#include <stdlib.h>
#include <string.h>

struct topazGL3_Program_t {
    GLint program;
    GLint vertexShader;
    GLint fragmentShader;
    // vertices 3 3 2 4 buffer
    GLint locationVBOpos;
    GLint locationVBOnormal;
    GLint locationVBOdata;
    GLint locationVBOuv;

    // mat4
    GLint locationVertexUniform_MV;
    // mat4
    GLint locationVertexUniform_Proj;
    // mat3
    GLint locationVertexUniform_Normal;

    // sampler2d (int)
    GLint locationFragmentUniform_TextureSampler_FB;
    // sampler2d (int)
    GLint locationFragmentUniform_TextureSampler_0;
    // sampler2d (int)
    GLint locationFragmentUniform_TextureSampler_1;
    // sampler2d (int)
    GLint locationFragmentUniform_TextureSampler_2;




    // 9 vectors
    GLint locationGlobalUniform_Material;    

    // vertex array object
    GLuint vao;

    topazGL3_Buffer_t * globalBuffer;

};


static char * vertex_pre = 
"attribute vec3 topaz_vertex_position;\n"
"attribute vec3 topaz_vertex_normal;\n"
"attribute vec2 topaz_vertex_uv;\n"
"attribute vec4 topaz_vertex_userdata;\n\n"

"varying vec2 _topaz_uv_out;\n"
"varying vec4 _topaz_userdata_out;\n"

"uniform mat4 topaz_mat_modelview;\n"
"uniform mat4 topaz_mat_projection;\n"
"uniform mat3 topaz_mat_normal;\n"

"uniform vec4 topaz_material[9];\n"


"void topazSetPosition(in vec4 a) {\n"
"   gl_Position = a;\n"
"}\n"

"void topazSetUV(in vec2 a) {\n"
"   _topaz_uv_out = a;\n"
"}\n"

"void topazSetUserData(in vec4 a) {\n"
"   _topaz_userdata_out = a;\n"
"}\n"



;



static char * fragment_pre = 

"varying vec2 _topaz_uv_out;\n"
"varying vec4 _topaz_userdata_out;\n"

"uniform sampler2D _topaz_t_fb;\n"
"uniform sampler2D _topaz_t_0;\n"
"uniform sampler2D _topaz_t_1;\n"
"uniform sampler2D _topaz_t_2;\n"



"uniform vec4 topaz_material[9];\n"



"void topazSetOutput(in vec4 a) {\n"
"   gl_FragColor = a;\n"
"}\n"


"vec4 topazSampleTexture0(in vec2 uvs) {\n"
"   return texture2D(_topaz_t_0, uvs);\n"
"}\n"

"vec4 topazSampleTexture1(in vec2 uvs) {\n"
"   return texture2D(_topaz_t_1, uvs);\n"
"}\n"

"vec4 topazSampleTexture2(in vec2 uvs) {\n"
"   return texture2D(_topaz_t_2, uvs);\n"
"}\n"

"vec4 topazGetUserData() {\n"
"   return _topaz_userdata_out;\n"
"}\n"
"vec2 topazGetUV() {\n"
"   return _topaz_uv_out;\n"
"}\n"

;




// Creates a program meant for rendering
topazGL3_Program_t * topaz_gl3_program_create(
    const topazString_t * vertexSrc,
    const topazString_t * fragSrc,
    topazString_t * logout
) {
    TOPAZ_GLES_FN_IN;
    topazGL3_Program_t * out = calloc(1, sizeof(topazGL3_Program_t));
    out->vertexShader   = glCreateShader(GL_VERTEX_SHADER);   TOPAZ_GLES_CALL_CHECK;
    out->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); TOPAZ_GLES_CALL_CHECK;
    out->program = glCreateProgram();TOPAZ_GLES_CALL_CHECK;


    topazString_t * vertexSrc_full = topaz_string_create_from_c_str(vertex_pre);
    topaz_string_concat(vertexSrc_full, vertexSrc);
    topazString_t * fragmentSrc_full = topaz_string_create_from_c_str(fragment_pre);
    topaz_string_concat(fragmentSrc_full, fragSrc);

    const char * source[2];
    source[0] = (char*) topaz_string_get_c_str(vertexSrc_full);
    source[1] = (char*) topaz_string_get_c_str(fragmentSrc_full);    



    GLint result;
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
        topaz_string_clear(logout);
        topaz_string_concat_printf(logout, "GL3 renderer program compilation: Vertex shader failed to compile. Log:\n%s\n", log);
        free(log);

        // cleanup
        topaz_string_destroy(vertexSrc_full);
        topaz_string_destroy(fragmentSrc_full);
        glDeleteShader(out->vertexShader);TOPAZ_GLES_CALL_CHECK;
        glDeleteShader(out->fragmentShader);TOPAZ_GLES_CALL_CHECK;
        free(out);
        
        return NULL;
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
        topaz_string_clear(logout);
        topaz_string_concat_printf(logout, "GL3 renderer program compilation: Fragment shader failed to compile. Log:\n%s\n", log);
        free(log);

        // cleanup
        topaz_string_destroy(vertexSrc_full);
        topaz_string_destroy(fragmentSrc_full);
        glDeleteShader(out->vertexShader);TOPAZ_GLES_CALL_CHECK;
        glDeleteShader(out->fragmentShader);TOPAZ_GLES_CALL_CHECK;
        free(out);
        return NULL;        
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
        topaz_string_clear(logout);
        topaz_string_concat_printf(logout, "GL3 renderer program compilation: Program failed to link. Log:\n%s\n", log);
        free(log);

        // cleanup
        topaz_string_destroy(vertexSrc_full);
        topaz_string_destroy(fragmentSrc_full);
        glDeleteShader(out->vertexShader);TOPAZ_GLES_CALL_CHECK;
        glDeleteShader(out->fragmentShader);TOPAZ_GLES_CALL_CHECK;
        free(out);
        return NULL;        
    }
    topaz_string_destroy(vertexSrc_full);
    topaz_string_destroy(fragmentSrc_full);
    glDeleteShader(out->vertexShader);TOPAZ_GLES_CALL_CHECK;
    glDeleteShader(out->fragmentShader);TOPAZ_GLES_CALL_CHECK;


    out->locationVertexUniform_MV = glGetUniformLocation(out->program, "topaz_mat_modelview");TOPAZ_GLES_CALL_CHECK;
    out->locationVertexUniform_Proj = glGetUniformLocation(out->program, "topaz_mat_projection");TOPAZ_GLES_CALL_CHECK;
    out->locationVertexUniform_Normal = glGetUniformLocation(out->program, "topaz_mat_normal");TOPAZ_GLES_CALL_CHECK;

    out->locationFragmentUniform_TextureSampler_0 = glGetUniformLocation(out->program, "_topaz_t_0");TOPAZ_GLES_CALL_CHECK;
    out->locationFragmentUniform_TextureSampler_1 = glGetUniformLocation(out->program, "_topaz_t_1");TOPAZ_GLES_CALL_CHECK;
    out->locationFragmentUniform_TextureSampler_2 = glGetUniformLocation(out->program, "_topaz_t_2");TOPAZ_GLES_CALL_CHECK;




    out->locationGlobalUniform_Material = glGetUniformLocation(out->program, "_topaz_material");TOPAZ_GLES_CALL_CHECK;

    glGenVertexArrays(1, &out->vao);
    glBindVertexArray(out->vao);

    out->locationVBOpos      = glGetAttribLocation(out->program, "topaz_vertex_position");TOPAZ_GLES_CALL_CHECK;
    out->locationVBOnormal   = glGetAttribLocation(out->program, "topaz_vertex_normal");TOPAZ_GLES_CALL_CHECK;
    out->locationVBOuv       = glGetAttribLocation(out->program, "topaz_vertex_uv");TOPAZ_GLES_CALL_CHECK;
    out->locationVBOdata     = glGetAttribLocation(out->program, "topaz_vertex_userdata");TOPAZ_GLES_CALL_CHECK;


    if (out->locationVBOpos > -1)    glEnableVertexAttribArray(out->locationVBOpos);TOPAZ_GLES_CALL_CHECK;
    if (out->locationVBOnormal > -1) glEnableVertexAttribArray(out->locationVBOnormal);TOPAZ_GLES_CALL_CHECK;
    if (out->locationVBOuv > -1)     glEnableVertexAttribArray(out->locationVBOuv);TOPAZ_GLES_CALL_CHECK;
    if (out->locationVBOdata > -1)   glEnableVertexAttribArray(out->locationVBOdata);TOPAZ_GLES_CALL_CHECK;


    glBindVertexArray(0);


    glUseProgram(out->program);TOPAZ_GLES_CALL_CHECK;
    glUniform1i(out->locationFragmentUniform_TextureSampler_0, 1);TOPAZ_GLES_CALL_CHECK;
    glUniform1i(out->locationFragmentUniform_TextureSampler_1, 2);TOPAZ_GLES_CALL_CHECK;
    glUniform1i(out->locationFragmentUniform_TextureSampler_2, 3);TOPAZ_GLES_CALL_CHECK;
    glUseProgram(0);TOPAZ_GLES_CALL_CHECK;



    return out;
}

// Updates the program's material section of uniforms.
// These will change basically every iteration.
void topaz_gl3_program_update_dynamic(
    topazGL3_Program_t * program,
    const float * material_float28,
    const float * mvmatrix_float16_rowmajor,
    const float * projmatrix_float16_rowmajor
) {
    TOPAZ_GLES_FN_IN;
    glUseProgram(program->program);
    // material
    if (material_float28 && program->locationGlobalUniform_Material > -1) {
        glUniform4fv(
            program->locationGlobalUniform_Material,
            9,
            material_float28
        );TOPAZ_GLES_CALL_CHECK;
    }

    if (mvmatrix_float16_rowmajor) {
        if (program->locationVertexUniform_Normal > -1) {
        
        
            float normal[9];
            float top[3];
            const float * m = mvmatrix_float16_rowmajor;
            top[0] = normal[0];
            top[1] = normal[1];            
            top[2] = normal[2];

            // MxM
            normal[0] = m[5]*m[10] - m[6]*m[9];
            normal[1] = m[4]*m[10] - m[6]*m[8];
            normal[2] = m[4]*m[9]  - m[5]*m[8];

            normal[3] = m[1]*m[10] - m[2]*m[9];
            normal[4] = m[0]*m[10] - m[2]*m[8];
            normal[5] = m[0]*m[9]  - m[1]*m[8];

            normal[6] = m[1]*m[6]  - m[2]*m[5];
            normal[7] = m[0]*m[6]  - m[2]*m[4];
            normal[8] = m[0]*m[5]  - m[1]*m[4];


            // cofactor
            normal[1] *= -1;
            normal[3] *= -1;
            normal[5] *= -1;
            normal[7] *= -1;

            float determinant = top[0] * normal[0] + top[1] * normal[1] + top[2] * normal[2];
            
            // invert (adjugate)
            float temp;
            temp = normal[1]; normal[1] = normal[3]; normal[3] = temp;
            temp = normal[2]; normal[2] = normal[6]; normal[6] = temp;
            temp = normal[5]; normal[5] = normal[7]; normal[7] = temp;
            
            
            int i;
            for(i = 0; i < 9; ++i) {
                normal[i] *= determinant;
            }
            
            glUniformMatrix3fv(
                program->locationVertexUniform_Normal,
                1,
                GL_FALSE, // already transposed
                normal                
            );TOPAZ_GLES_CALL_CHECK;

        }
        
        
        if (program->locationVertexUniform_MV > -1) {
            glUniformMatrix4fv(
                program->locationVertexUniform_MV,
                1,
                GL_TRUE,
                mvmatrix_float16_rowmajor
            );TOPAZ_GLES_CALL_CHECK;
        }
    }
    
    if (projmatrix_float16_rowmajor && program->locationVertexUniform_Proj > -1) {
        glUniformMatrix4fv(
            program->locationVertexUniform_Proj,
            1,
            GL_TRUE,
            projmatrix_float16_rowmajor
        );TOPAZ_GLES_CALL_CHECK;
    }
    glUseProgram(0);

}


// will auto update.
// is offline (malloc/freed)
// its size reflects the maximum amount that a user can rely on,
// subtracting the space required for:
// 
// - material data  (9 vec4)
// - matrices       (12 vec4, vertex shader only)
// - texture        (1 int4 + 3 vec4, fragment shader only)
topazGL3_Buffer_t * topaz_gl3_program_get_static_buffer(
    topazGL3_Program_t * program
) {
    return NULL;
}



void topaz_gl3_program_bind_texture(
    topazGL3_Program_t * program,
    int slotIndex, // either 0, 1, or 2
    topazGL3_Texture_t * tex,
    int filter
) {
    GLuint id;
    
    TOPAZ_GLES_FN_IN;
    glUseProgram(program->program);TOPAZ_GLES_CALL_CHECK;

    int location;
    switch(slotIndex) {
      case 0: 
        location = program->locationFragmentUniform_TextureSampler_0;
        if (location < 0) return;
        glActiveTexture(GL_TEXTURE0+0);TOPAZ_GLES_CALL_CHECK;
        glUniform1i(location, 0);
        break;

      case 1: 
        location = program->locationFragmentUniform_TextureSampler_1;

        if (location < 0) return;
        glActiveTexture(GL_TEXTURE0+1);TOPAZ_GLES_CALL_CHECK;
        glUniform1i(location, 1);
        break;

      case 2: 
        location = program->locationFragmentUniform_TextureSampler_2;
        
        if (location < 0) return;
        glActiveTexture(GL_TEXTURE0+2);TOPAZ_GLES_CALL_CHECK;
        glUniform1i(location, 2);
        break;


    }
    
    if (location < 0) return;
    id = topaz_gl3_texture_get_handle(tex);
    glBindTexture(GL_TEXTURE_2D, id);TOPAZ_GLES_CALL_CHECK;

    topaz_gl3_texture_set_filter(tex, filter);
}


// renders using the given program.
void topaz_gl3_program_render(
    // program
    topazGL3_Program_t * p,
    // 3 3 2 4 buffer
    topazGL3_Buffer_t * vertexBuffer,
    
    // indices
    const uint32_t * indices,

    // count of indices
    uint32_t count
) {
    TOPAZ_GLES_FN_IN;
    topaz_gl3_buffer_set_type(vertexBuffer, GL_ARRAY_BUFFER);

    glUseProgram(p->program); TOPAZ_GLES_CALL_CHECK;

    

    
    glBindVertexArray(p->vao);
    glBindBuffer(GL_ARRAY_BUFFER, topaz_gl3_buffer_get_buffer_id(vertexBuffer));TOPAZ_GLES_CALL_CHECK;

    if (p->locationVBOpos > -1)    glVertexAttribPointer(p->locationVBOpos,      3, GL_FLOAT, GL_FALSE, sizeof(float)*12, 0);TOPAZ_GLES_CALL_CHECK;
    if (p->locationVBOnormal > -1) glVertexAttribPointer(p->locationVBOnormal,   3, GL_FLOAT, GL_FALSE, sizeof(float)*12, (void*)(sizeof(float)*3));TOPAZ_GLES_CALL_CHECK;
    if (p->locationVBOuv > -1)     glVertexAttribPointer(p->locationVBOuv,       2, GL_FLOAT, GL_FALSE, sizeof(float)*12, (void*)(sizeof(float)*6));TOPAZ_GLES_CALL_CHECK;
    if (p->locationVBOdata > -1)   glVertexAttribPointer(p->locationVBOdata,     4, GL_FLOAT, GL_FALSE, sizeof(float)*12, (void*)(sizeof(float)*8));TOPAZ_GLES_CALL_CHECK;

    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, indices);TOPAZ_GLES_CALL_CHECK;


    glBindVertexArray(0);
    glUseProgram(0);TOPAZ_GLES_CALL_CHECK;


}
