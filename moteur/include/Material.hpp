#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <Texture.hpp>

class Material {
    public:
        std::string name;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 emissive;
        float shininess;
        std::vector<std::shared_ptr<Texture>> textures;

        Material() : ambient(1.0f), diffuse(1.0f), specular(1.0f), shininess(32.0f) {}

        Material(const std::string name, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess) : name(name), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}

        Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess) : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}

        void add_texture(std::shared_ptr<Texture> texture);
};