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
float vitesse = 0.5;
glm::vec3 F = glm::vec3(0., 0., 0.);
glm::vec3 a = glm::vec3(0., 0., 0.);

glm::mat4 view;
glm::mat4 proj;



// Permet d'aficher ou non (métrage + timer)
bool principal = true;
bool settings = false;
bool ESCAPE;

bool aa = false;


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
    ImFont* fontMenu = io.Fonts->AddFontFromFileTTF("../data/fonts/BebasNeue-Regular.ttf",50.0f);
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


    // Variables menu
    double hauteur = 0.;
    double MaxHeight = 0.;
    int currentRun = 0;

    double timing = 0.1;
    float fov = player->get_camera()->getFOV();
    float sensi = player->get_camera()->get_sensivity();

    // Render loop
    while (glfwWindowShouldClose(window.get_window()) == 0) {
        float currentFrame = glfwGetTime();
        double elapsedTime = currentFrame - timing;
        timing = currentFrame;

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
            if(principal == true) {
                player->update(deltaTime);
            }
            
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

        char TempsFormater[9];
        char TempsFormaterMenu[9];

        if(principal == true) {
            ImGui::SetNextWindowBgAlpha(0.5f);
            ImGui::Begin("Metrage_main", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
            ImGui::PushFont(font);
            //hauteur = std::floor(player->player_node->transform.get_translation().y - 3.0 * 1.54);
            hauteur = (player->player_node->transform.get_translation().y - 3.0) * 1.54;
            if(hauteur >= MaxHeight) {
                MaxHeight = std::max(hauteur, MaxHeight);
            }
            
            
            std::cout << hauteur << std::endl;
            std::cout << MaxHeight << std::endl;


            char HauteurFormater[6];
            if((int)hauteur == 0) {
                snprintf(HauteurFormater, sizeof(HauteurFormater), "0M");
            } else {
                snprintf(HauteurFormater, sizeof(HauteurFormater), "%.0lfM", hauteur);
            }

            ImVec2 textSize = ImGui::CalcTextSize(HauteurFormater);
            float posX_TimerMain = (ImGui::GetWindowWidth() - textSize.x) * 0.5f;
            float posY_TimerMain = (ImGui::GetWindowHeight() - textSize.y) * 0.5f;

            ImGui::SetCursorPosX(posX_TimerMain);
            ImGui::SetCursorPosY(posY_TimerMain);
            ImGui::SetWindowPos(ImVec2(0, 0));        

            ImGui::Text("%s", HauteurFormater);

            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();


            ImGui::SetNextWindowBgAlpha(0.5f);
            ImGui::Begin("Timer_main", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1., 0., 0., 0.5));
            ImGui::PushFont(font);

            int heures = static_cast<int>(elapsedTime / 3600);
            int minutes = static_cast<int>((elapsedTime - heures * 3600) / 60);
            int secondes = static_cast<int>(elapsedTime - heures * 3600 - minutes * 60);

            snprintf(TempsFormater, sizeof(TempsFormater), "%02d:%02d:%02d", heures, minutes, secondes);

            char chaineTemps[16];
            snprintf(chaineTemps, sizeof(chaineTemps), "%.02f", elapsedTime);

            float longueurTexte = ImGui::CalcTextSize(TempsFormater).x;
            float hauteurTexte = ImGui::CalcTextSize(TempsFormater).y;

            float longueurFenetre = ImGui::GetWindowWidth();
            float hauteurFenetre = ImGui::GetWindowHeight();

            float posX_MetrageMain = (longueurFenetre - longueurTexte) * 0.5f;
            float posY_MetrageMain = (hauteurFenetre - hauteurTexte) * 0.5f;

            ImGui::SetCursorPosX(posX_MetrageMain);
            ImGui::SetCursorPosY(posY_MetrageMain);
            ImGui::SetWindowPos(ImVec2(SCR_WIDTH - ImGui::GetWindowWidth(), 0));      

            ImGui::Text("%s", TempsFormater);

            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();        
        }


        // Menu
        ESCAPE = glfwGetKey(window.get_window(), GLFW_KEY_ESCAPE) == GLFW_PRESS;
        if (ESCAPE && settings == false) {
            principal = false;
            showMouse = true;

            // 1er affichage
            ImGui::SetNextWindowBgAlpha(0.7f);
            ImGui::Begin("Current_run", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
            ImGui::PushFont(font);

            char RunFormater[4];
            snprintf(RunFormater, sizeof(RunFormater), "#%d", currentRun);

            ImVec2 textSizerun = ImGui::CalcTextSize(RunFormater);
            float posXrun = (ImGui::GetWindowWidth() - textSizerun.x) * 0.5f;
            float posYrun = (ImGui::GetWindowHeight() - textSizerun.y) * 0.5f;

            ImGui::SetCursorPosX(posXrun);
            ImGui::SetCursorPosY(posYrun);
            ImGui::SetWindowPos(ImVec2(SCR_WIDTH - ImGui::GetWindowWidth() - 6, SCR_HEIGHT / 6));

            ImGui::Text("%s", RunFormater);

            ImGui::PushFont(fontMenu);
            ImGui::SetCursorPosX(5);
            ImGui::SetCursorPosY(2);
            ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.0f, 1.0f), "Current run");
            ImGui::PopFont();

            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();

            // 2ème affichage
            ImGui::SetNextWindowBgAlpha(0.7f);
            ImGui::Begin("Curr_height", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
            ImGui::PushFont(font);

            char CHFormater[6];
            if((int)hauteur == 0) {
                snprintf(CHFormater, sizeof(CHFormater), "0M");
            } else {
                snprintf(CHFormater, sizeof(CHFormater), "%.0lfM", hauteur);
            }

            ImVec2 textSizech = ImGui::CalcTextSize(CHFormater);
            float posXcurrheight = (ImGui::GetWindowWidth() - textSizech.x) * 0.5f;
            float posYcurrheight = (ImGui::GetWindowHeight() - textSizech.y) * 0.5f;

            ImGui::SetCursorPosX(posXcurrheight);
            ImGui::SetCursorPosY(posYcurrheight);
            ImGui::SetWindowPos(ImVec2(SCR_WIDTH - ImGui::GetWindowWidth() - 6, (SCR_HEIGHT / 6) * 2));

            ImGui::Text("%s", CHFormater);

            ImGui::PushFont(fontMenu);
            ImGui::SetCursorPosX(5);
            ImGui::SetCursorPosY(2);
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Current height");
            ImGui::PopFont();

            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();

            // 3ème affichage
            ImGui::SetNextWindowBgAlpha(0.7f);
            ImGui::Begin("Max_height", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
            ImGui::PushFont(font);

            char MXFormater[6];
            if((int)MaxHeight == 0) {
                snprintf(MXFormater, sizeof(MXFormater), "0M");
            } else {
                snprintf(MXFormater, sizeof(MXFormater), "%.0lfM", MaxHeight);
            }

            ImVec2 textSizemh = ImGui::CalcTextSize(MXFormater);
            float posXmaxheight = (ImGui::GetWindowWidth() - textSizemh.x) * 0.5f;
            float posYmaxheight = (ImGui::GetWindowHeight() - textSizemh.y) * 0.5f;

            ImGui::SetCursorPosX(posXmaxheight);
            ImGui::SetCursorPosY(posYmaxheight);
            ImGui::SetWindowPos(ImVec2(SCR_WIDTH - ImGui::GetWindowWidth() - 6, (SCR_HEIGHT / 6) * 3));

            ImGui::Text("%s", MXFormater);

            ImGui::PushFont(fontMenu);
            ImGui::SetCursorPosX(5);
            ImGui::SetCursorPosY(2);
            ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.0f, 1.0f), "Max height");
            ImGui::PopFont();

            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();

            // 4ème affichage
            ImGui::SetNextWindowBgAlpha(0.7f);
            ImGui::Begin("Timer_menu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1., 0., 0., 0.5));
            ImGui::PushFont(font);

            int heures_menu = static_cast<int>(elapsedTime / 3600);
            int minutes_menu = static_cast<int>((elapsedTime - heures_menu * 3600) / 60);
            int secondes_menu = static_cast<int>(elapsedTime - heures_menu * 3600 - minutes_menu * 60);

            snprintf(TempsFormaterMenu, sizeof(TempsFormaterMenu), "%02d:%02d:%02d", heures_menu, minutes_menu, secondes_menu);

            char chaineTempsMenu[16];
            snprintf(chaineTempsMenu, sizeof(chaineTempsMenu), "%.02f", elapsedTime);

            float longueurTexte = ImGui::CalcTextSize(TempsFormaterMenu).x;
            float hauteurTexte = ImGui::CalcTextSize(TempsFormaterMenu).y;

            float longueurFenetre = ImGui::GetWindowWidth();
            float hauteurFenetre = ImGui::GetWindowHeight();

            float posX_MetrageMain = (longueurFenetre - longueurTexte) * 0.5f;
            float posY_MetrageMain = (hauteurFenetre - hauteurTexte) * 0.5f;

            ImGui::SetCursorPosX(posX_MetrageMain);
            ImGui::SetCursorPosY(posY_MetrageMain);
            ImGui::SetWindowPos(ImVec2(SCR_WIDTH - ImGui::GetWindowWidth() - 6, (SCR_HEIGHT / 6) * 4));

            ImGui::Text("%s", TempsFormaterMenu);


            ImGui::PushFont(fontMenu);
            ImGui::SetCursorPosX(5);
            ImGui::SetCursorPosY(2);
            ImGui::TextColored(ImVec4(0.0f, 0.6f, 1.0f, 1.0f), "Current time");
            ImGui::PopFont();

            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End(); 


            // Paramètres
            ImGui::SetNextWindowBgAlpha(1.f);
            ImGui::Begin("Play", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove  | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
            ImGui::PushFont(fontMenu);


            float xPos = (ImGui::GetWindowWidth() - ImGui::CalcTextSize("Play").x) * 0.5f;
            float yPos = (ImGui::GetWindowHeight() - ImGui::CalcTextSize("Play").y) * 0.5f;
            ImGui::SetCursorPosX(xPos);
            ImGui::SetCursorPosY(yPos);
            ImGui::SetWindowPos(ImVec2(5, SCR_HEIGHT / 6));        
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            if (ImGui::Button("Play"))
            {
                ESCAPE = false;
            }
            ImGui::PopStyleColor();


            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();

    
            ImGui::SetNextWindowBgAlpha(1.f);
            ImGui::Begin("Restart run", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove  | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
            ImGui::PushFont(fontMenu);


            float xPos_res = (ImGui::GetWindowWidth() - ImGui::CalcTextSize("Restart run").x) * 0.5f;
            float yPos_res = (ImGui::GetWindowHeight() - ImGui::CalcTextSize("Restart run").y) * 0.5f;
            ImGui::SetCursorPosX(xPos_res);
            ImGui::SetCursorPosY(yPos_res);
            ImGui::SetWindowPos(ImVec2(5, (SCR_HEIGHT / 6) * 1.35));            
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            if (ImGui::Button("Restart run"))
            {
                currentRun += 1;
                player->player_node->transform.set_translation(glm::vec3(0.0f, 80.0f, -10.0f));
                elapsedTime = 0.;
                ESCAPE = false;
            }
            ImGui::PopStyleColor();


            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();


            ImGui::SetNextWindowBgAlpha(1.f);
            ImGui::Begin("Settings", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
            ImGui::PushFont(fontMenu);


            float xPos_set = (ImGui::GetWindowWidth() - ImGui::CalcTextSize("Settings").x) * 0.5f;
            float yPos_set = (ImGui::GetWindowHeight() - ImGui::CalcTextSize("Settings").y) * 0.5f;
            ImGui::SetCursorPosX(xPos_set);
            ImGui::SetCursorPosY(yPos_set);
            ImGui::SetWindowPos(ImVec2(5, (SCR_HEIGHT / 6) * 1.7));        
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            if (ImGui::Button("Settings"))
            {
                settings = true;
            }
            ImGui::PopStyleColor();


            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();


            ImGui::SetNextWindowBgAlpha(1.f);
            ImGui::Begin("Credits", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove  | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
            ImGui::PushFont(fontMenu);


            float xPos_cred = (ImGui::GetWindowWidth() - ImGui::CalcTextSize("Credits").x) * 0.5f;
            float yPos_cred = (ImGui::GetWindowHeight() - ImGui::CalcTextSize("Credits").y) * 0.5f;
            ImGui::SetCursorPosX(xPos_cred);
            ImGui::SetCursorPosY(yPos_cred);
            ImGui::SetWindowPos(ImVec2(5, (SCR_HEIGHT / 6) * 2.05));        
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            if (ImGui::Button("Credits"))
            {
            }
            ImGui::PopStyleColor();


            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();


            ImGui::SetNextWindowBgAlpha(1.f);
            ImGui::Begin("Exit", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove  | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
            ImGui::PushFont(fontMenu);


            float xPos_exit = (ImGui::GetWindowWidth() - ImGui::CalcTextSize("Exit").x) * 0.5f;
            float yPos_exit = (ImGui::GetWindowHeight() - ImGui::CalcTextSize("Exit").y) * 0.5f;
            ImGui::SetCursorPosX(xPos_exit);
            ImGui::SetCursorPosY(yPos_exit);
            ImGui::SetWindowPos(ImVec2(5, (SCR_HEIGHT / 6) * 2.4));        
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            if (ImGui::Button("Exit"))
            {
                glfwTerminate();
                exit(0);
            }
            ImGui::PopStyleColor();


            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();
  
        } else if(ESCAPE && settings == true) {
            
            ImGui::SliderFloat("FOV", &fov, 45.f, 120.f);
            player->get_camera()->setFOV(fov);


            ImGui::SliderFloat("Sensi", &sensi, 0.01f, 10.f);
            player->get_camera()->set_sensitivity(sensi);

        } else {
            principal = true; 
            showMouse = false;          
        }


        if(glfwGetKey(window.get_window(), GLFW_KEY_C) == GLFW_PRESS && !aa) {


            ImGui::Begin("Exit", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove  | ImGuiWindowFlags_NoScrollbar);
            ImGui::End();

            aa = true;
        } 
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
        if(key == GLFW_KEY_A && player->get_camera()->mode_cam==0) {
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