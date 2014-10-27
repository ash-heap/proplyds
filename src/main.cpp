#include <sciurus/sciurus.h>
#include <sciurus/window.h>
#include <sciurus/keyboard.h>
using namespace sc;
#include <cstdlib>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/random.hpp>

#include <gl1util.h>
#include <scenenode.h>
#include <SOIL/SOIL.h>

#include <heightmap.h>

//appname
static const char* appname = "Miles Rufat-Latre: Space Simulator 2014 Premium";
//window, infrastructure
static Window* window = NULL;
static const char* wtitle = appname;
static int scrw = 800; //screen width
static int scrh = 600; //screen height
//viewing parameters
static double fov = 70.0; //vertical field of view
static double aspect = (float)scrw / (float)scrh; //aspect ratio
static double cnear = 0.1; //near viewing plane
static double cfar = 3000.0; //far viewing plane
//window management, input
static unsigned int ms = 0;
static unsigned int dt = 0;
static bool resume = true;
static Keyboard kb;
//scene objects :D
static const char* shipmeshfile = "media/space_frigate_6/space_frigate_6.3DS";
static const char* shiptexturefile =
    "media/space_frigate_6/space_frigate_6_color.png";
static const char* grasstexturefile = "media/grass.jpg";
static GLuint grasstexid;
static GLuint shiptexid;
static void scenedrawmesh(void* data){drawmesh((aiMesh*)data);}
static void scenedrawterrain(void* data)
    {glBindTexture(GL_TEXTURE_2D, grasstexid); drawterrain((HeightMap*)data);}
static void drawship(void* data)
    {glBindTexture(GL_TEXTURE_2D, shiptexid); scenedrawmesh(data);}
static SceneNode* root = new SceneNode();
static SceneNode* left = new SceneNode(root);
static SceneNode* terrain = new SceneNode(root);
static SceneNode* pivot = new SceneNode(root);
static SceneNode* camera = new SceneNode(pivot);
static HeightMap map = HeightMap(256.f, 2.f);
static vec2 seed = vec2(2102.55f, 282.1221f);

static float generator(vec2 pos)
{
    pos = pos + seed;
    glm::vec2 poso100 = pos / 100.f;
    glm::vec2 posp100o40 = (pos + glm::vec2(100.f, 100.f)) / 40.f;
    glm::vec2 poso40 = pos / 40.f;
    float big = 8.f * glm::simplex(poso100);
    glm::vec2 med_offset = glm::vec2(glm::simplex(poso40), glm::simplex(posp100o40)) / 4.f;
    float med = 2.f * glm::simplex(poso40 + med_offset);

    return 2.f * (med + big);
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

static inline void initGL()
{
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glColor4f(0.5f, 0.5f, 0.5f, 1.f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    perspective();
}

static inline void initscene()
{
    map.gen(generator);
    terrain->data = (void*)&map;
    terrain->f = scenedrawterrain;
    terrain->t = glm::translate(mat4(1.f),
        vec3(-map.width * 0.5f, -30.f, -map.width * 0.5f));
    shiptexid = SOIL_load_OGL_texture(shiptexturefile, SOIL_LOAD_AUTO,
                                      SOIL_CREATE_NEW_ID,
                                      SOIL_FLAG_INVERT_Y);
    grasstexid = SOIL_load_OGL_texture(grasstexturefile, SOIL_LOAD_AUTO,
                                      SOIL_CREATE_NEW_ID,
                                      SOIL_FLAG_INVERT_Y);
    glBindTexture(GL_TEXTURE_2D, grasstexid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    aiMesh* shipmesh = loadmesh(shipmeshfile);
    if(!shipmesh)
    {
        fprintf(stderr, "Failed to load mesh: %s\n", shipmeshfile);
        exit(EXIT_FAILURE);
    }
    mat4 tf = glm::rotate(mat4(1.f),
            -glm::pi<float>() / 2.f, vec3(1.f, 0.f, 0.f));
    for(unsigned int i = 0; i < shipmesh->mNumVertices; i++)
    {
        aiVector3D v3 = shipmesh->mVertices[i];
        vec4 v4 = vec4(v3.x, v3.y, v3.z, 1.f);
        v4 = tf * v4;
        shipmesh->mVertices[i] = aiVector3D(v4.x, v4.y, v4.z);
    }
    root->data = (void*)shipmesh;
    root->f = drawship;
    left->data = root->data;
    left->f = drawship;
    left->t = glm::translate(mat4(1.f), vec3(0.f, 20.f, 0.f));
    left->t = glm::rotate(left->t, 2.f, vec3(0.f, 1.f, 0.f));
    camera->data = NULL;
    camera->f = SceneNode::nulldrawfunc;
    camera->t = glm::translate(mat4(1.f), vec3(0.f, 0.f, -60.f));
}

static unsigned int oldms = ms;
static inline void update()
{
    oldms = ms;
    ms = SDL_GetTicks();
    if(oldms > ms) dt = 0;
    else dt = ms - oldms;
    f32 dts = (float)dt / 1000.f;

    kb.reset();
    handleevents();
    if(kb.wasKeyPressed(SDLK_ESCAPE)) resume = false;
    if(kb.isKeyDown(SDLK_a))
        pivot->t = glm::rotate(pivot->t, dts, vec3(0.f, 1.f, 0.f));
    if(kb.isKeyDown(SDLK_d))
        pivot->t = glm::rotate(pivot->t, -dts, vec3(0.f, 1.f, 0.f));
    left->t = glm::rotate(left->t, dts * 0.5f, vec3(0.f, 0.f, 1.f));
}

static inline void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glMultMatrixf(glm::value_ptr(camera->getInvGlobTF()));
    root->drawAll();
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

static inline void cleanscene()
{
    delete (aiMesh*)root->data;
    delete root;
}

int main(int argc, char** argv)
{
    sc_init();
    
    window = new sc::Window(wtitle, scrw, scrh,
        SC_WINDOW_DEFAULT_FLAGS | SDL_WINDOW_HIDDEN);
    if(!window->valid())
    {
        fprintf(stderr, sc_getError().c_str());
        exit(EXIT_FAILURE);
    }
    initGL();
    
    initscene();
    
    //do one frame of execution before showing, to prevent blank screen
    update();
    draw();
    window->show();
    
    mainloop();
    
    cleanscene();
    
    delete window;
    sc_quit();

    exit(EXIT_SUCCESS);
}

