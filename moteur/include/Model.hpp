#include <algorithm>
#include <memory>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <Mesh.hpp>
#include <Texture.hpp>
#include <ModelCollider.hpp>

class Model {
    public:
        // Public attributes
        std::vector<std::unique_ptr<Mesh>> meshes;
        AABB bounding_box;
        ModelCollider collider;
        
        // Constructors
        Model() : collider(bounding_box) {}
        Model(std::string path) : collider(bounding_box) {
            load_model(path);
        }
        Model(Mesh& mesh) : collider(bounding_box) {}
        
        // Public methods
        void draw();
        void bind_shader_to_meshes(Shader shader);
        void bind_shader_to_meshes(const GLchar* vertex_path, const GLchar* fragment_path);
        void bind_texture_to_meshes(Texture texture);
        void computeBoundingBox();
        void updateGlobalBoundingBox(const glm::mat4 &modelMatrix);
    
    private:
        // Private attributes
        std::string directory;
        std::vector<std::string, std::shared_ptr<Texture>> textures_loaded;

        // Private methods
        void load_model(std::string path);
        void process_node(aiNode *node, const aiScene *scene);
        Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
        Material load_material_textures(aiMaterial *material);
        void load_textures_from_material(aiMaterial *material, aiTextureType tex_type, std::string tex_type_name, Material& mat);
};