#pragma once

#include"../../includes/GLAD/glad.h"
#include"../../includes/GLFW/glfw3.h"

#include"../../Core/Base/IApplication.h"

namespace OpenGL
{
    class OpenGL_Application : public Interface::IApplication
    {
        public:
        OpenGL_Application(ApplicationInfo appInfo);


    };
}