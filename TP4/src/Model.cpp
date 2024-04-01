#include <Model.hpp>

// Constructors
Model::Model() {}

Model::Model(std::string path) {
    load_model(path);
}

// Public methods
void Model::draw() {
    for(auto &mesh : meshes) {
        mesh.draw();
    }
}

void Model::bind_shader_to_meshes(Shader shader) {
    for(auto &mesh : meshes) {
        mesh.bind_shader(shader);
    }
}

void Model::bind_shader_to_meshes(const GLchar* vertex_path, const GLchar* fragment_path) {
    for(auto &mesh : meshes) {
        mesh.bind_shader(vertex_path,fragment_path);
    }
}



