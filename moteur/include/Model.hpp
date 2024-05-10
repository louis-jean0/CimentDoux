#include "assimp/matrix4x4.h"
#include <algorithm>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <Mesh.hpp>
#include <ModelCollider.hpp>
#include <memory>

struct MeshEntry {
    std::shared_ptr<Mesh> mesh;
    glm::mat4 transform;
};

class Model {
    public:
        // Public attributes
        std::vector<MeshEntry> entries;
        AABB bounding_box;
        ModelCollider collider;
        std::string directory;
        static std::vector<std::shared_ptr<Texture>> textures_loaded;
        
        // Constructors
        Model();
        Model(const std::string& path);
        Model(std::shared_ptr<Mesh> mesh);

        // Factory
        static std::shared_ptr<Model> create() {
            return std::make_shared<Model>();
        }

        static std::shared_ptr<Model> create(const std::string& path) {
            return std::make_shared<Model>(path);
        }

        static std::shared_ptr<Model> create(const std::string& path, std::shared_ptr<Shader> shader) {
            auto instance = std::make_shared<Model>(path);
            instance->bind_shader_to_meshes(shader);
            return instance;
        }

        static std::shared_ptr<Model> create(const std::string& path, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture) {
            auto instance = std::make_shared<Model>(path);
            instance->bind_shader_to_meshes(shader);
            instance->bind_texture_to_meshes(texture);
            return instance;
        }

        static std::shared_ptr<Model> create(std::shared_ptr<Mesh> mesh) {
            return std::make_shared<Model>(mesh);
        }
        
        // Public methods
        void draw();
        void bind_shader_to_meshes(std::shared_ptr<Shader> shader);
        void bind_shader_to_meshes(const GLchar* vertex_path, const GLchar* fragment_path);
        void bind_texture_to_meshes(std::shared_ptr<Texture> texture);
        void computeBoundingBox();
        void updateGlobalBoundingBox(const glm::mat4 &modelMatrix);
        glm::mat4 aiMatrixToGlm(aiMatrix4x4 from);
    
    private:
        // Private methods
        void load_model(const std::string& path);
        void process_node(aiNode *node, const aiScene *scene, glm::mat4 parent_transform);
        std::shared_ptr<Mesh> process_mesh(aiMesh *mesh, const aiScene *scene);
        std::shared_ptr<Material> load_material_textures(aiMaterial *material);
        void load_textures_from_material(aiMaterial *material, aiTextureType tex_type, std::string tex_type_name, std::shared_ptr<Material> mat);
};