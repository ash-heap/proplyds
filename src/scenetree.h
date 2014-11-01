#ifndef SCENETREE_H
#define SCENETREE_H

#include <sciurus/types.h>
#include <scenenode.h>
#include <gl1util.h>

//a system, in Entity/Component/System terms, for drawing a scene
class SceneTree
{
public:
    SceneNode* camera;
    std::vector<SceneNode*> nodes;
    
    inline void draw(){camera->drawScene();}
};

#endif

