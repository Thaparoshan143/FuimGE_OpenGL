#pragma once

#include"./Types.h"

// 256 buffer reserve by default if not mentioned
#define INIT_BUFFER_RESERVE 256

// Buffer storage format - position..color..uv
// 3 bit mask arrangment to find appropriate ..
typedef enum BufferFormat
{
    PP = 2,
    PPP = 3,
    PPPP = 4,

    PP_RGB = 26,
    PPP_RGB = 27,
    PPPP_RGB = 28,

    PP_RGBA = 34,
    PPP_RGBA = 35,
    PPPP_RGBA = 36,

    PP_RGB_UV = 154,
    PPP_RGB_UV = 155,
    PPPP_RGB_UV = 156,

    PPP_UV = 131,

    PPP_NNN = 1539,
    PPP_UV_NNN = 1667,
}BufferFormat;

#define BUFFER_MASKPPP 0x00000007
#define BUFFER_MASKRGB 0x00000038
#define BUFFER_MASKTEX 0x000001C0
#define BUFFER_MASKNORMAL 0x00000E00

namespace Interface
{
    // this is for single buffer system use for vertex, color, uv ...
    template<class T>
    class IBufferBase
    {
        public:
        IBufferBase(uint reserveSize = INIT_BUFFER_RESERVE)   {   m_buffer.reserve(reserveSize);  }

        virtual uint Bind() = 0;
        virtual void Unbind() = 0;
        // Move the data from CPU to GPU
        virtual void Offload() = 0;
        uint GetBufferHandle()  {    return m_bufferHandle;  }
        std::vector<T>& GetBufferVector()   {   return m_buffer;    }
        float* GetBufferRaw()   {   return m_buffer.data();    }
        void Clear()    {   m_buffer.clear();   }
        void SetUsage(uint usage)   {   m_usage = usage;    Offload();    }
        void AppendBuffer(std::vector<T> &buffer)  {   m_buffer.insert(m_buffer.end(), buffer.begin(), buffer.end());    }
        void AppendBuffer(T *buffer, uint size)    {    m_buffer.insert(m_buffer.end(), buffer, buffer+size);   }
        void CleanAdd(std::vector<T> &buffer)  {   m_buffer.insert(m_buffer.begin(), buffer.begin(), buffer.end());    }

        std::vector<T> m_buffer;
        uint m_bufferHandle, m_usage;
    };
}