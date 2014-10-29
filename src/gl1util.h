#ifndef GL1UTIL_H
#define GL1UTIL_H

#include <glinc.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <heightmap.h>

aiMesh* loadmesh(const char* filename, bool smooth = false);
GLuint loadtexture(const char* filename);
void drawmesh(aiMesh* mesh);
void drawnorms(aiMesh* mesh, float len = 1.f);
void drawterrain(HeightMap* map);
void drawaxes(float dim = 1.f);
void drawgrid(float xmin, float xmax,
              float ymin, float ymax,
              float zmin, float zmax,
              float gridsize);

#endif
