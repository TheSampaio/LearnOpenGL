#ifndef FRAMEBUFFER_CLASS_H
#define FRAMEBUFFER_CLASS_H

#include <iostream>
#include <glad/glad.h>
#include "Camera.h"
#include "ShaderClass.h"

static float Vertices[]
{
    // Coords    // texCoords
     1.0f, -1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    -1.0f,  1.0f,  0.0f, 1.0f,

     1.0f,  1.0f,  1.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f,  0.0f, 1.0f
};

class Framebuffer
{
private:
    GLuint VAO, VBO, FBO, RBO, Texture;

public:
    Framebuffer(Camera& camera);
    ~Framebuffer();

    void Start(Camera& camera, glm::vec3 backgroundcolor = glm::vec3{0.0f, 0.0f, 0.0f});
    void Finish(Shader& shader);
    void Delete();
};

#endif
