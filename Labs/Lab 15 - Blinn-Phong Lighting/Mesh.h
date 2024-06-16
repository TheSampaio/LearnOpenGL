#ifndef LEARNOPENGL_MESH_H
#define LEARNOPENGL_MESH_H

#include "Material.h"
#include "Shader.h"
#include "Shape.h"
#include "Transform.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class Mesh
{
public:
	Mesh(const Shape* shape, const Material* material);

	void Draw(Shader& shader, class Transform& transform);

private:
	Material* m_Material;
	Shape* m_Shape;
	VAO* m_VertexArray;
	VBO* m_VertexBuffer;
	EBO* m_ElementBuffer;
};

#endif // !LEARNOPENGL_MESH_H
