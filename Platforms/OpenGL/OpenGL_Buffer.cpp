#include"./OpenGL_Buffer.h"

namespace OpenGL
{
    VertexBufferObject::VertexBufferObject(uint reserveSize) : IBufferBase(reserveSize)
    {
        glGenBuffers(1, &this->m_bufferHandle);
        m_usage = GL_DYNAMIC_DRAW;
    }

    void VertexBufferObject::ReserveBuffer(uint size, uint loadMode)
    {
        Bind();
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, NULL, loadMode);
    }

    void VertexBufferObject::LoadSubBuffer(uint size, float *data)
    {
        Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * size, data);
    }

    IndexBufferObject::IndexBufferObject(uint reserveSize) : IBufferBase(reserveSize)
    {
        glGenBuffers(1, &this->m_bufferHandle);
        m_usage = GL_DYNAMIC_DRAW;
    }

    void IndexBufferObject::ReserveBuffer(uint size, uint loadMode)
    {
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * size, NULL, loadMode);
    }

    void IndexBufferObject::LoadSubBuffer(uint size, uint *data)
    {
        Bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint) * size, data);
    }

    VertexArrayObject::VertexArrayObject(BufferFormat bf)
    {
        m_format = bf;
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
        std::cout << "Stride Count : " << verStride << std::endl;
        std::cout << "### Position" << std::endl;
		glVertexAttribPointer(0, getMask(BUFFER_MASKPPP), GL_FLOAT, GL_FALSE, sizeof(float)*verStride, (const void*)0);
		glEnableVertexAttribArray(0);

		if(getMask(BUFFER_MASKRGB))
		{
            std::cout << "### RGB" << std::endl;
			uint colOffset = getMask(BUFFER_MASKPPP)*sizeof(float);
			glVertexAttribPointer(1, ((getMask(BUFFER_MASKRGB))>>3), GL_FLOAT, GL_FALSE, sizeof(float)*verStride, (const void*)(colOffset));
			glEnableVertexAttribArray(1);
		}
		
		if(getMask(BUFFER_MASKTEX))
		{
            std::cout << "### UV" << std::endl;
			uint texOffset = (getMask(BUFFER_MASKPPP)+(getMask(BUFFER_MASKRGB)>>3))*sizeof(float);
			glVertexAttribPointer(2, ((getMask(BUFFER_MASKTEX)>>6)), GL_FLOAT, GL_FALSE, sizeof(float)*verStride, (const void*)(texOffset));
			glEnableVertexAttribArray(2);
		}

		if(getMask(BUFFER_MASKNORMAL))
		{
            std::cout << "### Normal" << std::endl;
			uint norOffset = (getMask(BUFFER_MASKPPP)+(getMask(BUFFER_MASKRGB)>>3)+(getMask(BUFFER_MASKTEX)>>6))*sizeof(float);
			glVertexAttribPointer(3, ((getMask(BUFFER_MASKNORMAL)>>9)), GL_FLOAT, GL_FALSE, sizeof(float)*verStride, (const void*)(norOffset));
			glEnableVertexAttribArray(3);
		}
    }

    void VertexArrayObject::AppendBufferLink(VertexBufferObject *vbo, IndexBufferObject *ibo)
    {
        uint key = vbo->GetBufferHandle();
        BuffersHandle temp;
        temp.vbo = vbo;
        temp.ibo = ibo;
        m_attachBuffers.insert(std::pair<uint, BuffersHandle>(key, temp));
    }

    void VertexArrayObject::SetActiveVBOBufferHandle(uint vboBufferHandle)
    {
        glBindVertexArray(m_bufferHandle);
        if(m_attachBuffers.find(vboBufferHandle) != m_attachBuffers.end())
        {
            BuffersHandle temp = m_attachBuffers[vboBufferHandle];
            if(temp.vbo == nullptr)
            {
                // std::cout << "VBO attached is NULL" << std::endl;
            }
            else
            {
                temp.vbo->Bind();
            }
            if(temp.ibo == nullptr)
            {
                // std::cout << "No active attached IBO for this element" << std::endl;
            }
            else
            {
                temp.ibo->Bind();
            }
        }
    }

    uint VertexArrayObject::getVBOStride()
    {
		return (getMask(BUFFER_MASKPPP) + (getMask(BUFFER_MASKRGB)>>3) + (getMask(BUFFER_MASKTEX)>>6) + (getMask(BUFFER_MASKNORMAL)>>9));
    }
}