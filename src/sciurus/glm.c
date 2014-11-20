#include <math.h>
#include "glm.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>




// Trigonometry
//------------------------------------------------------------------------------


#define PI 3.14159265358979323846


void glmRadians3f(float out[3], const float degrees[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = (PI/180)*degrees[i];
}


void glmRadians3f_(float degrees[3])
{
    for (size_t i = 0; i < 3; ++i)
        degrees[i] *= (PI/180);
}




void glmDegrees3f(float out[3], const float radians[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = (180.0f/PI)*radians[i];
}


void glmDegrees3f_(float radians[3])
{
    for (size_t i = 0; i < 3; ++i)
        radians[i] *= (180.0f/PI);
}




void glmSin3f(float out[3], const float angle[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = sinf(angle[i]);
}


void glmSin3f_(float angle[3])
{
    glmSin3f(angle, angle);
}




void glmSinD3f(float out[3], const float angle[3])
{
    glmRadians3f(out, angle);
    glmSin3f(out, out);
}


void glmSinD3f_(float angle[3])
{
    glmRadians3f_(angle);
    glmSinD3f_(angle);
}




void glmCos3f(float out[3], const float angle[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = cosf(angle[i]);
}


void glmCos3f_(float angle[3])
{
    glmCos3f(angle, angle);
}




void glmCosD3f(float out[3], const float angle[3])
{
    glmRadians3f(out, angle);
    glmCos3f(out, out);
}


void glmCosD3f_(float angle[3])
{
    glmRadians3f_(angle);
    glmCosD3f_(angle);
}




void glmTan3f(float out[3], const float angle[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = tanf(angle[i]);
}


void glmTan3f_(float angle[3])
{
    glmCos3f(angle, angle);
}




void glmTanD3f(float out[3], const float angle[3])
{

    glmRadians3f(out, angle);
    glmTan3f(out, out);
}


void glmTanD3f_(float angle[3])
{
    glmRadians3f_(angle);
    glmTanD3f_(angle);
}




void glmASin3f(float out[3], const float x[3]){
    for (size_t i = 0; i < 3; ++i)
        out[i] = asinf(x[i]);
}


void glmASin3f_(float x[3])
{
    glmASin3f(x, x);
}




void glmASinD3f(float out[3], const float x[3])
{
    glmASin3f(out, x);
    glmDegrees3f_(out);
}


void glmASinD3f_(float x[3])
{
    glmASin3f_(x);
    glmDegrees3f_(x);
}




void glmACos3f(float out[3], const float x[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = acosf(x[i]);
}


void glmACos3f_(float x[3])
{
    glmACos3f(x, x);
}




void glmACosD3f(float out[3], const float x[3])
{
    glmACos3f(out, x);
    glmDegrees3f_(out);
}


void glmACosD3f_(float x[3])
{
    glmACos3f_(x);
    glmDegrees3f_(x);
}




void glmATan3f(float out[3], const float x[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = atanf(x[i]);
}


void glmATan3f_(float x[3])
{
    glmATan3f(x, x);
}




void glmATanD3f(float out[3], const float x[3])
{
    glmATan3f(out, x);
    glmDegrees3f_(out);
}


void glmATanD3f_(float x[3])
{
    glmATan3f_(x);
    glmDegrees3f_(x);
}


//------------------------------------------------------------------------------




// Exponential
//------------------------------------------------------------------------------


void glmPow3fs(float out[3], const float x[3], float y)
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = powf(x[i], y);
}


void glmPow3fs_(float x[3], float y)
{
    glmPow3fs(x, x, y);
}




void glmPow3fv(float out[3], const float x[3], const float y[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = powf(x[i], y[i]);
}


void glmPow3fv_(float x[3], const float y[3])
{
    glmPow3fv(x, x, y);
}




void glmExp3f(float out[3], const float x[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = expf(x[i]);
}


void glmExp3f_(float x[3])
{
    glmExp3f(x, x);
}




void glmLog3f(float out[3], const float x[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = logf(x[i]);
}


void glmLog3f_(float x[3])
{
    glmLog3f(x, x);
}




void glmExp23f(float out[3], const float x[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = exp2f(x[i]);
}


void glmExp23f_(float x[3])
{
    glmExp23f(x, x);
}




void glmLog23f(float out[3], const float x[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = log2f(x[i]);
}


void glmLog23f_(float x[3])
{
    glmLog23f(x, x);
}




void glmSqrt3f(float out[3], const float x[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = sqrtf(x[i]);
}


void glmSqrt3f_(float x[3])
{
    glmSqrt3f(x, x);
}




void glmInverseSqrt3f(float out[3], const float x[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = 1.0f/sqrtf(x[i]);
}


void glmInverseSqrt3f_(float x[3])
{
    glmInverseSqrt3f(x, x);
}


//------------------------------------------------------------------------------




// Common
//------------------------------------------------------------------------------


void glmAbs3f(float out[3], const float x[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = fabsf(x[i]);
}


void glmAbs3f_(float x[3])
{
    glmAbs3f(x, x);
}



// Idea for signum type function from: http://stackoverflow.com/a/1903975
void glmSign3f(float out[3], const float x[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = (x[i] > 0.0f) - (x[i] < 0.0f);
}


void glmSign3f_(float x[3])
{
    glmSign3f(x, x);
}




void glmFloor3f(float out[3], const float x[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = floorf(x[i]);
}


void glmFloor3f_(float x[3])
{
    glmFloor3f(x, x);
}




void glmCeil3f(float out[3], const float x[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = ceilf(x[i]);
}


void glmCeil3f_(float x[3])
{
    glmCeil3f(x, x);
}




void glmFract3f(float out[3], const float x[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] - floorf(x[i]);
}


void glmFract3f_(float x[3])
{
    glmFract3f(x, x);
}




void glmMod3fs(float out[3], const float x[3], float y)
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] - floorf(x[i]/y)*y;
}


void glmMod3fs_(float x[3], float y)
{
    glmMod3fs(x, x, y);
}




void glmMod3fv(float out[3], const float x[3], const float y[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] - floorf(x[i]/y[i])*y[i];
}


void glmMod3fv_(float x[3], const float y[3])
{
    glmMod3fv(x, x, y);
}




void glmMin3fs(float out[3], const float x[3], float y)
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = fminf(x[i], y);
}


void glmMin3fs_(float x[3], float y)
{
    glmMin3fs(x, x, y);
}




void glmMin3fv(float out[3], const float x[3], const float y[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = fminf(x[i], y[i]);
}


void glmMin3fv_(float x[3], const float y[3])
{
    glmMin3fv(x, x, y);
}




void glmMax3fs(float out[3], const float x[3], float y)
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = fmaxf(x[i], y);
}


void glmMax3fs_(float x[3], float y)
{
    glmMax3fs(x, x, y);
}




void glmMax3fv(float out[3], const float x[3], const float y[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = fmaxf(x[i], y[i]);
}


void glmMax3fv_(float x[3], const float y[3])
{
    glmMax3fv(x, x, y);
}




void glmClamp3fs(float out[3], const float x[3], float minVal, float maxVal)
{
    for (size_t i = 0; i < 3; ++i) {
        if (x[i] <= minVal) {
            out[i] = minVal;
        } else if (x[i] >= maxVal) {
            out[i] = maxVal;
        } else {
            out[i] = x[i];
        }
    }
}


void glmClamp3fs_(float x[3], float minVal, float maxVal)
{
    for (size_t i = 0; i < 3; ++i) {
        if (x[i] <= minVal) {
            x[i] = minVal;
        } else if (x[i] >= maxVal) {
            x[i] = maxVal;
        }
    }
}




void glmClamp3fv(float out[3], const float x[3],
                 const float minVal[3], const float maxVal[3])
{
    for (size_t i = 0; i < 3; ++i) {
        if (x[i] <= minVal[i]) {
            out[i] = minVal[i];
        } else if (x[i] >= maxVal[i]) {
            out[i] = maxVal[i];
        } else {
            out[i] = x[i];
        }
    }
}


void glmClamp3fv_(float x[3], const float minVal[3], const float maxVal[3])
{
    for (size_t i = 0; i < 3; ++i) {
        if (x[i] <= minVal[i]) {
            x[i] = minVal[i];
        } else if (x[i] >= maxVal[i]) {
            x[i] = maxVal[i];
        }
    }
}




void glmMix3fs(float out[3], const float x[3], const float y[3], float a)
{
    for (size_t i = 0; i < 3; ++i)
        out[3] = x[3]*((1 - a)*a*y[3]);
}


void glmMix3fs_(float x[3], const float y[3], float a)
{
    glmMix3fs(x, x, y, a);
}




void glmMix3fv(float out[3], const float x[3],
               const float y[3], const float a[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[3] = x[3]*((1 - a[i])*a[i]*y[3]);
}


void glmMix3fv_(float x[3], const float y[3], const float a[3])
{
    glmMix3fv(x, x, y, a);
}




void glmStep3fs(float out[3], const float x[3], float edge)
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] >= edge;
}


void glmStep3fs_(float x[3], float edge)
{
    glmStep3fs(x, x, edge);
}




void glmStep3fv(float out[3], const float x[3], const float edge[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] >= edge[i];
}


void glmStep3fv_(float x[3], const float edge[3])
{
    glmStep3fv(x, x, edge);
}




/* void glmSmoothStep3fs(float out[3], const float x[3], float edge0, float edge1) */
/* { */
/*     printf("Function \"glmSmoothStep3fs\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */
/*  */
/*  */
/* void glmSmoothStep3fs_(float x[3], float edge0, float edge1) */
/* { */
/*     printf("Function \"glmSmoothStep3fs_\" has not yet been implemented.\n"); */
/*     glmStep3fs_(x, edge0, edge1); */
/*     exit(EXIT_FAILURE); */
/* } */
/*  */
/*  */
/*  */
/*  */
/* void glmSmoothStep3fv(float out[3], const float x[3],  */
/*                       const float edge0[3], const float edge1[3]) */
/* { */
/*     printf("Function \"glmSmoothStep3fv\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */
/*  */
/*  */
/* void glmSmoothStep3fv_(float x[3], const float edge0[3], const float edge1[3]) */
/* { */
/*     printf("Function \"glmSmoothStep3fv_\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */


//------------------------------------------------------------------------------








// Geometric
//------------------------------------------------------------------------------


float glmLength3f(const float x[3])
{
    return sqrtf(glmDot3f(x, x));
}




float glmDistance3f(const float p0[3], const float p1[3])
{
    float v[3];
    glmSub3fv(v, p0, p1);
    return glmLength3f(v);
}




float glmDot3f(const float x[3], const float y[3])
{
    return x[0]*y[0] + x[1]*y[1] + x[2]*y[2];
}




void glmCross3f(float out[3], const float x[3], const float y[3])
{
    out[0] = x[1]*y[2] - x[2]*y[1];
    out[1] = x[2]*y[0] - x[0]*y[2];
    out[2] = x[0]*y[1] - x[1]*y[0];
}


void glmCross3f_(float x[3], const float y[3])
{
    float v[3];
    glmCopy3f(v, x);
    glmCross3f(x, v, y);
}




void glmNormalize3f(float out[3], const float x[3])
{
    glmDiv3fs(out, x, glmLength3f(x));
}


void glmNormalize3f_(float x[3])
{
    glmDiv3fs_(x, glmLength3f(x));
}




/* void glmFaceForward3f(float out[3], const float n[3], */
/*                       const float i[3], const float nRef[3]) */
/* { */
/*     printf("Function \"glmFaceForward3f\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */
/*  */
/*  */
/* void glmFaceForward3f_(const float n[3], const float i[3], const float nRef[3]) */
/* { */
/*     printf("Function \"glmFaceForward3f_\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */
/*  */
/*  */
/*  */
/*  */
/* void glmReflect3f(float out[3], const float i[3], const float n[3]) */
/* { */
/*     printf("Function \"glmReflect3f\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */
/*  */
/*  */
/* void glmReflect3f_(float i[3], const float n[3]) */
/* { */
/*     printf("Function \"glmReflect3f_\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */
/*  */
/*  */
/*  */
/*  */
/* void glmRefract3f(float out[3], const float i[3], const float n[3], float eta) */
/* { */
/*     printf("Function \"glmRefract3f\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */
/*  */
/*  */
/* void glmRefract3f_(float i[3], const float n[3], float eta) */
/* { */
/*     printf("Function \"glmRefract3f_\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */


//------------------------------------------------------------------------------




// Vector Relations
//------------------------------------------------------------------------------


void glmLessThan3fs(bool out[3], const float x[3], float y)
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] < y;
}




void glmLessThan3fv(bool out[3], const float x[3], const float y[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] < y[i];
}




void glmLessThanEqual3fs(bool out[3], const float x[3], float y)
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] <= y;
}




void glmLessThanEqual3fv(bool out[3], const float x[3], const float y[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] <= y[i];
}




void glmGreaterThan3fs(bool out[3], const float x[3], float y)
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] > y;
}




void glmGreaterThan3fv(bool out[3], const float x[3], const float y[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] > y[i];
}




void glmGreaterThanEqual3fs(bool out[3], const float x[3], float y)
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] >= y;
}




void glmGreaterThanEqual3fv(bool out[3], const float x[3], const float y[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] >= y[i];
}




void glmEqual3fs(bool out[3], const float x[3], float y)
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] == y;
}




void glmEqual3fv(bool out[3], const float x[3], const float y[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] == y[i];
}




void glmNotEqual3fs(bool out[3], const float x[3], float y)
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] != y;
}




void glmNotEqual3fv(bool out[3], const float x[3], const float y[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] != y[i];
}




bool glmAny3f(const bool x[3])
{
    return x[0] || x[1] || x[2];
}




bool glmAll3(const bool x[3])
{
    return x[0] && x[1] && x[2];
}




void glmNot3(bool out[3], const bool x[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = !x[i];
}


void glmNot3_(bool x[3])
{
    glmNot3(x, x);
}




void glmAnd3s(bool out[3], const bool x[3], bool y)
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] && y;
}


void glmAnd3s_(bool x[3], bool y)
{
    glmAnd3s(x, x, y);
}




void glmAnd3v(bool out[3], const bool x[3], const bool y[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] && y[i];
}


void glmAnd3v_(bool x[3], const bool y[3])
{
    glmAnd3v(x, x, y);
}




void glmOr3s(bool out[3], const bool x[3], bool y)
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] || y;
}


void glmOr3s_(bool x[3], bool y)
{
    glmOr3s(x, x, y);
}




void glmOr3v(bool out[3], const bool x[3], const bool y[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] || y[i];
}


void glmOr3v_(bool x[3], const bool y[3])
{
    glmOr3v(x, x, y);
}




void glmXor3s(bool out[3], const bool x[3], bool y)
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] != y;
}


void glmXor3s_(bool x[3], bool y)
{
    glmXor3s(x, x, y);
}




void glmXor3v(bool out[3], const bool x[3], const bool y[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] != y[i];
}


void glmXor3v_(bool x[3], const bool y[3])
{
    glmXor3v(x, x, y);
}


//------------------------------------------------------------------------------




// Vector
//------------------------------------------------------------------------------


void glmSet3f(float v[3], float x, float y, float z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
}


void glmCopy3f(float vDest[3], const float vSource[3])
{
    for (size_t i = 0; i < 3; ++i)
        vDest[i] = vSource[i];
}




void glmAdd3fs(float out[3], const float x[3], float y)
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] + y;
}


void glmAdd3fs_(float x[3], float y)
{
    for (size_t i = 0; i < 3; ++i)
        x[i] += y;
}




void glmAdd3fv(float out[3], const float x[3],  const float y[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] + y[i];
}


void glmAdd3fv_(float x[3], const float y[3])
{
    for (size_t i = 0; i < 3; ++i)
        x[i] += y[i];
}




void glmSub3fs(float out[3], const float x[3], float y)
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] - y;
}


void glmSub3fs_(float x[3], float y)
{
    for (size_t i = 0; i < 3; ++i)
        x[i] -= y;
}




void glmSub3fv(float out[3], const float x[3],  const float y[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i] - y[i];
}


void glmSub3fv_(float x[3], const float y[3])
{
    for (size_t i = 0; i < 3; ++i)
        x[i] -= y[i];
}




void glmCompMult3fs(float out[3], const float x[3], float y)
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i]*y;
}


void glmCompMult3fs_(float x[3], float y)
{
    for (size_t i = 0; i < 3; ++i)
        x[i] *= y;
}




void glmCompMult3fv(float out[3], const float x[3], const float y[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i]*y[i];
}


void glmCompMult3fv_(float x[3], const float y[3])
{
    for (size_t i = 0; i < 3; ++i)
        x[i] *= y[i];
}




void glmDiv3fs(float out[3], const float x[3], float y)
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i]/y;
}


void glmDiv3fs_(float x[3], float y)
{
    for (size_t i = 0; i < 3; ++i)
        x[i] /= y;
}




void glmDiv3fv(float out[3], const float x[3], const float y[3])
{
    for (size_t i = 0; i < 3; ++i)
        out[i] = x[i]/y[i];
}


void glmDiv3fv_(float x[3], const float y[3])
{
    for (size_t i = 0; i < 3; ++i)
        x[i] /= y[i];
}


//------------------------------------------------------------------------------








// Matrix
//------------------------------------------------------------------------------


/* void glmMatrixSet3fc(float m[3][3], float x[3], float y[3], float z[3]) */
/* { */
/*     printf("Function \"glmRefract3f_\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */
/*  */
/*  */
/* void glmMatrixSet3fr(float m[3][3], float x[3], float y[3], float z[3]) */
/* { */
/*     printf("Function \"glmRefract3f_\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */
/*  */
/*  */
/* void glmMatrixCopy3f(float mDest[3][3], const float mSource[3][3]) */
/* { */
/*     printf("Function \"glmRefract3f_\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */
/*  */
/*  */
/*  */
/*  */
/* void glmMatixCompMult3f(float out[3][3], */
/*                         const float x[3][3], const float y[3][3]) */
/* { */
/*     printf("Function \"glmRefract3f_\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */
/*  */
/*  */
/* void glmMatixCompMult3f_(float x[3][3], const float y[3][3]) */
/* { */
/*     printf("Function \"glmRefract3f_\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */
/*  */
/*  */
/*  */
/*  */
/* void glmMatixMatrixMult3fc(float out[3][3], */
/*                            const float x[3][3], const float y[3][3]) */
/* { */
/*     printf("Function \"glmRefract3f_\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */
/*  */
/*  */
/* void glmMatixMatrixMult3fc_(float x[3][3], const float y[3][3]) */
/* { */
/*     printf("Function \"glmRefract3f_\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */
/*  */
/*  */
/*  */
/*  */
/* void glmMatixMatrixMult3fr(float out[3][3], */
/*                            const float x[3][3], const float y[3][3]) */
/* { */
/*     printf("Function \"glmRefract3f_\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */
/*  */
/*  */
/* void glmMatixMatrixMult3fr_(float x[3][3], const float y[3][3]) */
/* { */
/*     printf("Function \"glmRefract3f_\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */
/*  */
/*  */
/*  */
/*  */
/* void glmMatixVectorMult3fc(float out[3], const float m[3][3], const float v[3]) */
/* { */
/*     printf("Function \"glmRefract3f_\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */
/*  */
/*  */
/* void glmMatixVectorMult3fc_(const float x[3][3], float y[3]) */
/* { */
/*     printf("Function \"glmRefract3f_\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */
/*  */
/*  */
/*  */
/*  */
/* void glmMatixVectorMult3fr(float out[3], const float m[3][3], const float v[3]) */
/* { */
/*     printf("Function \"glmRefract3f_\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */
/*  */
/*  */
/* void glmMatixVectorMult3fr_(const float x[3][3], float y[3]) */
/* { */
/*     printf("Function \"glmRefract3f_\" has not yet been implemented.\n"); */
/*     exit(EXIT_FAILURE); */
/* } */


//------------------------------------------------------------------------------











