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
#include <ModelCollider.hpp>

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

// Wireframe
bool wireframe = false;

// Timing
float last_frame_time = glfwGetTime();
float current_frame_time;
float delta_time;

SceneNode* model_node2;

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

    Model* model1 = new Model("../data/models/capsule/capsule.obj");
    model1->bind_shader_to_meshes(shader);
    SceneNode* model_node1 = new SceneNode(model1);
    ModelCollider collider1(model1->bounding_box);

    Model* model2 = new Model("../data/models/boombox/BoomBox.gltf");
    model2->bind_shader_to_meshes(shader);
    model_node2 = new SceneNode(model2);
    ModelCollider collider2(model2->bounding_box);

    myCamera.init();

    model_node2->transform.set_scale(glm::vec3(100.0f));

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

        myCamera.update(delta_time, window.get_window());

        // Model is computed directly thanks to the SceneNode

        // View
        glm::mat4 view = myCamera.getViewMatrix();

        // Projection
        glm::mat4 projection = myCamera.getProjectionMatrix();

        std::cout<<collider1.checkCollision(collider2)<<std::endl;
        
        std::cout<<model2->bounding_box.min.x<<std::endl;

        // Scene
        plane_node.draw(view, projection);
        //model_node1->transform.adjust_translation(glm::vec3(0.003f,0.0f,0.0f));
        //model_node2->transform.adjust_translation(glm::vec3(0.001f,0.0f,0.0f));
        model_node1->draw(view, projection);
        model_node2->draw(view, projection);

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
    }
}

void printUsage() {
}

void processInput(GLFWwindow *window) {
    float movementSpeed = 5.0f * delta_time;
    if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        if(model_node2 == nullptr) return;
        model_node2->transform.adjust_translation(glm::vec3(0.0f,0.0f,movementSpeed));
    }
    if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        model_node2->transform.adjust_translation(glm::vec3(0.0f,0.0f,-movementSpeed));
    }
    if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        model_node2->transform.adjust_translation(glm::vec3(movementSpeed,0.0f,0.0f));
    }
    if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        model_node2->transform.adjust_translation(glm::vec3(-movementSpeed,0.0f,0.0f));
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}