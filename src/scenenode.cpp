#include <scenenode.h>
#include <glinc.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/matrix_transform.hpp>
using namespace sc;
using namespace std;

SceneNode::SceneNode()
{
    t = mat4(1.f);
    f = nulldrawfunc;
    parent = NULL;
    children = vector<SceneNode*>();
}

SceneNode::SceneNode(SceneNode* parent)
{
    t = mat4(1.f);
    f = nulldrawfunc;
    this->parent = parent;
    parent->children.push_back(this);
    children = vector<SceneNode*>();
}

SceneNode::~SceneNode()
{
    u32 size = (u32)children.size();
    for(u32 i = 0; i < size; i++)
        delete children[i];
}

void SceneNode::drawAll()
{
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(t));
    f(data);
    u32 size = (u32)children.size();
    for(u32 i = 0; i < size; i++) children[i]->drawAll();
    glPopMatrix();
}

void SceneNode::orphan()
{
    for(std::vector<SceneNode*>::iterator i = parent->children.begin();
        i != parent->children.end(); i++)
    {
        if(*i == this)
            parent->children.erase(i);
    }
    parent = NULL;
}

bool SceneNode::sameTree(SceneNode* other)
{
    SceneNode* a = this;
    SceneNode* b = other;
    for(; !a->isRoot(); a = a->parent);
    for(; !b->isRoot(); b = b->parent);
    return a == b;
}

mat4 SceneNode::getGlobTF()
{
    mat4 a = t;
    SceneNode* n = this;
    for(; !n->isRoot(); n = n->parent) a = n->t * a;
    return glm::inverse(a);
}

mat4 SceneNode::getInvGlobTF()
{
    mat4 a = t;
    SceneNode* n = this;
    for(; !n->isRoot(); n = n->parent) a = n->t * a;
    return a;
}

void SceneNode::setGlobTF(mat4 tf)
{
    SceneNode* n = parent;
    mat4 a = n->t;
    while((n = n->parent)) a = n->t * a;
    t = glm::inverse(tf) * a;
}

