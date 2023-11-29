#pragma once

#include"../../includes/GLAD/glad.h"
#include"../../includes/GLFW/glfw3.h"

#include"../../Core/Base/Buffer.h"

namespace OpenGL
{
	class OpenGL_VertBuffObj : public Abstract::VertexBufferObject<float>
	{
		public:
		OpenGL_VertBuffObj(uint size = 2);
		
		void LoadBuffer(uint loadMode = GL_DYNAMIC_DRAW) override;
		void DirectLoad(float *data, uint count, uint loadMode = GL_DYNAMIC_DRAW);
		// Here the size mentioned for reserve buffer will later be multiplied by sizeof(float) so make sure to pass by count reference
		void ReserveBuffer(uint size, uint loadMode = GL_DYNAMIC_DRAW);
		void LoadSubBuffer(uint size, float *data);
		uint Bind() override;
		void Unbind() override;
		float* GetVertexData() {	return m_data.data();	}
	};

	// Since the Index buffer almost matches with the vertex buffer so, currently reusing the exisiting buffer, if needed we can change later
	class OpenGL_IndBuffObj : public Abstract::VertexBufferObject<uint>
	{
		public:
		OpenGL_IndBuffObj(uint size = 2);
		
		void LoadBuffer(uint loadMode = GL_DYNAMIC_DRAW) override;
		void DirectLoad(uint *data, uint count, uint loadMode = GL_DYNAMIC_DRAW);
		void ReserveBuffer(uint size, uint loadMode = GL_DYNAMIC_DRAW);
		void LoadSubBuffer(uint size, uint *data);
		uint Bind() override;
		void Unbind() override;
		uint* GetVertexData() {	return m_data.data();	}
	};

	class OpenGL_VertArrObj : public Abstract::VertexArrayObject
	{
		public:
		OpenGL_VertArrObj(Abstract::BufferFormat bf = Abstract::BufferFormat::PP);

		uint Bind() override;
		void Unbind() override;
		void EnableVertexAttrib();
		// This is just the interface for the specific purpose, (text rendering..) might not be useful for all
		void EnableVertexAttribMan(uint count);
		uint StrideCount() {	return getVBOStride();	}		
		
		private:
		uint getVBOStride();
	};

}