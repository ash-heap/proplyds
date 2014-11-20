#ifndef SCIURUS_POINTCLOUD_H
#define SCIURUS_POINTCLOUD_H


#include "glm.h"
#include <stdbool.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif




void pointcloudGenerate(bool (*f)(float coord[3]),
                        float center[3],
                        float halfSize[3],
                        size_t numPoints,
                        float coords[][3]);

void pointcloudGenerate_(bool (*f)(void* state, float coord[3]),
                         void* functionState,
                         float center[3],
                         float halfSize[3],
                         size_t numPoints,
                         float coords[][3]);


void pointcloudEllipsoid(float radius[3],
                         size_t numPoints, float coords[][3]);




#ifdef __cplusplus
}
#endif


#endif
