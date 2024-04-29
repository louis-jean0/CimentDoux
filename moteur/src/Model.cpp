#include <Model.hpp>

// Constructors
Model::Model() : collider(bounding_box) {}

Model::Model(std::string path) : collider(bounding_box) {
    load_model(path);
}

Model::Model(Mesh &mesh) : collider(bounding_box) {
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

void Model::computeBoundingBox() {
    if(meshes.empty()) {
        return;
    }
    glm::vec3 min = meshes[0].bounding_box.min;
    glm::vec3 max = meshes[0].bounding_box.max;
    for(auto& mesh : meshes) {
        min.x = std::min(min.x, mesh.bounding_box.min.x);
        min.y = std::min(min.y, mesh.bounding_box.min.y);
        min.z = std::min(min.z, mesh.bounding_box.min.z);
        max.x = std::max(max.x, mesh.bounding_box.max.x);
        max.y = std::max(max.y, mesh.bounding_box.max.y);
        max.z = std::max(max.z, mesh.bounding_box.max.z);
    }
    bounding_box = AABB(min,max);
}

void Model::updateGlobalBoundingBox(const glm::mat4& modelMatrix) {
    glm::vec3 minWorld = glm::vec3(modelMatrix * glm::vec4(bounding_box.originalMin, 1.0));
    glm::vec3 maxWorld = glm::vec3(modelMatrix * glm::vec4(bounding_box.originalMax, 1.0));

    glm::vec3 corners[8] = {
        glm::vec3(minWorld.x, minWorld.y, minWorld.z),
        glm::vec3(maxWorld.x, minWorld.y, minWorld.z),
        glm::vec3(minWorld.x, maxWorld.y, minWorld.z),
        glm::vec3(maxWorld.x, maxWorld.y, minWorld.z),
        glm::vec3(minWorld.x, minWorld.y, maxWorld.z),
        glm::vec3(maxWorld.x, minWorld.y, maxWorld.z),
        glm::vec3(minWorld.x, maxWorld.y, maxWorld.z),
        glm::vec3(maxWorld.x, maxWorld.y, maxWorld.z)
    };

    glm::vec3 newMin = corners[0];
    glm::vec3 newMax = corners[0];

    for (int i = 1; i < 8; ++i) {
        newMin = glm::min(newMin, corners[i]);
        newMax = glm::max(newMax, corners[i]);
    }
    bounding_box.min = newMin;
    bounding_box.max = newMax;
}

// Private methods
void Model::load_model(std::string path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenBoundingBoxes);
    if(!scene || scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout<<"ERROR::ASSIMP"<<importer.GetErrorString()<<std::endl;
    }
    directory = path.substr(0, path.find_last_of('/'));
    process_node(scene->mRootNode, scene);
    computeBoundingBox();
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
    AABB bounding_box;
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
    aiAABB aabb = mesh->mAABB;
    bounding_box.processAABB(glm::vec3(aabb.mMin.x, aabb.mMin.y, aabb.mMin.z), glm::vec3(aabb.mMax.x, aabb.mMax.y, aabb.mMax.z));
    return Mesh(vertices, indices, textures, bounding_box);
}