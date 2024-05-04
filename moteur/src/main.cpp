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
#include <cmath>
#include <iostream>

#include "Window.hpp"
#include "SceneNode.hpp"
#include "Texture.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "PlaneCollider.hpp"
#include "Camera.hpp"
#include "Cube.hpp"
#include "ModelCollider.hpp"
#include "Scene.hpp"
#include "PhysicsEngine.hpp"
#include "Player.hpp"

#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>
#include <vector>
#include <iostream>
#include <cstring>

bool Volume_up = false;
bool Volume_down = false;
float volume = 1.0f;
float distance, distanceMin = 1., distanceMax = 500.;

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
Scene scene;

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

Player* player;


// Fonction utilitaire pour charger un fichier audio à l'aide de libsndfile
bool loadAudioFile(const char* filename, std::vector<ALshort>& samples, SF_INFO& info) {
    SNDFILE* file = sf_open(filename, SFM_READ, &info);
    if (!file) {
        std::cerr << "Impossible de charger le fichier audio: " << filename << std::endl;
        return false;
    }

    samples.resize(info.frames * info.channels);
    sf_read_short(file, samples.data(), samples.size());
    sf_close(file);

    return true;
}

static void list_audio_devices(const ALCchar *devices)
{
	const ALCchar *device = devices, *next = devices + 1;
	size_t len = 0;

	fprintf(stdout, "Devices list:\n");
	fprintf(stdout, "----------\n");
	while (device && *device != '\0' && next && *next != '\0') {
		fprintf(stdout, "%s\n", device);
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
	}
	fprintf(stdout, "----------\n");
}





int main(int argc, char* argv[]) {
    // Initialize window
    Window window(4,1,SCR_WIDTH,SCR_HEIGHT,"Moteur de jeux",true);
    window.setup_GLFW();

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 410");

    glEnable(GL_DEPTH_TEST);

    Shader shader;
    shader.setShader("../shaders/vs.vert","../shaders/fs.frag");
    /*
    Plane* plane = new Plane(100, 100, 100,0);
    Texture plane_texture("../data/textures/pavement.jpg");

    plane->add_texture(plane_texture);

    plane->bind_shader(shader);
    SceneNode plane_node(plane);*/

    Model model("../data/models/capsule/capsule.gltf");
    model.bind_shader_to_meshes(shader);
    SceneNode* model_node = new SceneNode(&model);
    model_node->transform.set_scale(glm::vec3(1.0f));
    model_node->transform.set_translation(glm::vec3(20.0f));
    model_node->transform.set_rotation(glm::vec3(0.0f,0.0f,90.0f));
    player = new Player(model_node, window.get_window(), myCamera);
    pe.add_entity(&player->player_node->rigid_body);
    
    //scene.creation_plan("../data/textures/2k_neptune.jpg",100, 100, 100,0,shader);
    //scene.creationMap(shader);

    Model plane("../data/models/plane/plane.gltf");
    plane.bind_shader_to_meshes(shader);
    Texture texture("../data/textures/rubber.jpg");
    plane.bind_texture_to_meshes(texture);
    SceneNode* plane_node = new SceneNode(&plane);
    plane_node->transform.set_scale(glm::vec3(5000.0f,1.f,5000.f));
    pe.add_entity(&plane_node->rigid_body);
    
    Model obst1("../data/models/cube/Cube.gltf");
    obst1.bind_shader_to_meshes(shader);
    //obst1.bind_texture_to_meshes(texture);
    SceneNode* obst1_node = new SceneNode(&obst1);
    obst1_node->transform.set_scale(glm::vec3(1.0f));
    obst1_node->transform.set_translation(glm::vec3(15., 1.0f, 15.));
    //pe.add_entity(&obst1_node->rigid_body);
    
    //myCamera->init();
    
    Model obst2("../data/models/cube/Cube.gltf");
    obst2.bind_shader_to_meshes(shader);
    //obst2.bind_texture_to_meshes(texture);
    SceneNode* obst2_node = new SceneNode(&obst2);
    obst2_node->transform.set_scale(glm::vec3(1.0f));
    obst2_node->transform.set_translation(glm::vec3(15., 2.0f, 17));
    pe.add_entity(&obst2_node->rigid_body);

    Model obst3("../data/models/cube/Cube.gltf");
    obst3.bind_shader_to_meshes(shader);
    //obst3.bind_texture_to_meshes(texture);
    SceneNode* obst3_node = new SceneNode(&obst3);
    obst3_node->transform.set_scale(glm::vec3(1.0f));
    obst3_node->transform.set_translation(glm::vec3(15., 3.0f, 19));
    pe.add_entity(&obst3_node->rigid_body);

    Model obst4("../data/models/cube/Cube.gltf");
    obst4.bind_shader_to_meshes(shader);
    //obst4.bind_texture_to_meshes(texture);
    SceneNode* obst4_node = new SceneNode(&obst4);
    obst4_node->transform.set_scale(glm::vec3(2.0f,1.f,3.f));
    obst4_node->transform.set_translation(glm::vec3(15., 4.0f, 23));
    pe.add_entity(&obst4_node->rigid_body);

    Model obst5("../data/models/cube/Cube.gltf");
    obst5.bind_shader_to_meshes(shader);
    //obst5.bind_texture_to_meshes(texture);
    SceneNode* obst5_node = new SceneNode(&obst5);
    obst5_node->transform.set_scale(glm::vec3(3.0f,1.f,5.f));
    obst5_node->transform.set_translation(glm::vec3(15., 5.0f, 33));
    pe.add_entity(&obst5_node->rigid_body);

    Model obst6("../data/models/cube/Cube.gltf");
    obst6.bind_shader_to_meshes(shader);
    //obst6.bind_texture_to_meshes(texture);
    SceneNode* obst6_node = new SceneNode(&obst6);
    obst6_node->transform.set_scale(glm::vec3(2.0f,1.f,3.f));
    obst6_node->transform.set_translation(glm::vec3(15., 4.0f, 83.5));
    pe.add_entity(&obst6_node->rigid_body);
    
    //myCamera->init();

    Model plante("../data/models/plant1/eb_house_plant_01.fbx");
    plante.bind_shader_to_meshes(shader);
    plante.bind_texture_to_meshes(texture);
    SceneNode* plante_node = new SceneNode(&plante);
    plante_node->transform.set_scale(glm::vec3(0.1f));
    plante_node->transform.set_translation(glm::vec3(10.0f,0.0f,10.0f));
    pe.add_entity(&plante_node->rigid_body);

    glm::vec3 ambient = glm::vec3(1.0f,1.0f,1.0f);
    glm::vec3 diffuse = glm::vec3(0.5f,0.5f,0.5f);
    glm::vec3 specular = glm::vec3(0.2f,0.2f,0.2f);
    glm::vec3 position = glm::vec3(-1.0f,1.0f,1.0f);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    float temps_debut=glfwGetTime();

    // Obtenir une liste des périphériques audio disponibles
    const ALCchar* devices = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
    if (!devices) {
        std::cerr << "Aucun périphérique audio trouvé" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Choisissez le périphérique audio que vous souhaitez utiliser (par exemple, le premier périphérique de la liste)
    const char* deviceName = devices;
    std::cout << deviceName << std::endl;

    // Spécifiez le chemin du périphérique audio dans alcOpenDevice()
    ALCdevice* device = alcOpenDevice(deviceName);
    if (!device) {
        std::cerr << "Impossible d'ouvrir le périphérique audio spécifié" << std::endl;
        glfwTerminate();
        return -1;
    }

    ALCcontext* context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);

    // Charger le fichier audio
    std::vector<ALshort> samples;
    SF_INFO info;
    if (!loadAudioFile("../data/sounds/retro.wav", samples, info)) {
        alcDestroyContext(context);
        alcCloseDevice(device);
        glfwTerminate();
        return -1;
    }

    // Générer et remplir un buffer OpenAL
    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, (info.channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, samples.data(),
                 static_cast<ALsizei>(samples.size() * sizeof(ALshort)), info.samplerate);

    // Créer une source audio OpenAL
    ALuint source;
    alGenSources(1, &source);

    alSourcef(source, AL_GAIN, volume);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcei(source, AL_LOOPING, true);
    alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
    alSource3f(source, AL_POSITION, 0., 0., 0.);
    alSource3f(source, AL_VELOCITY, 1., 1., 1.);
    alSourcef(source, AL_PITCH, 1);

    // Jouer le son
    alSourcePlay(source);

    // Render loop
    while (glfwGetKey(window.get_window(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window.get_window()) == 0) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        lag += deltaTime;

        if (glfwGetKey(window.get_window(), GLFW_KEY_DOWN) == GLFW_PRESS) {
            volume -= 0.005;

            if(volume <= 0.0f) {
                volume = 0.0f;
            }
            Volume_down = true;
        }
        if (glfwGetKey(window.get_window(), GLFW_KEY_UP) == GLFW_PRESS) {
            volume += 0.005;

            if(volume >= 1.0f) {
                volume = 1.0f;
            }
            Volume_up = true;
        }

        distance = std::max(distance, AL_REFERENCE_DISTANCE);
        volume = AL_REFERENCE_DISTANCE / (AL_REFERENCE_DISTANCE + AL_ROLLOFF_FACTOR * (distance - AL_REFERENCE_DISTANCE));
        volume = std::max(0.0f, std::min(1.0f, volume));

        std::cout << "Volume: " << volume << std::endl;
        alSourcef(source, AL_GAIN, volume);


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

        //myCamera->updateInterface(deltaTime);

        while (lag >= MS_PER_UPDATE) {
            player->update(deltaTime);
            
            lag -= MS_PER_UPDATE;    
        }

        view = myCamera->getViewMatrix();
        proj = myCamera->getProjectionMatrix();  

        // Sending to shader

        glm::vec3 camPos = myCamera->getPosition();
        glm::vec3 camFront = myCamera->getCFront();
        cutOff = glm::cos(glm::radians(cutIn));
        outerCutOff = glm::cos(glm::radians(cutOut));

        shader.useShader();
        shader.setVPMatrix(view,proj);

        // Phong + Flashlight
        // shader.setBind3f("lightPos", camPos[0], camPos[1], camPos[2]);
        shader.setBind3f("viewPos", camPos[0], camPos[1], camPos[2]);
        // shader.setBind3f("lightDir", camFront[0], camFront[1], camFront[2]);
        // shader.setBind1f("cutOff", cutOff);
        // shader.setBind1f("outerCutOff", outerCutOff);

        // shader.setBind3f("lightambient", lightAmbiant[0], lightAmbiant[1], lightAmbiant[2]);
        // shader.setBind3f("lightdiffuse", lightDiffuse[0], lightDiffuse[1], lightDiffuse[2]);
        // shader.setBind3f("lightspecular", lightSpecular[0], lightSpecular[1], lightSpecular[2]);
        // shader.setBind3f("camPos", camPos[0], camPos[1], camPos[2]);

        // shader.setBind1f("constant", constant);
        // shader.setBind1f("linear", linear);
        // shader.setBind1f("quadratic", quadratic);

        // Scene
        plane_node->draw(view, proj);
        obst1_node->draw(view, proj);
        //model_node->draw(view, proj);
        //obst1_node->draw(view, proj);

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
        /*
        ImGui::Spacing();
        ImGui::SliderFloat("Metallic", &metallic, 0., 1.);
        ImGui::SliderFloat("Roughness", &roughness, 0., 1.);
        ImGui::SliderFloat("AO", &ao, 0., 1.);
        ImGui::SliderFloat("NormalStrength", &NormalStrength, 0., 5.);
        */
        //pour tous dessiner si ca fonctionne
        //scene.draw(view, proj);
        //scene.draw_plan(view, proj);

        //model_node->draw(view, proj);    
        obst1_node->draw(view, proj);          
        obst2_node->draw(view, proj);
        obst3_node->draw(view, proj);
        obst4_node->draw(view, proj);
        obst5_node->transform.adjust_translation(glm::vec3(0.f, 0.f,-sin(temps_debut-currentFrame)*deltaTime*20));
        obst5_node->draw(view, proj);
        obst6_node->draw(view, proj);
        plante_node->draw(view, proj);

        pe.update(deltaTime);

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
    alSourceStop(source);
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alcDestroyContext(context);
    alcCloseDevice(device);
    glfwTerminate();

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
        if(key == GLFW_KEY_T) {
            showMouse = myCamera->getShowMouse();
            showMouse = !showMouse;
            myCamera->setShowMouse(showMouse);
        }
        if (key == GLFW_KEY_H) {
            myCamera->mode_cam=(myCamera->mode_cam+1)%3;
            myCamera->reset();
        }
    }
    //player->handleSingleInput(key, scancode, action, mods);
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