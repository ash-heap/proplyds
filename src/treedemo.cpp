#include <sciurus/dlist.h>
#include <sciurus/octree.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>



int main(int argc, char** argv)
{

    char point1S[] = "1.0, 1.0, 1.0";
    float point1C[3] = {1.0f, 1.0f, 1.0f};

    char point2S[] = "-2.0, 1.0, 1.0";
    float point2C[3] = {-2.0f, 1.0f, 1.0f};

    char point3S[] = "2.0, 2.0, 2.0";
    float point3C[3] = {2.0f, 2.0f, 2.0f};

    char point4S[] = "-1.0, 1.0, -1.0";
    float point4C[3] = {-1.0f, 1.0f, -1.0f};

    char point5S[] = "3.0f, 3.0f, -3.0f";
    float point5C[3] = {3.0f, 3.0f, -3.0f};


    float octreeCenter[3] = {0.0f, 0.0f, 0.0f};
    float octreeHalfSize[3] = {3.0f, 3.0f, 3.0f};

    printf("Make octree.\n");
    struct Octree* octree = octreeNew(octreeCenter, octreeHalfSize);

    int err;
    printf("Insert Points.\n");
    err = octreeInsert(octree, point1C, point1S);
    printf("error code = %d\n", err);
    err = octreeInsert(octree, point2C, point2S);
    printf("error code = %d\n", err);
    err = octreeInsert(octree, point3C, point3S);
    printf("error code = %d\n", err);
    err = octreeInsert(octree, point4C, point4S);
    printf("error code = %d\n", err);
    err = octreeInsert(octree, point5C, point5S);
    printf("error code = %d\n", err);

    err = octreeInsert(octree, point5C, point5S);
    printf("error code = %d\n", err);

    printf("Test bounding box.\n");
    struct DList* dlist;

    float sphereCenter[3] = {0.0f, 0.0f, 0.0f};
    float sphereRadius = 3.47f;
    dlist = octreeBoundingSphere(octree, sphereCenter, sphereRadius);

    struct DListNode* node = dlistHead(dlist);
    while (node != NULL) {
        char* tmp = (char*)dlistNodeData(node);
        printf("%s\n", tmp);
        node = dlistNodeNext(node);
    }

    printf("Delete octree.\n");
    octreeDelete(octree);

    exit(EXIT_SUCCESS);
}
