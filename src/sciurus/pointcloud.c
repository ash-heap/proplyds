#include "pointcloud.h"
#include "random.h"
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>




static
bool _ellipsoid(void* radius, float coord[3]);




void pointcloudGenerate(bool (*f)(float coord[3]),
                        float center[3],
                        float halfSize[3],
                        size_t numPoints,
                        float coords[numPoints][3])
{
    float lower[3];
    float upper[3];
    glmSub3fv(lower, center, halfSize);
    glmAdd3fv(upper, center, halfSize);

    size_t idx = 0;
    while (idx < numPoints) {

        // Generate random coordinate.
        coords[idx][0] = randomRangef(lower[0], upper[0]);
        coords[idx][1] = randomRangef(lower[0], upper[1]);
        coords[idx][2] = randomRangef(lower[0], upper[2]);

        // Increment index if point is to be kept.
        if ((*f)(coords[idx]))
            ++idx;
    }
}




void pointcloudGenerate_(bool (*f)(void* state, float coord[3]),
                         void* functionState,
                         float center[3],
                         float halfSize[3],
                         size_t numPoints,
                         float coords[numPoints][3])
{
    float lower[3];
    float upper[3];
    glmSub3fv(lower, center, halfSize);
    glmAdd3fv(upper, center, halfSize);

    size_t idx = 0;
    while (idx < numPoints) {

        // Generate random coordinate.
        coords[idx][0] = randomRangef(lower[0], upper[0]);
        coords[idx][1] = randomRangef(lower[1], upper[1]);
        coords[idx][2] = randomRangef(lower[2], upper[2]);

        // Increment index if point is to be kept.
        if ((*f)(functionState, coords[idx]))
            ++idx;
    }
}




void pointcloudEllipsoid(float radius[3],
                         size_t numPoints, float coords[numPoints][3])
{
    float center[3] = {0.0f, 0.0f, 0.0f};

    pointcloudGenerate_(&_ellipsoid,
                        radius,
                        center,
                        radius, // halfSize
                        numPoints,
                        coords);
}




static
bool _ellipsoid(void* radius, float coord[3])
{
    float v[3];
    glmDiv3fv(v, coord, (float*)radius);
    return glmDot3f(v, v) <= 1.0f;
}
