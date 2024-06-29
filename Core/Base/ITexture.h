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
        virtual void SetTextureProp(uint texChannel, uint wrapMode, uint texTarget)
        {
            m_channel = texChannel;  m_wrapMode = wrapMode;  m_target = texTarget;
        }
        virtual uchar* loadTexture() = 0;
        void SetType(String type)   {   m_type = type;  }
        String& GetType()   {   return m_type;  }
        bool IsOfType(String check) const  {   return m_type == check;   }
        void SetSlot(uint slot)  {   m_slot = slot;  }

        // contains the handle to texture buffer
        // target of texture like 1d, 2d, 3d...
        // Number of channel to store in buffer
        // How to wrap in (U,V .) or (S, T .) mode
        // slot is texture number to activate/refer to
        uint m_textureHandle, m_target, m_channel, m_wrapMode, m_slot;
        String m_path;
        String m_type;
        iVec2 m_imgDim;
        uchar *m_buffer;
    };
}