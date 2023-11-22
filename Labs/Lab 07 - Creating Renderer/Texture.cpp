#include "PCH.h"
#include "Texture.h"

#include "Shader.h"

// Compile this file before using the library (Do it once!)
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
	#include <stb/stb_image.h>
#endif // !STB_IMAGE_IMPLEMENTATION

Texture::Texture(const char* FilePath, GLenum Slot, GLenum Format, GLenum Filter)
{
    // Initializes attributes
    m_Size = { 0, 0 };
    m_Column = 0;

    // Flips image on load
    stbi_set_flip_vertically_on_load(true);

    // Loads a image from disk
    unsigned char* Bytes = stbi_load(FilePath, &m_Size[0], &m_Size[1], &m_Column, 0);

    // Generates a texture
    glGenTextures(1, &m_Id);

    // Activates the texture slot 0
    glActiveTexture(Slot);

    // Bind a 2D texture
    glBindTexture(GL_TEXTURE_2D, m_Id);

    // Set-ups the minification and magnification filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter);

    // Set-ups the texture's wrap behavior
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set-ups the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, Format, m_Size[0], m_Size[1], 0, Format, GL_UNSIGNED_BYTE, Bytes);

    // Generates a mipmap for the 2D texture
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free the data loaded from disk
    stbi_image_free(Bytes);

    // Unbinds the 2D texture to avoid bugs
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_Id);
}
