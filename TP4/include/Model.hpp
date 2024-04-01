#include <Texture.hpp>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <Mesh.hpp>

class Model {
    public:
        // Constructors
        Model();
        Model(std::string path);
        
        // Public methods
        void draw();
        void bind_shader_to_meshes(Shader shader);
        void bind_shader_to_meshes(const GLchar* vertex_path, const GLchar* fragment_path);
    
    private:
        // Private attributes
        std::vector<Mesh> meshes;
        std::string directory;

        // Private methods
        void load_model(std::string path);
        void process_node(aiNode *node, const aiScene *scene);
        Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> load_material_textures(aiMaterial *material, aiTextureType *tex_type, std::string tex_type_name);
};