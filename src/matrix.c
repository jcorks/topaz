#include <topaz/compat.h>
#include <topaz/matrix.h>
#include <topaz/math.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef TOPAZDC_DEBUG
#include <assert.h>
#endif

static float identity_matrix[] = {
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f
};


static topazMatrix_t rotateXMatrix;
static topazMatrix_t rotateYMatrix;
static topazMatrix_t rotateZMatrix;
static topazMatrix_t scaleScratch;



void topaz_matrix_set_to_identity(topazMatrix_t * m) {
    memcpy(m->data, identity_matrix, sizeof(float)*16);
}


topazVector_t topaz_matrix_transform(const topazMatrix_t * m, const topazVector_t * v) {
    topazVector_t out;
    const float * op1 = (const float*)m->data;
    const float * op2 = (const float*)v;

    out.x = op1[0]  * op2[0] + op1[1]  * op2[1] +  op1[2]  * op2[2] + op1[3];
    out.y = op1[1]  * op2[0] + op1[5]  * op2[1] +  op1[6]  * op2[2] + op1[7];
    out.z = op1[2]  * op2[0] + op1[9]  * op2[1] +  op1[10] * op2[2] + op1[11];
    return out;    
}



topazString_t * topaz_matrix_to_string(const topazMatrix_t * m) {
    topazString_t * out = topaz_string_create();
    float * data = (float*)m->data;

    topaz_string_concat_printf(out, "%s", "Matrix[] = \n");
    for(int y = 0; y < 4; ++y) {
        for(int x = 0; x < 4; ++x) {
            topaz_string_concat_printf(out, "%f\t", data[x + y*4]);
        }
        topaz_string_concat_printf(out, "\n");
    }
    topaz_string_concat_printf(out, "\n");
    return out;
}


void topaz_matrix_transpose(topazMatrix_t * m) {
    float * data = (float*)m->data;
    float temp;
    temp = data[0]; data[0] = data[15]; data[15] = temp;
    temp = data[1]; data[1] = data[11]; data[11] = temp;
    temp = data[2]; data[2] = data[7];  data[7]  = temp;
    temp = data[4]; data[4] = data[14]; data[14] = temp;
    temp = data[5]; data[5] = data[10]; data[10] = temp;
    temp = data[8]; data[8] = data[13]; data[13] = temp;
}

/// Inverts the matrix.
///
void topaz_matrix_invert(topazMatrix_t * m) {
    float * data = (float*)m->data;
    float inv[16], det;
    int i;

    inv[0] = data[5]  * data[10] * data[15] - 
             data[5]  * data[11] * data[14] - 
             data[9]  * data[6]  * data[15] + 
             data[9]  * data[7]  * data[14] +
             data[13] * data[6]  * data[11] - 
             data[13] * data[7]  * data[10];

    inv[4] = -data[4]  * data[10] * data[15] + 
              data[4]  * data[11] * data[14] + 
              data[8]  * data[6]  * data[15] - 
              data[8]  * data[7]  * data[14] - 
              data[12] * data[6]  * data[11] + 
              data[12] * data[7]  * data[10];

    inv[8] = data[4]  * data[9] * data[15] - 
             data[4]  * data[11] * data[13] - 
             data[8]  * data[5] * data[15] + 
             data[8]  * data[7] * data[13] + 
             data[12] * data[5] * data[11] - 
             data[12] * data[7] * data[9];

    inv[12] = -data[4]  * data[9] * data[14] + 
               data[4]  * data[10] * data[13] +
               data[8]  * data[5] * data[14] - 
               data[8]  * data[6] * data[13] - 
               data[12] * data[5] * data[10] + 
               data[12] * data[6] * data[9];

    inv[1] = -data[1]  * data[10] * data[15] + 
              data[1]  * data[11] * data[14] + 
              data[9]  * data[2] * data[15] - 
              data[9]  * data[3] * data[14] - 
              data[13] * data[2] * data[11] + 
              data[13] * data[3] * data[10];

    inv[5] = data[0]  * data[10] * data[15] - 
             data[0]  * data[11] * data[14] - 
             data[8]  * data[2] * data[15] + 
             data[8]  * data[3] * data[14] + 
             data[12] * data[2] * data[11] - 
             data[12] * data[3] * data[10];

    inv[9] = -data[0]  * data[9] * data[15] + 
              data[0]  * data[11] * data[13] + 
              data[8]  * data[1] * data[15] - 
              data[8]  * data[3] * data[13] - 
              data[12] * data[1] * data[11] + 
              data[12] * data[3] * data[9];

    inv[13] = data[0]  * data[9] * data[14] - 
              data[0]  * data[10] * data[13] - 
              data[8]  * data[1] * data[14] + 
              data[8]  * data[2] * data[13] + 
              data[12] * data[1] * data[10] - 
              data[12] * data[2] * data[9];

    inv[2] = data[1]  * data[6] * data[15] - 
             data[1]  * data[7] * data[14] - 
             data[5]  * data[2] * data[15] + 
             data[5]  * data[3] * data[14] + 
             data[13] * data[2] * data[7] - 
             data[13] * data[3] * data[6];

    inv[6] = -data[0]  * data[6] * data[15] + 
              data[0]  * data[7] * data[14] + 
              data[4]  * data[2] * data[15] - 
              data[4]  * data[3] * data[14] - 
              data[12] * data[2] * data[7] + 
              data[12] * data[3] * data[6];

    inv[10] = data[0]  * data[5] * data[15] - 
              data[0]  * data[7] * data[13] - 
              data[4]  * data[1] * data[15] + 
              data[4]  * data[3] * data[13] + 
              data[12] * data[1] * data[7] - 
              data[12] * data[3] * data[5];

    inv[14] = -data[0]  * data[5] * data[14] + 
               data[0]  * data[6] * data[13] + 
               data[4]  * data[1] * data[14] - 
               data[4]  * data[2] * data[13] - 
               data[12] * data[1] * data[6] + 
               data[12] * data[2] * data[5];

    inv[3] = -data[1] * data[6] * data[11] + 
              data[1] * data[7] * data[10] + 
              data[5] * data[2] * data[11] - 
              data[5] * data[3] * data[10] - 
              data[9] * data[2] * data[7] + 
              data[9] * data[3] * data[6];

    inv[7] = data[0] * data[6] * data[11] - 
             data[0] * data[7] * data[10] - 
             data[4] * data[2] * data[11] + 
             data[4] * data[3] * data[10] + 
             data[8] * data[2] * data[7] - 
             data[8] * data[3] * data[6];

    inv[11] = -data[0] * data[5] * data[11] + 
               data[0] * data[7] * data[9] + 
               data[4] * data[1] * data[11] - 
               data[4] * data[3] * data[9] - 
               data[8] * data[1] * data[7] + 
               data[8] * data[3] * data[5];

    inv[15] = data[0] * data[5] * data[10] - 
              data[0] * data[6] * data[9] - 
              data[4] * data[1] * data[10] + 
              data[4] * data[2] * data[9] + 
              data[8] * data[1] * data[6] - 
              data[8] * data[2] * data[5];

    det = data[0] * inv[0] + data[1] * inv[4] + data[2] * inv[8] + data[3] * inv[12];

    if (det == 0)
        return;

    det = 1.0 / det;

    for(i = 0; i < 16; ++i)
        data[i] = inv[i] * det;

}


void topaz_matrix_reverse_majority(topazMatrix_t * m) {
    float * data = (float*)m->data;
    float temp;
    temp = data[4]; data[4] = data[1]; data[1] = temp;
    temp = data[8]; data[8] = data[2]; data[2] = temp;
    temp = data[3]; data[3] = data[12];  data[12]  = temp;
    temp = data[9]; data[9] = data[6]; data[6] = temp;
    temp = data[7]; data[7] = data[13]; data[13] = temp;
    temp = data[11]; data[11] = data[14]; data[14] = temp;
}




topazMatrix_t topaz_matrix_multiply(const topazMatrix_t * aSrc, const topazMatrix_t * bSrc) {
    topazMatrix_t out;
    const float * a = aSrc->data, 
                * b;
    for(int y = 0; y < 4; ++y) {
        b = bSrc->data;
        for(int x = 0; x < 4; ++x) {

            out.data[y*4 + x] = a[0] * b[0] +
                                a[1] * b[4] +
                                a[2] * b[8] +
                                a[3] * b[12];
            b++;

        }
        a += 4;

    }

    return out;

}


/// Rotates the matrix about the Euler angles psi, theta, and phi.
///
void topaz_matrix_rotate_by_angles(topazMatrix_t * m, float x, float y, float z) {
    float xRads = topaz_math_to_radians(x);
    float yRads = topaz_math_to_radians(y);
    float zRads = topaz_math_to_radians(z);

    rotateXMatrix.data[0] = 1;
    rotateXMatrix.data[5] = cos(xRads);
    rotateXMatrix.data[9] = sin(xRads);
    rotateXMatrix.data[6] = -sin(xRads);
    rotateXMatrix.data[10] = cos(xRads);
    rotateXMatrix.data[15] = 1;


    rotateYMatrix.data[5] = 1;
    rotateYMatrix.data[0] = cos(yRads);
    rotateYMatrix.data[8] = sin(yRads);
    rotateYMatrix.data[2] = -sin(yRads);
    rotateYMatrix.data[10] = cos(yRads);
    rotateYMatrix.data[15] = 1;

    rotateZMatrix.data[10] = 1;
    rotateZMatrix.data[5] = cos(zRads);
    rotateZMatrix.data[1] = sin(zRads);
    rotateZMatrix.data[4] = -sin(zRads);
    rotateZMatrix.data[0] = cos(zRads);
    rotateZMatrix.data[15] = 1;



    //model = model * (rotateXMatrix.data *
    //                 rotateYMatrix.data *
    //                 rotateZMatrix.data);
    topazMatrix_t o = topaz_matrix_multiply(m, &rotateXMatrix);
    topaz_matrix_multiply(&o, &rotateYMatrix);
    topaz_matrix_multiply(&o, &rotateZMatrix);
    *m = o;
}


void topaz_matrix_translate(topazMatrix_t * m, float x, float y, float z) {
    float * data = m->data;
    data[3]  += data[0] *x + data[1] *y + data[2] *z;
    data[7]  += data[4] *x + data[5] *y + data[6] *z;
    data[11] += data[8] *x + data[9] *y + data[10]*z;
    data[15] += data[12]*x + data[13]*y + data[14]*z;
}


void topaz_matrix_scale(topazMatrix_t * m, float x, float y, float z) {
    scaleScratch.data[0] = x;
    scaleScratch.data[5] = y;
    scaleScratch.data[10] = z;
    *m = topaz_matrix_multiply(m, &scaleScratch);
}





