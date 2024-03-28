#include <Mesh.hpp>

Mesh::Mesh() {}

Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<unsigned short> indices, std::vector<glm::vec2> uvs) {
    this->vertices = vertices;
    this->indices = indices;
    this->uvs = uvs;
}

Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<unsigned short> indices, std::vector<glm::vec2> uvs) {
    this->vertices = vertices;
    this->normals = normals;
    this->indices = indices;
    this->uvs = uvs;
}

void Mesh::setup_mesh() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
  
    glBindVertexArray(vao);

    // Positions
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  
    glEnableVertexAttribArray(0); // Vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);

    // Vertex positions
    

    glBindVertexArray(0);

}

void Mesh::add_texture(Texture texture) {
    this->textures.push_back(texture);
}

void Mesh::draw() {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    //std::cout<<"Taille tex : "<<textures.size()<<std::endl;
    for(int i = 0; i < textures.size(); i++) {
        //std::cout<<textures[i].ID<<std::endl;
        glActiveTexture(GL_TEXTURE0 + i);
        textures[i].use();
        std::string number;
        std::string name = textures[i].type;
        if(name == "texture_diffuse") {
            number = std::to_string(diffuseNr++);
        }
        else if(name == "texture_specular") {
            number = std::to_string(specularNr++);
        }
        this->shader.setBind1i((name + number).c_str(), i);
        //std::cout<<"test"<<std::endl;
    }
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
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