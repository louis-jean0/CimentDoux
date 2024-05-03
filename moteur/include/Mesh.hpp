#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <Shader.hpp>
#include <Material.hpp>
#include <AABB.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Mesh {
    public:
        // Public attributes
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        Material material;
        AABB bounding_box;
        Shader shader;

        // Constructors
        Mesh();
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material& material, AABB bounding_box);

        void bind_shader(Shader shader);
        void bind_shader(const GLchar* vertex_path, const GLchar* fragment_path);
        void unbind_shader();
        void draw();
        void setup_mesh();
        void add_texture(Texture texture);
        glm::vec3 getVerticeFromIndice(unsigned int indice);

    private:
        // Private attributes
        unsigned int vao,vbo,ebo;
};