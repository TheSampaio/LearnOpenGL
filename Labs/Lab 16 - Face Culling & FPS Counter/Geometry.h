#ifndef LEARNOPENGL_GEOMETRY_H
#define LEARNOPENGL_GEOMETRY_H

#include "Shape.h"

struct Geometry
{
    static const Shape* plane;
    static const Shape* pyramid;

private:
    Geometry() {};

    // Deletes copy contructor and assigment operator
    Geometry(const Geometry&) = delete;
    Geometry operator=(const Geometry&) = delete;
};

#endif // !LEARNOPENGL_GEOMETRY_H
