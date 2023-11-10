#ifndef H_TOPAZGL3PROGRAMM_INCLUDED
#define H_TOPAZGL3PROGRAMM_INCLUDED

#include "texture.h"
#include "buffer.h"
#include "context.h"
#include <topaz/backends/renderer.h>


typedef struct topazGL3_Program_t topazGL3_Program_t;

// Creates a program meant for rendering
topazGL3_Program_t * topaz_gl3_program_create(
    const topazString_t * vertexSrc,
    const topazString_t * fragSrc,
    topazString_t * log
);

// Updates the program's material section of uniforms.
// These will change basically every iteration.
void topaz_gl3_program_update_dynamic(
    topazGL3_Program_t * program,
    const float * material_float28,
    const float * mvmatrix_float16_rowmajor,
    const float * projmatrix_float16_rowmajor

);


// will auto update.
// is offline (malloc/freed)
// its size reflects the maximum amount that a user can rely on,
// subtracting the space required for:
// 
// - material data  (9 vec4)
// - matrices       (11 vec4, vertex shader only)
// - texture        (1 int4 + 3 vec4, fragment shader only)
topazGL3_Buffer_t * topaz_gl3_program_get_static_buffer(
    topazGL3_Program_t * program
);


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
void topaz_gl3_program_bind_texture(
    topazGL3_Program_t * program,
    int slotIndex, // either 0, 1, or 2
    topazGL3_Texture_t * tex 
);


// renders using the given program.
void topaz_gl3_program_render(
    // program
    topazGL3_Program_t * program,
    // 3 3 2 4 buffer
    topazGL3_Buffer_t * vertexBuffer,
    

    // indices
    const uint32_t * indices,

    // count of indices
    uint32_t count
);


#endif
