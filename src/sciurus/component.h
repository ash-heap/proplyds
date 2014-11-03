#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

namespace sc
{

//if people want to store information in this type, they should extend it
struct ComponentType
{
};

class Component
{
public:
    virtual ~Component();
    //this should be constant pointer at runtime for optimized interpretation
    //recommend static variable in subclass cpp file
    virtual const ComponentType* const getComponentType() = 0;
};

}

#endif
