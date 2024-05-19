#include "PCH.h"
#include "Texture.h"

#include "Shader.h"

// Compile this file before using the library (Do it once!)
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
	#include <stb/stb_image.h>
#endif // !STB_IMAGE_IMPLEMENTATION

Texture::Texture(const char* filePath, GLenum format, GLenum slot, GLenum filter)
{
    // Initializes attributes
    m_Size = { 0, 0 };
    m_Column = 0;
    m_Slot = slot;

    // Flips image on load
    stbi_set_flip_vertically_on_load(true);

    // Loads a image from disk
    unsigned char* bytes = stbi_load(filePath, &m_Size[0], &m_Size[1], &m_Column, 0);

    // Generates a texture
    glGenTextures(1, &m_Id);

    // Bind a 2D texture
    glBindTexture(GL_TEXTURE_2D, m_Id);

    // Set-ups the minification and magnification filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    // Set-ups the texture's wrap behavior
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set-ups the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, format, m_Size[0], m_Size[1], 0, format, GL_UNSIGNED_BYTE, bytes);

    // Generates a mipmap for the 2D texture
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free the data loaded from disk
    stbi_image_free(bytes);

    // Unbinds the 2D texture to avoid bugs
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_Id);
}

void Texture::Bind()
{
    glActiveTexture(m_Slot);
    glBindTexture(GL_TEXTURE_2D, m_Id);
}
