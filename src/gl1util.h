#ifndef GL1UTIL_H
#define GL1UTIL_H

#include <glinc.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

inline aiMesh* loadmesh(const char* filename, bool smooth = false)
{
    Assimp::Importer importer = Assimp::Importer();
    const aiScene* scene = importer.ReadFile(filename,
          aiProcess_CalcTangentSpace
        | aiProcess_JoinIdenticalVertices
        | aiProcess_Triangulate
        | aiProcess_RemoveRedundantMaterials
        | aiProcess_ImproveCacheLocality
        | aiProcess_FixInfacingNormals
        | aiProcess_OptimizeMeshes
        | aiProcess_OptimizeGraph
        | aiProcess_FindDegenerates
        | aiProcess_FindInvalidData
        | (smooth ? aiProcess_GenSmoothNormals : aiProcess_GenNormals)
    );
	if(!scene) return NULL;
    aiMesh* dst = new aiMesh();
    aiMesh* src = scene->mMeshes[0];
    dst->mNumVertices = src->mNumVertices;
    dst->mVertices = new aiVector3D[dst->mNumVertices];
    for(unsigned int i = 0; i < dst->mNumVertices; i++)
        dst->mVertices[i] = src->mVertices[i];
    dst->mNormals = new aiVector3D[dst->mNumVertices];
    dst->mNumFaces = src->mNumFaces;
    dst->mFaces = new aiFace[dst->mNumFaces];
    for(unsigned int i = 0; i < dst->mNumFaces; i++)
    {
        aiFace* df = &(dst->mFaces[i]);
        aiFace* sf = &(src->mFaces[i]);
        df->mNumIndices = sf->mNumIndices;
        df->mIndices = new unsigned int[df->mNumIndices];
        for(unsigned int j = 0; j < df->mNumIndices; j++)
            df->mIndices[j] = sf->mIndices[j];
    }
    return dst;
}
inline void drawmesh(aiMesh* mesh)
{
    aiVector3D* verts = mesh->mVertices;
    aiVector3D* norms = mesh->mNormals;
    glBegin(GL_TRIANGLES);
    for(unsigned int f = 0; f < mesh->mNumFaces; f++)
    {
        unsigned int* indices = mesh->mFaces[f].mIndices;
        glNormal3fv((float*)&norms[indices[0]]);
        glVertex3fv((float*)&verts[indices[0]]);
        glNormal3fv((float*)&norms[indices[1]]);
        glVertex3fv((float*)&verts[indices[1]]);
        glNormal3fv((float*)&norms[indices[2]]);
        glVertex3fv((float*)&verts[indices[2]]);
    }
    glEnd();
}
inline void drawaxes(float dim = 1.f)
{
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
        glColor4f(1.f, 0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.f, 0.f);
        glVertex3f(dim, 0.f, 0.f);
        glColor4f(0.f, 1.f, 0.f, 1.f);
        glVertex3f(0.f, 0.f, 0.f);
        glVertex3f(0.f, dim, 0.f);
        glColor4f(0.f, 0.f, 1.f, 1.f);
        glVertex3f(0.f, 0.f, 0.f);
        glVertex3f(0.f, 0.f, dim);
    glEnd();
    glEnable(GL_LIGHTING);
}

inline void drawgrid(float xmin, float xmax,
                            float ymin, float ymax,
                            float zmin, float zmax,
                            float gridsize)
{
    unsigned int nx = (unsigned int)((xmax - xmin) / gridsize);
    unsigned int ny = (unsigned int)((ymax - ymin) / gridsize);
    unsigned int nz = (unsigned int)((zmax - zmin) / gridsize);
    float xpos = xmin;
    float ypos = ymin;
    float zpos = zmin;
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    //draw the x-grid
    for(unsigned int y = 0; y <= ny; y++)
    {
        zpos = zmin;
        for(unsigned int z = 0; z <= nz; z++)
        {
            glVertex3f(xmin, ypos, zpos);
            glVertex3f(xmax, ypos, zpos);
            zpos += gridsize;
        }
        ypos += gridsize;
    }
    //draw the y-grid
    xpos = xmin;
    zpos = zmin;
    for(unsigned int x = 0; x <= nx; x++)
    {
        zpos = zmin;
        for(unsigned int z = 0; z <= nz; z++)
        {
            glVertex3f(xpos, ymin, zpos);
            glVertex3f(xpos, ymax, zpos);
            zpos += gridsize;
        }
        xpos += gridsize;
    }
    //draw the z-grid
    xpos = xmin;
    ypos = ymin;
    for(unsigned int x = 0; x <= nx; x++)
    {
        ypos = ymin;
        for(unsigned int y = 0; y <= ny; y++)
        {
            glVertex3f(xpos, ypos, zmin);
            glVertex3f(xpos, ypos, zmax);
            ypos += gridsize;
        }
        xpos += gridsize;
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

#endif
