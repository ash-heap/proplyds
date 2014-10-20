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
        sc_error += "#ERROR# Sciurus failed to create a window: "
                 + std::string(SDL_GetError()) + "\n";
    }
    context = SDL_GL_CreateContext(handle);
    SDL_GL_MakeCurrent(handle, context);
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        sc_error += "#ERROR# Sciurus failed to initialize GLEW: "
                 + std::string((char*)glewGetErrorString(err)) + "\n";
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
        sc_error += "#ERROR# Sciurus failed to set fullscreen"
                 + std::string(SDL_GetError()) + "\n";
        return false;
    }
    return true;
}

bool Window::relativeMouse(bool relative)
{
    int result = SDL_SetRelativeMouseMode(relative ? SDL_TRUE : SDL_FALSE);
    if(result < 0)
    {
        sc_error += "#ERROR# Sciurus failed to set relative mouse: "
                 + std::string(SDL_GetError()) + "\n";
        return false;
    }
    return true;
}

bool Window::hideCursor(bool hide = true)
{
    int result = SDL_ShowCursor(hide ? SDL_DISABLE : SDL_ENABLE);
    if(result < 0)
    {
        sc_error += "#ERROR# Sciurus failed to set cursor visibility: "
                 + std::string(SDL_GetError()) + "\n";
        return false;
    }
    return true;
}

}

