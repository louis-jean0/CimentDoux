#include <glm/glm.hpp>
#include <vector>
#include <Texture.hpp>

class Material {
    public:
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
        std::vector<Texture> textures;

        Material() : ambient(1.0f), diffuse(1.0f), specular(1.0f), shininess(32.0f) {}

        Material(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, float shininess) : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}

        void add_texture(Texture texture);
};