#pragma once

#include"../../includes/GLAD/glad.h"
#include"../../includes/GLFW/glfw3.h"

#include"../../Core/Base/IBuffer.h"
#include<map>

namespace OpenGL
{
    class VertexBufferObject : public Interface::IBufferBase<float>
    {
        public:
        VertexBufferObject(uint reserveSize = INIT_BUFFER_RESERVE);

        uint Bind() override    {   glBindBuffer(GL_ARRAY_BUFFER, this->m_bufferHandle);    return m_bufferHandle;    }
        void Unbind() override  {   glBindBuffer(GL_ARRAY_BUFFER, 0);  }
        void Offload() override {   Bind(); glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_buffer.size(), this->m_buffer.data(), m_usage);  }
    };

    class IndexBufferObject : public Interface::IBufferBase<uint>
    {
        public:
        IndexBufferObject(uint reserveSize);

        uint Bind() override    {   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_bufferHandle);    return m_bufferHandle;    }
        void Unbind() override  {   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);   }
        void Offload() override {   Bind(); glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*m_buffer.size(), this->m_buffer.data(), m_usage);    }
    };

    typedef struct BuffersHandle
    {
        VertexBufferObject *vbo;
        IndexBufferObject *ibo;
    }BuffersHandle;

    class VertexArrayObject 
    {
        public:
        VertexArrayObject();

        uint Bind() { SetBufferHandle(m_attachBuffers.begin()->first);  return m_bufferHandle;  }
        uint RawBind()  {   glBindVertexArray(m_bufferHandle);  return m_bufferHandle;  }
        void Unbind()   {   glBindVertexArray(0);   }
        void EnableVertexAttrMan(uint count);
        void EnableVertexAttrib();
        void AppendBufferLink(VertexBufferObject *vbo, IndexBufferObject *ibo);
        void SetBufferHandle(uint vboBufferHandle);
        void SetFormat(BufferFormat bf) {   m_format = bf;  }
        void ClearBufferLink()  {   m_attachBuffers.clear();    }

        protected:
        uint m_bufferHandle;
        BufferFormat m_format;
        std::map<uint, BuffersHandle> m_attachBuffers; 

        uint getVBOStride();

        uint getMask(uint maskFormat)  {    return m_format & maskFormat;   }
    };
}