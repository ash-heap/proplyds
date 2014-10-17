#ifndef SCIURUS_KEYBOARD_H
#define SCIURUS_KEYBOARD_H

#include <SDL2/SDL.h>

namespace sc
{

#define SCIURUS_SDL_NUM_SCANCODES 282
static const int NKEYS = SCIURUS_SDL_NUM_SCANCODES;
static const char KDOWN = 0x01; //button is currently down
static const char KPRESSED = 0x02; //button was pressed since last clearing
static const char KRELEASED = 0x04; //button was released since last clearing
static const char KPRESSEDNOW = KDOWN | KPRESSED; //do not use for reading
static const char KNOTDOWN = ~KDOWN; //do not use for reading

class Keyboard
{
protected:
    char kbstate[NKEYS];
    inline char presskey(char key){return key | KPRESSEDNOW;}
    inline char releasekey(char key){return (key & KNOTDOWN) | KRELEASED;}
    inline char clearkey(char key){return key & KDOWN;}

public:
    Keyboard();

    inline void wipe(){for(int i = 0; i < NKEYS; i++) kbstate[i] = 0x00;}
    inline void reset()
        {for(int i = 0; i < NKEYS; i++) kbstate[i] = clearkey(kbstate[i]);}
    inline void pressKey(SDL_Scancode s) //key press callback
        {kbstate[s] = presskey(kbstate[s]);}
    inline void pressKey(SDL_Keycode k)
        {pressKey(SDL_GetScancodeFromKey(k));}
    inline void releaseKey(SDL_Scancode s) //key release callback
        {kbstate[s] = releasekey(kbstate[s]);}
    inline void releaseKey(SDL_Keycode k)
        {releaseKey(SDL_GetScancodeFromKey(k));}
    inline char getKey(SDL_Scancode s)
        {return kbstate[s];}
    inline char getKey(SDL_Keycode key) //get raw key state
        {return getKey(SDL_GetScancodeFromKey(key));}
    inline bool isKeyDown(SDL_Scancode s)
        {return getKey(s) & KDOWN;}
    inline bool isKeyDown(SDL_Keycode k) //get key KDOWN state
        {return getKey(k) & KDOWN;}
    inline bool wasKeyPressed(SDL_Scancode s)
        {return getKey(s) & KPRESSED;}
    inline bool wasKeyPressed(SDL_Keycode k) //get key KPRESSED state
        {return getKey(k) & KPRESSED;}
    inline bool wasKeyReleased(SDL_Scancode s)
        {return getKey(s) & KRELEASED;}
    inline bool wasKeyReleased(SDL_Keycode k) //get key KRELEASED state
        {return getKey(k) & KRELEASED;}
};

}

#endif

