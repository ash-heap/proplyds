#ifndef SCENENODE_H
#define SCENENODE_H

#include <sciurus/types.h>
#include <vector>

class SceneNode
{
public:
    typedef void(*DrawFunction)(void*);
    static void nulldrawfunc(void*){}
    
    SceneNode* parent;
    std::vector<SceneNode*> children;
    sc::mat4 t;
    DrawFunction f;
    void* data;

    SceneNode();
    SceneNode(SceneNode* parent);
    ~SceneNode();

    inline void draw(){f(data);}
    void drawAll();
    
    void orphan();
    inline void adopt(SceneNode* newchild)
        {children.push_back(newchild); newchild->parent = this;}

    inline bool isRoot(){return parent == NULL;}
    bool sameTree(SceneNode* other);
    sc::mat4 getGlobTF();
    sc::mat4 getInvGlobTF();
    void setGlobTF(sc::mat4 tf);
};

#endif
