#include <gl1util.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>
using namespace sc;

static Assimp::Importer* importer = new Assimp::Importer();

//just in case
static inline void mknorms(aiMesh* mesh)
{
    for(unsigned int f = 0; f < mesh->mNumFaces; f++)
    {
        unsigned int* indices = mesh->mFaces[f].mIndices;
        aiVector3D a = mesh->mVertices[indices[0]];
        aiVector3D b = mesh->mVertices[indices[1]];
        aiVector3D c = mesh->mVertices[indices[2]];
        aiVector3D u = b - a;
        aiVector3D v = c - a;
        aiVector3D norm;
        norm.x = u[1] * v[2] - u[2] * v[1];
        norm.y = u[2] * v[0] - u[0] * v[2];
        norm.z = u[0] * v[1] - u[1] * v[0];
        norm = norm.Normalize();
        mesh->mNormals[indices[0]] = norm;
        mesh->mNormals[indices[1]] = norm;
        mesh->mNormals[indices[2]] = norm;
    }
}

aiMesh* loadmesh(const char* filename, bool smooth)
{
    const aiScene* scene = importer->ReadFile(filename,
          aiProcess_CalcTangentSpace
        | aiProcess_JoinIdenticalVertices
        | aiProcess_Triangulate
        | aiProcess_RemoveRedundantMaterials
        | aiProcess_ImproveCacheLocality
        | aiProcess_OptimizeMeshes
        | aiProcess_OptimizeGraph
        | (smooth ? aiProcess_GenSmoothNormals : aiProcess_GenNormals)
    );
	if(!scene) return NULL;
    aiMesh* dst = new aiMesh();
    aiMesh* src = scene->mMeshes[0];
    dst->mNumVertices = src->mNumVertices;
    dst->mVertices = new aiVector3D[dst->mNumVertices];
    dst->mNormals = new aiVector3D[dst->mNumVertices];
    for(unsigned int i = 0; i < src->GetNumUVChannels(); i++)
        dst->mTextureCoords[i] = new aiVector3D[dst->mNumVertices];
    for(unsigned int i = 0; i < dst->mNumVertices; i++)
    {
        dst->mNormals[i] = src->mNormals[i];
        dst->mVertices[i] = src->mVertices[i];
        for(unsigned int j = 0; j < src->GetNumUVChannels(); j++)
            dst->mTextureCoords[j][i] = src->mTextureCoords[j][i];
    }
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
    mknorms(dst);
    return dst;
}

GLuint loadtexture(const char* filename)
{
    return SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO,
                                        SOIL_CREATE_NEW_ID,
                                        SOIL_FLAG_INVERT_Y);
}

void drawmesh(aiMesh* mesh)
{
    aiVector3D* verts = mesh->mVertices;
    aiVector3D* norms = mesh->mNormals;
    aiVector3D* uvs = mesh->mTextureCoords[0];
    glBegin(GL_TRIANGLES);
    for(u32 f = 0; f < mesh->mNumFaces; f++)
    {
        unsigned int* indices = mesh->mFaces[f].mIndices;
        glTexCoord3fv((float*)&uvs[indices[0]]);
        glNormal3fv((float*)&norms[indices[0]]);
        glVertex3fv((float*)&verts[indices[0]]);
        glTexCoord3fv((float*)&uvs[indices[1]]);
        glNormal3fv((float*)&norms[indices[1]]);
        glVertex3fv((float*)&verts[indices[1]]);
        glTexCoord3fv((float*)&uvs[indices[2]]);
        glNormal3fv((float*)&norms[indices[2]]);
        glVertex3fv((float*)&verts[indices[2]]);
    }
    glEnd();
}

void drawnorms(aiMesh* mesh, float len)
{
    aiVector3D* verts = mesh->mVertices;
    aiVector3D* norms = mesh->mNormals;
    aiVector3D sum;
    glBegin(GL_LINES);
    for(u32 i = 0; i < mesh->mNumVertices; i++)
    {
        glVertex3fv((float*)&verts[i]);
        sum = verts[i] + (len * norms[i]);
        glVertex3fv((float*)&sum);
    }
    glEnd();
}

void drawterrain(HeightMap* map)
{
    u32 res = map->res;
    u32 lim = res - 1;
    f32 frac = map->sqwidth / map->width;
    vec2 tex = vec2(0.f, 0.f);
    u32 idx = 0;
    glBegin(GL_QUADS);
    for(u32 i = 0; i < lim; i++)
    {
        for(u32 j = 0; j < lim; j++)
        {
            glTexCoord2f(tex.x, tex.y);
            glNormal3fv(glm::value_ptr(map->norms[idx]));
            glVertex3fv(glm::value_ptr(map->data[idx]));
            glTexCoord2f(tex.x, tex.y + frac);
            glNormal3fv(glm::value_ptr(map->norms[idx + res]));
            glVertex3fv(glm::value_ptr(map->data[idx + res]));
            glTexCoord2f(tex.x + frac, tex.y + frac);
            glNormal3fv(glm::value_ptr(map->norms[idx + res + 1]));
            glVertex3fv(glm::value_ptr(map->data[idx + res + 1]));
            glTexCoord2f(tex.x + frac, tex.y);
            glNormal3fv(glm::value_ptr(map->norms[idx + 1]));
            glVertex3fv(glm::value_ptr(map->data[idx + 1]));
            idx++;
            tex.x += frac;
        }
        idx++;
        tex.y += frac;
        tex.x = 0.f;
    }
    glEnd();
}

void drawaxes(float dim)
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

void drawgrid(float xmin, float xmax,
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
