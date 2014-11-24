#include "octree.h"
#include "dlist.h"
#include "glm.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "stdio.h"




// Number points that are allowed to be in a leaf node.  
#define OCTREE_MAX_POINTS_PER_NODE 4




static
struct OctreeNode* _octreeNodeNewLeaf(const float center[3],
                                      const float halfSize[3]);

static
struct OctreeNode* _octreeNodeNewInterior(const float center[3],
                                          const float halfSize[3]);

static
void _octreeNodeDelete(struct OctreeNode* node);




static
struct OctreeNode* _octreeNodeInsert(struct OctreeNode* const node,
                                     const float coord[3],
                                     void* data);

static
struct OctreeNode* _octreeNodeRemove(struct OctreeNode* const node,
                                     const float coord[3],
                                     int* errorCode);




static
void _octreeNodeBoundingSphere(struct OctreeNode* const node,
                               const float center[3],
                               float radius,
                               struct DList* results);

static
void _octreeNodeAABB(struct OctreeNode* const node,
                     const float lower[3],
                     const float upper[3],
                     struct DList* results);




static
void _octreeNodeCoordAddress(const struct OctreeNode* const node,
                             const float coord[3],
                             size_t maxDepth,
                             size_t* depth,
                             unsigned char address[]);

static
void _octreeNodeGetAll(const struct OctreeNode* node,
                       struct DList* const coords,
                       struct DList* const data);

static
bool _isBoxInSphere(float const boxCenter[3], float const boxHalfSize[3],
                    float const sphereCenter[3], float sphereRadius);

static
void _subCenter(const struct OctreeNode* const node,
                size_t idx, float center[3]);

static
size_t _AABBOctants(const float center[3],
                    const float lower[3],
                    const float upper[3],
                    size_t indices[8]);




struct Octree {
    struct OctreeNode* root;
    float center[3];
    float halfSize[3];
};




// Octree node.
struct OctreeNode {

    // True if this is a leaf node containing data.
    bool leaf;

    // Center coordinate of node.
    float center[3];

    // Half size of node.
    float halfSize[3];

    union {

        struct {

            // Number of children that are not NULL.
            size_t numChildren;

            // Child nodes.
            struct OctreeNode* children[8];

        };

        struct {

            // Number of data points that contain data.
            size_t numPoints;

            // Coordinate of data.
            float coord[OCTREE_MAX_POINTS_PER_NODE][3];

            // Pointer to data.
            void* data[OCTREE_MAX_POINTS_PER_NODE];

        };
    };

};




struct Octree* octreeNew(const float center[3], const float halfSize[3])
{
    struct Octree* octree = malloc(sizeof(struct Octree));

    // Set root.
    octree->root = NULL;

    // Set attributes.
    memcpy(octree->center  , center  , 3*sizeof(float));
    memcpy(octree->halfSize, halfSize, 3*sizeof(float));

    return octree;
}




void octreeDelete(struct Octree* const octree)
{
    if (octree->root != NULL)
        _octreeNodeDelete(octree->root);

    free(octree);
}




int octreeInsert(struct Octree* const octree,
                  const float coord[3], void* data)
{
    assert(octree != NULL);

    // Check to make sure point is within the octree.
    if (!octreeIsPointWithin(octree, coord))
        return -1;

    // If root is NULL make new leaf node.
    if (octree->root == NULL)
        octree->root = _octreeNodeNewLeaf(octree->center, octree->halfSize);

    // Insert point.
    octree->root = _octreeNodeInsert(octree->root, coord, data);

    return 0;
}




int octreeRemove(struct Octree* const octree, const float coord[3])
{
    assert(octree != NULL);

    // Check to make sure point is within the octree.
    if (!octreeIsPointWithin(octree, coord))
        return -1;

    // Check if root is NULL.
    if (octree->root == NULL)
        return -2;

    // Remove point.
    int errorCode;
    octree->root = _octreeNodeRemove(octree->root, coord, &errorCode);

    return errorCode;
}




bool octreeIsPointWithin(const struct Octree* const octree, const float coord[3])
{
    float v[3];
    bool b[3];
    glmSub3fv(v, octree->center, coord);
    glmAbs3f_(v);
    glmLessThanEqual3fv(b, v, octree->halfSize);
    return glmAll3(b);
}




void octreeBoundingSphere(struct Octree* const octree,
                          const float center[3], float radius,
                          struct DList* results)
{
    assert(octree != NULL);

    // If root is NULL do nothing.
    struct OctreeNode* root = octree->root;
    if (root == NULL)
        return;

    // Check that sphere overlaps with octree.
    if (!_isBoxInSphere(root->center, root->halfSize, center, radius))
        return;

    // Apply bounding sphere to the octree.
    _octreeNodeBoundingSphere(root, center, radius, results);
}




void octreeAABBCenterSize(struct Octree* const octree,
                          const float center[3],
                          const float halfSize[3],
                          struct DList* results)
{
    assert(octree != NULL);

    // Convert bounding box to lower/upper coordinate form.
    float lower[3];
    float upper[3];
    glmSub3fv(lower, center, halfSize);
    glmAdd3fv(upper, center, halfSize);

    // Call the lower/upper bounding box function.
    octreeAABBLowerUpper(octree, lower, upper, results);
}




void octreeAABBLowerUpper(struct Octree* const octree,
                          const float lower[3],
                          const float upper[3],
                          struct DList* results)
{
    assert(octree != NULL);

    // Check root for NULL.
    if (octree->root == NULL)
        return;

    // Convert octree bounds to lower/upper.
    float _lower[3];
    float _upper[3];
    glmSub3fv(_lower, octree->center, octree->halfSize);
    glmAdd3fv(_upper, octree->center, octree->halfSize);

    // Calculate collision of bounding box with octree.
    glmStep3fv(_lower, upper, _lower);
    glmStep3fv_(_upper, lower);
    bool noCollision = 0.0f == glmDot3f(_lower, _upper);

    // If no collision simply return.
    if (noCollision)
        return;

    // Otherwise, call the bounding box function on the root node.
    // NOTE: The internal version assumes the check above has passed.
    _octreeNodeAABB(octree->root, lower, upper, results);
}




int octreeCoordAddress(const struct Octree* const octree,
                       const float coord[3],
                       size_t maxDepth,
                       size_t* depth,
                       unsigned char address[maxDepth])
{
    assert(octree != NULL);

    // Check to make sure point is within the octree.
    if (!octreeIsPointWithin(octree, coord))
        return -1;

    // Check root for NULL.
    if (octree->root == NULL)
        return -2;

    // Find address of coordinate (coord does not need to be in the tree).
    _octreeNodeCoordAddress(octree->root, coord, maxDepth, depth, address);

    return 0;
}




// NOTE: result must be at least depth in length
void octreeNeighborAddress(unsigned char direction,
                           size_t depth,
                           unsigned char result[],
                           const unsigned char address[])
{
    // Copy address to the result address.
    memcpy(result, address, depth*sizeof(unsigned char));

    int idx = depth - 1;;
    while ((direction != 0) && (idx >= 0)) {

        unsigned char newDirection = 0;

        if (direction & octreeMINUS_X) {

            switch (result[idx]) {
                case 0: result[idx] = 1; newDirection |= octreeMINUS_X; break;
                case 1: result[idx] = 0; break;
                case 2: result[idx] = 3; newDirection |= octreeMINUS_X; break;
                case 3: result[idx] = 2; break;
                case 4: result[idx] = 5; newDirection |= octreeMINUS_X; break;
                case 5: result[idx] = 4; break;
                case 6: result[idx] = 7; newDirection |= octreeMINUS_X; break;
                case 7: result[idx] = 6; break;
            }

        } else if (direction & octreePLUS_X) {

            switch (result[idx]) {
                case 0: result[idx] = 1; break;
                case 1: result[idx] = 0; newDirection |= octreePLUS_X; break;
                case 2: result[idx] = 3; break;
                case 3: result[idx] = 2; newDirection |= octreePLUS_X; break;
                case 4: result[idx] = 5; break;
                case 5: result[idx] = 4; newDirection |= octreePLUS_X; break;
                case 6: result[idx] = 7; break;
                case 7: result[idx] = 6; newDirection |= octreePLUS_X; break;
            }

        }

        if (direction & octreeMINUS_Y) {

            switch (result[idx]) {
                case 0: result[idx] = 2; newDirection |= octreeMINUS_Y; break;
                case 1: result[idx] = 3; newDirection |= octreeMINUS_Y; break;
                case 2: result[idx] = 0; break;
                case 3: result[idx] = 1; break;
                case 4: result[idx] = 6; newDirection |= octreeMINUS_Y; break;
                case 5: result[idx] = 7; newDirection |= octreeMINUS_Y; break;
                case 6: result[idx] = 4; break;
                case 7: result[idx] = 5; break;
            }

        } else if (direction & octreePLUS_Y) {

            switch (result[idx]) {
                case 0: result[idx] = 2; break;
                case 1: result[idx] = 3; break;
                case 2: result[idx] = 0; newDirection |= octreePLUS_Y; break;
                case 3: result[idx] = 1; newDirection |= octreePLUS_Y; break;
                case 4: result[idx] = 6; break;
                case 5: result[idx] = 7; break;
                case 6: result[idx] = 4; newDirection |= octreePLUS_Y; break;
                case 7: result[idx] = 5; newDirection |= octreePLUS_Y; break;
            }

        }

        if (direction & octreeMINUS_Z) {

            switch (result[idx]) {
                case 0: result[idx] = 4; newDirection |= octreeMINUS_Z; break;
                case 1: result[idx] = 5; newDirection |= octreeMINUS_Z; break;
                case 2: result[idx] = 6; newDirection |= octreeMINUS_Z; break;
                case 3: result[idx] = 7; newDirection |= octreeMINUS_Z; break;
                case 4: result[idx] = 0; break;
                case 5: result[idx] = 5; break;
                case 6: result[idx] = 2; break;
                case 7: result[idx] = 3; break;
            }

        } else if (direction & octreePLUS_Z) {

            switch (result[idx]) {
                case 0: result[idx] = 4; break;
                case 1: result[idx] = 5; break;
                case 2: result[idx] = 6; break;
                case 3: result[idx] = 7; break;
                case 4: result[idx] = 0; newDirection |= octreeMINUS_Z; break;
                case 5: result[idx] = 5; newDirection |= octreeMINUS_Z; break;
                case 6: result[idx] = 2; newDirection |= octreeMINUS_Z; break;
                case 7: result[idx] = 3; newDirection |= octreeMINUS_Z; break;
            }

        }

        // Update variables.
        direction = newDirection;
        --idx;
    }
}




void octreeAddressBox(const struct Octree* const octree,
                      size_t depth, const unsigned char address[],
                      float center[3], float halfSize[3])
{
    float v[3];

    memcpy(center, octree->center, 3*sizeof(float));
    memcpy(halfSize, octree->halfSize, 3*sizeof(float));

    for (size_t i = 0; i < depth; ++i) {

        // Calculate new half size.
        glmDiv3fs_(halfSize, 2.0f);

        // Choose octant.
        switch (address[i]) {
            case 0: glmSet3f(v, -1, -1, -1); break;
            case 1: glmSet3f(v, +1, -1, -1); break;
            case 2: glmSet3f(v, -1, +1, -1); break;
            case 3: glmSet3f(v, +1, +1, -1); break;
            case 4: glmSet3f(v, -1, -1, +1); break;
            case 5: glmSet3f(v, +1, -1, +1); break;
            case 6: glmSet3f(v, -1, +1, +1); break;
            case 7: glmSet3f(v, +1, +1, +1); break;
        }

        // Calculate new center.
        glmCompMult3fv_(v, halfSize);
        glmAdd3fv_(center, v);
    }
}




void octreeAddressLookup(const struct Octree* const octree,
                         size_t depth,
                         const unsigned char address[],
                         struct DList* const coords,
                         struct DList* const data)
{
    assert(octree != NULL);

    // Follow address.
    struct OctreeNode* node = octree->root;
    for (size_t idx = 0; idx < depth; ++idx) {

        // No data at address.
        if (node == NULL) {
            return;

        // Leaf node found, return the addressed data.
        } else if (node->leaf) {

            // Calculate center and half size of addressed node.
            float center[3];
            float halfSize[3];
            octreeAddressBox(octree, depth, address, center, halfSize);

            // Add points within addressed node to the results.
            float v[3];
            bool b[3];
            for (size_t i = 0; i < node->numPoints; ++i) {
                glmSub3fv(v, node->coord[i], center);
                glmAbs3f_(v);
                glmLessThanEqual3fv(b, v, halfSize);
                if (glmAll3(b)) {
                    if (coords != NULL)
                        dlistCons(coords, node->coord[i]);
                    if (data != NULL)
                        dlistCons(data, node->data[i]);
                }
            }

            return;

        // Addvance to next depth.
        } else {
            node = node->children[address[idx]];
        }

    }

    // If the address leads to an internal node get all the data.
    _octreeNodeGetAll(node, coords, data);
}








struct OctreeNode* _octreeNodeNewLeaf(const float center[3],
                                      const float halfSize[3])
{
    struct OctreeNode* node = malloc(sizeof(struct OctreeNode));

    // Set this to a leaf node.
    node->leaf = true;

    // Set attributes.
    memcpy(node->center  , center  , 3*sizeof(float));
    memcpy(node->halfSize, halfSize, 3*sizeof(float));

    // Set data points to NULL.
    node->numPoints = 0;
    memset(node->coord, 0, OCTREE_MAX_POINTS_PER_NODE*3*sizeof(float));
    memset(node->data , 0, OCTREE_MAX_POINTS_PER_NODE*sizeof(void*));

    return node;
}




struct OctreeNode* _octreeNodeNewInterior(const float center[3],
                                          const float halfSize[3])
{
    struct OctreeNode* node = malloc(sizeof(struct OctreeNode));

    // Set this to an interior node.
    node->leaf = false;

    // Set attributes.
    memcpy(node->center  , center  , 3*sizeof(float));
    memcpy(node->halfSize, halfSize, 3*sizeof(float));

    // Set octants to NULL.
    node->numChildren = 0;
    memset(node->children, 0, 8*sizeof(struct OctreeNode*));

    return node;
}




void _octreeNodeDelete(struct OctreeNode* node)
{
    // Check to see if node is NULL.
    if (node == NULL)
        return;

    // Delete children of interior node.
    if (!(node->leaf)) {
        for (size_t i = 0; i < node->numPoints; ++i) {
            _octreeNodeDelete(node->children[i]);
        }
    }

    // Free this node.
    free(node);
}




static
struct OctreeNode* _octreeNodeInsert(struct OctreeNode* const node,
                                     const float coord[3],
                                     void* data)
{
    // Insert data into leaf node.
    if (node->leaf) {

        // If there is another free data point then simply add it.
        if (node->numPoints < OCTREE_MAX_POINTS_PER_NODE) {
            glmCopy3f(node->coord[node->numPoints], coord);
            node->data[node->numPoints] = data;
            ++(node->numPoints);
            return node;

        // Otherwise, replace with interior node and reinsert all the data points.
        } else {

            // New node.
            struct OctreeNode* newNode =
                _octreeNodeNewInterior(node->center, node->halfSize);

            // Insert data.
            _octreeNodeInsert(newNode, coord, data);
            for (size_t i = 0; i < node->numPoints; ++i) {
                _octreeNodeInsert(newNode, node->coord[i], node->data[i]);
            }

            // Delete old node and return the new one.
            _octreeNodeDelete(node);
            return newNode;

        }

    // Insert data into interior node.
    } else {

        // Determine octant to insert data into.
        size_t idx = (-(coord[0] >= node->center[0]) & 1)
                   | (-(coord[1] >= node->center[1]) & 2)
                   | (-(coord[2] >= node->center[2]) & 4);

        // Extract child node.
        struct OctreeNode* child = node->children[idx];

        // If child is NULL then add new leaf node.
        if (child == NULL) {

            // Calculate new center
            float center[3];
            _subCenter(node, idx, center);

            // Calculate new half size.
            float halfSize[3];
            glmDiv3fs(halfSize, node->halfSize, 2.0f);

            // Make new leaf node.
            ++(node->numChildren);
            child = _octreeNodeNewLeaf(center, halfSize);
        }

        // Insert node.
        child = _octreeNodeInsert(child, coord, data);

        // Replace child node.
        node->children[idx] = child;
        return node;

    }

    return node;
}




static
struct OctreeNode* _octreeNodeRemove(struct OctreeNode* const node,
                                     const float coord[3],
                                     int* errorCode)
{
    // Initialize error code to success.
    *errorCode = 0;
    
    // Remove point from leaf node.
    if (node->leaf) {

        bool b[3];
        for (size_t i = 0; i < node->numPoints; ++i) {

            glmEqual3fv(b, node->coord[i], coord);
            if (glmAll3(b)) {

                // If this is the end point.
                if (i + 1 == (node->numPoints)) {

                    // If this was the last point then free self and return NULL.
                    if (--(node->numPoints) == 0) {
                        free(node);
                        return NULL;
                    }

                    // Set point to NULL.
                    node->data[node->numPoints] = NULL;

                // Otherwise
                } else {

                    // Move data pointer at end to removed pointer spot.
                    node->data[i] = node->data[--(node->numPoints)];
                    node->data[node->numPoints] = NULL;

                    // Copy coordinate from end to removed spot.
                    memcpy(node->coord[i],
                           node->coord[node->numPoints],
                           3*sizeof(float));
                }

                // Return node.
                return node;
            }
        }

        // Point not found.
        *errorCode = -3;
        return node;

    // Remove point from interior node.
    } else {

        // Determine octant to insert data into.
        size_t idx = (-(coord[0] >= node->center[0]) & 1)
                   | (-(coord[1] >= node->center[1]) & 2)
                   | (-(coord[2] >= node->center[2]) & 4);

        // Remove from child.
        node->children[idx] = _octreeNodeRemove(node->children[idx],
                                                coord,
                                                errorCode);

        // Decrement child count if necessary.
        if (node->children[idx] == NULL) {
            --(node->numChildren);

            // If this was last child then delete this interior node.
            if (node->numChildren == 0) {
                free(node);
                return NULL;
            }
        }

        return node;
    }

}




static
void _octreeNodeBoundingSphere(struct OctreeNode* const node,
                                        const float center[3],
                                        float radius,
                                        struct DList* results)
{
    if (node->leaf) {

        // Look through each point and see if it is within the bounding sphere.
        for (size_t i = 0; i < node->numPoints; ++i) {

            if(glmDistance3f(node->coord[i], center) <= radius)
                dlistCons(results, node->data[i]);

        }

    } else {

        // Loop through children.
        for (size_t i = 0; i < 8; ++i) {

            struct OctreeNode* child = node->children[i];
            if (child != NULL) {

                // If this point is on/within the sphere then apply the bounding
                // box to the child.
                bool boxInSphere = _isBoxInSphere(child->center,
                                                  child->halfSize,
                                                  center,
                                                  radius);
                if (boxInSphere) {
                    _octreeNodeBoundingSphere(child, center, radius, results);
                }

            }

        }

    }
}




static
void _octreeNodeAABB(struct OctreeNode* const node,
                              const float lower[3],
                              const float upper[3],
                              struct DList* results)
{
    assert(node != NULL);

    // Extract data from leaf node.
    if (node->leaf) {

        // Look through each point and see if it is within the bounding sphere.
        for (size_t i = 0; i < node->numPoints; ++i) {

            bool a[3];
            bool b[3];

            // Check if point is within bounding box.
            glmGreaterThanEqual3fv(a, node->coord[i], lower);
            glmLessThanEqual3fv(b, node->coord[i], upper);
            glmAnd3v_(a, b);

            // If it is add it to the results.
            if (glmAll3(a)) {
                dlistCons(results, node->data[i]);
            }
        }

    // Extract data from internal node.
    } else {

        size_t indices[8];
        size_t numElements = _AABBOctants(node->center, lower, upper, indices);

        for (size_t i = 0; i < numElements; ++i) {

            struct OctreeNode* child = node->children[indices[i]];

            if (child != NULL)
                _octreeNodeAABB(child, lower, upper, results);
        }
    }
}




static
void _octreeNodeCoordAddress(const struct OctreeNode* node,
                             const float coord[3],
                             size_t maxDepth,
                             size_t* depth,
                             unsigned char address[maxDepth])
{
    size_t _depth = 0;

    while (!(node == NULL) && !(node->leaf) && (_depth < maxDepth)) {

        size_t idx = (-(coord[0] >= node->center[0]) & 1)
                   | (-(coord[1] >= node->center[1]) & 2)
                   | (-(coord[2] >= node->center[2]) & 4);

        address[_depth++] = idx;
        node = node->children[idx];
    }

    *depth = _depth;
}




static
void _octreeNodeGetAll(const struct OctreeNode* node,
                       struct DList* const coords,
                       struct DList* const data)
{
    if (node == NULL)
        return;

    if (node->leaf) {

        // Get data points.
        for (size_t i = 0; i < node->numPoints; ++i) {
            if (coords != NULL)
                dlistCons(coords, (void*)(node->coord[i]));
            if (data != NULL)
                dlistCons(data, node->data[i]);
        }

    } else {

        // Loop through each child.
        for (size_t i = 0; i < 8; ++i) {
            if (node->children[i] != NULL)
                _octreeNodeGetAll(node->children[i], coords, data);
        }
    }
}




static
bool _isBoxInSphere(float const boxCenter[3], float const boxHalfSize[3],
                    float const sphereCenter[3], float sphereRadius)
{
    // Convert center and half size to lower and upper corner coordinates.
    float lower[3];
    float upper[3];
    glmSub3fv(lower, boxCenter, boxHalfSize);
    glmAdd3fv(upper, boxCenter, boxHalfSize);

    // Find closest point to center of sphere that is within/on the box.
    float closestPoint[3];
    glmClamp3fv(closestPoint, sphereCenter, lower, upper);

    // Return the result.
    return glmDistance3f(sphereCenter, closestPoint) <= sphereRadius;
}




// Get center of octant.
//
// NOTE: This is an internal function and does not have any checking.  Do not
//       give bad arguments.
static
void _subCenter(const struct OctreeNode* const node, size_t idx, float center[3]){

    // Debug checks.
    assert(node != NULL);
    assert(idx <= 7);

    // Choose octant.
    switch (idx) {
        case 0: glmSet3f(center, -1, -1, -1); break;
        case 1: glmSet3f(center, +1, -1, -1); break;
        case 2: glmSet3f(center, -1, +1, -1); break;
        case 3: glmSet3f(center, +1, +1, -1); break;
        case 4: glmSet3f(center, -1, -1, +1); break;
        case 5: glmSet3f(center, +1, -1, +1); break;
        case 6: glmSet3f(center, -1, +1, +1); break;
        case 7: glmSet3f(center, +1, +1, +1); break;
    }

    // Calculate new center.
    glmCompMult3fv_(center, node->halfSize);
    glmDiv3fs_(center, 2.0f);
    glmAdd3fv_(center, node->center);
}




static
size_t _AABBOctants(const float center[3],
                    const float lower[3],
                    const float upper[3],
                    size_t indices[8])
{
    float v[3];
    bool includeLower[3];
    bool includeUpper[3];
    size_t numElements = 0;

    // Decide which lower octants to include.
    glmStep3fv(v, lower, center);
    glmEqual3fs(includeLower, v, 0.0f);

    // Decide which upper octants to include.
    glmStep3fv(v, upper, center);
    glmEqual3fs(includeUpper, v, 1.0f);

    // Calculate indices.
    if (includeLower[2]) {
        if (includeLower[1]) {
            if (includeLower[0])
                indices[numElements++] = 0;
            if (includeUpper[0])
                indices[numElements++] = 1;
        }
        if (includeUpper[1]) {
            if (includeLower[0])
                indices[numElements++] = 2;
            if (includeUpper[0])
                indices[numElements++] = 3;
        }
    }
    if (includeUpper[2]) {
        if (includeLower[1]) {
            if (includeLower[0])
                indices[numElements++] = 4;
            if (includeUpper[0])
                indices[numElements++] = 5;
        }
        if (includeUpper[1]) {
            if (includeLower[0])
                indices[numElements++] = 6;
            if (includeUpper[0])
                indices[numElements++] = 7;
        }
    }

    return numElements;
}
