#ifndef TERRAIN_H
#define TERRAIN_H

#include <glm/glm.hpp>

namespace terrain
{

void seed(long seed);
float elevation(glm::vec2 pos);
float humidity(glm::vec2 pos);
float temperature(glm::vec2 pos);

float mountain(glm::vec2 pos); //steep, moderate, moderate
float dune(glm::vec2 pos); //decreases to produce more dunes in hill phase
float hill-weight(glm::vec2 pos);
float hill(glm::vec2 pos); //hill-offset
glm::vec4

float base(glm::vec2 pos);
float ocean(glm::vec2 pos);
float 

}

#endif
