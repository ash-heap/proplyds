#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

namespace sc
{

struct ComponentType
{
    std::string name; //name of the component type (ex: "transform")
    std::string info; //info of the component type
    //ex: "describes transformation of entity relative to scene parent"
    void* userdata; //pointers to other data for global access!
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
