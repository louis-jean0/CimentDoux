#include <glm/glm.hpp>
#include <Shader.hpp>

class Light {
    public:
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        Light(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular) : ambient(ambient), diffuse(diffuse), specular(specular) {}

        virtual void setup_shader(Shader& shader, int light_index) const = 0;
};