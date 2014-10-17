#include <cstdlib>
#include <cstdio>
#include <string>
#include <sciurus/sciurus.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/random.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace std;

//appname
static const char* appname = "Miles Rufat-Latre: Space Simulator 2014 Premium";
//window, infrastructure
static sc::Window* window = NULL;
static const char* wtitle = appname;
static const int scrix = SDL_WINDOWPOS_UNDEFINED; //initial screen x position
static const int scriy = SDL_WINDOWPOS_UNDEFINED; //initial screen y position
static int scrw = 800; //screen width
static int scrh = 600; //screen height
//viewing parameters
static double fov = 70.0; //vertical field of view
static double aspect = (float)scrw / (float)scrh; //aspect ratio
static double cnear = 0.1; //near viewing plane
static double cfar = 3000.0; //far viewing plane
static bool drawnorms = false;
//window management, input
static unsigned int ms = 0;
static unsigned int dt = 0;
static bool resume = true;
static sc::Keyboard kb;
/*
static const int nkeys = 282; //there are 282 scancodes
static char kbstate[nkeys]; //one keystate for each key
static const char KDOWN = 0x01; //button is currently down
static const char KPRESSED = 0x02; //button was pressed since last clearing
static const char KRELEASED = 0x04; //button was released since last clearing
static const char KPRESSEDNOW = KDOWN | KPRESSED; //do not use for reading
static const char KNOTDOWN = ~KDOWN; //do not use for reading
static inline char presskey(char key){return key | KPRESSEDNOW;}
static inline char releasekey(char key){return (key & KNOTDOWN) | KRELEASED;}
static inline char clearkey(char key){return key & KDOWN;}
static inline void initkb(){for(int i = 0; i < nkeys; i++) kbstate[i] = 0x00;}
static inline void clearkb()
    {for(int i = 0; i < nkeys; i++) kbstate[i] = clearkey(kbstate[i]);}
static inline void keydown(SDL_Scancode s) //key press callback
    {kbstate[s] = presskey(kbstate[s]);}
static inline void keyup(SDL_Scancode s) //key release callback
    {kbstate[s] = releasekey(kbstate[s]);}
static inline char getkey(SDL_Keycode key) //get raw key state
    {return kbstate[SDL_GetScancodeFromKey(key)];}
static inline bool iskeydown(SDL_Keycode key) //get key KDOWN state
    {return getkey(key) & KDOWN;}
static inline bool waskeypressed(SDL_Keycode key) //get key KPRESSED state
    {return getkey(key) & KPRESSED;}
static inline bool waskeyreleased(SDL_Keycode key) //get key KRELEASED state
    {return getkey(key) & KRELEASED;}
*/
//builds new normals for mesh, clobbering the old ones if they exist
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
static inline aiMesh* loadmesh(const char* filename)
{
    Assimp::Importer importer = Assimp::Importer();
    const aiScene* scene = importer.ReadFile(filename,
          aiProcess_JoinIdenticalVertices
        | aiProcess_Triangulate
        | aiProcess_RemoveRedundantMaterials
        | aiProcess_ImproveCacheLocality
        //| aiPocess_GenerateNormals
    );
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
    mknorms(dst);
    return dst;
}
static inline void drawmesh(aiMesh* mesh)
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
    if(drawnorms)
    {
        glDisable(GL_LIGHTING);
        glColor4f(1.f, 0.f, 0.f, 1.f);
        glBegin(GL_LINES);
        for(unsigned int v = 0; v < mesh->mNumVertices; v++)
        {
            aiVector3D a = mesh->mVertices[v];
            aiVector3D n = mesh->mNormals[v];
            aiVector3D b = a + (n * 0.2f);
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
        }
        glEnd();
        glEnable(GL_LIGHTING);
    }
}
static inline void drawaxes(float dim = 1.f)
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

static inline void drawgrid(float xmin, float xmax,
                            float ymin, float ymax,
                            float zmin, float zmax,
                            float gridsize)
{
    unsigned int nx = (xmax - xmin) / gridsize;
    unsigned int ny = (ymax - ymin) / gridsize;
    unsigned int nz = (zmax - zmin) / gridsize;
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

static inline void gouradsphere(aiMesh* sphere, float radius = 1.f)
{
    aiVector3D* verts = sphere->mVertices;
    aiVector3D* norms = sphere->mNormals;
    for(unsigned int i = 0; i < sphere->mNumVertices; i++)
    {
        norms[i] = verts[i].Normalize();
        verts[i] = norms[i] * radius;
    }
}
static inline float getasth(glm::vec3 pos)
{
    return glm::simplex(pos) * 0.3f;
}

static inline void genasteroid(aiMesh* mesh, float radius = 1.f)
{
    aiVector3D* verts = mesh->mVertices;
    glm::vec3 roff = glm::ballRand(10000.f);
    for(unsigned int v = 0; v < mesh->mNumVertices; v++)
    {
        aiVector3D c = verts[v];
        verts[v] =
            ((c + (c * getasth(glm::vec3(c.x, c.y, c.y) + roff))) * radius);
    }
}

static inline void perspective()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, aspect, cnear, cfar);
    glMatrixMode(GL_MODELVIEW);
}
static inline void resize(int w, int h)
{
    scrw = w; scrh = h;
    aspect = (float)scrw / (float)scrh;
    glViewport(0, 0, scrw, scrh);
    perspective();
}
static inline void setfov(double theta){fov = theta; perspective();}

static inline void windowevent(SDL_WindowEvent e)
{
    switch(e.event)
    {
        case SDL_WINDOWEVENT_RESIZED:
            resize(e.data1, e.data2);
            break;
        default:
            break;
    }
}

static inline void handleevents()
{
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        switch(e.type)
        {
            case SDL_QUIT:
                resume = false;
                break;
            case SDL_KEYDOWN:
                if(e.key.repeat) break;
                kb.pressKey(e.key.keysym.scancode);
                printf("KEY DN: %s\n", SDL_GetKeyName(e.key.keysym.sym));
                break;
            case SDL_KEYUP:
                if(e.key.repeat) break;
                kb.releaseKey(e.key.keysym.scancode);
                printf("KEY UP: %s\n", SDL_GetKeyName(e.key.keysym.sym));
                break;
            case SDL_WINDOWEVENT:
                windowevent(e.window);
                break;
            default:
                break;
        }
    }
}

static unsigned int oldms = ms;
static inline void update()
{
    oldms = ms;
    ms = SDL_GetTicks();
    if(oldms > ms) dt = 0;
    else dt = ms - oldms;
    
    kb.reset();
    handleevents();
    if(kb.wasKeyPressed(SDLK_ESCAPE)) resume = false;
}

static inline void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    window->swapBuffers();
}

static inline void mainloop()
{
    while(resume)
    {
        update();
        draw();
    }
}

static inline void initGL()
{
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glColor4f(1.f, 1.f, 1.f, 1.f);
}

static inline void initscene()
{

}

static inline void cleanscene()
{

}

int main(int argc, char** argv)
{
    sc::init();
    
    window = new sc::Window();
    if(!window->valid())
    {
        fprintf(stderr, sc::getError().c_str());
        return -1;
    }
    
    initGL();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    window->swapBuffers();
    //SDL_GL_SwapWindow(window);
    //SDL_ShowWindow(window);
    
    kb.wipe();
    
    initscene();
    mainloop();
    cleanscene();
    delete window;
    
    sc::quit();

    return 0;
}

