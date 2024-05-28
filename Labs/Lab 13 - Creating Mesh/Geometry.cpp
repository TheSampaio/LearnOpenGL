#include "PCH.h"
#include "Geometry.h"

#include "Vertex.h"

const std::vector<Vertex> planeVertices
{
    // === Positions                        // === Colours                  // === Normals                  // === UVs              // === Faces
    Vertex{glm::vec3{-0.5f, 0.0f,  0.5f},   glm::vec3{1.0f, 1.0f, 1.0f},    glm::vec3{0.0f, 1.0f, 0.0f},    glm::vec2{0.0f, 0.0f}}, // Bottom side
    Vertex{glm::vec3{-0.5f, 0.0f, -0.5f},   glm::vec3{1.0f, 1.0f, 1.0f},    glm::vec3{0.0f, 1.0f, 0.0f},    glm::vec2{0.0f, 5.0f}}, // Bottom side
    Vertex{glm::vec3{ 0.5f, 0.0f, -0.5f},   glm::vec3{1.0f, 1.0f, 1.0f},    glm::vec3{0.0f, 1.0f, 0.0f},    glm::vec2{5.0f, 5.0f}}, // Bottom side
    Vertex{glm::vec3{ 0.5f, 0.0f,  0.5f},   glm::vec3{1.0f, 1.0f, 1.0f},    glm::vec3{0.0f, 1.0f, 0.0f},    glm::vec2{5.0f, 0.0f}}  // Bottom side
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
    Vertex{ glm::vec3{ -0.5f, 0.0f, -0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ 0.0f, -1.0f, 0.0f },  glm::vec2{ 0.0f, 5.0f } }, // Bottom side
    Vertex{ glm::vec3{  0.5f, 0.0f, -0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ 0.0f, -1.0f, 0.0f },  glm::vec2{ 5.0f, 5.0f } }, // Bottom side
    Vertex{ glm::vec3{  0.5f, 0.0f,  0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ 0.0f, -1.0f, 0.0f },  glm::vec2{ 5.0f, 0.0f } }, // Bottom side

    Vertex{ glm::vec3{ -0.5f, 0.0f,  0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ -0.8f, 0.5f, 0.0f },  glm::vec2{ 0.0f, 0.0f } },    // Left Side
    Vertex{ glm::vec3{ -0.5f, 0.0f, -0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ -0.8f, 0.5f, 0.0f },  glm::vec2{ 5.0f, 0.0f } },    // Left Side
    Vertex{ glm::vec3{  0.0f, 0.8f,  0.0f },  glm::vec3{ 1.0f, 1.0f, 1.0f },  glm::vec3{ -0.8f, 0.5f, 0.0f },  glm::vec2{ 2.5f, 5.0f } },    // Left Side

    Vertex{ glm::vec3{ -0.5f, 0.0f, -0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ 0.0f, 0.5f, -0.8f },  glm::vec2{ 5.0f, 0.0f } },    // Back side
    Vertex{ glm::vec3{  0.5f, 0.0f, -0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ 0.0f, 0.5f, -0.8f },  glm::vec2{ 0.0f, 0.0f } },    // Back side
    Vertex{ glm::vec3{  0.0f, 0.8f,  0.0f },  glm::vec3{ 1.0f, 1.0f, 1.0f },  glm::vec3{ 0.0f, 0.5f, -0.8f },  glm::vec2{ 2.5f, 5.0f } } ,    // Back side

    Vertex{ glm::vec3{  0.5f, 0.0f, -0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ 0.8f, 0.5f,  0.0f },  glm::vec2{ 0.0f, 0.0f } },    // Right side
    Vertex{ glm::vec3{  0.5f, 0.0f,  0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ 0.8f, 0.5f,  0.0f },  glm::vec2{ 5.0f, 0.0f } },    // Right side
    Vertex{ glm::vec3{  0.0f, 0.8f,  0.0f },  glm::vec3{ 1.0f, 1.0f, 1.0f },  glm::vec3{ 0.8f, 0.5f,  0.0f },  glm::vec2{ 2.5f, 5.0f } },    // Right side

    Vertex{ glm::vec3{  0.5f, 0.0f,  0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ 0.0f, 0.5f,  0.8f },  glm::vec2{ 5.0f, 0.0f } },    // Front side
    Vertex{ glm::vec3{ -0.5f, 0.0f,  0.5f },  glm::vec3{ 0.8f, 0.8f, 0.8f },  glm::vec3{ 0.0f, 0.5f,  0.8f },  glm::vec2{ 0.0f, 0.0f } },    // Front side
    Vertex{ glm::vec3{  0.0f, 0.8f,  0.0f },  glm::vec3{ 1.0f, 1.0f, 1.0f },  glm::vec3{ 0.0f, 0.5f,  0.8f },  glm::vec2{ 2.5f, 5.0f } }     // Front side
};

// Indices's dynamic array
const std::vector<GLuint> pyramidIndices
{
    0, 1, 2,    // Bottom side
    0, 2, 3,    // Bottom side
    4, 6, 5,    // Left side
    7, 9, 8,    // Back side
    10, 12, 11, // Right side
    13, 15, 14  // Front side
};

// Pass array to a static shape
const Shape* Geometry::pyramid = new Shape(pyramidVertices, pyramidIndices);
