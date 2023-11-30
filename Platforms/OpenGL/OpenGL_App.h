#pragma once

#include"../../Core/Base/Application.h"

#include"./OpenGL_Win.h"

namespace OpenGL
{
	class OpenGL_App : public Abstract::Application
	{
		public:
		OpenGL_App(OpenGL_Win *window);
		~OpenGL_App() {	}

		inline ApplicationInfo GetAppInfo()	{	return m_window->GetWindowInfo();	}
	};
}