#include <sciurus/random.h>
#include <sciurus/pointcloud.h>
#include <sciurus/dlist.h>
#include <sciurus/octree.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>




int main(int argc, char** argv)
{
    int errCode;
    unsigned int time;

    // Make pointcloud.
    float radius[3]  = {2.0f, 2.0f, 2.0f};
    size_t numPoints = 10000000;
    float (*coords)[3] = (float (*)[3])malloc(numPoints*3*sizeof(float));
    pointcloudEllipsoid(radius, numPoints, coords);

    // Make octree.
    float octreeCenter[3] = {0.0f, 0.0f, 0.0f};
    float octreeHalfSize[3] = {2.0f, 2.0f, 2.0f};
    struct Octree* octree = octreeNew(octreeCenter, octreeHalfSize);

    // Insert points into octree.
    for (size_t i = 0; i < numPoints; ++i) {
        octreeInsert(octree, coords[i], coords[i]);
    }

    time = clock();

    struct DList* dlist = dlistNew();

    float bboxCenter[3] = {0.0f, 0.0f, 0.0f};
    float bboxHalfSize[3] = {0.5f, 0.5f, 0.5f};
    octreeAABBCenterSize(octree, bboxCenter, bboxHalfSize, dlist);

    // float sphereCenter[3] = {0.0f, 0.0f, 0.0f};
    // float sphereRadius = 0.5f;
    // octreeBoundingSphere(octree, sphereCenter, sphereRadius, dlist);

    time = clock() - time;
    printf("Octree Ticks: %d\n", time);

    time = clock();
    float (*v)[3] = (float (*)[3])malloc(numPoints*3*sizeof(float));
    float lower[3] = {-0.5f, -0.5f, -0.5f};
    float upper[3] = { 0.5f,  0.5f,  0.5f};
    bool a[3];
    bool b[3];
    size_t idx = 0;
    for (size_t i = 0; i < numPoints; ++i) {

        // Check if point is within bounding box.
        glmGreaterThanEqual3fv(a, coords[i], lower);
        glmLessThanEqual3fv(b, coords[i], upper);
        glmAnd3v_(a, b);

        // If it is add it to the results.
        if (glmAll3(a))
            memcpy(v[idx], coords[i], 3*sizeof(float));

    }


    time = clock() - time;
    printf("Vector Ticks: %d\n", time);

    float testVec[3] = {0.3, -1.33, 0.84241};
    const size_t maxDepth = 100;
    unsigned char address[100];
    size_t depth = 0;
    if ((errCode = octreeCoordAddress(octree, testVec, maxDepth, &depth, address))) {
        printf("octreeCoordAddress Error Code = %d\n", errCode);
        exit(EXIT_FAILURE);
    }

    printf("Coord = %0.6f, %0.6f, %0.6f\n", testVec[0], testVec[1], testVec[2]);
    printf("Address = ");
    for (size_t i = 0; i < depth; ++i) {
        printf("%d ", address[i]);
    }
    printf("\n");
    
    unsigned char resultAddress[100];
    octreeNeighborAddress(octreePLUS_X | octreePLUS_Y | octreeMINUS_Z, depth, resultAddress, address);
    printf("Result  = ");
    for (size_t i = 0; i < depth; ++i) {
        printf("%d ", resultAddress[i]);
    }
    printf("\n");

    struct DList* resultCoords = dlistNew();
    octreeAddressLookup(octree, depth, resultAddress, resultCoords, NULL);
    struct DListNode* node = dlistHead(resultCoords);
    while (node != NULL) {
        float *coord = (float*)dlistNodeData(node);
        printf("%0.6f,  %0.6f,  %0.6f\n", coord[0], coord[1], coord[2]);
        node = dlistNodeNext(node);
    }

    // struct DListNode* node = dlistHead(dlist);
    // while (node != NULL) {
    //     float *coord = (float*)dlistNodeData(node);
    //     printf("%0.6f,  %0.6f,  %0.6f\n", coord[0], coord[1], coord[2]);
    //     node = dlistNodeNext(node);
    // }

    // for (size_t i = 0; i < numPoints; ++i)
    //     printf("%0.6f,  %0.6f,  %0.6f\n", coords[i][0], coords[i][1], coords[i][2]);

    octreeDelete(octree);
    free(coords);
    free(v);
    exit(EXIT_SUCCESS);
}
