#include "PCH.h"
#include "Mesh.h"

#include "Renderer.h"
#include "Vertex.h"

Mesh::Mesh(const Shape* shape, const Material* material)
	: m_Shape(nullptr), m_Material(nullptr), m_VertexArray(nullptr), m_VertexBuffer(nullptr), m_ElementBuffer(nullptr)
{
	m_Shape = const_cast<Shape*>(shape);
	m_Material = const_cast<Material*>(material);

	// Creates all buffer objects
	m_VertexArray = new VAO();
	m_VertexBuffer = new VBO(m_Shape->vertices);
	m_ElementBuffer = new EBO(m_Shape->indices);

	// Set-ups VAO's layouts
	m_VertexArray->AttribPointer(0, 3, sizeof(Vertex), 0);									// Position
	m_VertexArray->AttribPointer(1, 3, sizeof(Vertex), offsetof(Vertex, Vertex::colour));	// Colour
	m_VertexArray->AttribPointer(2, 3, sizeof(Vertex), offsetof(Vertex, Vertex::normal));	// Normal
	m_VertexArray->AttribPointer(3, 2, sizeof(Vertex), offsetof(Vertex, Vertex::texture));	// UV

	// Unbinds all buffers to avoid bugs
	m_VertexBuffer->Unbind();
	m_VertexArray->Unbind();
	m_ElementBuffer->Unbind();
}

void Mesh::Draw(Shader& shader, Transform& transform)
{
	m_VertexArray->Bind();
	m_Material->Bind(shader);

	transform.Update(shader);
	Renderer::GetInstance().Draw(m_Shape->indices);

	m_Material->Unbind();
	m_VertexArray->Unbind();
}
