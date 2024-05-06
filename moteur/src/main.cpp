#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <assimp/Importer.hpp>
#include <memory>
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
#include <Player.hpp>
#include <PointLight.hpp>
#include <DirectionalLight.hpp>
#include <ShaderManager.hpp>
#include <Scene.hpp>

// Functions prototypes
void printUsage();
void processInput(GLFWwindow *window);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Window settings
const unsigned int SCR_WIDTH = 1440;
const unsigned int SCR_HEIGHT = 1080;
bool showMouse = true;

// Camera
Camera* myCamera = new Camera();

// Player
std::shared_ptr<Player> player;

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
float g = 9.81;
float hauteur = 3.0f;
float vitesse = 0.5;
glm::vec3 F = glm::vec3(0., 0., 0.);
glm::vec3 a = glm::vec3(0., 0., 0.);
PhysicsEngine pe;

glm::mat4 view;
glm::mat4 proj;

bool appuyer = false;
bool DebAnim = true;
bool Space = false;

// Phong
float cutIn = 25., cutOut = 35.;
float cutOff;
float outerCutOff;
glm::vec3 lightAmbiant = glm::vec3(0.1);
glm::vec3 lightSpecular = glm::vec3(1.);
glm::vec3 lightDiffuse = glm::vec3(0.8);
float constant = 1.;
float linear = 0.09;
float quadratic = 0.032;

int main(int argc, char* argv[]) {
    // Initialize window
    Window window(4,1,SCR_WIDTH,SCR_HEIGHT,"Moteur de jeux",true);
    window.setup_GLFW();
    glfwSetKeyCallback(window.get_window(), keyCallback); 

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 410");

    glEnable(GL_DEPTH_TEST);

    // Shader
    auto shader = ShaderManager::getShader();

    // Player
    player = Player::create(window.get_window());
    pe.add_player(player);

    auto capsule = Model::create("../data/models/capsule/capsule.gltf", shader);
    auto capsule_node = SceneNode::create(capsule);
    capsule_node->set_rotation(glm::vec3(0.0f,0.0f,90.0f));
    capsule_node->set_translation(glm::vec3(0.0f,1.0f,0.0f));
    //capsule_node->enable_physics(true);
    pe.add_entity(capsule_node);

    // Scene
    Scene* scene = new Scene();
    scene->setup_scene();
    scene->add_entities_into_physics_engine(pe);

    float temps_debut=glfwGetTime();

    // Render loop
    while (glfwGetKey(window.get_window(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window.get_window()) == 0) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        lag += deltaTime;

        pe.update(deltaTime);
    
        // Input
        if(showMouse) {
            glfwSetInputMode(window.get_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(window.get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //myCamera->updateInterface(deltaTime);

        while (lag >= MS_PER_UPDATE) {
            player->update(deltaTime);
            
            lag -= MS_PER_UPDATE;    
        }

        view = player->get_view_matrix();
        proj = player->get_projection_matrix(); 

        // Sending to shader

        glm::vec3 camPos = player->get_position();
        glm::vec3 camFront = player->getCFront();
        cutOff = glm::cos(glm::radians(cutIn));
        outerCutOff = glm::cos(glm::radians(cutOut));

        shader->useShader();
        shader->setVPMatrix(view,proj);

        // Phong + Flashlight
        // shader.setBind3f("lightPos", camPos[0], camPos[1], camPos[2]);
        shader->setBind3f("viewPos", camPos[0], camPos[1], camPos[2]);

        // Scene
        scene->draw(view, proj);
        capsule_node->draw(view, proj);
        //std::cout<<scene->scene_nodes[0]->mesh->bounding_box.min.x<<std::endl;


        ImGui::Begin("Paramètres");
        ImGui::Text("Delta time : %f", deltaTime);
        ImGui::SliderFloat("Vitesse du saut", &vitesse, 0.01, 5.);
        ImGui::InputFloat("// Vitesse du saut", &vitesse, 0.1);
        ImGui::SliderFloat("Hauteur du saut", &hauteur, 0., 10.);
        ImGui::InputFloat("// Hauteur du saut", &hauteur, 0.1);
        ImGui::Spacing();
        ImGui::SliderFloat("cutOff", &cutIn, 0., 180.);
        ImGui::SliderFloat("outerCutOff", &cutOut, 0., 180.);
        ImGui::SliderFloat3("Light Ambiant", &lightAmbiant[0], 0., 1.);
        ImGui::SliderFloat3("Light Diffuse", &lightDiffuse[0], 0., 1.);
        ImGui::SliderFloat3("Light Specular", &lightSpecular[0], 0., 1.);
        ImGui::SliderFloat("constant", &constant, 0., 1.);
        ImGui::SliderFloat("linear", &linear, 0., 0.1);
        ImGui::SliderFloat("quadratic", &quadratic, 0., 0.01);

        ImGui::Begin("Paramètres");
        ImGui::Text("Delta time : %f", deltaTime);
        //std::cout << deltaTime << std::endl;
        ImGui::End();

        //std::cout << deltaTime << std::endl;
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
        if(key == GLFW_KEY_T) {
            showMouse = player->get_camera()->getShowMouse();
            showMouse = !showMouse;
            player->get_camera()->setShowMouse(showMouse);
        }
        if (key == GLFW_KEY_H) {
            player->get_camera()->mode_cam=(player->get_camera()->mode_cam+1)%3;
            player->get_camera()->reset();
        }
    }
    //player->handleSingleInput(key, scancode, action, mods);
}


void processInput(GLFWwindow *window) {
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}