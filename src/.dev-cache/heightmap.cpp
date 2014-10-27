#include <heightmap.h>
using namespace sc;

HeightMap::HeightMap()
{
    dim.x = 212;
    dim.y = 243;
    width = 300.f;
    data = new vec3*[dim.y];
    for(u32 r = 0; r < dim.y; r++)
    {
        data[r] = new vec3[dim.x];
        for(u32 c = 0; c < dim.x; c++)
        {
            vec2 pos = getPosition(uvec2(c, r));
            data[r][c] = vec3(pos.x, glm::length(pos), pos.y);
        }
    }
}

HeightMap::HeightMap(sc::f32 width, sc::u32 ntriangles, sc::u32 lods)
{

}

HeightMap::~HeightMap()
{
    for(u32 i = 0; i < dim.y; i++)
    {
        delete[] data[i];
    }
    delete[] data;
}


