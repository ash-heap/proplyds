#ifndef SCIURUS_WINDOW_H
#define SCIURUS_WINDOW_H

#include <sciurus/types.h>
#include <SDL2/SDL.h>
#include <string>

#ifndef SC_WINDOW_DEFAULT_WIDTH
#define SC_WINDOW_DEFAULT_WIDTH 800
#endif
#ifndef SC_WINDOW_DEFAULT_HEIGHT
#define SC_WINDOW_DEFAULT_HEIGHT 600
#endif
#ifndef SC_WINDOW_DEFAULT_FLAGS
#define SC_WINDOW_DEFAULT_FLAGS SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS
#endif
/*
NOTE: Before creating your window, specify context attributes as follows...
SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 16);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
//see documentation for SDL_GL_SetAttribute for additional details
*/

namespace sc
{

class Window
{
protected:
    SDL_Window* handle;
    SDL_GLContext context;
    
public:
    Window(std::string title = "Sciurus Window",
            u32 width = SC_WINDOW_DEFAULT_WIDTH,
            u32 height = SC_WINDOW_DEFAULT_HEIGHT,
            i32 flags = SC_WINDOW_DEFAULT_FLAGS,
            u32 xinitpos = SDL_WINDOWPOS_UNDEFINED,
            u32 yinitpos = SDL_WINDOWPOS_UNDEFINED);
    ~Window();
    
    inline SDL_Window* getHandle(){return handle;}
    inline bool valid(){return handle ? true : false;}
    inline SDL_GLContext getContext(){return context;}
    inline void swapBuffers(){SDL_GL_SwapWindow(handle);}
    inline void setTitle(std::string title)
        {SDL_SetWindowTitle(handle, title.c_str());}
    inline std::string getTitle(){return SDL_GetWindowTitle(handle);}
    inline ivec2 getDimensions()
        {ivec2 r; SDL_GetWindowSize(handle, &r.x, &r.y); return r;}
    inline void setDimensions(ivec2 dim)
        {SDL_SetWindowSize(handle, dim.x, dim.y);}
    inline void hide(){SDL_HideWindow(handle);}
    inline void show(){SDL_ShowWindow(handle);}
    bool fullscreen(bool fullscreen, bool desktop = false);
    
    static bool relativeMouse(bool relative = true);
    static bool hideCursor(bool hide);
    static inline bool isCursorHidden()
        {return SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE ? true : false;}
};

}

#endif

