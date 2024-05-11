#pragma once

#include <memory>

class PointLight;

class ShadowMap {
public:
    std::weak_ptr<PointLight> point_light;
    unsigned int depthMapFBO, depthCubemap;
    const unsigned int SHADOW_WIDTH = 1440, SHADOW_HEIGHT = 1080;

    ShadowMap(std::shared_ptr<PointLight> point_light);
    ~ShadowMap();

    static std::shared_ptr<ShadowMap> create(std::shared_ptr<PointLight> point_light);

    void bind();
    void unbind();
};
