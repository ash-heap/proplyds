#include <ioupdatefunc.h>
using namespace sc;

ComponentType IOUpdateFunc::type = 
{
    "IOUpdateFunc",
    "Used to update IO states of entities",
    NULL
};

const ComponentType* const IOUpdateFunc::getComponentType()
    {return &type;}

IOUpdateFunc::IOUpdateFunc(Entity* owner)
{
    this->owner = owner;
    if(owner) owner->addComponent(this);
    userdata = NULL;
}

IOUpdateFunc::~IOUpdateFunc()
{
    if(owner) owner->rmComponent(this);
}

