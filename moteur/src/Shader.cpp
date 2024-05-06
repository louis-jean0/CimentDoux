#include "glm/gtc/type_ptr.hpp"
#include <glad/glad.h>
#include <Shader.hpp>
#include <fstream>
#include <sstream>
#include <GLFW/glfw3.h>

void Shader::setShader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
    // Shaders reading
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream;
        std::stringstream fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        std::cout << "Shaders successfully loaded !" << std::endl;
    }

    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar * fShaderCode = fragmentCode.c_str();

    // Shaders compilation
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED \n{}" << vertexPath << std::endl << infoLog << std::endl;
    }
    else
    {
        std::cout << "SHADER::VERTEX::COMPILATION_COMPLETED \n{}" << vertexPath << std::endl;
    }

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED \n{}" << fragmentPath  << std::endl << infoLog << std::endl;
    }
    else
    {
        std::cout << "SHADER::FRAGMENT::COMPILATION_COMPLETED \n{}" << fragmentPath  << std::endl;
    }

    // Shader Program
    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(this->Program, 512, nullptr, infoLog);
        std::cerr << "Shader linking error: " << infoLog << std::endl;
    }
    else
    {
        std::cout << "SHADER::PROGRAM::LINKING_COMPLETED" << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::useShader()
{
    glUseProgram(this->Program);
}

void Shader::deleteShader()
{
    glDeleteProgram(this->Program);
}

void Shader::setBind1f(const GLchar* name, GLfloat v0)
{
    glUniform1f(glGetUniformLocation(this->Program, name), v0);
}

void Shader::setBind2f(const GLchar* name, GLfloat v0, GLfloat v1)
{
    glUniform2f(glGetUniformLocation(this->Program, name), v0, v1);
}

void Shader::setBind3f(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2)
{
    glUniform3f(glGetUniformLocation(this->Program, name), v0, v1, v2);
}

void Shader::setBindMatrix4fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    glUniformMatrix4fv(glGetUniformLocation(this->Program, name), count, transpose, value);
}

void Shader::setBindMatrix3fv(const GLchar* name, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    glUniformMatrix3fv(glGetUniformLocation(this->Program, name), count, transpose, value);
}

void Shader::setBind1i(const GLchar* name, GLint v0)
{
    glUniform1i(glGetUniformLocation(this->Program, name), v0);
}

void Shader::setBool(const GLchar* name, GLboolean v0) { // Just for verbosity purposes lol
    setBind1i(name, v0); 
}

void Shader::setVec3(const GLchar* name, glm::vec3 vector) {
    this->setBind3f(name, vector.x, vector.y, vector.z);
}

void Shader::setMVPMatrix(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
    this->setBindMatrix4fv("model", 1, 0, glm::value_ptr(model));
    this->setBindMatrix4fv("view", 1, 0, glm::value_ptr(view));
    this->setBindMatrix4fv("projection", 1, 0, glm::value_ptr(projection));
}

void Shader::setVPMatrix(const glm::mat4& view, const glm::mat4& projection) {
    this->setBindMatrix4fv("view", 1, 0, glm::value_ptr(view));
    this->setBindMatrix4fv("projection", 1, 0, glm::value_ptr(projection));
}

// // Texture
// void Shader::setTex(const char* texPath, std::string texName, bool texFlip) {
//     this->tex[this->set].setTexture(texPath, texName, texFlip);
//     this->set+=1; 
//     if(this->set >= 10) { 
//         set = 0; 
//     }
// }

// void Shader::useTex() {
//     this->tex[this->use].useTexture();
//     this->use+=1; 
//     if(this->use >= 10) { 
//         use = 0; 
//     }
// }
