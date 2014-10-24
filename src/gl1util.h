#ifndef GL1UTIL_H
#define GL1UTIL_H

#include <glinc.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

aiMesh* loadmesh(const char* filename, bool smooth = false);
void drawmesh(aiMesh* mesh);
void drawaxes(float dim = 1.f);
void drawgrid(float xmin, float xmax,
              float ymin, float ymax,
              float zmin, float zmax,
              float gridsize);

#endif
