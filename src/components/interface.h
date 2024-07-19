#pragma once

#include<math.cpp>
#include<vector>

// 256 buffer reserve by default if not mentioned
#define INIT_BUFFER_RESERVE 256

namespace Interface
{
    class IRenderableEntity
    {
        public: 
        IRenderableEntity() {   }
        virtual void Render() = 0;
        
        protected:
        String name;
        bool isActive;
        Math::Transform transform;
    };

    template<class T>
    class IBufferBase
    {
        public:
        IBufferBase(uint32_t reserveSize = INIT_BUFFER_RESERVE)   {   m_buffer.reserve(reserveSize);  }

        virtual uint32_t Bind() = 0;
        virtual void Unbind() = 0;
        // Move the data from CPU to GPU
        virtual void Offload() = 0;

        uint32_t GetBufferHandle()  {    return m_bufferHandle;  }
        std::vector<T>& GetBuffer()   {   return m_buffer;    }
        float* GetBufferRaw()   {   return m_buffer.data();    }
        void Clear()    {   m_buffer.clear();   }
        void SetUsage(uint32_t usage)   {   m_usage = usage;    Offload();    }
        void AppendBuffer(std::vector<T> &buffer)  {   m_buffer.insert(m_buffer.end(), buffer.begin(), buffer.end());    }
        void AppendBuffer(T *buffer, uint32_t size)    {    m_buffer.insert(m_buffer.end(), buffer, buffer+size);   }
        void CleanAdd(std::vector<T> &buffer)  {   m_buffer.insert(m_buffer.begin(), buffer.begin(), buffer.end());    }

        std::vector<T> m_buffer;
        uint32_t m_bufferHandle, m_usage;
    };
}