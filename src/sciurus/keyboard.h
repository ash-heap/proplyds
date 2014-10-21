#ifndef SCIURUS_KEYBOARD_H
#define SCIURUS_KEYBOARD_H

#include <sciurus/types.h>
#include <SDL2/SDL.h>

namespace sc
{

#define SCIURUS_SDL_NUM_SCANCODES 282
static const u32 NKEYS = SCIURUS_SDL_NUM_SCANCODES;
static const u8 KDOWN = 0x01; //button is currently down
static const u8 KPRESSED = 0x02; //button was pressed since last clearing
static const u8 KRELEASED = 0x04; //button was released since last clearing
static const u8 KPRESSEDNOW = KDOWN | KPRESSED; //do not use for reading
static const u8 KNOTDOWN = ~KDOWN; //do not use for reading

class Keyboard
{
protected:
    u8 kbstate[NKEYS];
    inline u8 presskey(u8 key){return key | KPRESSEDNOW;}
    inline u8 releasekey(u8 key){return (key & KNOTDOWN) | KRELEASED;}
    inline u8 clearkey(u8 key){return key & KDOWN;}

public:
    Keyboard();

    inline void wipe(){for(u32 i = 0; i < NKEYS; i++) kbstate[i] = 0x00;}
    inline void reset()
        {for(u32 i = 0; i < NKEYS; i++) kbstate[i] = clearkey(kbstate[i]);}
    inline void pressKey(SDL_Scancode s) //key press callback
        {kbstate[s] = presskey(kbstate[s]);}
    inline void pressKey(SDL_Keycode k)
        {pressKey(SDL_GetScancodeFromKey(k));}
    inline void releaseKey(SDL_Scancode s) //key release callback
        {kbstate[s] = releasekey(kbstate[s]);}
    inline void releaseKey(SDL_Keycode k)
        {releaseKey(SDL_GetScancodeFromKey(k));}
    inline u8 getKey(SDL_Scancode s)
        {return kbstate[s];}
    inline u8 getKey(SDL_Keycode key) //get raw key state
        {return getKey(SDL_GetScancodeFromKey(key));}
    inline bool isKeyDown(SDL_Scancode s)
        {return (getKey(s) & KDOWN) == 0 ? false : true;}
    inline bool isKeyDown(SDL_Keycode k) //get key KDOWN state
        {return (getKey(k) & KDOWN) == 0 ? false : true;}
    inline bool wasKeyPressed(SDL_Scancode s)
        {return (getKey(s) & KPRESSED) == 0 ? false : true;}
    inline bool wasKeyPressed(SDL_Keycode k) //get key KPRESSED state
        {return (getKey(k) & KPRESSED) == 0 ? false : true;}
    inline bool wasKeyReleased(SDL_Scancode s)
        {return (getKey(s) & KRELEASED) == 0 ? false : true;}
    inline bool wasKeyReleased(SDL_Keycode k) //get key KRELEASED state
        {return (getKey(k) & KRELEASED) == 0 ? false : true;}
};

}

#endif

