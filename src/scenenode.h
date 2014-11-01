#ifndef SCENENODE_H
#define SCENENODE_H

#include <sciurus/types.h>
#include <sciurus/entity.h>
#include <sciurus/component.h>
#include <vector>

class SceneNode : public sc::Component
{
protected:
    friend class SceneTree;
    static sc::ComponentType type;
    sc::Entity* owner;

public:
    typedef void(*DrawFunction)(void*);
    static void nulldrawfunc(void*){}

    SceneNode* parent;
    std::vector<SceneNode*> children;
    sc::mat4 t;
    DrawFunction f;
    void* data;

    SceneNode();
    SceneNode(SceneNode* parent, sc::Entity* owner = NULL);
    ~SceneNode();

    void draw();
    void drawScene();

    void orphan();
    inline void adopt(SceneNode* newchild)
        {children.push_back(newchild); newchild->parent = this;}

    SceneNode* getRoot();
    inline bool isRoot(){return parent == NULL;}
    bool sameTree(SceneNode* other);
    sc::mat4 getGlobTF();
    sc::mat4 getInvGlobTF();
    void setGlobTF(sc::mat4 tf);
    
    const sc::ComponentType* const getComponentType();
};

#endif
