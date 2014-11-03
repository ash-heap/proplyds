#include <sciurus/entity.h>
#include <sciurus/types.h>
using namespace sc;

void Entity::addComponent(Component* toAdd)
{
    Iterator i = begin();
    for(; i != end(); i++)
    {
        if(*i >= toAdd)
        {
            c.insert(i, toAdd);
            return;
        }
    }
    c.push_back(toAdd);
}

bool Entity::rmComponent(Component* toRemove)
{
    u32 first = 0;
    u32 last = c.size();
    u32 mid = last / 2;
    while(first <= last)
    {
        if(c[mid] == toRemove)
        {
            c.erase(begin() + mid);
            return true;
        }
        else if(c[mid] < toRemove) last = mid - 1;
        else first = mid + 1;
    }
    return false;
}

bool Entity::hasComponent(Component* toFind)
{
    u32 first = 0;
    u32 last = c.size();
    while(first <= last)
    {
        u32 mid = first + ((last - first) / 2);
        if(c[mid] == toFind) return true;
        else if(c[mid] < toFind) last = mid - 1;
        else first = mid + 1;
    }
    return false;
}

