#include <Mesh.hpp>
#include <cstddef>
#include <memory>
#include <string>

void Mesh::setup_mesh() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
  
    // Vertices
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW); 

    // Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Vertex normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Vertex tangent
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    // Vertex bitangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    // Vertex uv
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glBindVertexArray(0);
}

void Mesh::add_texture(std::shared_ptr<Texture> texture) {
    this->material->textures.push_back(texture);
}

void Mesh::draw() {
    // Material
    shader->setVec3("material.ambient", material->ambient);
    shader->setVec3("material.diffuse", material->diffuse);
    shader->setVec3("material.specular", material->specular);
    shader->setVec3("material.emissive", material->emissive);
    shader->setBind1f("material.shininess", material->shininess);

    // Textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalMapNr = 1;
    bool hasDiffuse = false;
    bool hasSpecular = false;
    bool hasNormalMap = false;
    for(int i = 0; i < material->textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = material->textures[i]->type;
        if(name == "texture_diffuse") {
            number = std::to_string(diffuseNr++);
            hasDiffuse = true;
        }
        else if(name == "texture_specular") {
            number = std::to_string(specularNr++);
            hasSpecular = true;
        }
        else if(name == "normal_map") {
            number = std::to_string(normalMapNr++);
            hasNormalMap = true;
        }
        shader->setBind1i((name + number).c_str(), i);
        material->textures[i]->use();
    }
    shader->setBool("hasDiffuse", hasDiffuse);
    shader->setBool("hasSpecular", hasSpecular);
    shader->setBool("hasNormalMap", hasNormalMap);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    for (int i = 0; i < material->textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0); // Reset the texture for each active texture unit
    }
}

void Mesh::bindVAO() {
    glBindVertexArray(vao);
}

void Mesh::drawElements() {
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::bind_shader(std::shared_ptr<Shader> shader) {
    this->shader = shader;
}

void Mesh::bind_shader(const GLchar *vertex_path,const GLchar *fragment_path) {
    shader->setShader(vertex_path,fragment_path);
}

void Mesh::unbind_shader() {
    shader->Program = 0;
}

glm::vec3 Mesh::getVerticeFromIndice(unsigned int indice) {
    return this->vertices[indice].position;
}

void Mesh::updateGlobalBoundingBox(const glm::mat4 &modelMatrix) {
    bounding_box.updateAABB(modelMatrix);
}