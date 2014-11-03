#ifndef IOUPDATER_H
#define IOUPDATER_H

#include <sciurus/types.h>
#include <sciurus/keyboard.h>
#include <ioupdatefunc.h>
#include <vector>

class IOUpdater
{
protected:
    sc::f32 dt;
    sc::Keyboard kb;

public:
    std::vector<IOUpdateFunc*> funcs;
    
    void update();
};

#endif
