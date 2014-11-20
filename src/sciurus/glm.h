#ifndef SCIURUS_GLM_H
#define SCIURUS_GLM_H

#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif




// Trigonometry
//------------------------------------------------------------------------------


void glmRadians3f(float out[3], const float degrees[3]);
void glmRadians3f_(float degrees[3]);


void glmDegrees3f(float out[3], const float radians[3]);
void glmDegrees3f_(float radians[3]);


void glmSin3f(float out[3], const float angle[3]);
void glmSin3f_(float angle[3]);


void glmSinD3f(float out[3], const float angle[3]);
void glmSinD3f_(float angle[3]);


void glmCos3f(float out[3], const float angle[3]);
void glmCos3f_(float angle[3]);


void glmCosD3f(float out[3], const float angle[3]);
void glmCosD3f_(float angle[3]);


void glmTan3f(float out[3], const float angle[3]);
void glmTan3f_(float angle[3]);


void glmTanD3f(float out[3], const float angle[3]);
void glmTanD3f_(float angle[3]);


void glmASin3f(float out[3], const float x[3]);
void glmASin3f_(float x[3]);


void glmASinD3f(float out[3], const float x[3]);
void glmASinD3f_(float x[3]);


void glmACos3f(float out[3], const float x[3]);
void glmACos3f_(float x[3]);


void glmACosD3f(float out[3], const float x[3]);
void glmACosD3f_(float x[3]);


void glmATan3f(float out[3], const float x[3]);
void glmATan3f_(float x[3]);


void glmATanD3f(float out[3], const float x[3]);
void glmATanD3f_(float x[3]);


//------------------------------------------------------------------------------




// Exponential
//------------------------------------------------------------------------------


void glmPow3fs(float out[3], const float x[3], float y);
void glmPow3fs_(float x[3], float y);


void glmPow3fv(float out[3], const float x[3], const float y[3]);
void glmPow3fv_(float x[3], const float y[3]);


void glmExp3f(float out[3], const float x[3]);
void glmExp3f_(float x[3]);


void glmLog3f(float out[3], const float x[3]);
void glmLog3f_(float x[3]);


void glmExp23f(float out[3], const float x[3]);
void glmExp23f_(float x[3]);


void glmLog23f(float out[3], const float x[3]);
void glmLog23f_(float x[3]);


void glmSqrt3f(float out[3], const float x[3]);
void glmSqrt3f_(float x[3]);


void glmInverseSqrt3f(float out[3], const float x[3]);
void glmInverseSqrt3f_(float x[3]);


//------------------------------------------------------------------------------




// Common
//------------------------------------------------------------------------------


void glmAbs3f(float out[3], const float x[3]);
void glmAbs3f_(float x[3]);


void glmSign3f(float out[3], const float x[3]);
void glmSign3f_(float x[3]);


void glmFloor3f(float out[3], const float x[3]);
void glmFloor3f_(float x[3]);


void glmCeil3f(float out[3], const float x[3]);
void glmCeil3f_(float x[3]);


void glmFract3f(float out[3], const float x[3]);
void glmFract3f_(float x[3]);


void glmMod3fs(float out[3], const float x[3], float y);
void glmMod3fs_(float x[3], float y);


void glmMod3fv(float out[3], const float x[3], const float y[3]);
void glmMod3fv_(float x[3], const float y[3]);


void glmMin3fs(float out[3], const float x[3], float y);
void glmMin3fs_(float x[3], float y);


void glmMin3fv(float out[3], const float x[3], const float y[3]);
void glmMin3fv_(float x[3], const float y[3]);


void glmMax3fs(float out[3], const float x[3], float y);
void glmMax3fs_(float x[3], float y);


void glmMax3fv(float out[3], const float x[3], const float y[3]);
void glmMax3fv_(float x[3], const float y[3]);


void glmClamp3fs(float out[3], const float x[3], float minVal, float maxVal);
void glmClamp3fs_(float x[3], float minVal, float maxVal);


void glmClamp3fv(float out[3], const float x[3],
                 const float minVal[3], const float maxVal[3]);
void glmClamp3fv_(float x[3], const float minVal[3], const float maxVal[3]);


void glmMix3fs(float out[3], const float x[3], const float y[3], float a);
void glmMix3fs_(float x[3], const float y[3], float a);


void glmMix3fv(float out[3], const float x[3],
               const float y[3], const float a[3]);
void glmMix3fv_(float x[3], const float y[3], const float a[3]);


void glmStep3fs(float out[3], const float x[3], float edge);
void glmStep3fs_(float x[3], float edge);


void glmStep3fv(float out[3], const float x[3], const float edge[3]);
void glmStep3fv_(float x[3], const float edge[3]);


void glmSmoothStep3fs(float out[3], const float x[3], float edge0, float edge1);
void glmSmoothStep3fs_(float x[3], float edge0, float edge1);


void glmSmoothStep3fv(float out[3], const float x[3], 
                      const float edge0[3], const float edge1[3]);
void glmSmoothStep3fv_(float x[3], const float edge0[3], const float edge1[3]);


//------------------------------------------------------------------------------




// Geometric
//------------------------------------------------------------------------------


float glmLength3f(const float x[3]);


float glmDistance3f(const float p0[3], const float p1[3]);


float glmDot3f(const float x[3], const float y[3]);


void glmCross3f(float out[3], const float x[3], const float y[3]);
void glmCross3f_(float x[3], const float y[3]);


void glmNormalize3f(float out[3], const float x[3]);
void glmNormalize3f_(float x[3]);


void glmFaceForward3f(float out[3], const float n[3],
                      const float i[3], const float nRef[3]);
void glmFaceForward3f_(const float n[3], const float i[3], const float nRef[3]);


void glmReflect3f(float out[3], const float i[3], const float n[3]);
void glmReflect3f_(float i[3], const float n[3]);


void glmRefract3f(float out[3], const float i[3], const float n[3], float eta);
void glmRefract3f_(float i[3], const float n[3], float eta);


//------------------------------------------------------------------------------




// Vector Relations
//------------------------------------------------------------------------------


void glmLessThan3fs(bool out[3], const float x[3], float y);


void glmLessThan3fv(bool out[3], const float x[3], const float y[3]);


void glmLessThanEqual3fs(bool out[3], const float x[3], float y);


void glmLessThanEqual3fv(bool out[3], const float x[3], const float y[3]);


void glmGreaterThan3fs(bool out[3], const float x[3], float y);


void glmGreaterThan3fv(bool out[3], const float x[3], const float y[3]);


void glmGreaterThanEqual3fs(bool out[3], const float x[3], float y);


void glmGreaterThanEqual3fv(bool out[3], const float x[3], const float y[3]);


void glmEqual3fs(bool out[3], const float x[3], float y);


void glmEqual3fv(bool out[3], const float x[3], const float y[3]);


void glmNotEqual3fs(bool out[3], const float x[3], float y);


void glmNotEqual3fv(bool out[3], const float x[3], const float y[3]);


bool glmAny3(const bool x[3]);


bool glmAll3(const bool x[3]);


void glmNot3(bool out[3], const bool x[3]);
void glmNot3_(bool x[3]);


void glmAnd3s(bool out[3], const bool x[3], bool y);
void glmAnd3s_(bool x[3], bool y);


void glmAnd3v(bool out[3], const bool x[3], const bool y[3]);
void glmAnd3v_(bool x[3], const bool y[3]);


void glmOr3s(bool out[3], const bool x[3], bool y);
void glmOr3s_(bool x[3], bool y);


void glmOr3v(bool out[3], const bool x[3], const bool y[3]);
void glmOr3v_(bool x[3], const bool y[3]);


void glmXor3v(bool out[3], const bool x[3], const bool y[3]);
void glmXor3v_(bool x[3], const bool y[3]);


void glmXor3s(bool out[3], const bool x[3], bool y);
void glmXor3s_(bool x[3], bool y);


//------------------------------------------------------------------------------




// Vector
//------------------------------------------------------------------------------


void glmSet3f(float v[3], float x, float y, float z);
void glmCopy3f(float vDest[3], const float vSource[3]);


void glmAdd3fs(float out[3], const float x[3], float y);
void glmAdd3fs_(float x[3], float y);


void glmAdd3fv(float out[3], const float x[3],  const float y[3]);
void glmAdd3fv_(float x[3], const float y[3]);


void glmSub3fs(float out[3], const float x[3], float y);
void glmSub3fs_(float x[3], float y);


void glmSub3fv(float out[3], const float x[3],  const float y[3]);
void glmSub3fv_(float x[3], const float y[3]);


void glmCompMult3fs(float out[3], const float x[3], float y);
void glmCompMult3fs_(float x[3], float y);


void glmCompMult3fv(float out[3], const float x[3], const float y[3]);
void glmCompMult3fv_(float x[3], const float y[3]);


void glmDiv3fs(float out[3], const float x[3], float y);
void glmDiv3fs_(float x[3], float y);


void glmDiv3fv(float out[3], const float x[3], const float y[3]);
void glmDiv3fv_(float x[3], const float y[3]);


//------------------------------------------------------------------------------




// Matrix
//------------------------------------------------------------------------------


void glmMatrixSet3fc(float m[3][3], float x[3], float y[3], float z[3]);
void glmMatrixSet3fr(float m[3][3], float x[3], float y[3], float z[3]);
void glmMatrixCopy3f(float mDest[3][3], const float mSource[3][3]);


void glmMatixCompMult3f(float out[3][3],
                        const float x[3][3], const float y[3][3]);
void glmMatixCompMult3f_(float x[3][3], const float y[3][3]);


void glmMatixMatrixMult3fc(float out[3][3],
                           const float x[3][3], const float y[3][3]);
void glmMatixMatrixMult3fc_(float x[3][3], const float y[3][3]);


void glmMatixMatrixMult3fr(float out[3][3],
                           const float x[3][3], const float y[3][3]);
void glmMatixMatrixMult3fr_(float x[3][3], const float y[3][3]);


void glmMatixVectorMult3fc(float out[3], const float m[3][3], const float v[3]);
void glmMatixVectorMult3fc_(const float x[3][3], float y[3]);


void glmMatixVectorMult3fr(float out[3], const float m[3][3], const float v[3]);
void glmMatixVectorMult3fr_(const float x[3][3], float y[3]);


//------------------------------------------------------------------------------




// Texture Lookup
//------------------------------------------------------------------------------
// TODO
//------------------------------------------------------------------------------




#ifdef __cplusplus
}
#endif


#endif
