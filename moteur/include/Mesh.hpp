#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <Shader.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
}

struct Texture {
    unsigned int id;
    string type;
}

class Mesh {
    public:
        // Public attributes
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        Shader shader;

        // Constructor
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

        // Getters
        std::vector<Vertex> get_vertices();
        std::vector<unsigned int> get_indices();
        std::vector<Texture> get_textures();

        // Public methods
        void bind_shader(const GLchar* vertex_path, const GLchar* fragment_path);
        void unbind_shader();
        void draw();

    private:
        // Private attributes
        unsigned int vao,vbo,ebo;

        // Private methods
        void setup_mesh();
}