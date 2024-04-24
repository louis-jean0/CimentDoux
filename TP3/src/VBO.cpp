#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <VBO.hpp>

// Constructeur de la classe
VBO::VBO() {
    this->VertexBufferObject = 0;
}

// Fonction pour générer les VBO
void VBO::genVBO(std::vector<float> verts) {
    glGenBuffers(1, &this->VertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, this->VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), &verts[0], GL_STATIC_DRAW);
}

void VBO::delVBO() {
    glDeleteBuffers(1, &this->VertexBufferObject);
}