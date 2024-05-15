#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
    private : 
        int height, width;
        const char* title;
        unsigned int major, minor;
        GLFWwindow* window;
        GLFWmonitor* ecran;
        bool Vsync;

    public :
        Window(unsigned int major, unsigned int minor, int width, int height, const char *title, bool Vsync);
        ~Window();
        void setup_GLFW();

        int get_height();
        int get_width();
        const char* get_title();

        unsigned int get_major();
        unsigned int get_minor();
        GLFWwindow* get_window();  
        GLFWmonitor* get_ecran();
        int get_maxWidth();
        int get_maxHeight();
        
        const GLFWvidmode* windowParams;
};
//