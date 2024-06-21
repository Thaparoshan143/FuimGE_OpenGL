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
        void ReserveBuffer(uint size, uint loadMode = GL_DYNAMIC_DRAW);
		void LoadSubBuffer(uint size, float *data);
    };

    class IndexBufferObject : public Interface::IBufferBase<uint>
    {
        public:
        IndexBufferObject(uint reserveSize = INIT_BUFFER_RESERVE);

        uint Bind() override    {   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_bufferHandle);    return m_bufferHandle;    }
        void Unbind() override  {   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);   }
        void Offload() override {   Bind(); glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*m_buffer.size(), this->m_buffer.data(), m_usage);    }
        void ReserveBuffer(uint size, uint loadMode = GL_DYNAMIC_DRAW);
		void LoadSubBuffer(uint size, uint *data);
    };

    typedef struct BuffersHandle
    {
        VertexBufferObject *vbo;
        IndexBufferObject *ibo;
    }BuffersHandle;

    class VertexArrayObject 
    {
        public:
        VertexArrayObject(BufferFormat bf = BufferFormat::PPP_RGB);

        uint Bind() { SetActiveVBOBufferHandle(m_attachBuffers.begin()->first);  glBindVertexArray(m_bufferHandle);  return m_bufferHandle;  }
        uint RawBind()  {   glBindVertexArray(m_bufferHandle);  return m_bufferHandle;  }
        void Unbind()   {   glBindVertexArray(0);   }
        void EnableVertexAttrMan(uint count);
        void EnableVertexAttrib();
        void AppendBufferLink(VertexBufferObject *vbo, IndexBufferObject *ibo);
        void SetActiveVBOBufferHandle(uint vboBufferHandle);
        uint GetBufferHandle() const {  return m_bufferHandle; }
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