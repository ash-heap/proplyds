#include <sciurus/window.h>
#include <sciurus/opengl.h>

namespace sc
{

extern std::string sc_error;

static bool glewinit = false;

Window::Window(std::string title, u32 width, u32 height,
                int flags, u32 xinitpos, u32 yinitpos)
{
    handle = SDL_CreateWindow(title.c_str(), xinitpos, yinitpos,
                                width, height, (SDL_WindowFlags)flags);
    if(!handle)
    {
        sc_error += "Sciurus failed to create a window: ";
        sc_error += SDL_GetError();
        sc_error += "\n";
    }
    context = SDL_GL_CreateContext(handle);
    SDL_GL_MakeCurrent(handle, context);
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        sc_error += "Sciurus failed to initialize GLEW: ";
        sc_error += (char*)glewGetErrorString(err);
        sc_error += "\n";
    }
    glewinit = true;
}

Window::~Window()
{
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(handle);
    handle = NULL;
}

bool Window::fullscreen(bool fullscreen, bool desktop)
{
    int result = SDL_SetWindowFullscreen
    (
        handle,
        fullscreen ? SDL_WINDOW_FULLSCREEN :
        (desktop ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0)
    );
    if(result)
    {
        sc_error = SDL_GetError();
        return false;
    }
    return true;
}

}

