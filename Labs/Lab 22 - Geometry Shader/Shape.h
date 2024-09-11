#ifndef LEARNOPENGL_SHAPE_H
#define LEARNOPENGL_SHAPE_H

#include "Vertex.h"

struct Shape
{
    Shape(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
};

#endif // !LEARNOPENGL_SHAPE_H
