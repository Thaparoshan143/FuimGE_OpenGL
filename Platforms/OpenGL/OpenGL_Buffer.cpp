#include"./OpenGL_Buffer.h"

namespace OpenGL
{
    VertexBufferObject::VertexBufferObject(uint reserveSize) : IBufferBase(reserveSize)
    {
        glGenBuffers(1, &this->m_bufferHandle);
    }

    VertexArrayObject::VertexArrayObject()
    {
        glGenVertexArrays(1, &this->m_bufferHandle);
    }

    void VertexArrayObject::EnableVertexAttrMan(uint count)
    {
        Bind();
        glVertexAttribPointer(0, count, GL_FLOAT, GL_FALSE, count * sizeof(float), 0);
        glEnableVertexAttribArray(0);
    }

    void VertexArrayObject::EnableVertexAttrib()
    {
        Bind();

		// taking all the vertex data and organizing for the shader use case.. first position, color, uv texture, 
		int verStride = getVBOStride();
		glVertexAttribPointer(0, getMask(BUFFER_MASKPPP), GL_FLOAT, GL_FALSE, sizeof(float)*verStride, (const void*)0);
		glEnableVertexAttribArray(0);

		if(getMask(BUFFER_MASKRGB))
		{
			uint colOffset = getMask(BUFFER_MASKPPP)*sizeof(float);
			glVertexAttribPointer(1, ((getMask(BUFFER_MASKRGB))>>3), GL_FLOAT, GL_FALSE, sizeof(float)*verStride, (const void*)(colOffset));
			glEnableVertexAttribArray(1);
		}
		
		if(getMask(BUFFER_MASKTEX))
		{
			uint texOffset = (getMask(BUFFER_MASKPPP)+(getMask(BUFFER_MASKRGB)>>3))*sizeof(float);
			glVertexAttribPointer(2, ((getMask(BUFFER_MASKTEX)>>6)), GL_FLOAT, GL_FALSE, sizeof(float)*verStride, (const void*)(texOffset));
			glEnableVertexAttribArray(2);
		}
    }

    void VertexArrayObject::AppendBufferLink(VertexBufferObject *vbo, IndexBufferObject *ibo = nullptr)
    {
        uint key = vbo->GetBufferHandle();
        BuffersHandle temp;
        temp.vbo = vbo;
        temp.ibo = ibo;
        m_attachBuffers.insert(std::pair<uint, BuffersHandle>(key, temp));
    }

    void VertexArrayObject::SetBufferHandle(uint vboBufferHandle)
    {
        glBindVertexArray(m_bufferHandle);
        if(m_attachBuffers.find(vboBufferHandle) != m_attachBuffers.end())
        {
            BuffersHandle temp = m_attachBuffers[vboBufferHandle];
            if(temp.vbo == nullptr)
            {
                std::cout << "VBO attached is NULL" << std::endl;
            }
            else
            {
                temp.vbo->Bind();
            }
            if(temp.ibo == nullptr)
            {
                std::cout << "No active attached IBO for this element" << std::endl;
            }
            else
            {
                temp.ibo->Bind();
            }
        }
    }

    uint VertexArrayObject::getVBOStride()
    {
		return (getMask(BUFFER_MASKPPP) + (getMask(BUFFER_MASKRGB)>>3) + (getMask(BUFFER_MASKTEX)>>6));
    }
}