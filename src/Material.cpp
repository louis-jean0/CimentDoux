#include <Material.hpp>

void Material::add_texture(std::shared_ptr<Texture> texture) {
    if(texture) {
        textures.push_back(texture);
    }
}