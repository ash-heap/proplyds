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
void scenedrawmesh(void* data){drawmesh((aiMesh*)data);}
void voiddrawfunc(void*){}
SceneNode* root = new SceneNode();
SceneNode* left = new SceneNode(root);
SceneNode* camera = new SceneNode(root);

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
    perspective();
}

static inline void initscene()
{
    root->data = (void*)loadmesh("media/testmesh.ply");
    root->f = scenedrawmesh;
    left->data = root->data;
    left->f = scenedrawmesh;
    left->t = glm::translate(mat4(1.f), vec3(0.f, 3.f, 0.f));
    left->t = glm::rotate(left->t, 2.f, vec3(0.f, 1.f, 0.f));
    camera->data = NULL;
    camera->f = SceneNode::nulldrawfunc;
    camera->t = glm::translate(mat4(1.f), vec3(0.f, 0.f, -5.f));
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
    
    root->t = glm::rotate(root->t, (float)dt / 1000.f, vec3(0.f, 1.f, 0.f));
    left->t = glm::rotate(left->t, (float)dt / 500.f, vec3(0.f, 0.f, 1.f));
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
        return -1;
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

    return 0;
}

