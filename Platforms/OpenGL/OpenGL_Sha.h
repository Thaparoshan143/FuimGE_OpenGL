#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <map>

#include"../../includes/GLAD/glad.h"
#include"../../includes/GLFW/glfw3.h"

#include"../../Core/Base/Shader.h"

namespace OpenGL
{
	class OpenGL_Sha : public Abstract::Shader
	{
		public:
		OpenGL_Sha(String path = "../res/Shaders/");

		~OpenGL_Sha() {	glDeleteProgram(this->m_programID);	}

		uint CreateProgram() override;
		void DeleteProgram() override	{	glDeleteProgram(m_programID);	};
		void UseProgram() override	{	glUseProgram(this->CreateProgram());	}
		void SetUniformInt(String name, int value);
		void SetUniformFloat(String name, float value);
		void SetUniformVec3(String name, fVec3 value);
		void SetUniformMat4(String name, Mat4 value);

		protected:
		uint compileShader(uint t);
		String parseShader(String sha);
		uint queryUniformMapping(String name);
		std::map<String, uint> m_uniformList;
	};

}