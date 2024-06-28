#pragma once

#include"../../Core/Base/ITexture.h"

#include"../../includes/GLAD/glad.h"

namespace OpenGL
{
    class Texture : public Interface::ITexture
    {
        public:
        Texture(String path);

        uint Bind() override    {   glActiveTexture(GL_TEXTURE0);   glBindTexture(m_type, m_textureHandle);  return m_textureHandle;    }
        void Unbind() override  {   glBindTexture(m_type, 0);    }
        void Offload() override;
        void SetTextureProp(uint texChannel, uint wrapMode = GL_REPEAT, uint texType = GL_TEXTURE_2D) override;
        void GenMipMap()    {   glGenerateMipmap(m_type);   }

        protected:
        uchar* loadTexture() override;
    };
}