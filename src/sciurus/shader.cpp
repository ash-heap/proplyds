#include <sciurus/shader.h>
#include <cstdio>
#include <error.h>
#include <cerrno>
using namespace sc;

static void printShaderLog(GLint id, std::string filename)
{
    int len = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
    if(len > 1)
    {
        int n = 0;
        char* buffer = new char[len];
        if (!buffer)
            error(1, 0, "Cannot allocate %d bytes of text for shader log\n",len);
        glGetShaderInfoLog(id,len,&n,buffer);
        fprintf(stderr, "%s:\n%s\n", filename.c_str(), buffer);
        delete[] buffer;
    }
    glGetShaderiv(id, GL_COMPILE_STATUS, &len);
    if (!len) error(1, 0, "Error compiling %s\n", filename.c_str());
}

Shader::Shader(GLenum type, std::string filename)
{
    id = glCreateShader(type);
    
    FILE* file = fopen(filename.c_str(), "r");
    if(!file) error(3, errno, "Failed to open file %s", filename.c_str());
    fseek(file, 0, SEEK_END);
    unsigned long size = ftell(file);
    rewind(file);
    source = new char[size + 1];
    if(fread((void*)source, sizeof(char), size, file) < size)
        error(4, errno, "Failed to read file %s", filename.c_str());
    source[size] = 0;
    fclose(file);
    
    glShaderSource(id, 1, (const char**)&source, NULL);
    printf("compiling shader: %s\n", filename.c_str());
    glCompileShader(id);
    printShaderLog(id, filename);
}

Shader::~Shader()
{
    delete[] source;
    if(id > 0) glDeleteShader(id);
    id = -1;
}

