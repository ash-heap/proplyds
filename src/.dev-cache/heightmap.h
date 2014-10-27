#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <sciurus/types.h>

/*
Algorithm Explained...
    http://graphics.cs.williams.edu/papers/IsoSIGGRAPH04/cs05-14.pdf

Invariants...
        tile dimensions = C x R (columns x rows)
        #triangles / tile = (2C - 7)(R - 3)
        R % 2 == 1
        abs(R * sin(pi / 3) / C) - 1 < small (0.01?)
        C < (2 ^ 14) * sqrt(3) IF USING SHORTS TO INDEX DATA
        (C - 4), (1/2)(R - 3) must be multiples of 2 ^ (L - 1),
            where L is the number of levels of detail wanted
        x(c, r) = 2c + (r & 1) + ((c == 1 && !(r & 1)) ? 1 : (c == C - 2 && !(r & 1)) ? -1 : 0) - 3
        y(c, r) = r - 1
        size = width of tile
        xf(x) = x * (size / 2(C - 4))
        yf(y) = y * (size / (R - 3))
        
        to generate dimensions C, R for desired L, #Tris...
            find i, j that minimize
                abs(((R * sin(pi / 3)) / C) - 1)
            for
                R = i * (2 ^ (L - 1)) + 3
            and
                C = j * (2 ^ (L - 1)) + 4

Valid Dimensions...
    C       R       L       Aspect  #Tris
    6       5       1       0.866   6
    24      29      1       1.046   1k
    49      57      1       1.007   5k
    12      11      3       0.794   136
    24      27      3       0.974   1k
    48      55      3       0.992   5k
    52      67      4       1.116   6k
    68      83      4       1.057   10k
    156     179     4       0.994   50k
    212     243     4       0.993   100k
    2564    3075    10      1.039   15M


*/

struct HeightMap
{
    sc::vec3** data;
    sc::uvec2 dim;
    sc::f32 width;
    
    HeightMap();
    HeightMap(sc::f32 width, sc::u32 ntriangles, sc::u32 lods);
    ~HeightMap();
    
    inline sc::vec3& at(sc::uvec2 p){return data[p.y][p.x];}
    inline sc::u32 getIndex(sc::uvec2 p){return p.x * p.y + p.y;}
    inline sc::uvec2 getParaIndex(sc::u32 i)
        {return sc::uvec2(i / dim.y, i % dim.y);}
    inline sc::vec2 getPosition(sc::uvec2 p)
    {
        return sc::vec2
        (
            (float)
            (
                (2 * p.x) + (p.y & 1) +
                (
                    (p.x == 1 && !(p.y & 1)) ? 1 :
                        (p.x == dim.x - 2 && !(p.y & 1)) ? -1 : 0
                ) - 3
            ) * (width / (2.f * (float)(dim.x - 4)))
            ,
            (float)(dim.y - 1) * (width / (float)(dim.y - 3))
        );
    }
    
    /*
    inline sc::uvec3 getTriangle(sc::vec2 pos)
    {
        
    }
    */
};

#endif
