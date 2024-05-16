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
#include <Camera.hpp>
#include <ModelCollider.hpp>
#include <Player.hpp>
#include <PointLight.hpp>
#include <DirectionalLight.hpp>
#include <ShaderManager.hpp>
#include <Scene.hpp>

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

// Functions prototypes
void printUsage();
void processInput(GLFWwindow *window);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void APIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam); 

// Window settings
unsigned int SCR_WIDTH = 1920;
unsigned int SCR_HEIGHT = 1080;
bool showMouse = true;

// Player
std::shared_ptr<Player> player;
std::shared_ptr<Scene> scene;

// Wireframe
bool wireframe = false;

// Gameloop
float deltaTime = 0.0f;	
float lastFrame = 0.0f;
float lag = 0.0f;
const float MS_PER_UPDATE = 0.008333f;

glm::mat4 view;
glm::mat4 proj;

// Permet d'aficher ou non (métrage + timer)
bool principal = true;
bool settings = false;
bool credits = false;
bool ESCAPE;

bool toucheCPresseePrecedemment = false;
bool afficherMenu = false;

bool AZERTY = true;
bool QWERTY = false;

bool Fullscreen = true;
bool fin = false;
bool facticeEnd = false;

glm::vec3 globalPos = glm::vec3(0.);

ma_result result, result2, result3;
ma_engine engine, engine2, engine3;
ma_sound sound, sound2, sound3;

void display_return_ESCAPE(ImFont* fontMenu) {
    ImGui::SetNextWindowBgAlpha(1.f);
    ImGui::Begin("Return", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove  | ImGuiWindowFlags_NoScrollbar);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
    ImGui::PushFont(fontMenu);

    float xPos_ret = (ImGui::GetWindowWidth() - ImGui::CalcTextSize("Return").x) * 0.5f;
    float yPos_ret = (ImGui::GetWindowHeight() - ImGui::CalcTextSize("Return").y) * 0.5f;
    ImGui::SetCursorPosX(xPos_ret);
    ImGui::SetCursorPosY(yPos_ret);
    ImGui::SetWindowPos(ImVec2(0, SCR_HEIGHT - ImGui::GetWindowHeight()));        
    ImGui::SetWindowSize(ImVec2(205, 55));  
            
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    if (ImGui::Button("Return"))
    {
        ESCAPE = false;
    }
    ImGui::PopStyleColor();

    ImGui::PopFont();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    ImGui::End();
}

void display_return_SETTINGS(ImFont* fontMenu) {
    ImGui::SetNextWindowBgAlpha(1.f);
    ImGui::Begin("Return", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove  | ImGuiWindowFlags_NoScrollbar);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
    ImGui::PushFont(fontMenu);

    float xPos_ret = (ImGui::GetWindowWidth() - ImGui::CalcTextSize("Return").x) * 0.5f;
    float yPos_ret = (ImGui::GetWindowHeight() - ImGui::CalcTextSize("Return").y) * 0.5f;
    ImGui::SetCursorPosX(xPos_ret);
    ImGui::SetCursorPosY(yPos_ret);
    ImGui::SetWindowPos(ImVec2(0, SCR_HEIGHT - ImGui::GetWindowHeight()));        
    ImGui::SetWindowSize(ImVec2(205, 55));  
            
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    if (ImGui::Button("Return"))
    {
        settings = false;
    }
    ImGui::PopStyleColor();

    ImGui::PopFont();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    ImGui::End();
}

void display_return_CREDITS(ImFont* fontMenu) {
    ImGui::SetNextWindowBgAlpha(1.f);
    ImGui::Begin("Return", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove  | ImGuiWindowFlags_NoScrollbar);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
    ImGui::PushFont(fontMenu);

    float xPos_ret = (ImGui::GetWindowWidth() - ImGui::CalcTextSize("Return").x) * 0.5f;
    float yPos_ret = (ImGui::GetWindowHeight() - ImGui::CalcTextSize("Return").y) * 0.5f;
    ImGui::SetCursorPosX(xPos_ret);
    ImGui::SetCursorPosY(yPos_ret);
    ImGui::SetWindowPos(ImVec2(0, SCR_HEIGHT - ImGui::GetWindowHeight()));        
    ImGui::SetWindowSize(ImVec2(205, 55));  
            
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    if (ImGui::Button("Return"))
    {
        credits = false;
    }
    ImGui::PopStyleColor();

    ImGui::PopFont();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    ImGui::End();
}

int main(int argc, char* argv[]) {
    // Initialize window
    Window window(4,1,SCR_WIDTH,SCR_HEIGHT,"Ciment doux",true);
    window.setup_GLFW();
    glfwSetKeyCallback(window.get_window(), keyCallback); 
    glfwSetMouseButtonCallback(window.get_window(), mouse_button_callback);

    // // OpenGL debug
    // glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    // glDebugMessageCallback(openglCallbackFunction, nullptr);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImFont* font = io.Fonts->AddFontFromFileTTF("../data/fonts/BebasNeue-Regular.ttf", 65.0f);
    ImFont* fontMenu = io.Fonts->AddFontFromFileTTF("../data/fonts/BebasNeue-Regular.ttf", 50.0f);
    ImFont* fontMenu2 = io.Fonts->AddFontFromFileTTF("../data/fonts/BebasNeue-Regular.ttf", 40.0f);
    ImFont* fontDefault = io.Fonts->AddFontDefault();
    ImFont* fontEndt = io.Fonts->AddFontFromFileTTF("../data/fonts/BebasNeue-Regular.ttf", 150.0f);


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
    scene = Scene::create(player);
    scene->add_entities_into_physics_engine(pe);

    // Moving cube
    auto texture = Texture::create("../data/textures/pierre_rough.jpg");
    auto obst2 = Model::create("../data/models/cube/Cube.gltf", shader);
    auto obst2_node = SceneNode::create(obst2);
    obst2_node->transform.set_scale(glm::vec3(2.0f,0.5f,2.f));
    obst2_node->transform.set_translation(glm::vec3(-20.f,27.f,8.6f));
    obst2_node->rigid_body->is_in_motion=true;
    pe->add_entity(obst2_node);

    // Variables menu
    double hauteur = 0.;
    double MaxHeight = 0.;
    int currentRun = 0;

    double timing = 0.0, acc;
    float fov = player->get_camera()->getFOV();
    float sensi = player->get_camera()->get_sensivity();

    float volume = 0.3;
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS){
        std::cout << "Failed to initialized miniaudio engine\n";
        ma_engine_uninit(&engine);
        return -1;
    }
    result = ma_sound_init_from_file(&engine, "../data/sounds/BetonBrutal3.mp3", 0, NULL, NULL, &sound);
    if (result != MA_SUCCESS){
        std::cout << "Impossible de charger le son\n";
        ma_engine_uninit(&engine);
        return -1;
    }

    result2 = ma_engine_init(NULL, &engine2);
    if (result2 != MA_SUCCESS){
        std::cout << "Failed to initialized miniaudio engine\n";
        ma_engine_uninit(&engine);
        return -1;
    }
    result2 = ma_sound_init_from_file(&engine2, "../data/sounds/ciment doux.wav", 0, NULL, NULL, &sound2);
    if (result2 != MA_SUCCESS){
        std::cout << "Impossible de charger le son\n";
        ma_engine_uninit(&engine2);
        return -1;
    }

    result3 = ma_engine_init(NULL, &engine3);
    if (result3 != MA_SUCCESS){
        std::cout << "Failed to initialized miniaudio engine\n";
        ma_engine_uninit(&engine3);
        return -1;
    }
    result3 = ma_sound_init_from_file(&engine3, "../data/sounds/ciment doux.wav", 0, NULL, NULL, &sound3);
    if (result3 != MA_SUCCESS){
        std::cout << "Impossible de charger le son\n";
        ma_engine_uninit(&engine3);
        return -1;
    }

    float temps_debut = glfwGetTime();
    std::cout << deltaTime << std::endl;

    // Render loop
    while (glfwWindowShouldClose(window.get_window()) == 0) {
        float currentFrame = glfwGetTime();

        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        lag += deltaTime;

        if(ESCAPE == false && fin == false) {
            acc += deltaTime;

            if (acc >= 1.) {
                timing += 1.0;
                acc = 0.;
            }            
        } 

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ma_engine_set_volume(&engine, volume);
        ma_engine_set_volume(&engine2, volume);
        ma_engine_set_volume(&engine3, volume);

        // Input
        if(showMouse) {
            glfwSetInputMode(window.get_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(window.get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        if(Fullscreen == false) {
            SCR_WIDTH = 1920;
            SCR_HEIGHT = 1080;
            glfwSetWindowMonitor(window.get_window(), NULL, SCR_WIDTH/4, 0, SCR_WIDTH, SCR_HEIGHT, window.windowParams->refreshRate);
        } else {
            SCR_WIDTH = window.get_maxWidth();
            SCR_HEIGHT = window.get_maxHeight();
            glfwSetWindowMonitor(window.get_window(), window.get_ecran(), 0, 0, window.get_maxWidth(), window.get_maxHeight(), window.windowParams->refreshRate);        
        }

        view = player->get_view_matrix();
        proj = player->get_projection_matrix();
        scene->draw(view, proj, SCR_WIDTH, SCR_HEIGHT);
        
        globalPos = player->get_camera()->getPosition();
        fin = globalPos.y >= 83. && globalPos.x <= -27.;

        while (lag >= MS_PER_UPDATE) {
            if(principal == true) {
                player->update(MS_PER_UPDATE);
                scene->updateAABB();
                pe->update(MS_PER_UPDATE);
            }
            lag -= MS_PER_UPDATE;    

            // Moving cube
            obst2_node->transform.adjust_translation(glm::vec3(-sin(temps_debut-currentFrame)*10*MS_PER_UPDATE,0.f,0.f));
            obst2_node->updateAABB();

            if(obst2_node->rigid_body->is_child){
                glm::vec3 acc=obst2_node->transform.get_translation()-player->player_node->transform.get_translation();
                player->player_node->transform.adjust_translation(glm::vec3(-sin(temps_debut-currentFrame)*10*MS_PER_UPDATE,0.f,0.f));
            }

        }



        view = player->get_view_matrix();
        proj = player->get_projection_matrix();


        // Draw moving cube
        obst2_node->draw(view,proj);    

        char TempsFormater[9];
        char TempsFormaterMenu[9];

        if(glfwGetKey(window.get_window(), GLFW_KEY_R) == GLFW_PRESS) {
            player->player_node->transform.set_translation(glm::vec3(-21.0f, 4.0f, 23.4f));
            player->get_camera()->setRotationDegrees(glm::vec3(0., 90., 0.));
        }

        if(principal == true) {
            showMouse = false;  

            ImGui::SetNextWindowBgAlpha(0.5f);
            ImGui::Begin("Metrage_main", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
            ImGui::PushFont(font);
            hauteur = (player->player_node->transform.get_translation().y - 3.0) * 0.6;
            if(hauteur >= MaxHeight) {
                MaxHeight = std::max(hauteur, MaxHeight);
            }

            char HauteurFormater[6];
            if((int)hauteur == 0) {
                snprintf(HauteurFormater, sizeof(HauteurFormater), "0M");
            } else {
                snprintf(HauteurFormater, sizeof(HauteurFormater), "%.0lfM", hauteur);
            }

            ImVec2 textSize = ImGui::CalcTextSize(HauteurFormater);
            float posX_MetrageMain = (ImGui::GetWindowWidth() - textSize.x) * 0.5f;
            float posY_MetrageMain = (ImGui::GetWindowHeight() - textSize.y) * 0.5f;

            ImGui::SetCursorPosX(posX_MetrageMain);
            ImGui::SetCursorPosY(posY_MetrageMain);
            ImGui::SetWindowPos(ImVec2(0, 0));        
            ImGui::SetWindowSize(ImVec2(200, 60));  

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

            int heures = static_cast<int>(timing / 3600);
            int minutes = static_cast<int>((timing - heures * 3600) / 60);
            int secondes = static_cast<int>(timing - heures * 3600 - minutes * 60);

            snprintf(TempsFormater, sizeof(TempsFormater), "%02d:%02d:%02d", heures, minutes, secondes);

            char chaineTemps[16];
            snprintf(chaineTemps, sizeof(chaineTemps), "%.02f", timing);

            float longueurTexte = ImGui::CalcTextSize(TempsFormater).x;
            float hauteurTexte = ImGui::CalcTextSize(TempsFormater).y;

            float longueurFenetre = ImGui::GetWindowWidth();
            float hauteurFenetre = ImGui::GetWindowHeight();

            float posX_TimerMain = (longueurFenetre - longueurTexte) * 0.5f;
            float posY_TimerMain = (hauteurFenetre - hauteurTexte) * 0.5f;

            ImGui::SetCursorPosX(posX_TimerMain);
            ImGui::SetCursorPosY(posY_TimerMain);
            ImGui::SetWindowPos(ImVec2(SCR_WIDTH - ImGui::GetWindowWidth(), 0));      
            ImGui::SetWindowSize(ImVec2(200, 60));  

            ImGui::Text("%s", TempsFormater);

            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::End();        
        } else {
            showMouse = true;
        }

        // Menu

        bool toucheEscapePressee = glfwGetKey(window.get_window(), GLFW_KEY_ESCAPE) == GLFW_PRESS;
        if (toucheEscapePressee && !toucheCPresseePrecedemment) {
            ESCAPE = !ESCAPE;
        }
        toucheCPresseePrecedemment = toucheEscapePressee;

        if (ESCAPE && settings == false && credits == false) {

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
            ImGui::SetWindowSize(ImVec2(1000, 175));  

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
            ImGui::SetWindowSize(ImVec2(1000, 175));  

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
            ImGui::SetWindowSize(ImVec2(1000, 175));  

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

            int heures_menu = static_cast<int>(timing / 3600);
            int minutes_menu = static_cast<int>((timing - heures_menu * 3600) / 60);
            int secondes_menu = static_cast<int>(timing - heures_menu * 3600 - minutes_menu * 60);

            snprintf(TempsFormaterMenu, sizeof(TempsFormaterMenu), "%02d:%02d:%02d", heures_menu, minutes_menu, secondes_menu);

            char chaineTempsMenu[16];
            snprintf(chaineTempsMenu, sizeof(chaineTempsMenu), "%.02f", timing);

            float longueurTexte = ImGui::CalcTextSize(TempsFormaterMenu).x;
            float hauteurTexte = ImGui::CalcTextSize(TempsFormaterMenu).y;

            float longueurFenetre = ImGui::GetWindowWidth();
            float hauteurFenetre = ImGui::GetWindowHeight();

            float posX_MetrageMain = (longueurFenetre - longueurTexte) * 0.5f;
            float posY_MetrageMain = (hauteurFenetre - hauteurTexte) * 0.5f;

            ImGui::SetCursorPosX(posX_MetrageMain);
            ImGui::SetCursorPosY(posY_MetrageMain);
            ImGui::SetWindowPos(ImVec2(SCR_WIDTH - ImGui::GetWindowWidth() - 6, (SCR_HEIGHT / 6) * 4));
            ImGui::SetWindowSize(ImVec2(1000, 175));  

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
            ImGui::SetWindowSize(ImVec2(205, 55));  

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            if (ImGui::Button("Play"))
            {
                ESCAPE = false;
                player->get_camera()->setPosition(player->get_camera()->getPosition());
                player->get_camera()->setRotationDegrees(player->get_camera()->getRotationDegrees());
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
            ImGui::SetWindowSize(ImVec2(205, 55));  

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            if (ImGui::Button("Restart run"))
            {
                currentRun += 1;
                MaxHeight = std::min(0., hauteur);
                player->player_node->transform.set_translation(glm::vec3(-20.0f, 95.0f, -17.0f));
                timing = 0.;
                acc = 0.0;
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
            ImGui::SetWindowSize(ImVec2(205, 55));  
            
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
            ImGui::SetWindowSize(ImVec2(205, 55));  
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            if (ImGui::Button("Credits"))
            {
                credits = true;
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
            ImGui::SetWindowSize(ImVec2(205, 55));  
            
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

            display_return_ESCAPE(fontMenu);
        } else if(ESCAPE == true && settings == true) {
            if(Fullscreen == true) {
                ImGui::SetNextWindowBgAlpha(0.7f);
                ImGui::Begin("Slides", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
                ImGui::SetWindowPos(ImVec2(SCR_WIDTH / 12, SCR_HEIGHT / 10));
                ImGui::SetWindowSize(ImVec2(SCR_WIDTH / 2 + SCR_WIDTH / 3, SCR_HEIGHT / 1.25));  

                float contentWidth = ImGui::GetContentRegionAvail().x;
                float contentHeight = ImGui::GetContentRegionAvail().y;

                float textHeight = ImGui::GetTextLineHeightWithSpacing(); 
                float posY = (contentHeight - textHeight * 12) * 0.5f;

                float sliderWidth = 200.0f;
                float textWidthFOV = ImGui::CalcTextSize("FOV").x;
                float textWidthSensi = ImGui::CalcTextSize("Mouse sensitivity").x;
                float textWidthVolume = ImGui::CalcTextSize("Sound volume").x;
                float textWidthCom = ImGui::CalcTextSize("Commands AZERTY / QWERTY :").x;
                float textWidthFullscreen = ImGui::CalcTextSize("Fullscreen :").x;

                float textPosXFOV = (contentWidth - textWidthFOV) * 0.53f;
                float textPosXSensi = (contentWidth - textWidthSensi) * 0.53f;
                float textPosXVolume = (contentWidth - textWidthVolume) * 0.53f;
                float textPosXCom = (contentWidth - textWidthCom) * 0.53f;
                float textPosXFullscreen = (contentWidth - textWidthFullscreen) * 0.53f;

                float sliderPosX = (contentWidth - sliderWidth) * 0.5f;

                ImGui::SetCursorPosX(textPosXFOV);
                ImGui::SetCursorPosY(posY);
                ImGui::Text("FOV");

                float imguiWindowSize = ImGui::GetWindowSize().x;
                float sliderPosOffsetFOV = imguiWindowSize / 4;
                float sliderPosXFOV = (contentWidth - sliderWidth) * 0.5f - sliderPosOffsetFOV;
                ImGui::SetCursorPosX(sliderPosXFOV);
                ImGui::SliderFloat("##FOV", &fov, 45.f, 120.f);
                player->get_camera()->setFOV_set(fov);
                player->get_camera()->setFOV(fov);

                ImGui::SetCursorPosX(textPosXSensi);
                ImGui::SetCursorPosY(posY + textHeight + 100);
                ImGui::Text("Mouse sensitivity");

                float imguiWindowSizee = ImGui::GetWindowSize().x;
                float sliderPosOffsetSensi = imguiWindowSizee / 4;
                float sliderPosXSensi = (contentWidth - sliderWidth) * 0.5f - sliderPosOffsetSensi;
                ImGui::SetCursorPosX(sliderPosXSensi);;
                ImGui::SliderFloat("##Sensibilite", &sensi, 0.01f, 10.f);
                player->get_camera()->set_sensitivity(sensi);

                ImGui::SetCursorPosX(textPosXVolume);
                ImGui::SetCursorPosY(posY + textHeight + 270);
                ImGui::Text("Sound volume");

                float imguiWindowSizeee = ImGui::GetWindowSize().x;
                float sliderPosOffsetVolume = imguiWindowSizeee / 4;
                float sliderPosXVolume = (contentWidth - sliderWidth) * 0.5f - sliderPosOffsetVolume;
                ImGui::SetCursorPosX(sliderPosXVolume);;
                ImGui::SliderFloat("##Volume", &volume, 0.0f, 1.f);

                ImGui::SetCursorPosX(textPosXCom);
                ImGui::SetCursorPosY(posY + textHeight + 450);
                ImGui::Text("Commands AZERTY / QWERTY :");

                float imguiWindowSizeeee = ImGui::GetWindowSize().x;
                float sliderPosOffsetCom = imguiWindowSizeeee / 4;
                float sliderPosXCom = (contentWidth - sliderWidth) * 0.5f - sliderPosOffsetCom;
                float buttonWidth = 25;
                float totalButtonWidth = 2 * buttonWidth;
                float spaceBetweenButtons = imguiWindowSizeeee - totalButtonWidth;
                float offsetX = (SCR_WIDTH / 2.85); 
                ImGui::SetCursorPosX(offsetX);
                if(ImGui::Checkbox("AZERTY", &AZERTY)) {
                    player->get_camera()->mode_cam = 1;
                    QWERTY = false;
                }
                ImGui::SameLine();
                if(ImGui::Checkbox("QWERTY", &QWERTY)) {
                    player->get_camera()->mode_cam = 0;
                    AZERTY = false;
                }

                ImGui::SetCursorPosX(textPosXFullscreen);
                ImGui::SetCursorPosY(posY + textHeight + 625);
                ImGui::Text("Fullscreen :");

                float checkboxWidth = ImGui::CalcTextSize("").x;
                float offsetXFSC = (SCR_WIDTH - checkboxWidth) / 2.4;
                ImGui::SetCursorPosX(offsetXFSC);
                ImGui::Checkbox("##", &Fullscreen);
                ImGui::End();

                display_return_SETTINGS(fontMenu);
            } else {
                ImGui::SetNextWindowBgAlpha(0.7f);
                ImGui::Begin("Slides", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
                ImGui::SetWindowPos(ImVec2(SCR_WIDTH / 12, SCR_HEIGHT / 10));
                ImGui::SetWindowSize(ImVec2(SCR_WIDTH / 2 + SCR_WIDTH / 3, SCR_HEIGHT / 1.25));  

                float contentWidth = ImGui::GetContentRegionAvail().x;
                float contentHeight = ImGui::GetContentRegionAvail().y;

                float textHeight = ImGui::GetTextLineHeightWithSpacing(); 
                float posY = (contentHeight - textHeight * 12) * 0.5f;

                float sliderWidth = 200.0f;
                float textWidthFOV = ImGui::CalcTextSize("FOV").x;
                float textWidthSensi = ImGui::CalcTextSize("Mouse sensitivity").x;
                float textWidthVolume = ImGui::CalcTextSize("Sound volume").x;
                float textWidthCom = ImGui::CalcTextSize("Commands AZERTY / QWERTY :").x;
                float textWidthFullscreen = ImGui::CalcTextSize("Fullscreen :").x;

                float textPosXFOV = (contentWidth - textWidthFOV) * 0.5f;
                float textPosXSensi = (contentWidth - textWidthSensi) * 0.5f;
                float textPosXVolume = (contentWidth - textWidthVolume) * 0.5f;
                float textPosXCom = (contentWidth - textWidthCom) * 0.5f;
                float textPosXFullscreen = (contentWidth - textWidthFullscreen) * 0.5f;

                float sliderPosX = (contentWidth - sliderWidth) * 0.5f;

                ImGui::SetCursorPosX(textPosXFOV);
                ImGui::SetCursorPosY(posY);
                ImGui::Text("FOV");

                float imguiWindowSize = ImGui::GetWindowSize().x;
                float sliderPosOffsetFOV = imguiWindowSize / 4;
                float sliderPosXFOV = (contentWidth - sliderWidth) * 0.5f - sliderPosOffsetFOV;
                ImGui::SetCursorPosX(sliderPosXFOV);
                ImGui::SliderFloat("##FOV", &fov, 45.f, 120.f);
                player->get_camera()->setFOV(fov);

                ImGui::SetCursorPosX(textPosXSensi);
                ImGui::SetCursorPosY(posY + textHeight + 100);
                ImGui::Text("Mouse sensitivity");

                float imguiWindowSizee = ImGui::GetWindowSize().x;
                float sliderPosOffsetSensi = imguiWindowSizee / 4;
                float sliderPosXSensi = (contentWidth - sliderWidth) * 0.5f - sliderPosOffsetSensi;
                ImGui::SetCursorPosX(sliderPosXSensi);;
                ImGui::SliderFloat("##Sensibilite", &sensi, 0.01f, 10.f);
                player->get_camera()->set_sensitivity(sensi);

                ImGui::SetCursorPosX(textPosXVolume);
                ImGui::SetCursorPosY(posY + textHeight + 270);
                ImGui::Text("Sound volume");

                float imguiWindowSizeee = ImGui::GetWindowSize().x;
                float sliderPosOffsetVolume = imguiWindowSizeee / 4;
                float sliderPosXVolume = (contentWidth - sliderWidth) * 0.5f - sliderPosOffsetVolume;
                ImGui::SetCursorPosX(sliderPosXVolume);;
                ImGui::SliderFloat("##Volume", &volume, 0.0f, 1.f);

                ImGui::SetCursorPosX(textPosXCom);
                ImGui::SetCursorPosY(posY + textHeight + 450);
                ImGui::Text("Commands AZERTY / QWERTY :");

                float imguiWindowSizeeee = ImGui::GetWindowSize().x;
                float sliderPosOffsetCom = imguiWindowSizeeee / 4;
                float sliderPosXCom = (contentWidth - sliderWidth) * 0.5f - sliderPosOffsetCom;
                float buttonWidth = 25;
                float totalButtonWidth = 2 * buttonWidth;
                float spaceBetweenButtons = imguiWindowSizeeee - totalButtonWidth;
                float offsetX = (SCR_WIDTH / 4); 
                ImGui::SetCursorPosX(offsetX);
                if(ImGui::Checkbox("AZERTY", &AZERTY)) {
                    player->get_camera()->mode_cam = 1;
                    QWERTY = false;
                }
                ImGui::SameLine();
                if(ImGui::Checkbox("QWERTY", &QWERTY)) {
                    player->get_camera()->mode_cam = 0;
                    AZERTY = false;
                }

                ImGui::SetCursorPosX(textPosXFullscreen);
                ImGui::SetCursorPosY(posY + textHeight + 625);
                ImGui::Text("Fullscreen :");

                float checkboxWidth = ImGui::CalcTextSize("").x;
                float offsetXFSC = (SCR_WIDTH - checkboxWidth) / 2.65;
                ImGui::SetCursorPosX(offsetXFSC);
                ImGui::Checkbox("##", &Fullscreen);
                ImGui::End();

                display_return_SETTINGS(fontMenu);
            }

        } else if(ESCAPE == true && credits == true) {
            ma_sound_stop(&sound);
            ma_sound_set_looping(&sound2,true);
            ma_sound_start(&sound2);

            if(Fullscreen == true) {
                ImGui::SetNextWindowBgAlpha(0.7f);
                ImGui::Begin("Creditss", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
                ImGui::PushFont(font);
                ImGui::SetWindowPos(ImVec2(SCR_WIDTH / 9.2, SCR_HEIGHT / 8));
                ImGui::SetWindowSize(ImVec2(SCR_WIDTH / 2 + SCR_WIDTH / 3 - 80, 800));  

                const char* CHFormater0 = "Developers";
                const char* CHFormater1 = "SERVA Benjamin";
                const char* CHFormater2 = "JEAN Louis";
                const char* CHFormater3 = "COMBOT Evan";
                const char* CHFormater5 = "Collaborators";
                const char* CHFormater6 = "RONTEIX Jonas - Level Designer";
                const char* CHFormater7 = "Brian & Loïc - sound of the end & credits";

                ImVec2 textSizech0 = ImGui::CalcTextSize(CHFormater0);
                ImVec2 textSizech1 = ImGui::CalcTextSize(CHFormater1);
                ImVec2 textSizech2 = ImGui::CalcTextSize(CHFormater2);
                ImVec2 textSizech3 = ImGui::CalcTextSize(CHFormater3);
                ImVec2 textSizech5 = ImGui::CalcTextSize(CHFormater5);
                ImVec2 textSizech6 = ImGui::CalcTextSize(CHFormater6);
                ImVec2 textSizech7 = ImGui::CalcTextSize(CHFormater7);

                // Center each text horizontally
                float posX0 = (ImGui::GetWindowWidth() - textSizech0.x) * 0.5f;
                float posX1 = (ImGui::GetWindowWidth() - textSizech1.x) * 0.53f;
                float posX2 = (ImGui::GetWindowWidth() - textSizech2.x) * 0.515f;
                float posX3 = (ImGui::GetWindowWidth() - textSizech3.x) * 0.52f;
                float posX5 = (ImGui::GetWindowWidth() - textSizech5.x) * 0.5f;
                float posX6 = (ImGui::GetWindowWidth() - textSizech6.x) * 0.6f;
                float posX7 = (ImGui::GetWindowWidth() - textSizech7.x) * 0.7f;

                float baseY = ImGui::GetCursorPosY() + ImGui::GetWindowHeight() / 8; 
                float spacing = textSizech1.y * 1.25f;

                ImGui::SetCursorPos(ImVec2(posX0, baseY));
                ImGui::Text("%s", CHFormater0);

                baseY += spacing;

                ImGui::PushFont(fontMenu2);
                ImGui::SetCursorPos(ImVec2(posX1, baseY));
                ImGui::Text("%s", CHFormater1);

                baseY += spacing;
                ImGui::SetCursorPos(ImVec2(posX2, baseY));
                ImGui::Text("%s", CHFormater2);

                baseY += spacing;
                ImGui::SetCursorPos(ImVec2(posX3, baseY));
                ImGui::Text("%s", CHFormater3);
                ImGui::PopFont();

                baseY += spacing;
                ImGui::SetCursorPos(ImVec2(posX5, baseY));
                ImGui::Text("%s", CHFormater5);

                baseY += spacing;

                ImGui::PushFont(fontMenu2);
                ImGui::SetCursorPos(ImVec2(posX6, baseY));
                ImGui::Text("%s", CHFormater6);

                baseY += spacing;
                ImGui::SetCursorPos(ImVec2(posX7, baseY));
                ImGui::Text("%s", CHFormater7);
                ImGui::PopFont();

                ImGui::PopFont();
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();
                ImGui::End();

                display_return_CREDITS(fontMenu);
            } else {

                ImGui::SetNextWindowBgAlpha(0.7f);
                ImGui::Begin("Creditss", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
                ImGui::PushFont(font);
                ImGui::SetWindowPos(ImVec2(SCR_WIDTH / 9.2, SCR_HEIGHT / 8));
                ImGui::SetWindowSize(ImVec2(SCR_WIDTH / 2 + SCR_WIDTH / 3 - 80, 800));  

                const char* CHFormater0 = "Developers";
                const char* CHFormater1 = "SERVA Benjamin";
                const char* CHFormater2 = "JEAN Louis";
                const char* CHFormater3 = "COMBOT Evan";
                const char* CHFormater5 = "Collaborators";
                const char* CHFormater6 = "RONTEIX Jonas - Level Designer";
                const char* CHFormater7 = "Brian & Loïc - sound of the end & credits";

                ImVec2 textSizech0 = ImGui::CalcTextSize(CHFormater0);
                ImVec2 textSizech1 = ImGui::CalcTextSize(CHFormater1);
                ImVec2 textSizech2 = ImGui::CalcTextSize(CHFormater2);
                ImVec2 textSizech3 = ImGui::CalcTextSize(CHFormater3);
                ImVec2 textSizech5 = ImGui::CalcTextSize(CHFormater5);
                ImVec2 textSizech6 = ImGui::CalcTextSize(CHFormater6);
                ImVec2 textSizech7 = ImGui::CalcTextSize(CHFormater7);

                // Center each text horizontally
                float posX0 = (ImGui::GetWindowWidth() - textSizech0.x) * 0.5f;
                float posX1 = (ImGui::GetWindowWidth() - textSizech1.x) * 0.56f;
                float posX2 = (ImGui::GetWindowWidth() - textSizech2.x) * 0.53f;
                float posX3 = (ImGui::GetWindowWidth() - textSizech3.x) * 0.54f;
                float posX5 = (ImGui::GetWindowWidth() - textSizech5.x) * 0.5f;
                float posX6 = (ImGui::GetWindowWidth() - textSizech6.x) * 0.68f;
                float posX7 = (ImGui::GetWindowWidth() - textSizech7.x) * 0.80f;

                float baseY = ImGui::GetCursorPosY() + ImGui::GetWindowHeight() / 8; 
                float spacing = textSizech1.y * 1.25f;

                ImGui::SetCursorPos(ImVec2(posX0, baseY));
                ImGui::Text("%s", CHFormater0);

                baseY += spacing;

                ImGui::PushFont(fontMenu2);
                ImGui::SetCursorPos(ImVec2(posX1, baseY));
                ImGui::Text("%s", CHFormater1);

                baseY += spacing;
                ImGui::SetCursorPos(ImVec2(posX2, baseY));
                ImGui::Text("%s", CHFormater2);

                baseY += spacing;
                ImGui::SetCursorPos(ImVec2(posX3, baseY));
                ImGui::Text("%s", CHFormater3);
                ImGui::PopFont();

                baseY += spacing;
                ImGui::SetCursorPos(ImVec2(posX5, baseY));
                ImGui::Text("%s", CHFormater5);

                baseY += spacing;

                ImGui::PushFont(fontMenu2);
                ImGui::SetCursorPos(ImVec2(posX6, baseY));
                ImGui::Text("%s", CHFormater6);

                baseY += spacing;
                ImGui::SetCursorPos(ImVec2(posX7, baseY));
                ImGui::Text("%s", CHFormater7);
                ImGui::PopFont();

                ImGui::PopFont();
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();
                ImGui::End();

                display_return_CREDITS(fontMenu);                
            }
            ma_sound_stop(&sound);
            ma_sound_set_looping(&sound2,true);
            ma_sound_start(&sound2);
        } else {
            ma_sound_stop(&sound2);
            ma_sound_set_looping(&sound,true);
            ma_sound_start(&sound);
            player->get_camera()->setPosition(player->get_camera()->getPosition());
            player->get_camera()->setRotationDegrees(player->get_camera()->getRotationDegrees());

            principal = true; 
            settings = false;    
            credits = false;    
        }


        if (fin == true) {
            showMouse = true;
            ma_sound_stop(&sound);
            ma_sound_set_looping(&sound3,true);
            ma_sound_start(&sound3);             

            if(Fullscreen == true) {
                ImGui::SetNextWindowBgAlpha(0.7f);
                ImVec2 screenSize = ImVec2(SCR_WIDTH, SCR_HEIGHT);
                ImVec2 textSize = ImGui::CalcTextSize("You won");
                ImVec2 windowPos = ImVec2((screenSize.x - textSize.x) * 0.45f, (screenSize.y - textSize.y) * 0.1f);

                ImGui::Begin("You won", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
                ImGui::SetWindowPos(windowPos);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0., 0., 0., 0.)); 
                ImGui::PushFont(fontEndt);

                const char* message = "You won";
                ImVec2 textSizeText = ImGui::CalcTextSize(message);
                float padding = 10.0f;
                textSizeText.x += 2 * padding;
                textSizeText.y += 2 * padding;

                ImGui::SetWindowSize(textSizeText);

                float xPosYouWon = (textSizeText.x - ImGui::CalcTextSize(message).x) * 0.5f;
                float yPosYouWon = (textSizeText.y - ImGui::CalcTextSize(message).y) * 0.5f;
                ImGui::SetCursorPosX(xPosYouWon);
                ImGui::SetCursorPosY(yPosYouWon);

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
                ImGui::Text("You won");
                ImGui::PopStyleColor();

                ImGui::PopFont();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();
                ImGui::End();

                ImGui::SetNextWindowBgAlpha(0.7f);
                ImGui::Begin("Timer_menu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1., 0., 0., 0.5));
                ImGui::PushFont(font);

                int heures_menu = static_cast<int>(timing / 3600);
                int minutes_menu = static_cast<int>((timing - heures_menu * 3600) / 60);
                int secondes_menu = static_cast<int>(timing - heures_menu * 3600 - minutes_menu * 60);

                snprintf(TempsFormaterMenu, sizeof(TempsFormaterMenu), "%02d:%02d:%02d", heures_menu, minutes_menu, secondes_menu);

                char chaineTempsMenu[16];
                snprintf(chaineTempsMenu, sizeof(chaineTempsMenu), "%.02f", timing);

                float longueurTexte = ImGui::CalcTextSize(TempsFormaterMenu).x;
                float hauteurTexte = ImGui::CalcTextSize(TempsFormaterMenu).y;

                float longueurFenetre = ImGui::GetWindowWidth();
                float hauteurFenetre = ImGui::GetWindowHeight();

                float posX_MetrageMain = (longueurFenetre - longueurTexte) * 0.5f;
                float posY_MetrageMain = (hauteurFenetre - hauteurTexte) * 0.5f;

                ImGui::SetCursorPosX(posX_MetrageMain);
                ImGui::SetCursorPosY(posY_MetrageMain);
                ImGui::SetWindowPos(ImVec2(SCR_WIDTH / 4, SCR_HEIGHT / 3 * 2.));
                ImGui::SetWindowSize(ImVec2(1000, 400));  

                ImGui::Text("%s", TempsFormaterMenu);


                ImGui::PushFont(fontMenu);
                ImGui::SetCursorPosX(5);
                ImGui::SetCursorPosY(2);
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Your time");
                ImGui::PopFont();

                ImGui::PopFont();
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();
                ImGui::End(); 

                ImGui::SetNextWindowBgAlpha(1.f);
                ImGui::Begin("Restart runr", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
                ImGui::PushFont(fontMenu);

                float xPos_res = (ImGui::GetWindowWidth() - ImGui::CalcTextSize("Restart run").x) * 0.5f;
                float yPos_res = (ImGui::GetWindowHeight() - ImGui::CalcTextSize("Restart run").y) * 0.5f;
                ImGui::SetCursorPosX(xPos_res);
                ImGui::SetCursorPosY(yPos_res);
                ImGui::SetWindowPos(ImVec2(0, SCR_HEIGHT - ImGui::GetWindowHeight()));            
                ImGui::SetWindowSize(ImVec2(205, 55));  

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                if (ImGui::Button("Restart run"))
                {
                    currentRun += 1;
                    MaxHeight = std::min(0., hauteur);
                    player->player_node->transform.set_translation(glm::vec3(-21.0f, 5.0f, 23.4f));
                    player->get_camera()->setRotationDegrees(glm::vec3(0., 90., 0.));

                    timing = 0.;
                    acc = 0.0;
                }
                ImGui::PopStyleColor();


                ImGui::PopFont();
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();
                ImGui::End();
            } else {
                ImGui::SetNextWindowBgAlpha(0.7f);
                ImVec2 screenSize = ImVec2(SCR_WIDTH, SCR_HEIGHT);
                ImVec2 textSize = ImGui::CalcTextSize("You won");
                ImVec2 windowPos = ImVec2((screenSize.x - textSize.x) * 0.45f, (screenSize.y - textSize.y) * 0.1f);

                ImGui::Begin("You won", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
                ImGui::SetWindowPos(windowPos);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0., 0., 0., 0.)); 
                ImGui::PushFont(fontEndt);

                const char* message = "You won";
                ImVec2 textSizeText = ImGui::CalcTextSize(message);
                float padding = 10.0f;
                textSizeText.x += 2 * padding;
                textSizeText.y += 2 * padding;

                ImGui::SetWindowSize(textSizeText);

                float xPosYouWon = (textSizeText.x - ImGui::CalcTextSize(message).x) * 0.5f;
                float yPosYouWon = (textSizeText.y - ImGui::CalcTextSize(message).y) * 0.5f;
                ImGui::SetCursorPosX(xPosYouWon);
                ImGui::SetCursorPosY(yPosYouWon);

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
                ImGui::Text("You won");
                ImGui::PopStyleColor();

                ImGui::PopFont();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();
                ImGui::End();

                ImGui::SetNextWindowBgAlpha(0.7f);
                ImGui::Begin("Timer_menu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1., 0., 0., 0.5));
                ImGui::PushFont(font);

                int heures_menu = static_cast<int>(timing / 3600);
                int minutes_menu = static_cast<int>((timing - heures_menu * 3600) / 60);
                int secondes_menu = static_cast<int>(timing - heures_menu * 3600 - minutes_menu * 60);

                snprintf(TempsFormaterMenu, sizeof(TempsFormaterMenu), "%02d:%02d:%02d", heures_menu, minutes_menu, secondes_menu);

                char chaineTempsMenu[16];
                snprintf(chaineTempsMenu, sizeof(chaineTempsMenu), "%.02f", timing);

                float longueurTexte = ImGui::CalcTextSize(TempsFormaterMenu).x;
                float hauteurTexte = ImGui::CalcTextSize(TempsFormaterMenu).y;

                float longueurFenetre = ImGui::GetWindowWidth();
                float hauteurFenetre = ImGui::GetWindowHeight();

                float posX_MetrageMain = (longueurFenetre - longueurTexte) * 0.5f;
                float posY_MetrageMain = (hauteurFenetre - hauteurTexte) * 0.5f;

                ImGui::SetCursorPosX(posX_MetrageMain);
                ImGui::SetCursorPosY(posY_MetrageMain);
                ImGui::SetWindowPos(ImVec2(SCR_WIDTH / 6, SCR_HEIGHT / 3));
                ImGui::SetWindowSize(ImVec2(1000, 400));  

                ImGui::Text("%s", TempsFormaterMenu);


                ImGui::PushFont(fontMenu);
                ImGui::SetCursorPosX(5);
                ImGui::SetCursorPosY(2);
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "Your time");
                ImGui::PopFont();

                ImGui::PopFont();
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();
                ImGui::End(); 

                ImGui::SetNextWindowBgAlpha(1.f);
                ImGui::Begin("Restart runr", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0., 0., 0., 0.));
                ImGui::PushFont(fontMenu);

                float xPos_res = (ImGui::GetWindowWidth() - ImGui::CalcTextSize("Restart run").x) * 0.5f;
                float yPos_res = (ImGui::GetWindowHeight() - ImGui::CalcTextSize("Restart run").y) * 0.5f;
                ImGui::SetCursorPosX(xPos_res);
                ImGui::SetCursorPosY(yPos_res);
                ImGui::SetWindowPos(ImVec2(0, SCR_HEIGHT - ImGui::GetWindowHeight()));            
                ImGui::SetWindowSize(ImVec2(205, 55));  

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                if (ImGui::Button("Restart run"))
                {        
                    currentRun += 1;
                    MaxHeight = std::min(0., hauteur);
                    player->player_node->transform.set_translation(glm::vec3(-21.0f, 5.0f, 23.4f));
                    player->get_camera()->setRotationDegrees(glm::vec3(0., 90., 0.));

                    timing = 0.;
                    acc = 0.0;
                }
                ImGui::PopStyleColor();

                ImGui::PopFont();
                ImGui::PopStyleColor();
                ImGui::PopStyleVar();
                ImGui::End();
            }
        } else {
            ma_sound_stop(&sound3);
            ma_sound_set_looping(&sound,true);
            ma_sound_start(&sound); 
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
            // wireframe = !wireframe;
            // if(wireframe) {
            //     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            // }
            // else {
            //     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            // }
        }
        if(key == GLFW_KEY_A && player->get_camera()->mode_cam==0) {
            // wireframe = !wireframe;
            // if(wireframe) {
            //     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            // }
            // else {
            //     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            // }
        }
        if(key == GLFW_KEY_T) {
            // showMouse = player->get_camera()->getShowMouse();
            // showMouse = !showMouse;
            // player->get_camera()->setShowMouse(showMouse);
        }
        if (key == GLFW_KEY_H) {
            // player->get_camera()->mode_cam=(player->get_camera()->mode_cam+1)%3;
            // player->get_camera()->reset();
        }

    }
    //player->handleSingleInput(key, scancode, action, mods);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && ESCAPE==false) {
        scene->mode_torch_light_player();
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && ESCAPE==false) {
        scene->on_off_torch_light_player();
    }
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

void APIENTRY openglCallbackFunction(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam) {

    (void)source; (void)type; (void)id; 
    (void)severity; (void)length; (void)userParam;
    fprintf(stderr, "%s\n", message);
    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        fprintf(stderr, "Aborting...\n");
        abort();
    }
}