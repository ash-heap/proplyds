#include "random.h"
#include <time.h>
#include <stdlib.h>




void randomInitialize(int seed)
{
    srand(seed);
}




void randomInitializeWithTime()
{
    srand((unsigned int)time(NULL) + (unsigned int)clock());
}




float randomf()
{
    return (float)rand()/(float)RAND_MAX;
}




float randomRangef(float min, float max)
{
    return randomf()*(max - min) + min;
}




void randomFillf(size_t length, float numbers[length])
{
    for (size_t i = 0; i < length; ++i)
        numbers[i] = randomf();
}




void randomRangeFillf(float min, float max,
                     size_t length, float numbers[length])
{
    for (size_t i = 0; i < length; ++i)
        numbers[i] = randomRangef(min, max);
}
