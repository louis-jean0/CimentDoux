#include <climits>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Window.hpp>
#include <Mesh.hpp>
#include <objloader.hpp>
#include <Sphere.hpp>
#include <SceneNode.hpp>
#include <Texture.hpp>

// Functions prototypes
void printUsage();
void processInput(GLFWwindow *window);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Window settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// Camera settings
glm::vec3 camera_pos = glm::vec3(1.0f,1.0f,5.0f);
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
    Window window(4,6,SCR_WIDTH,SCR_HEIGHT,"Moteur de jeux - TP Graphe de scène",true);
    window.setup_GLFW();

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    glEnable(GL_DEPTH_TEST);

    // Setup shaders
    Shader shader;
    shader.setShader("../shaders/vs.vert", "../shaders/fs.frag");
    Shader star_shader;
    star_shader.setShader("../shaders/star_shader.vert", "../shaders/star_shader.frag");

    // Textures
    Texture sun_texture("../data/textures/2k_sun.jpg");
    Texture earth_texture("../data/textures/2k_earth_daymap.jpg");
    Texture moon_texture("../data/textures/2k_moon.jpg");

    // Sun
    Mesh* sun_mesh = new Sphere(glm::vec3(0.0f,0.0f,0.0f),1.0f,50,50);
    sun_mesh->add_texture(sun_texture);
    sun_mesh->bind_shader(shader);
    sun_mesh->setup_mesh();
    SceneNode* sun = new SceneNode(sun_mesh);
    float sun_rotation_speed = 10.0f;

    // Earth
    Mesh* earth_mesh = new Sphere(glm::vec3(0.0f,0.0f,0.0f),0.5f,50,50);
    earth_mesh->add_texture(earth_texture);
    earth_mesh->bind_shader(shader);
    earth_mesh->setup_mesh();
    SceneNode* earth = new SceneNode(earth_mesh);
    float earth_rotation_speed = 20.0f;
    float earth_to_sun_distance = 3.0f;
    earth->transform.translation = glm::vec3(earth_to_sun_distance,0.0f,0.0f);
    earth->transform.rotation.x = 23.44f;
    sun->add_child(earth);

    // Moon
    Mesh* moon_mesh = new Sphere(glm::vec3(0.0f,0.0f,0.0f),0.25f,50,50);
    moon_mesh->add_texture(moon_texture);
    moon_mesh->bind_shader(shader);
    moon_mesh->setup_mesh();
    SceneNode* moon = new SceneNode(moon_mesh);
    float moon_rotation_speed = 50.0f;
    float moon_to_earth_distance = 1.0f;
    moon->transform.translation = glm::vec3(moon_to_earth_distance,0.0f,0.0f);
    moon->transform.rotation.x = 6.68f;
    earth->add_child(moon);

    // Stars 
    std::vector<glm::vec3> star_positions;
    int number_of_stars = 2500;
    for(int i = 0; i < number_of_stars; ++i) {
        float x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 200.0f - 100.0f;
        float y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 200.0f - 100.0f;
        float z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 200.0f - 100.0f;
        star_positions.push_back(glm::vec3(x, y, z));
    }

    GLuint starsVAO, starsVBO;
    glGenVertexArrays(1, &starsVAO);
    glGenBuffers(1, &starsVBO);
    glBindVertexArray(starsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, starsVBO);
    glBufferData(GL_ARRAY_BUFFER, star_positions.size() * sizeof(glm::vec3), &star_positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    printUsage();

    // Render loop
    while (!glfwWindowShouldClose(window.get_window())) {

        // Controls
        processInput(window.get_window());
        glfwSetKeyCallback(window.get_window(), keyCallback);

        current_frame_time = glfwGetTime();
        delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Model is computed directly thanks to the SceneNode

        // View
        camera_pos.x = camera_target.x + radius * cos(vertical_angle) * sin(horizontal_angle);
        camera_pos.y = camera_target.y + radius * sin(vertical_angle);
        camera_pos.z = camera_target.z + radius * cos(vertical_angle) * cos(horizontal_angle);
        glm::mat4 view = glm::lookAt(camera_pos,camera_target,camera_up);

        // Projection
        glm::mat4 projection = glm::perspective(45.0f,(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,1000.0f);

        // Sending to shader
        shader.useShader();
        shader.setBindMatrix4fv("view", 1, 0, glm::value_ptr(view));
        shader.setBindMatrix4fv("projection", 1, 0, glm::value_ptr(projection));

        // Solar system
        float sun_angle = sun_rotation_speed * delta_time;
        sun->transform.rotation.y += sun_rotation_speed * delta_time;
        earth->transform.rotation.y += earth_rotation_speed * delta_time;
        earth->transform.translation.x = earth_to_sun_distance;
        moon->transform.translation.x = moon_to_earth_distance;
        moon->transform.rotation.y += moon_rotation_speed * delta_time;

        sun->draw();

        // Star shader
        star_shader.useShader();
        star_shader.setBindMatrix4fv("view", 1, 0, glm::value_ptr(view));
        star_shader.setBindMatrix4fv("projection", 1, 0, glm::value_ptr(projection));
        glBindVertexArray(starsVAO);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glDrawArrays(GL_POINTS, 0, star_positions.size());
        glBindVertexArray(0);

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Paramètres");
        ImGui::Separator();
        ImGui::Text("Vitesse du système");
        ImGui::SliderFloat("Vitesse de rotation du Soleil", &sun_rotation_speed, 1.0f, 200.0f);
        ImGui::SliderFloat("Vitesse de rotation de la Terre", &earth_rotation_speed, 1.0f, 200.0f);
        ImGui::SliderFloat("Vitesse de rotation de la Lune", &moon_rotation_speed, 1.0f, 200.0f);
        ImGui::Separator();
        ImGui::Text("Distances du système");
        ImGui::SliderFloat("Distance Terre-Soleil", &earth_to_sun_distance, 2.0f, 5.0f);
        ImGui::SliderFloat("Distance Terre-Lune", &moon_to_earth_distance, 1.0f, 5.0f);
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
    float camera_speed = 3.0f * delta_time;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) horizontal_angle -= camera_speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) horizontal_angle += camera_speed;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) vertical_angle += camera_speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) vertical_angle -= camera_speed;

    // Zoom
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) radius -= 10*camera_speed; // 10 fois pour que ça aille plus vite sans modifier la vitesse de l'orbite
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) radius += 10*camera_speed;

    vertical_angle = std::max(-1.57f, std::min(1.57f, vertical_angle));
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