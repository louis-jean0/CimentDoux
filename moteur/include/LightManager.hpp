#pragma once

#include <Light.hpp>
#include <PointLight.hpp>
#include <DirectionalLight.hpp>
#include <vector>

class LightManager {
    public:
        std::vector<std::shared_ptr<Light>> lights;

        void add_light(std::shared_ptr<Light> light);
        void setup_lights(std::shared_ptr<Shader> shader) const;
};