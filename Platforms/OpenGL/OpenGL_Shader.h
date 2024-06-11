#pragma once 

#include"../../includes/GLAD/glad.h"
#include"../../includes/GLFW/glfw3.h"
#include"../../includes/GLM/glm/gtc/matrix_transform.hpp"
#include"../../includes/GLM/glm/gtc/type_ptr.hpp"

#include"../../Core/Base/IShader.h"

#include <map>
#include <fstream>
#include <sstream>

namespace OpenGL
{
    class OpenGL_Shader : public Interface::IShader
    {
        public:
        OpenGL_Shader(String dir, String vFile, String fFile) : IShader(dir, vFile, fFile)   {   }
        ~OpenGL_Shader()    {   glDeleteProgram(this->m_programHandle); }

        uint CreateProgram() override;
        bool CompileProgram() override;
        void DeleteProgram() override { glDeleteProgram(m_programHandle);   }
        void UseProgram() override  {   m_programHandle == 0 ? glUseProgram(CreateProgram()) : glUseProgram(m_programHandle);  }

		void SetUniformInt(String name, int value);
		void SetUniformFloat(String name, float value);
		void SetUniformVec3(String name, fVec3 value);
		void SetUniformMat4(String name, Mat4 value);

        protected:
        uint compileShader(uint type);
        // Move to readFile binary later
		const char* parseShader(String fileName);
		uint queryUniformMapping(String name);
        std::map<String, uint> m_uniformMap;
    };
}