#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <string>

#include <Shader.hpp>
#include <Texture.hpp>

class Mesh {
    public:
        // Public attributes
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<unsigned short> indices;
        std::vector<Texture> textures;
        std::vector<glm::vec2> uvs;
        Shader shader;

        // Constructors
        Mesh();
        Mesh(std::vector<glm::vec3> vertices, std::vector<unsigned short> indices, std::vector<glm::vec2> uv);
        Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<unsigned short> indices, std::vector<glm::vec2> uv);

        // Public methods
        void bind_shader(Shader shader);
        void bind_shader(const GLchar* vertex_path, const GLchar* fragment_path);
        void unbind_shader();
        void draw();
        void setup_mesh();
        void add_texture(Texture texture);

    private:
        // Private attributes
        unsigned int vao,vbo,ebo;
};