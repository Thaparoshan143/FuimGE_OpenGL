#include"OpenGL_App.h"

namespace OpenGL
{
	OpenGL_App::OpenGL_App(OpenGL_Win *window) : Application(window) 
	{
		std::cout << "OpenGL Application created!" << std::endl;
	}
}