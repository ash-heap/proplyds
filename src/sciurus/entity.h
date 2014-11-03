#ifndef ENTITY_H
#define ENTITY_H

#include <sciurus/component.h>
#include <vector>

namespace sc
{

class Entity
{
protected:
    friend class Component;
    std::vector<Component*> c;

public:
    typedef std::vector<Component*>::iterator Iterator;
    void addComponent(Component* toAdd);
    bool rmComponent(Component* toRemove);
    bool hasComponent(Component* toFind);
    inline Iterator begin(){return c.begin();};
    inline Iterator end(){return c.end();};
};

}

#endif
