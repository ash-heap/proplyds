#include <sciurus/shaderprogram.h>
#include <cstdio>
#include <error.h>
using namespace sc;

static void printProgramLog(GLint id)
{
    int len = 0;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
    if (len > 1)
    {
        int n = 0;
        char* buffer = new char[len];
        if (!buffer)
            error(1, 0, "Cannot allocate %d bytes of text for program log\n", len);
        glGetProgramInfoLog(id, len, &n, buffer);
        fprintf(stderr, "%s\n", buffer);
        delete[] buffer;
    }
    glGetProgramiv(id, GL_LINK_STATUS, &len);
    if (!len) error(1, 0, "Error linking program\n");
}

static void setupProgram(GLint id, Shader* vert, Shader* frag)
{
    id = glCreateProgram();
    GLint vertid = vert->getID();
    GLint fragid = frag->getID();
    glAttachShader(id, vertid);
    glAttachShader(id, fragid);
    glLinkProgram(id);
    printProgramLog(id);
}

ShaderProgram::ShaderProgram(std::string vert, std::string frag)
{
    this->vert = new Shader(GL_VERTEX_SHADER, vert);
    this->frag = new Shader(GL_FRAGMENT_SHADER, frag);
    setupProgram(id, this->vert, this->frag);
}

ShaderProgram::ShaderProgram(Shader* vert, Shader* frag)
{
    this->vert = vert;
    this->frag = frag;
    setupProgram(id, vert, frag);
}

ShaderProgram::~ShaderProgram()
{
    vert = NULL;
    frag = NULL;
    glDeleteProgram(id);
}

void ShaderProgram::makeCurrent()
{
    
}

