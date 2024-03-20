#pragma once

#include <glad/glad.h>
#include <string>
#include <stb_image.hpp>

class Texture {
    public:
        unsigned int ID;
        std::string type;
        std::string path;

        Texture(const char* image_path);
        
        void use();
};