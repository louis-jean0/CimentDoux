#include <Mesh.hpp>
#include <cstddef>

Mesh::Mesh() {}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material& material, AABB bounding_box) {
    this->vertices = vertices;
    this->indices = indices;
    this->material = material;
    this->bounding_box = bounding_box;
    setup_mesh();
}

void Mesh::setup_mesh() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
  
    glBindVertexArray(vao);

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  
    glEnableVertexAttribArray(0); // Vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Vertex normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Vertex uv
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glBindVertexArray(0);

}

void Mesh::add_texture(Texture texture) {
    this->material.textures.push_back(texture);
}

void Mesh::draw() {
    // Material
    this->shader.setVec3("material.ambient", material.ambient);
    this->shader.setVec3("material.diffuse", material.diffuse);
    this->shader.setVec3("material.specular", material.specular);
    this->shader.setBind1f("material.shininess", material.shininess);

    // Textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for(int i = 0; i < material.textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = material.textures[i].type;
        if(name == "texture_diffuse") {
            number = std::to_string(diffuseNr++);
        }
        else if(name == "texture_specular") {
            number = std::to_string(specularNr++);
        }
        this->shader.setBind1i(("material" + name + number).c_str(), i);
        material.textures[i].use();
    }
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::bind_shader(Shader shader) {
    this->shader = shader;
}

void Mesh::bind_shader(const GLchar *vertex_path,const GLchar *fragment_path) {
    shader.setShader(vertex_path,fragment_path);
}

void Mesh::unbind_shader() {
    shader.Program = 0;
}

glm::vec3 Mesh::getVerticeFromIndice(unsigned int indice) {
    return this->vertices[indice].position;
}