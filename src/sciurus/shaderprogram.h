#ifndef SCIURUS_SHADERPROGRAM_H
#define SCIURUS_SHADERPROGRAM_H

#include <sciurus/shader.h>
#include <string>

namespace sc
{

class ShaderProgram
{
protected:
    Shader* vert;
    Shader* frag;
    GLint id;

public:
    ShaderProgram(std::string vert, std::string frag);
    ShaderProgram(Shader* vert, Shader* frag);
    ~ShaderProgram();
    
    void makeCurrent();
    
    inline GLint getID(){return id;}
    inline Shader* getVert(){return vert;}
    inline Shader* getFrag(){return frag;}
};

}

#endif

