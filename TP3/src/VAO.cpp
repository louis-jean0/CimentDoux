#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <VAO.hpp>

// Constructeur de la classe
VAO::VAO() {
    this->VertexAttribObject = 0;
}

void VAO::genVAO() {
    glGenVertexArrays(1, &this->VertexAttribObject);
    glBindVertexArray(this->VertexAttribObject);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Textures
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

unsigned int VAO::getVAO() {
    return this->VertexAttribObject;
}

// Fonction pour supprimer les VAO
void VAO::delVAO() {
    glDeleteVertexArrays(1, &this->VertexAttribObject);
}