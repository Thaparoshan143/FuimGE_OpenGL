#include"OpenGL_Sha.h"

namespace OpenGL
{
	OpenGL_Sha::OpenGL_Sha(String path) : Shader(path)
	{
		m_programID = 0;
	}

	uint OpenGL_Sha::CreateProgram()
	{
		if(this->m_programID!=0)
		{
			std::cout << "Program already created!!" << std::endl;
			return this->m_programID;
		}
		else
		{
			m_programID = glCreateProgram();
		}

		uint vs = compileShader(GL_VERTEX_SHADER);
		uint fs = compileShader(GL_FRAGMENT_SHADER);

		glAttachShader(m_programID, vs);
		glAttachShader(m_programID, fs);
		glLinkProgram(m_programID);
		glValidateProgram(m_programID);

		glDeleteShader(vs);
		glDeleteShader(fs);
		
		return m_programID;
	}

	void OpenGL_Sha::SetUniformInt(String name, int value)
	{
		uint location = queryUniformMapping(name);
		glUniform1i(location, value);
	}

	void OpenGL_Sha::SetUniformFloat(String name, float value)
	{
		uint location = queryUniformMapping(name);
		glUniform1f(location, value);
	}

	void OpenGL_Sha::SetUniformVec3(String name, fVec3 value)
	{
		uint location = queryUniformMapping(name);
		glUniform3f(location, value.r, value.g, value.b);
	}

	void OpenGL_Sha::SetUniformMat4(String name, Mat4 value)
	{
		uint location = queryUniformMapping(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}


	uint OpenGL_Sha::compileShader(uint t)
	{
		uint id = glCreateShader(t);
		String s = (t==GL_VERTEX_SHADER)?parseShader("ver.shader"):parseShader("frag.shader");
		const char *src = s.c_str();
		glShaderSource(id, 1, &src ,NULL);
		glCompileShader(id);

		int result;
		char infoLog[512];
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			glGetShaderInfoLog(id, 512, NULL, infoLog);
			std::cout << "ERROR::Shader::" << (t == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		return id;
	}

	String OpenGL_Sha::parseShader(String sha)
	{
		std::ifstream verSrc(this->m_dirPath+sha);
		std::string line;
		std::stringstream s;
		while(getline(verSrc,line))
		{
			s << line << "\n";
		}
		return s.str();
	}

	uint OpenGL_Sha::queryUniformMapping(String name)
	{
		if(m_uniformList.count(name)==0)
		{
			uint location = glGetUniformLocation(m_programID, name.c_str());
			m_uniformList.insert({name, location});
		}

		return m_uniformList[name];
	}
}