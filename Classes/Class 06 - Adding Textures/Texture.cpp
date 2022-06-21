#include "Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
    type = texType; // Storing texture's type

    int widthImg, heightImg, numColChannel; // Defining the texture's width, height and number column channel
    stbi_set_flip_vertically_on_load(true); // To avoid inverted textures

    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColChannel, 0); // Create bytes data

    glGenTextures(1, &ID);      // Generating texture
    glActiveTexture(slot);      // Chooseing number column channel slot
    glBindTexture(texType, ID); // Binding texture with texture type

    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Setting textures's mode (LINEAR of NEAREST) for MIN filter
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Setting textures's mode (LINEAR of NEAREST) for MAG filter

    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT); // Setting textures's wrap for S axis (X)
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT); // Setting textures's wrap for T axis (Y)
    glTexParameteri(texType, GL_TEXTURE_WRAP_R, GL_REPEAT); // Setting textures's wrap for R axis (Z)

    // If you want to use GL_CLAMP_TO_BORDER
    // float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    // glTexParameterfv(texType, GL_TEXTURE_BORDER_COLOR, flatColor);

    glTexImage2D(texType, 0, GL_RGB, widthImg, heightImg, 0, format, pixelType, bytes); // Setting texture's data

    glGenerateMipmap(texType);  // Generating texture's mipmap

    stbi_image_free(bytes);     // Delete bytes that was used
    glBindTexture(texType, 0);  // Unbinds texture so that it can't accidentally be modified
}

void Texture::texUnit(Shader shader, const char* uniform, GLuint unit)
{
    GLuint texUni = glGetUniformLocation(shader.ID, uniform); // Get the uniform's location
    
    shader.Active();            // Shader needs to be activated before changing the value of a uniform
    glUniform1i(texUni, unit);  // Set a value for the uniform
}

void Texture::Bind()
{
    glBindTexture(type, ID); // Binds texture
}

void Texture::Unbind()
{
    glBindTexture(type, 0); // Unbinds texture
}

void Texture::Delete()
{
    glDeleteTextures(1, &ID);  // Delete texture
}