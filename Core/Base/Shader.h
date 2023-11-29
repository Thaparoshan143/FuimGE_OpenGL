#pragma once

#include "../../includes/GLM/glm/gtc/matrix_transform.hpp"
#include "../../includes/GLM/glm/gtc/type_ptr.hpp"

#include"./Types.h"

namespace Abstract
{
	class Shader
	{
		public:
		Shader(String path = "../res/Shaders/") {	m_dirPath = path;	}

		virtual uint CreateProgram() = 0;
		void UpdatePath(String path) {	m_dirPath = path;	};
		inline uint GetProgramID() {	return m_programID;	};
		virtual void DeleteProgram() = 0;
		virtual void UseProgram() = 0;

		protected:
		String m_dirPath;
		uint m_programID;
	};
}