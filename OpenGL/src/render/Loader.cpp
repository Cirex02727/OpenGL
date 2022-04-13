#include "Loader.h"

#include "Renderer.h"
#include "vendor/stb_image/stb_image.h"

std::vector<unsigned int> Loader::m_Textures;

unsigned int Loader::loadTexture(const std::string& path)
{
    int width, height, channels;

    stbi_set_flip_vertically_on_load(1);
    unsigned char* buffer = stbi_load(path.c_str(), &width, &height, &channels, 4);

    unsigned int id;
    GLCall(glGenTextures(1, &id));
    GLCall(glBindTexture(GL_TEXTURE_2D, id));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer));
    //GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if (buffer)
        stbi_image_free(buffer);

    Loader::m_Textures.push_back(id);

    return id;
}

void Loader::unloadResources()
{
    for (const auto& id : Loader::m_Textures)
    {
        GLCall(glDeleteTextures(1, &id));
    }
}
