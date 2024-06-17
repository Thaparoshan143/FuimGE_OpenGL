#include"./OpenGL_Shader.h"

namespace OpenGL
{
    uint OpenGL_Shader::CreateProgram()
    {
		if(m_programHandle == 0)
		{
			CompileProgram();
		}

		return m_programHandle;
    }

	bool OpenGL_Shader::CompileProgram()
	{
		if(this->m_programHandle!=0)
		{
			std::cout << "Program already complied and created!" << std::endl;
			return false;
		}
		else
		{
			m_programHandle = glCreateProgram();
		}

		uint vs = compileShader(GL_VERTEX_SHADER);
		uint fs = compileShader(GL_FRAGMENT_SHADER);

		glAttachShader(m_programHandle, vs);
		glAttachShader(m_programHandle, fs);
		glLinkProgram(m_programHandle);
		glValidateProgram(m_programHandle);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return true;
	}

	void OpenGL_Shader::SetUniformInt(String name, int value)
	{
		uint location = queryUniformMapping(name);
		glUniform1i(location, value);
	}

	void OpenGL_Shader::SetUniformFloat(String name, float value)
	{
		uint location = queryUniformMapping(name);
		glUniform1f(location, value);
	}

	void OpenGL_Shader::SetUniformVec3(String name, fVec3 value)
	{
		uint location = queryUniformMapping(name);
		glUniform3f(location, value.r, value.g, value.b);
	}

	void OpenGL_Shader::SetUniformMat4(String name, Mat4 value)
	{
		uint location = queryUniformMapping(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}


    uint OpenGL_Shader::compileShader(uint type)
	{
		uint id = glCreateShader(type);
		const char *src = (type == GL_VERTEX_SHADER) ? parseShader(m_vertFile) : parseShader(m_fragFile);
		glShaderSource(id, 1, &src, NULL);
		glCompileShader(id);

		int result;
		char infoLog[512];
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			glGetShaderInfoLog(id, 512, NULL, infoLog);
			std::cout << "ERROR::Shader::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		delete[] src;
		return id;
	}

	const char* OpenGL_Shader::parseShader(String fileName)
	{
		File::InpFileStream temp(this->m_dir + fileName);
		return temp.ReadRaw();
	}
		
	uint OpenGL_Shader::queryUniformMapping(String name)
	{
		if(m_uniformMap.count(name)==0)
		{
			uint location = glGetUniformLocation(m_programHandle, name.c_str());
			m_uniformMap.insert({name, location});
		}

		return m_uniformMap[name];
	}
}