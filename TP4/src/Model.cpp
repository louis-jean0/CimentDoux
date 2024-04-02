#include <Model.hpp>

// Constructors
Model::Model() {}

Model::Model(std::string path) {
    load_model(path);
}

Model::Model(Mesh &mesh) {
    meshes.push_back(mesh);
}

// Public methods
void Model::draw() {
    for(auto &mesh : meshes) {
        mesh.draw();
    }
}

void Model::bind_shader_to_meshes(Shader shader) {
    for(auto &mesh : meshes) {
        mesh.bind_shader(shader);
    }
}

void Model::bind_shader_to_meshes(const GLchar* vertex_path, const GLchar* fragment_path) {
    for(auto &mesh : meshes) {
        mesh.bind_shader(vertex_path,fragment_path);
    }
}

// Private methods
void Model::load_model(std::string path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate || aiProcess_FlipUVs);
    if(!scene || scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout<<"ERROR::ASSIMP"<<importer.GetErrorString()<<std::endl;
    }
    directory = path.substr(0, path.find_last_of('/'));
    process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode *node, const aiScene *scene)  {
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(process_mesh(mesh, scene));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        process_node(node->mChildren[i], scene);
    }
}

std::vector<Texture> Model::load_material_textures(aiMaterial *material, aiTextureType tex_type, std::string tex_type_name) {
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < material->GetTextureCount(tex_type); i++) {
        aiString str;
        material->GetTexture(tex_type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++) {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip) {
            Texture texture(str.C_Str(),directory,tex_type_name);
            textures.push_back(texture);
        }
    }
    return textures;
}

Mesh Model::process_mesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    // Vertices data
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 temp;
        temp.x = mesh->mVertices[i].x;
        temp.y = mesh->mVertices[i].y;
        temp.z = mesh->mVertices[i].z;
        vertex.position = temp;
        temp.x = mesh->mNormals[i].x;
        temp.y = mesh->mNormals[i].y;
        temp.z = mesh->mNormals[i].z;
        vertex.normal = temp;
        if(mesh->mTextureCoords[0]) {
            glm::vec2 temp_uv;
            temp_uv.x = mesh->mTextureCoords[0][i].x;
            temp_uv.y = mesh->mTextureCoords[0][i].y;
            vertex.uv = temp_uv;
        }
        else {
            vertex.uv = glm::vec2(0.0f,0.0f);
        }
        vertices.push_back(vertex);
    }
    // Indices data
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    // Textures data
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(),diffuse_maps.begin(),diffuse_maps.end());
        std::vector<Texture> specular_maps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(),specular_maps.begin(),specular_maps.end());
    }
    return Mesh(vertices, indices, textures);
}