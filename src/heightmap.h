#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <sciurus/types.h>

struct HeightMap
{
    typedef sc::f32 (*Generator)(sc::vec2);

    sc::vec3* data;
    sc::u32 res;
    sc::u32 tot;
    sc::f32 width;
    sc::f32 sqwidth;
    
    HeightMap(sc::f32 width, sc::f32 sqwidth);
    ~HeightMap();
    
    void gen(Generator g);
};

#endif

