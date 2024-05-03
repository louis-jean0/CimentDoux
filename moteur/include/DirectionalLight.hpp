#include <Light.hpp>

class DirectionalLight : public Light {
    public:
        glm::vec3 direction;

        DirectionalLight(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, glm::vec3& direction) : Light(ambient, diffuse, specular), direction(direction) {}

        void setup_shader(Shader& shader, int light_index) const override;
};