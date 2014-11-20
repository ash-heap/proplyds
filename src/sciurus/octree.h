#ifndef SCIURUS_OCTREE_H
#define SCIURUS_OCTREE_H

#include "dlist.h"
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif




struct Octree;




struct Octree* const octreeNew(const float center[3], const float halfSize[3]);

void octreeDelete(struct Octree* octree);




int octreeInsert(struct Octree* const octree,
                  const float coord[3], void* data);

int octreeRemove(struct Octree* const octree, const float coord[3]);




bool octreeIsPointWithin(struct Octree* const octree, const float coord[3]);

struct DList* const octreeBoundingSphere(struct Octree* const octree,
                                   const float center[3], float radius);

struct DList* const octreeAABBCenterSize(struct Octree* const octree,
                                         const float center[3],
                                         const float halfSize[3]);

struct DList* const octreeAABBLowerUpper(struct Octree* const octree,
                                         const float lower[3],
                                         const float upper[3]);




#ifdef __cplusplus
}
#endif


#endif
