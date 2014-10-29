#include <heightmap.h>
using namespace sc;

HeightMap::HeightMap(f32 initwidth, f32 initsqwidth)
{
    width = initwidth;
    sqwidth = initsqwidth;
    res = (u32)(width / sqwidth) + 1;
    tot = res * res;
    data = new vec3[tot];
    norms = new vec3[tot];
    u32 idx = 0;
    vec2 pos = vec2(0.f, 0.f);
    for(u32 i = 0; i < res; i++)
    {
        for(u32 j = 0; j < res; j++)
        {
            norms[idx] = vec3(0.f, 1.f, 0.f);
            data[idx] = vec3(pos.x, 0.f, pos.y);
            pos.x += sqwidth;
            idx++;
        }
        pos.y += sqwidth;
        pos.x = 0.f;
    }
}

HeightMap::~HeightMap()
{
    delete[] data;
}

void HeightMap::gen(Generator g)
{
    for(u32 i = 0; i < tot; i++)
    {
        vec3 curr = data[i];
        data[i].y = g(vec2(curr.x, curr.z));
    }
    u32 lim = res - 1;
    u32 idx = res + 1;
    for(u32 i = 1; i < lim; i++)
    {
        for(u32 j = 1; j < lim; j++)
        {
            vec3 a = data[idx - res - 1] - data[idx - res + 1];
            vec3 b = data[idx + 1] - data[idx - res + 1];
            norms[idx] = glm::normalize(glm::cross(a, b));
            idx++;
        }
        idx += 2;
    }
}

