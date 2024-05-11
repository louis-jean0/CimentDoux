#pragma once

#include <memory>

class PointLight;

class ShadowMap {
public:
    unsigned int depthMapFBO, depthCubemap;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    ShadowMap(std::weak_ptr<PointLight> point_light);
    ~ShadowMap();

    static std::shared_ptr<ShadowMap> create(std::weak_ptr<PointLight> point_light);

    void bind();
    void unbind();
};
