
//Aluno: Eduardo Colissi Wiceskoski
//Compilar com: g++ m4.cpp -o m4 src/glad.c -I include -L/usr/lib/x86_64-linux-gnu/ -L/usr/lib64 -lGLEW -lGL -lX11 -lglfw -lrt -lm -ldl
// Rodar com: ./m4

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGl", NULL, NULL);
    if (window == NULL) {
        cout << "###Erro: Falha na criação da janela GLFW !" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "###Erro: Falha na inicialização do GLAD !" << endl;
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const int MAX_TEXTURES = 6;

    Shader customShader[MAX_TEXTURES];

    for (int i=0; i<MAX_TEXTURES; i++) {
        customShader[i] = Shader("./shader.vs", "./shader.fs");
    }

    float vertices[] = {
         1.0f,  1.0f, 0.0f,   1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,   1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,   0.0f, 1.0f,
        -1.0f,  1.0f, 0.0f,   0.0f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    string customTexture[MAX_TEXTURES] = {
        "background.png",
        "rock1.png",
        "rock2.png",
        "rock3.png",
        "object1.png",
        "object2.png"
    };

    unsigned int texture[MAX_TEXTURES];
    for (int i=0; i<MAX_TEXTURES; i++) {
        glGenTextures(1, &texture[i]);
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int width, height, nrChannels;
        string rscPath = "./resources/textures/";
        rscPath += customTexture[i];
        unsigned char *data = stbi_load(rscPath.c_str(), &width, &height, &nrChannels, 0); //
        if (data) {
            cout << "rscPath =" << rscPath << " | "; // Debug
            cout << "nrChannels =" << nrChannels << " | "; // Debug
            cout << "width =" << width << " | "; // Debug
            cout << "height =" << height << endl; // Debug
            (nrChannels == 3) ?
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data) :
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load texture: " << customTexture[i] << std::endl;
        }
        stbi_image_free(data);
    }

    while(!glfwWindowShouldClose(window)) {
        glm::mat4 projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);

        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glm::mat4 transform0 = glm::mat4(1.0f);
        transform0 = glm::scale(transform0, glm::vec3(1.235, 1.235, 1.235));
        customShader[0].use();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        unsigned int transformLoc0 = glGetUniformLocation(customShader[0].ID, "transform");
        glUniformMatrix4fv(transformLoc0, 1, GL_FALSE, glm::value_ptr(transform0));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glm::mat4 transform1 = glm::mat4(1.0f);
        transform1 = glm::scale(transform1, glm::vec3(0.1, 0.1, 0.0));

        customShader[0].use();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        unsigned int transformLoc1 = glGetUniformLocation(customShader[0].ID, "transform");
        glUniformMatrix4fv(transformLoc1, 1, GL_FALSE, glm::value_ptr(transform1));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindTexture(GL_TEXTURE_2D, texture[2]);
        glm::mat4 transform2 = glm::mat4(1.0f);
        transform2 = glm::scale(transform2, glm::vec3(0.1, 0.1, 0.0));
        transform2 = glm::translate(transform2, glm::vec3(2.3, -0.8, 0.0));
        customShader[2].use();
        unsigned int transformLoc2 = glGetUniformLocation(customShader[2].ID, "transform");
        glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(transform2));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glm::mat4 transform3 = glm::mat4(1.0f);
        transform3 = glm::scale(transform3, glm::vec3(0.1, 0.1, 0.0));
        transform3 = glm::translate(transform3, glm::vec3(-2.0, 1.5, 0.0));
        customShader[3].use();
        unsigned int transformLoc3 = glGetUniformLocation(customShader[3].ID, "transform");
        glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(transform3));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glm::mat4 transform4 = glm::mat4(1.0f);
        transform4 = glm::scale(transform4, glm::vec3(0.2, 0.2, 0.0));
        transform4 = glm::translate(transform4, glm::vec3(3.0, 2.0, 0.0));
        customShader[4].use();
        unsigned int transformLoc4 = glGetUniformLocation(customShader[4].ID, "transform");
        glUniformMatrix4fv(transformLoc4, 1, GL_FALSE, glm::value_ptr(transform4));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glm::mat4 transform5 = glm::mat4(1.0f);
        transform5 = glm::scale(transform5, glm::vec3(0.2, 0.2, 0.0));
        transform5 = glm::translate(transform5, glm::vec3(0.0, -4.0, 0.0));
        customShader[5].use();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        unsigned int transformLoc5 = glGetUniformLocation(customShader[5].ID, "transform");
        glUniformMatrix4fv(transformLoc5, 1, GL_FALSE, glm::value_ptr(transform5));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
