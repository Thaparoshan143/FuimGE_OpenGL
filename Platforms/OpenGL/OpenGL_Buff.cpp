#include"OpenGL_Buff.h"

namespace OpenGL
{
	// Vertex Buffer Object part
	OpenGL_VertBuffObj::OpenGL_VertBuffObj(uint size) : VertexBufferObject(size)
	{
		glGenBuffers(1, &this->m_id);
	}

	void OpenGL_VertBuffObj::LoadBuffer(uint loadMode)
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->m_id);
		glBufferData(GL_ARRAY_BUFFER, this->m_data.size() * sizeof(float), this->m_data.data(), loadMode);
	}

	void OpenGL_VertBuffObj::DirectLoad(float *data, uint count, uint loadMode)
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->m_id);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, loadMode);
	}

	void OpenGL_VertBuffObj::ReserveBuffer(uint size, uint loadMode)
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->m_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, NULL, loadMode);
	}

	void OpenGL_VertBuffObj::LoadSubBuffer(uint size, float *data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->m_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * size, data);
	}

	uint OpenGL_VertBuffObj::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->m_id);
		return this->m_id;
	}

	void OpenGL_VertBuffObj::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Index Buffer Object part
	OpenGL_IndBuffObj::OpenGL_IndBuffObj(uint size) : VertexBufferObject(size)
	{
		glGenBuffers(1, &this->m_id);
	}

	void OpenGL_IndBuffObj::LoadBuffer(uint loadMode)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_data.size() * sizeof(uint), this->m_data.data(), loadMode);
	}

	void OpenGL_IndBuffObj::DirectLoad(uint *data, uint count, uint loadMode)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint), data, loadMode);
	}

	void OpenGL_IndBuffObj::ReserveBuffer(uint size, uint loadMode)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * size, NULL, loadMode);
	}

	void OpenGL_IndBuffObj::LoadSubBuffer(uint size, uint *data)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint) * size, data);
	}

	uint OpenGL_IndBuffObj::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id);
		return this->m_id;
	}

	void OpenGL_IndBuffObj::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// Vertex Array Object part
	OpenGL_VertArrObj::OpenGL_VertArrObj(Abstract::BufferFormat bf) : VertexArrayObject(bf)
	{
		glGenVertexArrays(1, &m_id);
	}

	uint OpenGL_VertArrObj::Bind()
	{
		glBindVertexArray(m_id);
		return m_id;
	}

	void OpenGL_VertArrObj::Unbind()
	{
		glBindVertexArray(0);
	}

	uint OpenGL_VertArrObj::getVBOStride()
	{
		return (getMask(BUFFER_MASKPPP) + (getMask(BUFFER_MASKRGB)>>3) + (getMask(BUFFER_MASKTEX)>>6));
	}

	void OpenGL_VertArrObj::EnableVertexAttribMan(uint count)
	{
		glBindVertexArray(this->m_id);
		glVertexAttribPointer(0, count, GL_FLOAT, GL_FALSE, count * sizeof(float), 0);
		glEnableVertexAttribArray(0);
	}

	void OpenGL_VertArrObj::EnableVertexAttrib()
	{
		glBindVertexArray(this->m_id);
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
}