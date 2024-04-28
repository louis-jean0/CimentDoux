#include <ShaderManager.hpp>

Shader& ShaderManager::getAABBShader() {
    static Shader aabbShader;
    static bool isLoaded = false;
    if(!isLoaded) {
        aabbShader.setShader("../shaders/aabb.vert", "../shaders/aabb.frag");
        isLoaded = true;
    }
    return aabbShader;
}