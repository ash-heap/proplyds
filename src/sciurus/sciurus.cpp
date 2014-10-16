#include <sciurus/sciurus.h>
#include <SDL2/SDL.h>

namespace sc
{

static bool inited = false;
std::string sc_error;

bool init()
{
    if(inited) return true;
    if(SDL_Init(SDL_INIT_EVERYTHING))
    {
        sc_error = SDL_GetError();
        inited = false;
        return false;
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    inited = true;
    return true;
}

std::string getError()
{
    return sc_error;
}

}

