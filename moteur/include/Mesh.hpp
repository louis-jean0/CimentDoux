#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <Shader.hpp>
#include <Material.hpp>
#include <AABB.hpp>
#include <ModelCollider.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    glm::vec2 uv;
};

class Mesh {
    public:
        // Public attributes
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::shared_ptr<Material> material;
        AABB bounding_box;
        ModelCollider collider;
        std::shared_ptr<Shader> shader;
        

        // Constructors
        Mesh() : collider(bounding_box) {}
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::shared_ptr<Material> material, AABB& bounding_box) : vertices(std::move(vertices)), indices(std::move(indices)), material(material), bounding_box(bounding_box), collider(this->bounding_box) {
            setup_mesh();
        }

        // Destructor
        ~Mesh() {
            glDeleteBuffers(1, &vbo);
            glDeleteVertexArrays(1, &vao);
        }

        void bind_shader(std::shared_ptr<Shader> shader);
        void bind_shader(const GLchar* vertex_path, const GLchar* fragment_path);
        void unbind_shader();
        void draw();
        void setup_mesh();
        void bindVAO();
        void drawElements();
        void add_texture(std::shared_ptr<Texture> texture);
        glm::vec3 getVerticeFromIndice(unsigned int indice);
        void updateGlobalBoundingBox(const glm::mat4& modelMatrix);
    
    private:
        // Private attributes
        GLuint vao,vbo,ebo;
};