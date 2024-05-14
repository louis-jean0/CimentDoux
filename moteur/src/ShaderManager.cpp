#include <ShaderManager.hpp>
#include <memory>

std::shared_ptr<Shader> ShaderManager::getAABBShader() {
    static std::shared_ptr<Shader> aabbShader;
    static bool isLoaded = false;
    if(!isLoaded) {
        aabbShader = Shader::create("../shaders/aabb.vert", "../shaders/aabb.frag");
        isLoaded = true;
    }
    //std::cout<<"aabbShader : "<<aabbShader->Program<<std::endl;
    return aabbShader;
}

std::shared_ptr<Shader> ShaderManager::getShader() {
    static std::shared_ptr<Shader> shader;
    static bool isLoaded = false;
    if(!isLoaded) {
        shader = Shader::create("../shaders/test.vs", "../shaders/test.fs");
        isLoaded = true;
    }
    //std::cout<<"shader : "<<shader->Program<<std::endl;
    return shader;
}

std::shared_ptr<Shader> ShaderManager::getShadowShader() {
    static std::shared_ptr<Shader> shadow_shader;
    static bool isLoaded = false;
    if(!isLoaded) {
        shadow_shader = Shader::create("../shaders/shadow.vs", "../shaders/shadow.fs");
        isLoaded = true;
    }
    //std::cout<<"shadow_shader : "<<shadow_shader->Program<<std::endl;
    return shadow_shader;
}