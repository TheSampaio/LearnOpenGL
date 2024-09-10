#include "PCH.h"
#include "Shape.h"

Shape::Shape(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
	: vertices(vertices), indices(indices)
{
}
