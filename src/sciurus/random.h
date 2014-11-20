#ifndef SCIURUS_RANDOM_H
#define SCIURUS_RANDOM_H


#include "glm.h"
#include <stdbool.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif




void randomInitialize(int seed);
void randomInitializeWithTime();


// Random float between 0.0f and 1.0f
float randomf();


// Random float between min and max (inclusive).
float randomRangef(float min, float max);


// Fill array of floats with random number between 0.0f and 1.0f
void randomFillf(size_t length, float numbers[]);


// Fill array of floats with random numbers between min and max (inclusive).
void randomRangeFillf(float min, float max,
                      size_t length, float numbers[]);




#ifdef __cplusplus
}
#endif


#endif
