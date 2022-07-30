#ifndef SKYBOX_CLASS_H
#define SKYBOX_CLASS_H

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stb/stb_image.h>

#include "ShaderClass.h"
#include "Camera.h"

static GLfloat vertices[]
{
    -1.0f, -1.0f, -1.0f, //          4 ---------- 5
     1.0f, -1.0f, -1.0f, //        / |          / |
    -1.0f, -1.0f,  1.0f, //       /  |         /  |
     1.0f, -1.0f,  1.0f, //      6 --|------- 7   |
                         //      |   0 -------|-- 1
    -1.0f,  1.0f, -1.0f, //      |  /         |  /
     1.0f,  1.0f, -1.0f, //      | /          | /
    -1.0f,  1.0f,  1.0f, //      |/           |/
     1.0f,  1.0f,  1.0f  //      2 ---------- 3
};

static GLuint indices[]
{
    // Bottom
    0, 3, 2,
    0, 1, 3,

    // Top
    4, 6, 7,
    7, 5, 4,

    // Left
    3, 1, 7,
    1, 5, 7,

    // Right
    0, 2, 6,
    0, 6, 4,

    // Back
    0, 4, 5,
    0, 5, 1,

    // Front
    2, 3, 6,
    3, 7, 6
};

class Skybox
{
private:
    GLuint VAO, VBO, EBO, texture;
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

public:
    Skybox();

    void Draw(Shader& Shader, Camera& Camera, float FOV = 80.0f);
    void Delete();
    void SetTextures(std::string texturesPath[]);
};


#endif // !SKYBOX_CLASS_H
