#ifndef SCIURUS_OCTREE_H
#define SCIURUS_OCTREE_H

#include "dlist.h"
#include <stdbool.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif




#define octreeMINUS_X 1
#define octreePLUS_X  2
#define octreeMINUS_Y 4
#define octreePLUS_Y  8
#define octreeMINUS_Z 16
#define octreePLUS_Z  32





struct Octree;




struct Octree* octreeNew(const float center[3], const float halfSize[3]);

void octreeDelete(struct Octree* octree);




int octreeInsert(struct Octree* const octree,
                 const float coord[3], void* data);

int octreeRemove(struct Octree* const octree, const float coord[3]);




bool octreeIsPointWithin(const struct Octree* const octree, const float coord[3]);

void octreeBoundingSphere(struct Octree* const octree,
                          const float center[3], float radius,
                          struct DList* results);

void octreeAABBCenterSize(struct Octree* const octree,
                          const float center[3],
                          const float halfSize[3],
                          struct DList* results);

void octreeAABBLowerUpper(struct Octree* const octree,
                          const float lower[3],
                          const float upper[3],
                          struct DList* results);

int octreeNearestNeighbor(const struct Octree* const octree,
                          const float point[3],
                          float coord[3],
                          void** data);




int octreeCoordAddress(const struct Octree* const octree,
                       const float coord[3],
                       size_t maxDepth,
                       size_t* depth,
                       unsigned char address[]);

int octreeNeighborAddress(unsigned char direction,
                          size_t depth,
                          const unsigned char address[],
                          unsigned char result[]);

void octreeAddressBox(const struct Octree* const octree,
                      size_t depth, const unsigned char address[],
                      float center[3], float halfSize[3]);

void octreeAddressLookup(const struct Octree* const octree,
                         size_t depth,
                         const unsigned char address[],
                         struct DList* const coords,
                         struct DList* const data);




#ifdef __cplusplus
}
#endif


#endif
