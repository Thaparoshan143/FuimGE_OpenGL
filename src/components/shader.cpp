#ifndef SHADER_CPP
#define SHADER_CPP

#include<structure.cpp>
#include<math.cpp>
#include<glad.h>
#include<map>
#include<fstream>
#include<sstream>
#include<ios>
#include <logger.h>

namespace Component
{
    class Shader
    {
        public:
        Shader(std::string dir, std::string vFile, std::string fFile) : m_dir(dir), m_vFile(vFile), m_fFile(fFile)  {  m_programHandle = glCreateProgram();    }
        ~Shader()    {   glDeleteProgram(this->m_programHandle); }

        bool CompileProgram()
        {
            uint32_t vs = compileShader(GL_VERTEX_SHADER);
            uint32_t fs = compileShader(GL_FRAGMENT_SHADER);

            glAttachShader(m_programHandle, vs);
            glAttachShader(m_programHandle, fs);
            glLinkProgram(m_programHandle);
            glValidateProgram(m_programHandle);

            glDeleteShader(vs);
            glDeleteShader(fs);

            return true;
        }
        void DeleteProgram() { glDeleteProgram(m_programHandle);   }
        void UseProgram()  {   glUseProgram(m_programHandle);  }

		void SetUniformInt(std::string name, int value)
        {
            uint location = queryUniformMapping(name);
            glUniform1i(location, value);
        }
		void SetUniformFloat(std::string name, float value)
        {
            uint location = queryUniformMapping(name);
		    glUniform1f(location, value);
        }
		void SetUniformVec3(std::string name, fVec3 value)
        {
            uint location = queryUniformMapping(name);
		    glUniform3f(location, value.r, value.g, value.b);
        }
		void SetUniformVec4(std::string name, fVec4 value)
        {
            uint location = queryUniformMapping(name);
            glUniform4f(location, value.r, value.g, value.b, value.a);
        }
		void SetUniformMat4(std::string name, Math::Mat4 value)
        {
            uint location = queryUniformMapping(name);
            glUniformMatrix4fv(location, 1, GL_FALSE, (value.data));
        }

        protected:
        uint32_t m_programHandle;
        std::string m_dir, m_vFile, m_fFile;
        uint32_t compileShader(uint32_t type)
        {
            uint id = glCreateShader(type);
            const char *src = (type == GL_VERTEX_SHADER) ? parseShader(m_vFile) : parseShader(m_fFile);
            glShaderSource(id, 1, &src, NULL);
            glCompileShader(id);

            int result;
            char infoLog[512];
            glGetShaderiv(id, GL_COMPILE_STATUS, &result);

            if (result == GL_FALSE)
            {
                Logger& logger = Logger::getInstance();
                glGetShaderInfoLog(id, 512, NULL, infoLog);
                std::cout << "ERROR::Shader::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << infoLog << std::endl;
                logger.log(Logger::LogLevel::ERROR, "ERROR::Shader::" + std::string((type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")) + "::COMPILATION_FAILED\n");
            }
            delete[] src;
            return id;
        }

        // Move to readFile binary later
		const char* parseShader(std::string fileName)
        {
            std::ifstream tempStream(std::string(m_dir + fileName), std::ios_base::in);
            std::stringstream tempBuffer;
            if(!tempStream.is_open())
            {
                Logger& logger = Logger::getInstance();
                std::cout << "Failed to load file for read ! : " << m_dir + fileName << std::endl;
                logger.log(Logger::LogLevel::ERROR, "Failed to load file for read" + std::string(m_dir) + std::string(fileName));
            }
            tempBuffer << tempStream.rdbuf();
            tempStream.close();
            
            std::string temp = tempBuffer.str();
            char *tempBuf = new char[temp.size()];
            memcpy(tempBuf, temp.data(), temp.size());
            return tempBuf;
        }

		uint32_t queryUniformMapping(std::string name)
        {
            if(m_uniformMap.count(name)==0)
            {
                uint location = glGetUniformLocation(m_programHandle, name.c_str());
                m_uniformMap.insert({name, location});
            }

            return m_uniformMap[name];
        }
        std::map<std::string, uint32_t> m_uniformMap;        
    };
}
#endif