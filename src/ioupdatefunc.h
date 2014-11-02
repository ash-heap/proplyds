#ifndef IOUPDATEFUNC_H
#define IOUPDATEFUNC_H

#include <sciurus/types.h>
#include <sciurus/entity.h>

class IOUpdateFunc : sc::Component
{
protected:
    static sc::ComponentType type;
    sc::Entity* owner;

public:
    IOUpdateFunc(sc::Entity* owner = NULL);
    ~IOUpdateFunc();

    const sc::ComponentType* const getComponentType();
    
    void (*func)(sc::f32 dt, void* userdata, sc::Entity* owner);
    void* userdata;
    
    inline void call(sc::f32 dt){func(dt, userdata, owner);}
};

#endif
