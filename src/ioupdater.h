#ifndef IOUPDATER_H
#define IOUPDATER_H

#include <sciurus/types.h>
#include <ioupdatefunc.h>
#include <vector>

class IOUpdater
{
protected:
    sc::f32 dt;

public:
    std::vector<IOUpdateFunc*> funcs;
    
    inline void update()
        {for(sc::u32 i = 0; i < funcs.size(); i++) funcs[i]->call(dt);}
};

#endif
