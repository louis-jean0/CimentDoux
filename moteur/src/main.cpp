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

glm::mat4 view;
glm::mat4 proj;

int main(int argc, char* argv[]) {
    // Initialize window
    Window window(4,1,SCR_WIDTH,SCR_HEIGHT,"Moteur de jeux",true);
    window.setup_GLFW();
    glfwSetKeyCallback(window.get_window(), keyCallback); 

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImFont* font = io.Fonts->AddFontFromFileTTF("../data/fonts/BebasNeue-Regular.ttf", 65.0f);
    ImFont* fontDefault = io.Fonts->AddFontDefault();

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 410");

    glEnable(GL_DEPTH_TEST);

    // Shader
    auto shader = ShaderManager::getShader();

    // Physics engine
    auto pe = std::make_shared<PhysicsEngine>();

    // Player
    player = Player::create(window.get_window());
    pe->add_player(player);

    // Scene
    auto scene = Scene::create();
    scene->add_entities_into_physics_engine(pe);

    float temps_debut=glfwGetTime();

    // Render loop
    while (glfwGetKey(window.get_window(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window.get_window()) == 0) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        lag += deltaTime;
    
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

        pe->update(deltaTime);

        while (lag >= MS_PER_UPDATE) {
            player->update(deltaTime);
            lag -= MS_PER_UPDATE;    
        }

        view = player->get_view_matrix();
        proj = player->get_projection_matrix(); 

        // Sending to shader

        glm::vec3 camPos = player->get_position();
        glm::vec3 camFront = player->getCFront();

        shader->useShader();
        shader->setVPMatrix(view,proj);

        // Phong + Flashlight
        // shader.setBind3f("lightPos", camPos[0], camPos[1], camPos[2]);
        shader->setBind3f("viewPos", camPos[0], camPos[1], camPos[2]);

        // Scene
        scene->draw(view, proj);
        //capsule_node->draw(view, proj);
        //std::cout<<scene->scene_nodes[0]->mesh->bounding_box.min.x<<std::endl;

        ImGui::Begin("Clock", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
        ImGui::PushFont(font);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);

        double currentTime = glfwGetTime();

        int heures = static_cast<int>(currentTime / 3600);
        int minutes = static_cast<int>((currentTime - heures * 3600) / 60);
        int secondes = static_cast<int>(currentTime - heures * 3600 - minutes * 60);

        char TempsFormater[9];
        snprintf(TempsFormater, sizeof(TempsFormater), "%02d:%02d:%02d", heures, minutes, secondes);

        char chaineTemps[16];
        snprintf(chaineTemps, sizeof(chaineTemps), "%.02f", glfwGetTime());

        float longueurTexte = ImGui::CalcTextSize(TempsFormater).x;
        float hauteurTexte = ImGui::CalcTextSize(TempsFormater).y;

        float longueurFenetre = ImGui::GetWindowWidth();
        float hauteurFenetre = ImGui::GetWindowHeight();

        float posX = (longueurFenetre - longueurTexte) * 0.5f;
        float posY = (hauteurFenetre - hauteurTexte) * 1.f;

        ImGui::SetCursorPosX(posX);
        ImGui::SetCursorPosY(posY);

        ImGui::Text("%s", TempsFormater);

        ImGui::PopFont();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        ImGui::End();

        ImGui::Begin("Clocke", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
        ImGui::PushFont(font);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);

        int hauteur = 0;

        char HauteurFormater[5];
        snprintf(HauteurFormater, sizeof(HauteurFormater), "%dM", hauteur);

        ImVec2 textSize = ImGui::CalcTextSize(HauteurFormater);
        float posXX = (ImGui::GetWindowWidth() - textSize.x) * 0.5f;
        float posYY = (ImGui::GetWindowHeight() - textSize.y) * 1.f;

        ImGui::SetCursorPosX(posXX);
        ImGui::SetCursorPosY(posYY);

        ImGui::Text("%s", HauteurFormater);

        ImGui::PopFont();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        ImGui::End();

        //std::cout << deltaTime << std::endl;

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
        if(key == GLFW_KEY_Z && player->get_camera()->mode_cam!=0) {
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