#include "program.h"
#include "context.h"
#include <stdlib.h>
#include <string.h>

struct topazES2_Program_t {
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


    // vec4
    GLint locationFragmentUniform_Texture0Data;
    // vec4
    GLint locationFragmentUniform_Texture1Data;
    // vec4
    GLint locationFragmentUniform_Texture2Data;

    // 9 vectors
    GLint locationGlobalUniform_Material;    


    topazES2_Buffer_t * globalBuffer;

};


static char * vertex_pre = 
"attribute vec3 topaz_vertex_position;\n"
"attribute vec3 topaz_vertex_normal;\n"
"attribute vec2 topaz_vertex_uv;\n"
"attribute vec4 topaz_vertex_userdata;\n\n"

"varying vec2 _topaz_uv_out;\n"
"varying vec2 _topaz_userdata_out;\n"

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

"varying mediump vec2 _topaz_uv_out;\n"
"varying mediump vec2 _topaz_userdata_out;\n"

"uniform sampler2D _topaz_t_fb;\n"
"uniform sampler2D _topaz_t_0;\n"
"uniform sampler2D _topaz_t_1;\n"
"uniform sampler2D _topaz_t_2;\n"
"uniform vec4 _topaz_t0;\n"
"uniform vec4 _topaz_t1;\n"
"uniform vec4 _topaz_t2;\n"

"uniform vec4 topaz_material[9];\n"



"void topazSetOutput(in vec4 a) {\n"
"   gl_FragColor = a;\n"
"}\n"


"vec4 topazSampleFramebuffer(in vec2 uvs) {\n"
"   return texture2D(_topaz_t_fb, uvs);\n"
"}\n"

"vec4 topazSampleTexture0(in vec2 uvs) {\n"
"   vec2 uvs_conv = vec2((uvs.x / _topaz_t0.y) + _topaz_t0.x, (uvs.y / _topaz_t0.w) + _topaz_t0.z);\n"
"   return texture2D(_topaz_t_0, uvs_conv);\n"
"}\n"

"vec4 topazSampleTexture1(in vec2 uvs) {\n"
"   vec2 uvs_conv = vec2((uvs.x / _topaz_t1.y) + _topaz_t1.x, (uvs.y / _topaz_t1.w) + _topaz_t1.z);\n"
"   return texture2D(_topaz_t_1, uvs_conv);\n"
"}\n"

"vec4 topazSampleTexture2(in vec2 uvs) {\n"
"   vec2 uvs_conv = vec2((uvs.x / _topaz_t2.y) + _topaz_t2.x, (uvs.y / _topaz_t2.w) + _topaz_t2.z);\n"
"   return texture2D(_topaz_t_2, uvs_conv);\n"
"}\n"
;




// Creates a program meant for rendering
topazES2_Program_t * topaz_es2_program_create(
    const topazString_t * vertexSrc,
    const topazString_t * fragSrc,
    topazString_t * logout
) {
    TOPAZ_GLES_FN_IN;
    topazES2_Program_t * out = calloc(1, sizeof(topazES2_Program_t));
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
        topaz_string_concat_printf(logout, "ES2 renderer program compilation: Vertex shader failed to compile. Log:\n%s\n", log);
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
        topaz_string_concat_printf(logout, "ES2 renderer program compilation: Fragment shader failed to compile. Log:\n%s\n", log);
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
        topaz_string_concat_printf(logout, "ES2 renderer program compilation: Program failed to link. Log:\n%s\n", log);
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

    out->locationFragmentUniform_TextureSampler_FB = glGetUniformLocation(out->program, "_topaz_t_fb");TOPAZ_GLES_CALL_CHECK;
    out->locationFragmentUniform_TextureSampler_0 = glGetUniformLocation(out->program, "_topaz_t_0");TOPAZ_GLES_CALL_CHECK;
    out->locationFragmentUniform_TextureSampler_1 = glGetUniformLocation(out->program, "_topaz_t_1");TOPAZ_GLES_CALL_CHECK;
    out->locationFragmentUniform_TextureSampler_2 = glGetUniformLocation(out->program, "_topaz_t_2");TOPAZ_GLES_CALL_CHECK;

    out->locationFragmentUniform_Texture0Data = glGetUniformLocation(out->program, "_topaz_t0");TOPAZ_GLES_CALL_CHECK;
    out->locationFragmentUniform_Texture1Data = glGetUniformLocation(out->program, "_topaz_t1");TOPAZ_GLES_CALL_CHECK;
    out->locationFragmentUniform_Texture2Data = glGetUniformLocation(out->program, "_topaz_t2");TOPAZ_GLES_CALL_CHECK;

    out->locationGlobalUniform_Material = glGetUniformLocation(out->program, "_topaz_material");TOPAZ_GLES_CALL_CHECK;

    out->locationVBOpos      = glGetAttribLocation(out->program, "topaz_vertex_position");TOPAZ_GLES_CALL_CHECK;
    out->locationVBOnormal   = glGetAttribLocation(out->program, "topaz_vertex_normal");TOPAZ_GLES_CALL_CHECK;
    out->locationVBOuv       = glGetAttribLocation(out->program, "topaz_vertex_uv");TOPAZ_GLES_CALL_CHECK;
    out->locationVBOdata     = glGetAttribLocation(out->program, "topaz_vertex_userdata");TOPAZ_GLES_CALL_CHECK;



    glUseProgram(out->program);TOPAZ_GLES_CALL_CHECK;
    glUniform1i(out->locationFragmentUniform_TextureSampler_FB, 0);TOPAZ_GLES_CALL_CHECK;
    glUniform1i(out->locationFragmentUniform_TextureSampler_0, 1);TOPAZ_GLES_CALL_CHECK;
    glUniform1i(out->locationFragmentUniform_TextureSampler_1, 2);TOPAZ_GLES_CALL_CHECK;
    glUniform1i(out->locationFragmentUniform_TextureSampler_2, 3);TOPAZ_GLES_CALL_CHECK;
    glUseProgram(0);TOPAZ_GLES_CALL_CHECK;



    return out;
}

// Updates the program's material section of uniforms.
// These will change basically every iteration.
void topaz_es2_program_update_dynamic(
    topazES2_Program_t * program,
    const float * material_float28,
    const float * mvmatrix_float16_rowmajor,
    const float * projmatrix_float16_rowmajor
) {
    TOPAZ_GLES_FN_IN;
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

}


// will auto update.
// is offline (malloc/freed)
// its size reflects the maximum amount that a user can rely on,
// subtracting the space required for:
// 
// - material data  (9 vec4)
// - matrices       (12 vec4, vertex shader only)
// - texture        (1 int4 + 3 vec4, fragment shader only)
topazES2_Buffer_t * topaz_es2_program_get_static_buffer(
    topazES2_Program_t * program
) {
    return NULL;
}


// sets the parameters to texture slots.
// raw data:

//
// sampler2D topaz_tex[index]_realID
// vec4      topaz_tex[index]_data = {
/*      
        C_x, // = (localX / atlasWidth) + (localWidth / atlasWidth)
        atlasWidth,
        C_y, // = (localY / atlasHeight) + (localHeight / atlasHeight)
        atlasHeight
*/
// 
// }
//
// In shader, localUV to realUV looks like:
//
//  realU = C_x + localU / atlasWidth;
//  realV = C_y + localV / atlasHeight;
void topaz_es2_program_bind_texture(
    topazES2_Program_t * program,
    int slotIndex, // either 0, 1, or 2
    topazES2_Texture_t * tex 
) {
    float local_x;
    float local_y;
    float local_w;
    float local_h;
    float atlas_w;
    float atlas_h;
    GLuint id;
    
    TOPAZ_GLES_FN_IN;
    glUseProgram(program->program);TOPAZ_GLES_CALL_CHECK;

    int location;
    switch(slotIndex) {
      case 0: 
        location = program->locationFragmentUniform_TextureSampler_0;
        if (location < 0) return;
        glActiveTexture(GL_TEXTURE0+0);TOPAZ_GLES_CALL_CHECK;
        break;

      case 1: 
        location = program->locationFragmentUniform_TextureSampler_1;
        if (location < 0) return;
        glActiveTexture(GL_TEXTURE0+1);TOPAZ_GLES_CALL_CHECK;
        break;

      case 2: 
        location = program->locationFragmentUniform_TextureSampler_2;
        if (location < 0) return;
        glActiveTexture(GL_TEXTURE0+2);TOPAZ_GLES_CALL_CHECK;
        break;


    }
    
    topaz_es2_texture_get_info(
        tex,
        &local_x,
        &local_y,
        &local_w,
        &local_h,

        &atlas_w,
        &atlas_h,
        &id
    );

    glBindTexture(GL_TEXTURE_2D, id);TOPAZ_GLES_CALL_CHECK;



    float values[4];
    values[0] = (local_x / atlas_w) + (local_w / atlas_w);
    values[1] = atlas_w;
    values[2] = (local_y / atlas_h) + (local_h / atlas_h);
    values[3] = atlas_h;
    
    
    glUniform4fv(
        location,
        4,
        values
    );TOPAZ_GLES_CALL_CHECK;
}


// renders using the given program.
void topaz_es2_program_render(
    // program
    topazES2_Program_t * p,
    // 3 3 2 4 buffer
    topazES2_Buffer_t * vertexBuffer,

    // samp (-1 if not used)
    GLuint framebufferTexID,    
    
    // indices
    const uint32_t * indices,

    // count of indices
    uint32_t count
) {
    TOPAZ_GLES_FN_IN;
    glUseProgram(p->program); TOPAZ_GLES_CALL_CHECK;
    
    if (p->locationFragmentUniform_TextureSampler_FB > -1) 
        glUniform1i(p->locationFragmentUniform_TextureSampler_FB, framebufferTexID);

    glEnableVertexAttribArray(p->locationVBOpos);TOPAZ_GLES_CALL_CHECK;
    glEnableVertexAttribArray(p->locationVBOnormal);TOPAZ_GLES_CALL_CHECK;
    glEnableVertexAttribArray(p->locationVBOuv);TOPAZ_GLES_CALL_CHECK;
    glEnableVertexAttribArray(p->locationVBOdata);TOPAZ_GLES_CALL_CHECK;

    glBindBuffer(GL_ARRAY_BUFFER, topaz_es2_buffer_get_buffer_id(vertexBuffer));TOPAZ_GLES_CALL_CHECK;

    glVertexAttribPointer(p->locationVBOpos,      3, GL_FLOAT, GL_FALSE, sizeof(float)*12, 0);TOPAZ_GLES_CALL_CHECK;
    glVertexAttribPointer(p->locationVBOnormal,   3, GL_FLOAT, GL_FALSE, sizeof(float)*12, (void*)(sizeof(float)*3));TOPAZ_GLES_CALL_CHECK;
    glVertexAttribPointer(p->locationVBOuv,       2, GL_FLOAT, GL_FALSE, sizeof(float)*12, (void*)(sizeof(float)*6));TOPAZ_GLES_CALL_CHECK;
    glVertexAttribPointer(p->locationVBOdata,     4, GL_FLOAT, GL_FALSE, sizeof(float)*12, (void*)(sizeof(float)*8));TOPAZ_GLES_CALL_CHECK;



    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, indices);TOPAZ_GLES_CALL_CHECK;


    glDisableVertexAttribArray(p->locationVBOpos);TOPAZ_GLES_CALL_CHECK;
    glDisableVertexAttribArray(p->locationVBOnormal);TOPAZ_GLES_CALL_CHECK;
    glDisableVertexAttribArray(p->locationVBOuv);TOPAZ_GLES_CALL_CHECK;
    glDisableVertexAttribArray(p->locationVBOdata);TOPAZ_GLES_CALL_CHECK;

    glUseProgram(0);TOPAZ_GLES_CALL_CHECK;


}
