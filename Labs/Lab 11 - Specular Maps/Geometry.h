#ifndef LEARNOPENGL_GEOMETRY_H
#define LEARNOPENGL_GEOMETRY_H

struct Shape
{
    Shape(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices) : vertices(vertices), indices(indices) {};

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
};

struct Geometry
{
    static const Shape plane;
    static const Shape pyramid;
};

#endif // !LEARNOPENGL_GEOMETRY_H
