#pragma once

#include <Shader.hpp>
#include <memory>

class ShaderManager {
    public:
        static std::shared_ptr<Shader> getAABBShader();
        static std::shared_ptr<Shader> getShader();
        static std::shared_ptr<Shader> getShadowShader();
};