#pragma once

#include <Light.hpp>
#include <PointLight.hpp>
#include <DirectionalLight.hpp>
#include <TorchLight.hpp>
#include <vector>

class LightManager {
    public:
        std::shared_ptr<DirectionalLight> directional_light;
        std::vector<std::shared_ptr<PointLight>> point_lights;

        void add_directional_light(std::shared_ptr<DirectionalLight> directional_light);
        void add_point_light(std::shared_ptr<PointLight> point_light);
        void setup_lights(std::shared_ptr<Shader> shader) const;
};