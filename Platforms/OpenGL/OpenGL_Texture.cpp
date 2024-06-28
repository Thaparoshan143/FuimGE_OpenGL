#include"./OpenGL_Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include"../../includes/stb/stb_image.h"

namespace OpenGL
{
    Texture::Texture(String path) : ITexture(path)
    {
        glGenTextures(1, &m_textureHandle);
        m_type = GL_TEXTURE_2D;
        m_channel = 3;
        m_wrapMode = GL_REPEAT;
    }

    void Texture::SetTextureProp(uint texChannel, uint wrapMode, uint texType)
    {
        m_channel = texChannel;
        m_wrapMode = wrapMode;
        m_type = texType;
    }

    void Texture::Offload()
    {
        Bind();
        m_buffer = loadTexture();
        glTexParameteri(m_type, GL_TEXTURE_WRAP_S, m_wrapMode);
        glTexParameteri(m_type, GL_TEXTURE_WRAP_T, m_wrapMode);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if(m_buffer != nullptr)
        {
            glTexImage2D(m_type, 0, GL_RGBA, m_imgDim.x, m_imgDim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_buffer);
        }
        else
        {
            std::cout << "unable to assign to texture buffer";
        }
        stbi_image_free(m_buffer);
        m_buffer = nullptr;
    }

    uchar* Texture::loadTexture()
    {
        stbi_set_flip_vertically_on_load(true);
        return stbi_load(m_path.c_str(), &m_imgDim.x, &m_imgDim.y, (int*)(&m_channel), 0);
    }
}