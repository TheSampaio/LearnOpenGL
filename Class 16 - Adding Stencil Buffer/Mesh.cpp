#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures)
{
	Mesh::vertices = vertices; // Store vertice's data
	Mesh::indices =   indices; // Store indice's data
	Mesh::textures = textures; // Store texture's data

    VAO.Bind();         // Bind the created VAO from "Mesh.h"
    VBO VBO(vertices);  // Create a Vertex Buffer Object and links it to vertices
    EBO EBO(indices);   // Create a Element Buffer Object and links it to indices

    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*) 0);                  // Configure VAO's buffer (Cordinates)
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float))); // Configure VAO's buffer (Normals)
    VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float))); // Configure VAO's buffer (Colors)
    VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float))); // Configure VAO's buffer (Textures)

    VAO.Unbind(); // Unbinds the VAO
    VBO.Unbind(); // Unbinds the VBO
    EBO.Unbind(); // Unbinds the EBO
}

void Mesh::Draw(Shader& shader, Camera& camera, glm::mat4 matrix, glm::vec3 translation, glm::quat rotation, glm::vec3 scale) // Draw call function
{
    shader.Active(); // Active shader program
    VAO.Bind();      // Binds the VAO

    unsigned int numDiffuse = 0;  // To use multiple diffuse textures
    unsigned int numSpecular = 0; // To use multiple specular textures

    for (unsigned int i = 0; i < textures.size(); i++) // Increment system
    {
        std::string num;
        std::string type = textures[i].type;

        if (type == "diffuse")
        {
            num = std::to_string(numDiffuse++);
        }

        else if (type == "specular")
        {
            num = std::to_string(numSpecular++);
        }

        textures[i].texUnit(shader, (type + num).c_str(), i); // Use texture's uniform
        textures[i].Bind();                                   // Binds the texture
    }

    // Enable camera's visualization
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
    camera.Matrix(shader, "camMatrix");

    // Initialize all matrices
    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 rot =   glm::mat4(1.0f);
    glm::mat4 sca =   glm::mat4(1.0f);

    // Transform the matrices to their correct form
    trans = glm::translate(trans, translation);
    rot =   glm::mat4_cast(rotation);
    sca =   glm::scale(sca, scale);

    // Push the matrices to the vertex shader
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(-rot));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

    // Draw what we need
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
}