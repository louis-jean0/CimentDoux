#include <PointLight.hpp>

class ShadowMap {
public:
    unsigned int depthMapFBO, depthCubemap;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    ShadowMap(PointLight& point_light);
    ~ShadowMap();

    void bind();
    void unbind();
};
