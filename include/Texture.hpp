#pragma once

#include <glad/glad.h>
#include <string>
#include <stb_image.hpp>
#include <ShaderManager.hpp>

class Texture {
    public:
        unsigned int ID;
        std::string type;
        std::string path;

        Texture() {}
        Texture(const char* image_path);
        Texture(const char* image_path, const std::string &directory, const std::string &tex_type);
        Texture(std::string &image_path, const std::string &directory, const std::string &tex_type);
        Texture(const std::string &filename_with_dir, const std::string &tex_type);
        
        // Factory
        static std::shared_ptr<Texture> create(const char* image_path) {
            return std::make_shared<Texture>(image_path);
        }

        static unsigned int texture_from_file(const char* image_path, const std::string &directory);
        void use();
        void unbind();
};