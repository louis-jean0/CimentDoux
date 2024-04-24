#pragma once

#include <GLFW/glfw3.h>
#include <iostream>

class Shader
{
    private:
        int use = 0, set = 0;
    public:
        GLuint Program;
        //Texture tex[10];    // 10 textures max
        
        Shader();
        void setShader(const GLchar* vertexPath, const GLchar* fragmentPath);        
        void useShader();
        void deleteShader();

        void setBind1f(const GLchar* name, GLfloat v0);
        void setBind2f(const GLchar* name, GLfloat v0, GLfloat v1);
        void setBind3f(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2);
        void setBindMatrix4fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat *value);
        void setBindMatrix3fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat *value);
        void setBind1i(const GLchar* name, GLint v0);
};

