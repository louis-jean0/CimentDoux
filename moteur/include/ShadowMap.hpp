#pragma once

#include <memory>

class TorchLight;

class ShadowMap {
public:
    std::weak_ptr<TorchLight> torch_light;
    unsigned int depth, depthMapFBO, depthMap;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    ShadowMap(std::shared_ptr<TorchLight> point_light);
    ~ShadowMap();

    static std::shared_ptr<ShadowMap> create(std::shared_ptr<TorchLight> torch_light);

    void bind();
    void unbind();
};
