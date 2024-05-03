#include <Light.hpp>

class PointLight : public Light {
    public:
        glm::vec3 position;
        float constant;
        float linear;
        float quadratic;

        PointLight(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, glm::vec3& position, float constant, float linear, float quadratic) : Light(ambient, diffuse, specular), position(position), constant(constant), linear(linear), quadratic(quadratic) {}

        void setup_shader(Shader& shader, int light_index) const override;
};