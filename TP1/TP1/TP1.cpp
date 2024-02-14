// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// Include noise
#include "FastNoiseLit.h"

using namespace glm;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

void processInput(GLFWwindow *window);
void set_plane(std::vector<glm::vec3> &indexed_vertices, std::vector<unsigned short> &indices, int nX, int nY);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

// Variables pour le plan (ici car nécessaire pour la caméra)
int nX = 16;
int nY = 16;

// Variables globales pour l'orbite
float horizontal_angle = 3.14f; 
float vertical_angle = 0.0f; 
float radius = 20.0f;

// camera
glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camera_target = glm::vec3(nX/2,nY/2,0.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f,  0.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//rotation
float angle = 0.;
float zoom = 1.;
/*******************************************************************************/

int main(void)
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "TP1 - GLFW", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Pour voir les arêtes
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    //glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "vertex_shader.glsl", "fragment_shader.glsl" );

    /****************************************/
    std::vector<unsigned short> indices; //Triangles concaténés dans une liste
    std::vector<std::vector<unsigned short>> triangles;
    std::vector<glm::vec3> indexed_vertices;

    // Chargement du fichier de maillage
    // std::string filename("suzanne.off");
    // loadOFF(filename, indexed_vertices, indices, triangles);

    // Création du plan
    set_plane(indexed_vertices,indices,nX,nY);

    // Load it into a VBO

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    do{

        // Measure speed
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);


        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        /*****************TODO***********************/
        // Model matrix : an identity matrix (model will be at the origin) then change

        glm::mat4 model_matrix = glm::mat4(1.0f);

        // View matrix : camera/view transformation lookat() utiliser camera_position camera_target camera_up

        camera_position.x = camera_target.x + radius * cos(vertical_angle) * sin(horizontal_angle);
        camera_position.y = camera_target.y + radius * sin(vertical_angle);
        camera_position.z = camera_target.z + radius * cos(vertical_angle) * cos(horizontal_angle);
        glm::mat4 view_matrix = glm::lookAt(camera_position,camera_target,camera_up);

        // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units

        glm::mat4 projection_matrix = glm::perspective(45.0f,(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,500.0f);

        /*****************TODO***********************/
        // Get a handle for our "Model View Projection" matrices uniforms

        // Send our transformation to the currently bound shader,
        // in the "Model View Projection" to the shader uniforms

        GLuint model_matrix_location = glGetUniformLocation(programID,"model_matrix");
        glUniformMatrix4fv(model_matrix_location,1,GL_FALSE,glm::value_ptr(model_matrix));
        GLuint view_matrix_location = glGetUniformLocation(programID,"view_matrix");
        glUniformMatrix4fv(view_matrix_location,1,GL_FALSE,glm::value_ptr(view_matrix));
        GLuint projection_matrix_location = glGetUniformLocation(programID,"projection_matrix");
        glUniformMatrix4fv(projection_matrix_location,1,GL_FALSE,glm::value_ptr(projection_matrix));
        
        /****************************************/

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    );

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // Draw the triangles !
        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices.size(),    // count
                    GL_UNSIGNED_SHORT,   // type
                    (void*)0           // element array buffer offset
                    );

        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    // Sensibilité de la caméra
    float camera_speed = 3.0f * deltaTime;

    // Ajustement des angles en fonction des touches pressées
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) horizontal_angle -= camera_speed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) horizontal_angle += camera_speed;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) vertical_angle += camera_speed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) vertical_angle -= camera_speed;

    // Ajuster le zoom
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) radius -= 10*camera_speed; // 10 fois pour que ça aille plus vite sans modifier la vitesse de l'orbite
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) radius += 10*camera_speed;

    // Limiter l'angle vertical
    vertical_angle = std::max(-1.57f, std::min(1.57f, vertical_angle));
}

void set_plane(std::vector<glm::vec3> &indexed_vertices, std::vector<unsigned short> &indices, int nX, int nY) {

    indexed_vertices.clear();
    indices.clear();

    static FastNoiseLite noise;

    // Génération des points
    for(int i = 0; i < nX; i++) {
        for(int j = 0; j < nY; j++) {
            float x = i;
            float y = j;
            float z = std::min(0.0f,noise.GetNoise((float)i*1000,(float)j*100));

            indexed_vertices.push_back(glm::vec3(x,y,z));

        }
    }

    // Génération des triangles
    for(int i = 0; i < nX - 1; i++) { 
        for(int j = 0; j < nY - 1; j++) {
            unsigned short topLeft = i * nY + j;
            unsigned short topRight = topLeft + 1;
            unsigned short bottomLeft = topLeft + nY;
            unsigned short bottomRight = bottomLeft + 1;

            // Premier triangle
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);

            // Second triangle
            indices.push_back(topLeft);
            indices.push_back(bottomRight);
            indices.push_back(topRight);
        }
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
