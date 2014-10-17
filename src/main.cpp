#include <sciurus/sciurus.h>
#include <cstdlib>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/random.hpp>
#include <bullet/btBulletDynamicsCommon.h>

#include <gl1util.h>

//appname
static const char* appname = "Miles Rufat-Latre: Space Simulator 2014 Premium";
//window, infrastructure
static sc::Window* window = NULL;
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
static sc::Keyboard kb;

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

int main(int argc, char** argv)
{
    sc::init();
    
    window = new sc::Window(wtitle, scrw, scrh,
        SC_WINDOW_DEFAULT_FLAGS | SDL_WINDOW_HIDDEN);
    if(!window->valid())
    {
        fprintf(stderr, sc::getError().c_str());
        return -1;
    }
    initGL();
    
    //do one frame of execution before showing, to prevent blank screen
    update();
    draw();
    window->show();
    
    mainloop();
    
    delete window;
    sc::quit();

    return 0;
}

