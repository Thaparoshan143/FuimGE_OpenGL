#include"./OpenGL_Application.h"

namespace OpenGL
{
    OpenGL_Application::OpenGL_Application(ApplicationInfo appInfo) : IApplication(appInfo)
    {
        std::cout << "OpenGL Application created ! " << std::endl;
    }

}