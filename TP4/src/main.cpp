#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>

#include <Window.hpp>
#include <Mesh.hpp>
#include <objloader.hpp>
#include <Sphere.hpp>
#include <SceneNode.hpp>
#include <Texture.hpp>

// Functions prototypes
void printUsage();
void processInput(GLFWwindow *window);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Window settings
const unsigned int SCR_WIDTH = 2560;
const unsigned int SCR_HEIGHT = 1440;

// Camera settings
glm::vec3 camera_pos = glm::vec3(1.0f,1.0f,5.0f);
glm::vec3 camera_target = glm::vec3(0.0f,0.0f,0.0f);
glm::vec3 camera_up = glm::vec3(0.0f,1.0f,0.0f);
float horizontal_angle = 3.14f; 
float vertical_angle = 0.0f; 
float radius = 10.0f;

// Wireframe
bool wireframe = false;

// Timing
float last_frame_time = glfwGetTime();
float current_frame_time;
float delta_time;

int main(int argc, char* argv[]) {

    current_frame_time = glfwGetTime();
    delta_time = current_frame_time - last_frame_time;
    last_frame_time = current_frame_time;

    // Initialize window
    Window window(4,6,SCR_WIDTH,SCR_HEIGHT,"Moteur de jeux - TP Graphe de scène",true);
    window.setup_GLFW();

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    glEnable(GL_DEPTH_TEST);

    Shader shader;

    printUsage();

    // Render loop
    while (!glfwWindowShouldClose(window.get_window())) {

        // Controls
        processInput(window.get_window());
        glfwSetKeyCallback(window.get_window(), keyCallback);

        current_frame_time = glfwGetTime();
        delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Model is computed directly thanks to the SceneNode

        // View
        camera_pos.x = camera_target.x + radius * cos(vertical_angle) * sin(horizontal_angle);
        camera_pos.y = camera_target.y + radius * sin(vertical_angle);
        camera_pos.z = camera_target.z + radius * cos(vertical_angle) * cos(horizontal_angle);
        glm::mat4 view = glm::lookAt(camera_pos,camera_target,camera_up);

        // Projection
        glm::mat4 projection = glm::perspective(45.0f,(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,1000.0f);

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Paramètres");
        ImGui::End();

        // Render window & ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 
        glfwSwapBuffers(window.get_window());
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    window.~Window();

    return 0;
}

void processInput(GLFWwindow *window) {
    // Sensibilité de la caméra
    float camera_speed = 3.0f * delta_time;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) horizontal_angle -= camera_speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) horizontal_angle += camera_speed;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) vertical_angle += camera_speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) vertical_angle -= camera_speed;

    // Zoom
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) radius -= 10*camera_speed; // 10 fois pour que ça aille plus vite sans modifier la vitesse de l'orbite
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) radius += 10*camera_speed;

    vertical_angle = std::max(-1.57f, std::min(1.57f, vertical_angle));
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS) {
        if(key == GLFW_KEY_Z) {
            wireframe = !wireframe;
            if(wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
    }
}

void printUsage() {
    std::cout<<std::endl;
    std::cout << "Commandes disponibles :" << std::endl;
    std::cout << "  - ESC : Quitter le programme." << std::endl;
    std::cout << "  - W : Activer/Désactiver le mode wireframe." << std::endl;
    std::cout << "  - Z/Q/S/D : Contrôler la caméra orbitale" << std::endl;
    std::cout << "  - Flèches haut/bas : Zoomer/dézoomer" << std::endl;
    std::cout << "  - Voir panel ImGui pour d'autres paramètres" << std::endl;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}