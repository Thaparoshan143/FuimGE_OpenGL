#ifndef BUFFER_CPP
#define BUFFER_CPP

#include<interface.h>
#include<glad.h>

// 256 buffer reserve by default if not mentioned
#define INIT_BUFFER_RESERVE 256

// Buffer storage format - position..color..uv..normal
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

namespace Component
{
    template<class T, uint32_t buffType>
    class BufferObject
    {
        public:
        BufferObject(uint32_t type = buffType, uint32_t reserveSize = INIT_BUFFER_RESERVE)
        {
            glGenBuffers(1, &this->m_bufferHandle);
            m_type = type;
            m_buffer.reserve(INIT_BUFFER_RESERVE);
        }

        uint32_t GetBufferHandle() const                {   return m_bufferHandle;  }
        std::vector<T>& GetBuffer()                     {   return m_buffer;    }
        uint32_t GetBufferSize()                        {   return m_buffer.size(); }
        void Clear()                                    {   m_buffer.clear();   }
        void AppendBuffer(std::vector<T> &buffer)       {   m_buffer.insert(m_buffer.end(), buffer.begin(), buffer.end());    }
        void AppendBuffer(T *buffer, uint32_t size)     {   m_buffer.insert(m_buffer.end(), buffer, buffer+size);   }

        inline uint32_t Bind() const                    {   glBindBuffer(m_type, this->m_bufferHandle);    return m_bufferHandle;    }
        void Unbind() const                             {   glBindBuffer(m_type, 0);  }
        void Offload(uint32_t usage=GL_STATIC_DRAW)     {   Bind(); glBufferData(m_type, sizeof(T)*m_buffer.size(), m_buffer.data(), usage);  }
		void LoadSubBuffer(T *data, uint32_t size)      {   Bind(); glBufferSubData(m_type, 0, sizeof(T) * size, data); }
        void ReserveBuffer(uint32_t size, uint32_t usage = GL_STATIC_DRAW)  {   Bind();    glBufferData(m_type, sizeof(T) * size, NULL, usage);    }

        protected:
        std::vector<T> m_buffer;
        uint32_t m_bufferHandle, m_type;
    };

    typedef BufferObject<float, GL_ARRAY_BUFFER> VertexBufferObject;
    typedef BufferObject<uint32_t, GL_ELEMENT_ARRAY_BUFFER> IndexBufferObject;

    class VertexArrayObject 
    {
        public:
        VertexArrayObject(BufferFormat bf = BufferFormat::PPP_RGB)
        {
            m_format = bf;
            glGenVertexArrays(1, &this->m_bufferHandle);
        }

        uint32_t Bind() { glBindVertexArray(m_bufferHandle);  return m_bufferHandle;  }
        void Unbind()   {   glBindVertexArray(0);   }
        void EnableVertexAttrMan(uint32_t count)
        {
            Bind();
            glVertexAttribPointer(0, count, GL_FLOAT, GL_FALSE, count * sizeof(float), 0);
            glEnableVertexAttribArray(0);
        }
        void EnableVertexAttrib()
        {
            Bind();

            // taking all the vertex data and organizing for the shader use case.. first position, color, uv texture, 
            int verStride = getVBOStride();
            // std::cout << "Stride Count : " << verStride << std::endl;
            // std::cout << "### Position" << std::endl;
            glVertexAttribPointer(0, getMask(BUFFER_MASKPPP), GL_FLOAT, GL_FALSE, sizeof(float)*verStride, (const void*)0);
            glEnableVertexAttribArray(0);

            if(getMask(BUFFER_MASKRGB))
            {
                // std::cout << "### RGB" << std::endl;
                uint colOffset = getMask(BUFFER_MASKPPP)*sizeof(float);
                glVertexAttribPointer(1, ((getMask(BUFFER_MASKRGB))>>3), GL_FLOAT, GL_FALSE, sizeof(float)*verStride, (const void*)(colOffset));
                glEnableVertexAttribArray(1);
            }
            
            if(getMask(BUFFER_MASKTEX))
            {
                // std::cout << "### UV" << std::endl;
                uint texOffset = (getMask(BUFFER_MASKPPP)+(getMask(BUFFER_MASKRGB)>>3))*sizeof(float);
                glVertexAttribPointer(2, ((getMask(BUFFER_MASKTEX)>>6)), GL_FLOAT, GL_FALSE, sizeof(float)*verStride, (const void*)(texOffset));
                glEnableVertexAttribArray(2);
            }

            if(getMask(BUFFER_MASKNORMAL))
            {
                // std::cout << "### Normal" << std::endl;
                uint norOffset = (getMask(BUFFER_MASKPPP)+(getMask(BUFFER_MASKRGB)>>3)+(getMask(BUFFER_MASKTEX)>>6))*sizeof(float);
                glVertexAttribPointer(3, ((getMask(BUFFER_MASKNORMAL)>>9)), GL_FLOAT, GL_FALSE, sizeof(float)*verStride, (const void*)(norOffset));
                glEnableVertexAttribArray(3);
            }
        }
        uint32_t GetBufferHandle() const    {  return m_bufferHandle; }
        void SetFormat(BufferFormat bf)     {   m_format = bf;  }

        protected:
        uint32_t m_bufferHandle;
        BufferFormat m_format;

        uint32_t getVBOStride()
        {
		    return (getMask(BUFFER_MASKPPP) + (getMask(BUFFER_MASKRGB)>>3) + (getMask(BUFFER_MASKTEX)>>6) + (getMask(BUFFER_MASKNORMAL)>>9));
        }
        uint32_t getMask(uint32_t maskFormat)  {    return m_format & maskFormat;   }
    };
}
#endif