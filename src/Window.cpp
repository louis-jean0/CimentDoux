#include <Window.hpp>

Window::Window(unsigned int major, unsigned int minor, int width, int height, const char *title, bool Vsync) {
    this->minor = minor;
    this->major = major;
    this->height = height;
    this->width = width;
    this->title = title;
    this->Vsync = Vsync;
}

void Window::setup_GLFW() {
    // Initializing GLFW
    glfwInit();
    if (!glfwInit()) {
        exit(0);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,  this->minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creating window
    this->window = glfwCreateWindow(this->width, this->height, this->title, NULL, NULL);
    if (window == NULL) {
        std::cout<<"Failed to create GLFW window"<<std::endl;
        glfwTerminate();
        exit(1);
    }


    this->ecran = glfwGetPrimaryMonitor();
    if (!this->ecran) {
        std::cerr << "ERREUR : Impossible d'obtenir le moniteur principal !" << std::endl;
        glfwDestroyWindow(this->window);
        glfwTerminate();
        exit(1);
    }

    this->windowParams = glfwGetVideoMode(this->ecran);
    if (!this->windowParams) {
        std::cerr << "ERREUR : Impossible d'obtenir le mode vidéo du moniteur principal !" << std::endl;
        glfwDestroyWindow(this->window);
        glfwTerminate();
        exit(1);
    }


    glfwMakeContextCurrent(this->window);


    if(this->Vsync == true) {
        glfwSwapInterval(1);
    } else if (this->Vsync == false) {
        glfwSwapInterval(0);
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(2);
    }

    glViewport(0, 0, this->width, this->height);

    glEnable(GL_DEPTH_TEST);
}

Window::~Window() {
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

int Window::get_height() {
    return this->height;
}

int Window::get_width() {
    return this->width;
}

const char * Window::get_title() {
    return this->title;
}

unsigned int Window::get_major() {
    return this->major;
}

unsigned int Window::get_minor() {
    return this->minor;
}

GLFWwindow* Window::get_window() {
    return this->window;
}

GLFWmonitor* Window::get_ecran() {
    return this->ecran;
}

int Window::get_maxWidth() {
    return this->windowParams->width;
}

int Window::get_maxHeight() {
    return this->windowParams->height;
}