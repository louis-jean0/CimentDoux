#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <EBO.hpp>

// Constructeur de la classe
EBO::EBO() {
    this->ElementsBufferObject = 0;
}

// Fonction pour générer les EBO
void EBO::genEBO(std::vector<unsigned int> indics) {
    glGenBuffers(1, &this->ElementsBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ElementsBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indics.size() * sizeof(unsigned int), &indics[0], GL_STATIC_DRAW);
}

// Fonction pour supprimer les EBO
void EBO::delEBO() {
    glDeleteBuffers(1, &this->ElementsBufferObject);
}