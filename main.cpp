//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "SpotLight.h"
#include "sphere.h"
#include "tool.h"
#include "cylinders.h"
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void drawCylinder(unsigned int& cVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);

void scene(unsigned int& cubeVAO, unsigned int& p1VAO, Shader& lightingShader, Shader& Shader);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
float s_ambient = 1;
float s_diffuse = 1;
float s_specular = 1;
float p_ambient = 1;
float p_diffuse = 1;
float p_specular = 1;
float d_ambient = 1;
float d_diffuse = 1;
float d_specular = 1;
// camera
Camera camera(glm::vec3(0.0f, 1.1f, 5.2f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);



// light settings
bool onOff = true;
bool ambientonOff = true;
bool diffuseonOff = true;
bool specularonOff = true;

bool pointlightonOff = true;
bool directionallightonOff = true;
bool spotlightonOff = true;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

glm::mat4 transforamtion(float tx, float ty, float tz, float sx, float sy, float sz) {
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(tx, ty, tz));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(sx, sy, sz));
    model = translateMatrix * scaleMatrix;
    return model;
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    //for cylinders
    float ver_arr[] = {

        1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.809017f, 1.0f, 0.587785f, 0.809017f, 1.0f, 0.587785f,
        0.309017f, 1.0f, 0.951057f, 0.309017f, 1.0f, 0.951057f,
        -0.309017f, 1.0f, 0.951057f, -0.309017f, 1.0f, 0.951057f,
        -0.809017f, 1.0f, 0.587785f, -0.809017f, 1.0f, 0.587785f,
        -1.0f, 1.0f, 1.22465e-16f, -1.0f, 1.0f, 1.22465e-16f,
        -0.809017f, 1.0f, -0.587785f, -0.809017f, 1.0f, -0.587785f,
        -0.309017f, 1.0f, -0.951057f, -0.309017f, 1.0f, -0.951057f,
        0.309017f, 1.0f, -0.951057f, 0.309017f, 1.0f, -0.951057f,
        0.809017f, 1.0f, -0.587785f, 0.809017f, 1.0f, -0.587785f,

        1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
        0.809017f, -1.0f, 0.587785f, 0.809017f, -1.0f, 0.587785f,
        0.309017f, -1.0f, 0.951057f, 0.309017f, -1.0f, 0.951057f,
        -0.309017f, -1.0f, 0.951057f, -0.309017f, -1.0f, 0.951057f,
        -0.809017f, -1.0f, 0.587785f, -0.809017f, -1.0f, 0.587785f,
        -1.0f, -1.0f, 1.22465e-16f, -1.0f, -1.0f, 1.22465e-16f,
        -0.809017f, -1.0f, -0.587785f, -0.809017f, -1.0f, -0.587785f,
        -0.309017f, -1.0f, -0.951057f, -0.309017f, -1.0f, -0.951057f,
        0.309017f, -1.0f, -0.951057f, 0.309017f, -1.0f, -0.951057f,
        0.809017f, -1.0f, -0.587785f, 0.809017f, -1.0f, -0.587785f,


        1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
        0.809017f, -1.0f, 0.587785f, 0.809017f, -1.0f, 0.587785f,
        0.309017f, -1.0f, 0.951057f, 0.309017f, -1.0f, 0.951057f,
        -0.309017f, -1.0f, 0.951057f, -0.309017f, -1.0f, 0.951057f,
        -0.809017f, -1.0f, 0.587785f, -0.809017f, -1.0f, 0.587785f,
        -1.0f, -1.0f, 1.22465e-16f, -1.0f, -1.0f, 1.22465e-16f,
        -0.809017f, -1.0f, -0.587785f, -0.809017f, -1.0f, -0.587785f,
        -0.309017f, -1.0f, -0.951057f, -0.309017f, -1.0f, -0.951057f,
        0.309017f, -1.0f, -0.951057f, 0.309017f, -1.0f, -0.951057f,
        0.809017f, -1.0f, -0.587785f, 0.809017f, -1.0f, -0.587785f,

        1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.809017f, 1.0f, 0.587785f, 0.809017f, 1.0f, 0.587785f,
        0.309017f, 1.0f, 0.951057f, 0.309017f, 1.0f, 0.951057f,
        -0.309017f, 1.0f, 0.951057f, -0.309017f, 1.0f, 0.951057f,
        -0.809017f, 1.0f, 0.587785f, -0.809017f, 1.0f, 0.587785f,
        -1.0f, 1.0f, 1.22465e-16f, -1.0f, 1.0f, 1.22465e-16f,
        -0.809017f, 1.0f, -0.587785f, -0.809017f, 1.0f, -0.587785f,
        -0.309017f, 1.0f, -0.951057f, -0.309017f, 1.0f, -0.951057f,
        0.309017f, 1.0f, -0.951057f, 0.309017f, 1.0f, -0.951057f,
        0.809017f, 1.0f, -0.587785f, 0.809017f, 1.0f, -0.587785f,


        0.0,-1.0,0.0, 0.0,-1.0,0.0,
        0.0,1.0,0.0, 0.0,1.0,0.0

    };

    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    //for cylinder
    unsigned int ind_arr[] = {
        0, 11, 1,
        11, 0, 10,
        1, 12, 2,
        12, 1, 11,
        2, 13, 3,
        13, 2, 12,
        3, 14, 4,
        14, 3, 13,
        4, 15, 5,
        15, 4, 14,
        5, 16, 6,
        16, 5, 15,
        6, 17, 7,
        17, 6, 16,
        7, 18, 8,
        18, 7, 17,
        8, 19, 9,
        19, 8, 18,
        9, 10, 0,
        10, 9, 19,

        40,20,21,
        40,21,22,
        40,22,23,
        40,23,24,
        40,24,25,
        40,25,26,
        40,26,27,
        40,27,28,
        40,28,29,
        40,29,20,

        41,30,31,
        41,31,32,
        41,32,33,
        41,33,34,
        41,34,35,
        41,35,36,
        41,36,37,
        41,37,38,
        41,38,39,
        41,39,30


        /*
        21,10,11,
        21,11,12,
        21,12,13,
        21,13,14,
        21,14,15,
        21,15,16,
        21,16,17,
        21,17,18,
        21,18,19,
        21,19,10*/

    };
    //cylinder
    unsigned int cVAO, cVBO, cEBO;
    glGenVertexArrays(1, &cVAO);
    glGenBuffers(1, &cVBO);
    glGenBuffers(1, &cEBO);

    glBindVertexArray(cVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ver_arr), ver_arr, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind_arr), ind_arr, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    //cube
    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Sphere sphere = Sphere();
    //Tool tool = Tool();


    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------
    SpotLight spotlight[1];// , spotlight1[3];
    PointLight pointlight[1];
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        //spotlight
        spotlight[0].position = glm::vec3(.5, 1.95, -1.5);
        spotlight[0].Number = 0;
        spotlight[0].s_ambient = s_ambient;
        spotlight[0].s_diffuse = s_diffuse;
        spotlight[0].s_specular = s_specular;
        spotlight[0].setUpspotLight(lightingShader);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        //Pointlight
        pointlight[0].position = glm::vec3(1.5, 1.5, -3.7);
        pointlight[0].Number = 0;
        pointlight[0].p_ambient = p_ambient;
        pointlight[0].p_diffuse = p_diffuse;
        pointlight[0].p_specular = p_specular;
        pointlight[0].setUpPointLight(lightingShader);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    
        
        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

        glm::vec3 d_a = glm::vec3(0.3f, 0.3f, 0.3f) * d_ambient;
        glm::vec3 d_d = glm::vec3(0.6f, 0.6f, 0.6f) * d_diffuse;
        glm::vec3 d_s = glm::vec3(1.0f, 1.0f, 1.0f) * d_specular;
        lightingShader.setVec3("direcLight.direction", 4.5f, -3.0f, -4.0f);
        lightingShader.setVec3("direcLight.ambient", d_a);
        lightingShader.setVec3("direcLight.diffuse", d_d);
        lightingShader.setVec3("direcLight.specular", d_s);
        lightingShader.setBool("dlighton", directionallightonOff);


        glm::vec3 e_a = glm::vec3(0.1f, 0.1f, 0.1f) * d_ambient;
        glm::vec3 e_d = glm::vec3(0.8f, 0.8f, 0.8f) * d_diffuse;
        glm::vec3 e_s = glm::vec3(1.0f, 1.0f, 1.0f) * d_specular;
        lightingShader.setVec3("emissionlight.position", 5, 3, 3.0);
        lightingShader.setVec3("emissionlight.ambient", e_a);
        lightingShader.setVec3("emissionlight.diffuse", e_d);
        lightingShader.setVec3("emissionlight.specular", e_s);
        lightingShader.setFloat("emissionlightk_c", 1);
        lightingShader.setFloat("emissionlight.k_l", .08);
        lightingShader.setFloat("emissionlight.k_q", .03);
        lightingShader.setVec3("emissionlight.emission", 0.5f, 0.5f, 0.5f);
        lightingShader.setBool("elighton", false);

        // Modelling Transformation
        
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model,model1,model2;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        //model1 = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        //model2 = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        //lightingShader.setMat4("model", model1);
        //lightingShader.setMat4("model", model2);
        

        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        //for spotlight
        /*
        model = transforamtion(.5, -1.95, -1.5, .5, .5, 0.5);
        ourShader.setMat4("model", model);
        if (spotlightonOff) {
            ourShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
        }
        else {
            ourShader.setVec3("color", glm::vec3(0.0f, 0.0f, 0.0f));
        }
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        */
        //for point light
        /*
        model = transforamtion(1.5, -1.5, -3.7, .02, .02, 0.02);
        ourShader.setMat4("model", model);
        if (pointlightonOff) {
            ourShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
        }
        else {
            ourShader.setVec3("color", glm::vec3(0.0f, 0.0f, 0.0f));
        }
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        //for emission light
        model = transforamtion(5, 3, 3.0, .05, .05, 0.05);
        ourShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        */

        scene(cubeVAO, cVAO, lightingShader, ourShader);

        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawCylinder(unsigned int& cVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    lightingShader.setFloat("material.shininess", 60.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cVAO);
    glDrawElements(GL_TRIANGLES, 120, GL_UNSIGNED_INT, 0);
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    lightingShader.setFloat("material.shininess", 60.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}


void scene(unsigned int& cubeVAO, unsigned int& cVAO, Shader& lightingShader, Shader& ourShader)
{
    
    glm::mat4 model = glm::mat4(1.0f);
   
    //Cylinder
    for (int i = 0; i < 3; i++) {
        model = transforamtion(5.5, -.27, -7+i*3, .1, 0.5, 0.1);
        drawCylinder(cVAO, lightingShader, model, 1.0, 0.0, 0.0);

        model = transforamtion(5.5, .23, -7+i*3, 0.7, 0.1, 0.7);
        drawCylinder(cVAO, lightingShader, model, 1.0, 0.0, 0.0);

    }
    

    //Floor
    model = transforamtion(-2.5, -.8, -9, 10, 0.05, 12);
    drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0);
    
    //front walls
    model = transforamtion(-2.5, -.75, -9, 10, 3.5, 0.1);
    drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0);

    
    //sidewalls
    //left side
    model = transforamtion(-2.5, -.75, -9, .1, 3.5, 12);
    drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0);
    //right side
    model = transforamtion(7.5, -.75, -9, .1, 3.5, 12);
    drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0);
    

    //Rack
    //backside of a rack
    model = transforamtion(-2.35, -0.75, -7, .1, 2.5, 8.5);
    drawCube(cubeVAO, lightingShader, model, 0.7098, 0.694, 0.6627);

    //both side of rack
    //inner side
    model = transforamtion(-2.25, -0.75, -7, 1, 2.5, 0.1);
    drawCube(cubeVAO, lightingShader, model, 0.7098, 0.694, 0.6627);

    //outer side
    model = transforamtion(-2.25, -0.75, 1.4, 1, 2.5, 0.1);
    drawCube(cubeVAO, lightingShader, model, 0.7098, 0.694, 0.6627);

    //3 racks holding utensils
    //first rack
    model = transforamtion(-2.25, -.625, -6.9, 1, .1, 8.3);
    drawCube(cubeVAO, lightingShader, model, 0.7098, 0.694, 0.6627);

    //second rack
    model = transforamtion(-2.25, 0.375, -6.9, 1, .1, 8.3);
    drawCube(cubeVAO, lightingShader, model, 0.7098, 0.694, 0.6627);

    //third rack
    model = transforamtion(-2.25, 1.375, -6.9, 1, .1, 8.3);
    drawCube(cubeVAO, lightingShader, model, 0.7098, 0.694, 0.6627);

    //counter
    model = transforamtion(-0.75, -0.75, -7, .75, 1, 8.9);
    drawCube(cubeVAO, lightingShader, model, 0.2745, 0.13, 0.0588);

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        if (directionallightonOff)
            directionallightonOff = false;
        else
            directionallightonOff = true;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        if (pointlightonOff) {
            pointlightonOff = false;
            p_ambient = 0;
            p_diffuse = 0;
            p_specular = 0;
        }

        else {
            pointlightonOff = true;
            p_ambient = 1;
            p_diffuse = 1;
            p_specular = 1;
        }

    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        if (spotlightonOff) {
            spotlightonOff = false;
            s_ambient = 0;
            s_diffuse = 0;
            s_specular = 0;
        }

        else {
            spotlightonOff = true;
            s_ambient = 1;
            s_diffuse = 1;
            s_specular = 1;
        }

    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        if (ambientonOff) {
            ambientonOff = false;
            if (directionallightonOff) {
                d_ambient = 0;
            }
            if (pointlightonOff) {
                p_ambient = 0;
            }

            if (spotlightonOff) {
                s_ambient = 0;
            }

        }

        else {
            ambientonOff = true;
            if (directionallightonOff) {
                d_ambient = 1;
            }
            if (pointlightonOff) {
                p_ambient = 1;
            }

            if (spotlightonOff) {
                s_ambient = 1;
            }



        }

    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        if (diffuseonOff) {
            diffuseonOff = false;
            if (directionallightonOff) {
                d_diffuse = 0;
            }
            if (pointlightonOff) {
                p_diffuse = 0;
            }

            if (spotlightonOff) {
                s_diffuse = 0;
            }



        }

        else {
            diffuseonOff = true;
            if (directionallightonOff) {
                d_diffuse = 1;
            }
            if (pointlightonOff) {
                p_diffuse = 1;
            }

            if (spotlightonOff) {
                s_diffuse = 1;;
            }



        }

    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        if (specularonOff) {
            specularonOff = false;
            if (directionallightonOff) {
                d_specular = 0;
            }
            if (pointlightonOff) {
                p_specular = 0;
            }

            if (spotlightonOff) {
                s_specular = 0;
            }



        }

        else {
            specularonOff = true;
            if (directionallightonOff) {
                d_specular = 1;
            }
            if (pointlightonOff) {
                p_specular = 1;
            }

            if (spotlightonOff) {
                s_specular = 1;
            }



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


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
