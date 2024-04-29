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


// Window settings
const unsigned int SCR_WIDTH = 1440;
const unsigned int SCR_HEIGHT = 1080;
bool showMouse = true;

// Camera
Camera myCamera;

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
    shader.setShader("../shaders/LampeTorche.vert","../shaders/LampeTorche.frag");

    Shader shaderMesh;
    shaderMesh.setShader("../shaders/mesh.vert","../shaders/mesh.frag");

    Plane* plane = new Plane(100, 100, 100,0);
    Texture plane_texture("../data/textures/pavement.jpg");

    plane->add_texture(plane_texture);

    plane->bind_shader(shader);
    SceneNode plane_node(plane);

    Model model("../data/models/capsule/capsule.gltf");
    model.bind_shader_to_meshes(shader);
    SceneNode* model_node = new SceneNode(&model);
    model_node->transform.scale = glm::vec3(1.0f);
    model_node->transform.translation = glm::vec3(1.0f);
    model_node->transform.rotation.z = 90.0f;

    Model obst1("../data/models/platform/GreyBricks.glb");
    obst1.bind_shader_to_meshes(shader);
    SceneNode* obst1_node = new SceneNode(&obst1);
    obst1_node->transform.scale = glm::vec3(300.0f);
    obst1_node->transform.translation = glm::vec3(15., 3.0f, 15.);

    myCamera.init();
    

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
                // Dérivée de la conservation de l'énergie
                v0_Vitesse = sqrt(2.0f * g * hauteur);
                Space = true;
            } else if (glfwGetKey(window.get_window(), GLFW_KEY_SPACE) == GLFW_RELEASE) {
                Space = false;
            }

            v0_Vitesse -= deltaTime * vitesse;
            model_node->transform.translation.y += v0_Vitesse * deltaTime;

            /*
            if (model_node->transform.translation.y <= 0.0f) {
                model_node->transform.translation.y = 0.0f;
                v0_Vitesse = 0.0f;
            }*/

            // Avec rebonds (pour tests)
            
            if (model_node->transform.translation.y <= 0.0f) {
                model_node->transform.translation.y = -model_node->transform.translation.y;
                v0_Vitesse = sqrt(2.0f * g * hauteur);
            }
            
            lag -= MS_PER_UPDATE;    
        }

        view = myCamera.getViewMatrix();
        proj = myCamera.getProjectionMatrix();  

        // Sending to shader

        glm::vec3 camPos = myCamera.getPosition();
        glm::vec3 camFront = myCamera.getCFront();
        cutOff = glm::cos(glm::radians(cutIn));
        outerCutOff = glm::cos(glm::radians(cutOut));

        shader.useShader();
        shader.setBindMatrix4fv("view", 1, 0, glm::value_ptr(view));
        shader.setBindMatrix4fv("projection", 1, 0, glm::value_ptr(proj));

        // Phong + Flashlight
        shader.setBind3f("lightPos", camPos[0], camPos[1], camPos[2]);
        shader.setBind3f("viewPos", camPos[0], camPos[1], camPos[2]);
        shader.setBind3f("lightDir", camFront[0], camFront[1], camFront[2]);
        shader.setBind1f("cutOff", cutOff);
        shader.setBind1f("outerCutOff", outerCutOff);

        shader.setBind3f("lightambient", lightAmbiant[0], lightAmbiant[1], lightAmbiant[2]);
        shader.setBind3f("lightdiffuse", lightDiffuse[0], lightDiffuse[1], lightDiffuse[2]);
        shader.setBind3f("lightspecular", lightSpecular[0], lightSpecular[1], lightSpecular[2]);
        shader.setBind3f("camPos", camPos[0], camPos[1], camPos[2]);

        shader.setBind1f("constant", constant);
        shader.setBind1f("linear", linear);
        shader.setBind1f("quadratic", quadratic);


        // Scene
        plane_node.draw();
        model_node->draw();
        obst1_node->draw();


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