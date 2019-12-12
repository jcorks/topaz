#include <topaz/compat.h>
#include <topaz/math.h>

#include <stdlib.h>
#include <string.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif



topazVector_t topaz_math_parallaxize(
    const topazVector_t * p, 
    const topazVector_t * camera,

    float screenWidth, 
    float screenHeight, 
    float screenMaxDepth
) {
    topazVector_t out;

    topazVector_t vanishingVector = *camera;
    vanishingVector.x += screenWidth/2;
    vanishingVector.y += screenHeight/2;



    float influence = 1 - (p->z / screenMaxDepth);
    topaz_math_clamp(influence, 0.f, 2.f);


    out.x = vanishingVector.x + (p->x - vanishingVector.x)*influence;
    out.y = vanishingVector.y + (p->y - vanishingVector.y)*influence;
    out.z = 0.f;
    return out;
}
