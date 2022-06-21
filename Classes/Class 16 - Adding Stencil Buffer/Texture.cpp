#include "Texture.h"

Texture::Texture(const char* image, const char* texType, GLuint slot)
{
    type = texType; // Storing texture's type

    int widthImg, heightImg, numColChannel; // Defining the texture's width, height and number column channel
    stbi_set_flip_vertically_on_load(true); // To avoid inverted textures

    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColChannel, 0); // Create bytes data

    glGenTextures(1, &ID);                  // Generating texture
    glActiveTexture(GL_TEXTURE0 + slot);    // Chooseing number column channel slot
    unit = slot;
    glBindTexture(GL_TEXTURE_2D, ID);             // Binding texture with texture type

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);               // Setting textures's MIN filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Setting textures's MAG filter

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Setting textures's wrap for S axis (X)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Setting textures's wrap for T axis (Y)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT); // Setting textures's wrap for R axis (Z)

    // If you want to use GL_CLAMP_TO_BORDER
    // float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

    if (numColChannel == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    }

    else if (numColChannel == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
    }

    else if (numColChannel == 1)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);
    }

    else
    {
        throw std::invalid_argument("Automatic texture type recognition failed");
    }

    glGenerateMipmap(GL_TEXTURE_2D);  // Generating texture's mipmap

    stbi_image_free(bytes);     // Delete bytes that was used
    glBindTexture(GL_TEXTURE_2D, 0);  // Unbinds texture so that it can't accidentally be modified
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
    GLuint texUni = glGetUniformLocation(shader.ID, uniform); // Get the uniform's location
    
    shader.Active();          // Shader needs to be activated before changing the value of a uniform
    glUniform1i(texUni, unit);  // Set a value for the uniform
}

void Texture::Bind()
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID); // Binds texture
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0); // Unbinds texture
}

void Texture::Delete()
{
    glDeleteTextures(1, &ID);  // Delete texture
}