#pragma once

#include <glad/glad.h>
#include <string>
#include <stb_image.hpp>

class Texture {
    public:
        unsigned int ID;
        std::string type;
        std::string path;

        Texture() {}
        Texture(const char* image_path);
        Texture(const char* image_path, const std::string &directory, const std::string &tex_type);
        
        static unsigned int texture_from_file(const char* image_path, const std::string &directory);
        void use();
        void unbind();
};