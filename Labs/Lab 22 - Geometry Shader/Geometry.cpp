#include "PCH.h"
#include "Geometry.h"

#include "Vertex.h"

const std::vector<Vertex> planeVertices
{
    // === Positions                        // === Colours                  // === Normals                  // === UVs              // === Faces
    Vertex{glm::vec3{-0.5f, 0.0f,  0.5f},   glm::vec3{1.0f, 1.0f, 1.0f},    glm::vec3{0.0f, 1.0f, 0.0f},    glm::vec2{0.0f, 0.0f}}, // Bottom side
    Vertex{glm::vec3{-0.5f, 0.0f, -0.5f},   glm::vec3{1.0f, 1.0f, 1.0f},    glm::vec3{0.0f, 1.0f, 0.0f},    glm::vec2{0.0f, 1.0f}}, // Bottom side
    Vertex{glm::vec3{ 0.5f, 0.0f, -0.5f},   glm::vec3{1.0f, 1.0f, 1.0f},    glm::vec3{0.0f, 1.0f, 0.0f},    glm::vec2{1.0f, 1.0f}}, // Bottom side
    Vertex{glm::vec3{ 0.5f, 0.0f,  0.5f},   glm::vec3{1.0f, 1.0f, 1.0f},    glm::vec3{0.0f, 1.0f, 0.0f},    glm::vec2{1.0f, 0.0f}}  // Bottom side
};

// Indices's dynamic array
const std::vector<GLuint> planeIndices
{
    0, 1, 2,    // Bottom side
    0, 2, 3,    // Bottom side
};

// Pass array to a static shape
const Shape* Geometry::plane = new Shape(planeVertices, planeIndices);

// Vertices's dynamic array
const std::vector<Vertex> pyramidVertices
{
    // === Positions                           // === Colours                    // === Normals                    // === UVs                 // === Faces
    Vertex{ glm::vec3{ -0.5f, 0.0f,  0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ 0.0f, -1.0f, 0.0f },  glm::vec2{ 0.0f, 0.0f } }, // Bottom side
    Vertex{ glm::vec3{ -0.5f, 0.0f, -0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ 0.0f, -1.0f, 0.0f },  glm::vec2{ 0.0f, 1.0f } }, // Bottom side
    Vertex{ glm::vec3{  0.5f, 0.0f, -0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ 0.0f, -1.0f, 0.0f },  glm::vec2{ 1.0f, 1.0f } }, // Bottom side
    Vertex{ glm::vec3{  0.5f, 0.0f,  0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ 0.0f, -1.0f, 0.0f },  glm::vec2{ 1.0f, 0.0f } }, // Bottom side

    Vertex{ glm::vec3{ -0.5f, 0.0f,  0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ -0.8f, 0.5f, 0.0f },  glm::vec2{ 0.0f, 0.0f } },    // Left Side
    Vertex{ glm::vec3{ -0.5f, 0.0f, -0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ -0.8f, 0.5f, 0.0f },  glm::vec2{ 1.0f, 0.0f } },    // Left Side
    Vertex{ glm::vec3{  0.0f, 0.8f,  0.0f },  glm::vec3{ 1.0f, 1.0f, 1.0f },  glm::vec3{ -0.8f, 0.5f, 0.0f },  glm::vec2{ 2.5f, 1.0f } },    // Left Side

    Vertex{ glm::vec3{ -0.5f, 0.0f, -0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ 0.0f, 0.5f, -0.8f },  glm::vec2{ 1.0f, 0.0f } },    // Back side
    Vertex{ glm::vec3{  0.5f, 0.0f, -0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ 0.0f, 0.5f, -0.8f },  glm::vec2{ 0.0f, 0.0f } },    // Back side
    Vertex{ glm::vec3{  0.0f, 0.8f,  0.0f },  glm::vec3{ 1.0f, 1.0f, 1.0f },  glm::vec3{ 0.0f, 0.5f, -0.8f },  glm::vec2{ 2.5f, 1.0f } } ,    // Back side

    Vertex{ glm::vec3{  0.5f, 0.0f, -0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ 0.8f, 0.5f,  0.0f },  glm::vec2{ 0.0f, 0.0f } },    // Right side
    Vertex{ glm::vec3{  0.5f, 0.0f,  0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ 0.8f, 0.5f,  0.0f },  glm::vec2{ 1.0f, 0.0f } },    // Right side
    Vertex{ glm::vec3{  0.0f, 0.8f,  0.0f },  glm::vec3{ 1.0f, 1.0f, 1.0f },  glm::vec3{ 0.8f, 0.5f,  0.0f },  glm::vec2{ 2.5f, 1.0f } },    // Right side

    Vertex{ glm::vec3{  0.5f, 0.0f,  0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ 0.0f, 0.5f,  0.8f },  glm::vec2{ 1.0f, 0.0f } },    // Front side
    Vertex{ glm::vec3{ -0.5f, 0.0f,  0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ 0.0f, 0.5f,  0.8f },  glm::vec2{ 0.0f, 0.0f } },    // Front side
    Vertex{ glm::vec3{  0.0f, 0.8f,  0.0f },  glm::vec3{ 1.0f, 1.0f, 1.0f },  glm::vec3{ 0.0f, 0.5f,  0.8f },  glm::vec2{ 2.5f, 1.0f } }     // Front side
};

// Indices's dynamic array
const std::vector<GLuint> pyramidIndices
{
    2, 1, 0,    // Bottom side
    3, 2, 0,    // Bottom side
    5, 6, 4,    // Left side
    8, 9, 7,    // Back side
    11, 12, 10, // Right side
    14, 15, 13  // Front side
};

// Pass array to a static shape
const Shape* Geometry::pyramid = new Shape(pyramidVertices, pyramidIndices);
