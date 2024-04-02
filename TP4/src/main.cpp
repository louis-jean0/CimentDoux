#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <sys/sysinfo.h>
#include <sstream>
#include <fstream>
#include <unistd.h>

#include <Window.hpp>
#include <SceneNode.hpp>
#include <Texture.hpp>
#include <Plane.hpp>
#include <Sphere.hpp>
#include <PlaneCollider.hpp>

// Functions prototypes
void printUsage();
void processInput(GLFWwindow *window);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

long long GetMemoryUsage() {
    long long memory_usage = 0;
    std::ifstream status_file("/proc/self/status");
    if (status_file.is_open()) {
        std::string line;
        while (std::getline(status_file, line)) {
            if (line.find("VmRSS:") != std::string::npos) {
                std::istringstream iss(line);
                std::string label;
                long long value;
                iss >> label >> value;
                memory_usage = value;
                break;
            }
        }
        status_file.close();
    }
    return memory_usage;
}

// Window settings
const unsigned int SCR_WIDTH = 1440;
const unsigned int SCR_HEIGHT = 1080;

// Camera settings
glm::vec3 sphere_position = glm::vec3(0.0f,1.0f,0.0f);
float sphere_radius = 0.5f;
glm::vec3 camera_pos = sphere_position + glm::vec3(0.0f,5.0f,2.0f);
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
    Window window(4,6,SCR_WIDTH,SCR_HEIGHT,"Moteur de jeux - TP Mouvement",true);
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
    shader.setShader("../shaders/vs.vert","../shaders/fs.frag");

    Plane* plane_plane = new Plane(5,5);
    Mesh* plane_mesh = static_cast<Mesh*>(plane_plane);
    Texture plane_texture("../data/textures/terrain.jpg");
    plane_mesh->add_texture(plane_texture);
    plane_mesh->bind_shader(shader);
    SceneNode* plane = new SceneNode(plane_mesh);
    plane->transform.scale = glm::vec3(10.0f);
    PlaneCollider* plane_collider = new PlaneCollider(*plane_plane);

    Mesh* sphere_mesh = new Sphere(sphere_position,sphere_radius,20,20);
    Texture sphere_texture("../data/textures/2k_neptune.jpg");
    sphere_mesh->add_texture(sphere_texture);
    sphere_mesh->bind_shader(shader);
    Model* backpack = new Model("../data/models/backpack/backpack.obj");
    SceneNode* sphere = new SceneNode(backpack);

    printUsage();

    // Render loop
    while (!glfwWindowShouldClose(window.get_window())) {
        float fps = ImGui::GetIO().Framerate;
        float ms_per_frame = 1000.0f / fps;
        current_frame_time = glfwGetTime();
        delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        // Controls
        processInput(window.get_window());
        glfwSetKeyCallback(window.get_window(), keyCallback);

        current_frame_time = glfwGetTime();
        delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Model is computed directly thanks to the SceneNode

        // View
        glm::mat4 view = glm::lookAt(camera_pos,camera_target,camera_up);

        // Projection
        glm::mat4 projection = glm::perspective(45.0f,(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,1000.0f);

        // Sending to shader
        shader.useShader();
        shader.setBindMatrix4fv("view", 1, 0, glm::value_ptr(view));
        shader.setBindMatrix4fv("projection", 1, 0, glm::value_ptr(projection));

        plane->draw();
        sphere->transform.translation = sphere_position;
        //plane_collider->check_collision_with_sphere(sphere_radius,sphere_position);
        sphere->draw();

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Paramètres");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Métriques de l'ordinateur (Debug) :");
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        char hostname[256];
        gethostname(hostname, sizeof(hostname));
        ImGui::Text("Nom de l'ordinateur: %s", hostname);

        FILE *fp = popen("uname -sr", "r");
        char os[256];
        fgets(os, sizeof(os), fp);
        pclose(fp);
        ImGui::Text("Système d'exploitation: %s", os);

        struct sysinfo si;
        sysinfo(&si);
        double ram_totale = si.totalram * si.mem_unit / (1024 * 1024 * 1024); // Convertir en Go
        ImGui::Text("Mémoire RAM totale : %.2f Go", ram_totale);
        long long ram_utilisee_programme = GetMemoryUsage();
        double ram_utilisee_go = static_cast<double>(ram_utilisee_programme) / (1024 * 1024); // Convertir en Mo
        double pourcentage_ram_utilisee = (ram_utilisee_go / ram_totale) * 100.0; // Calculer le pourcentage
        ImGui::Text("Mémoire RAM utilisée par le programme : %.4f Mo", ram_utilisee_go);
        ImGui::Text("Mémoire RAM utilisée par le programme (en %%) : %.4f%%", pourcentage_ram_utilisee);
        ImGui::Text("FPS : %.2f / MS (Milliseconds Per Frame) : %.2f", fps, ms_per_frame);
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
    float camera_speed = 10.0f * delta_time;
    float movement_speed = 2.5f * delta_time;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) sphere_position.x -= movement_speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) sphere_position.x += movement_speed;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) sphere_position.z -= movement_speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) sphere_position.z += movement_speed;

    // Zoom
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) radius -= 10*camera_speed; // 10 fois pour que ça aille plus vite sans modifier la vitesse de l'orbite
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) radius += 10*camera_speed;

    radius = std::max(1.0f,radius);

    camera_target = sphere_position;
    camera_pos = camera_target + glm::vec3(0.0f,1.0f,2.0f);
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