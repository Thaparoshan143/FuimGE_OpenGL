#pragma once

#include"./Types.h"

namespace Interface
{
    class ITexture
    {
        public:
        ITexture(String path) { m_path = path;  }

        virtual uint Bind() = 0;
        virtual void Unbind() = 0;
        virtual void Offload() = 0;
        virtual void SetTextureProp(uint texChannel, uint wrapMode, uint texType)
        {
            m_channel = texChannel;  m_wrapMode = wrapMode;  m_type = texType;
        }
        virtual uchar* loadTexture() = 0;

        // contains the handle to texture buffer
        // type of texture like 1d, 2d, 3d...
        // Number of channel to store in buffer
        // How to wrap in (U,V .) or (S, T .) mode
        uint m_textureHandle, m_type, m_channel, m_wrapMode;
        String m_path;
        iVec2 m_imgDim;
        uchar *m_buffer;
    };
}