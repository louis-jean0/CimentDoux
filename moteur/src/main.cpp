#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
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
#include <Camera.hpp>
#include <Cube.hpp>

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
bool showMouse = true;

// Camera
Camera myCamera;

// Cube
Cube* cube;
float cubeSize = 3;
glm::vec3 cubePosition = glm::vec3(0.0f,0.0f,0.0f);
float launchSpeed = 100.0f;

// Plane collider
float friction_coefficient = 0.5f;
float restitution_coefficient = 0.1f;

// Sphere
float sphereRadius = 3;
glm::vec3 spherePosition = glm::vec3(20.0f,0.0f,20.0f);

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

    Plane* plane = new Plane(1000,1000,10000,0);
    Texture plane_texture("../data/textures/2k_neptune.jpg");
    plane->add_texture(plane_texture);
    plane->bind_shader(shader);
    SceneNode plane_node(plane);
    PlaneCollider* plane_collider = new PlaneCollider(*plane);

    Sphere* sphere = new Sphere(spherePosition,sphereRadius,20,20);
    sphere->add_texture(nullptr);
    sphere->bind_shader(shader);
    SceneNode* sphere_node = new SceneNode(sphere);
    sphere->setCenter(glm::vec3(20.0f,50.0f,20.0f));
    sphere_node->transform.translation = sphere->getCenter();

    cube = new Cube(cubePosition,cubeSize);
    cube->add_texture(nullptr);
    cube->bind_shader(shader);
    SceneNode* cube_node = new SceneNode(cube);
    cube->setCenter(glm::vec3(10.0f,50.0f,10.0f));
    cube_node->transform.translation = cube->getCenter();

    Model model("../data/models/backpack/backpack.obj");
    model.bind_shader_to_meshes(shader);
    SceneNode* model_node = new SceneNode(&model);

    Shader aabbShader;
    aabbShader.setShader("../shaders/aabb.vert","../shaders/aabb.frag");

    myCamera.init();

    //model_node->transform.scale = glm::vec3(100.0f);

    printUsage();

    // Render loop
    while (!glfwWindowShouldClose(window.get_window()) && glfwGetKey(window.get_window(), GLFW_KEY_ESCAPE) != GLFW_PRESS) {
        float fps = ImGui::GetIO().Framerate;
        float ms_per_frame = 1000.0f / fps;
        current_frame_time = glfwGetTime();
        delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        glfwSetKeyCallback(window.get_window(), keyCallback);
    
        //input
        if(showMouse) {
            glfwSetInputMode(window.get_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(window.get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED); // To be in "full screen", way easier to move with mouse
        }

        processInput(window.get_window());

        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Metrics");
        ImGui::Separator();
        ImGui::Spacing();

        char hostname[256];
        gethostname(hostname, sizeof(hostname));
        ImGui::Text("Computer name: %s", hostname);

        FILE *fp = popen("uname -sr", "r");
        char os[256];
        fgets(os, sizeof(os), fp);
        pclose(fp);
        ImGui::Text("OS : %s", os);

        struct sysinfo si;
        sysinfo(&si);
        double ram_totale = si.totalram * si.mem_unit / (1024 * 1024 * 1024); // Convertir en Go
        ImGui::Text("Total RAM : %.2f Go", ram_totale);
        long long ram_utilisee_programme = GetMemoryUsage();
        double ram_utilisee_go = static_cast<double>(ram_utilisee_programme) / (1024 * 1024); // Convertir en Mo
        double pourcentage_ram_utilisee = (ram_utilisee_go / ram_totale); // Calculer le pourcentage
        ImGui::Text("RAM used by the program : %.4f Mo / %.4f %%", ram_utilisee_go, pourcentage_ram_utilisee);
        ImGui::Text("FPS : %.2f / ms per frame : %.2f", fps, ms_per_frame);
        ImGui::End();

        ImGui::Begin("Cube");
        ImGui::SliderFloat("Launch speed", &launchSpeed, 1.0f,500.0f);
        if(ImGui::Button("Launch cube")) {
            cube->launchCube(myCamera.getPosition(), myCamera.getCFront(), launchSpeed);
        }
        ImGui::End();

        ImGui::Begin("Plane");
        ImGui::SliderFloat("Friction coefficient", &friction_coefficient, 0.0f, 1.0f);
        ImGui::SliderFloat("Restitution coefficient", &restitution_coefficient, 0.0f, 1.0f);
        // Not good to put here but ok for this TP
        plane_collider->setFrictionCoefficient(friction_coefficient);
        plane_collider->setRestitutionCoefficient(restitution_coefficient);
        ImGui::End();

        ImGui::Begin("Sphere");
        if(ImGui::Button("Translation")) {

        }
        ImGui::End();

        myCamera.update(delta_time, window.get_window());

        // Model is computed directly thanks to the SceneNode

        // View
        glm::mat4 view = myCamera.getViewMatrix();

        // Projection
        glm::mat4 projection = myCamera.getProjectionMatrix();

        // Sending to shader
        shader.useShader();
        shader.setBindMatrix4fv("view", 1, 0, glm::value_ptr(view));
        shader.setBindMatrix4fv("projection", 1, 0, glm::value_ptr(projection));

        // Scene
        plane_node.draw();

        // Cube
        cube->update(delta_time);
        cube_node->transform.translation = cube->getCenter();
        plane_collider->check_collision_with_cube(*cube);
        cube_node->draw();

        // Sphere
        sphere->update(delta_time);
        plane_collider->check_collision_with_sphere(*sphere);
        sphere_node->transform.translation = sphere->getCenter();
        sphere_node->draw();

        model_node->draw();

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
        if (key == GLFW_KEY_T) {
            showMouse = myCamera.getShowMouse();
            showMouse = !showMouse;
            myCamera.setShowMouse(showMouse);
        }
        if(key == GLFW_KEY_SPACE) {
            cube->launchCube(myCamera.getPosition(), myCamera.getCFront(), launchSpeed);
        }
    }
}

void printUsage() {
}

void processInput(GLFWwindow *window) {
    // float movementSpeed = 5.0f * delta_time;
    // if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
    //     cubePosition.z -= movementSpeed;
    // }
    // if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
    //     cubePosition.z += movementSpeed;
    // }
    // if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
    //     cubePosition.x -= movementSpeed;
    // }
    // if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
    //     cubePosition.x += movementSpeed;
    // }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}