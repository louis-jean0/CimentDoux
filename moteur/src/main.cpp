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

// Wireframe
bool wireframe = false;

// Gameloop
float deltaTime = 0.0f;	
float lastFrame = 0.0f;
float lag = 0.0f;
float MS_PER_UPDATE = 0.001;

// Trans
float PasTranslationCube = 0.01;

// Physique
double v0_Vitesse = 0.01f;
glm::vec3 v0 = glm::vec3(0., v0_Vitesse, 0.);
float m = 100.0f;
float g = 9.81;
float hauteur = 5.0f;
glm::vec3 F = glm::vec3(0., 0., 0.);
glm::vec3 a = glm::vec3(0., 0., 0.);


glm::mat4 view;
glm::mat4 proj;

bool appuyer = false;
bool DebAnim = true;
bool Space = false;

int main(int argc, char* argv[]) {
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

    Plane* plane = new Plane(100, 100, 100,0);
    Texture plane_texture("../data/textures/2k_neptune.jpg");
    plane->add_texture(plane_texture);
    plane->bind_shader(shader);
    SceneNode plane_node(plane);


    Model model("../data/models/capsule/capsule.obj");
    model.bind_shader_to_meshes(shader);
    SceneNode* model_node = new SceneNode(&model);
    model_node->transform.scale = glm::vec3(1.0f);
    model_node->transform.translation = glm::vec3(1.0f);

    Model obst1("../data/models/cube/Cube.gltf");
    obst1.bind_shader_to_meshes(shader);
    SceneNode* obst1_node = new SceneNode(&obst1);
    obst1_node->transform.scale = glm::vec3(1.0f);
    obst1_node->transform.translation = glm::vec3(15., 1.0f, 15.);

    Model obst2("../data/models/cube/Cube.gltf");
    obst2.bind_shader_to_meshes(shader);
    SceneNode* obst2_node = new SceneNode(&obst2);
    obst2_node->transform.scale = glm::vec3(1.0f);
    obst2_node->transform.translation = glm::vec3(16., 2.0f, 18.);

    myCamera.init();


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render loop
    while (glfwGetKey(window.get_window(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window.get_window()) == 0) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        lag += deltaTime;

        glfwSetKeyCallback(window.get_window(), keyCallback);
    
        //input
        if(showMouse) {
            glfwSetInputMode(window.get_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(window.get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        processInput(window.get_window());

        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        myCamera.updateInterface(deltaTime);


        while (lag >= MS_PER_UPDATE) {
            myCamera.update(deltaTime, window.get_window());  
            glm::vec3 forwardVector = glm::normalize(myCamera.getRotation() * glm::vec3(0.0f, 1.0f, 0.0f));

            if(glfwGetKey(window.get_window(), GLFW_KEY_LEFT) == GLFW_PRESS) {
                model_node->transform.translation += glm::vec3(PasTranslationCube, 0., 0.);
            }

            if(glfwGetKey(window.get_window(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
                model_node->transform.translation -=  glm::vec3(PasTranslationCube, 0., 0.);
            }

            if(glfwGetKey(window.get_window(), GLFW_KEY_UP) == GLFW_PRESS) {
                model_node->transform.translation +=  glm::vec3(0., 0., PasTranslationCube);
            }

            if(glfwGetKey(window.get_window(), GLFW_KEY_DOWN) == GLFW_PRESS) {
                model_node->transform.translation -=  glm::vec3(0., 0., PasTranslationCube);
            }

            if (glfwGetKey(window.get_window(), GLFW_KEY_SPACE) == GLFW_PRESS && !Space) {
                v0_Vitesse = sqrt(2.0f * g * hauteur);
                Space = true;
            } else if (glfwGetKey(window.get_window(), GLFW_KEY_SPACE) == GLFW_RELEASE) {
                Space = false;
            }

            v0_Vitesse -= g * deltaTime;
            model_node->transform.translation.y += v0_Vitesse * deltaTime;

            if (model_node->transform.translation.y <= 0.0f) {
                model_node->transform.translation.y = 0.0f;
                v0_Vitesse = 0.0f;
            } 
            
            lag -= MS_PER_UPDATE;    
        }

        view = myCamera.getViewMatrix();
        proj = myCamera.getProjectionMatrix();  

        // Sending to shader
        shader.useShader();
        shader.setBindMatrix4fv("view", 1, 0, glm::value_ptr(view));
        shader.setBindMatrix4fv("projection", 1, 0, glm::value_ptr(proj));

        // Scene
        plane_node.draw();
        model_node->draw();
        obst1_node->draw();
        obst2_node->draw();

        ImGui::Begin("Paramètres");
        ImGui::Text("Delta time : %f", deltaTime);
        std::cout << deltaTime << std::endl;
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


void processInput(GLFWwindow *window) {
    //float movementSpeed = 5.0f * delta_time;
    //if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
      //   cubePosition.z -= movementSpeed;
    //}
    //if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
     //cubePosition.z += movementSpeed;
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