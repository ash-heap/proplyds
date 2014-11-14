#ifndef SCIURUS_SHADER_H
#define SCIURUS_SHADER_H

#include <sciurus/opengl.h>
#include <string>

namespace sc
{

class Shader
{
protected:
    GLenum type;
    std::string filename;
    char* source;
    GLint id;

public:
    Shader(GLenum type, std::string filename);
    ~Shader();
    
    inline std::string getFilename(){return filename;}
    inline char* getSource(){return source;}
    inline GLint getID(){return id;}
    inline GLenum getType(){return type;}
};

}

#endif

