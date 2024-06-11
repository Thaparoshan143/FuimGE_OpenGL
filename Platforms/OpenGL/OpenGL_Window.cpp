#include"./OpenGL_Window.h"

namespace OpenGL
{
    OpenGL_Window::OpenGL_Window(WindowInfo winInfo) : IWindow(winInfo)
    {
        initialize(winInfo.width, winInfo.height, winInfo.title);
    }

    void OpenGL_Window::initialize(int w, int h, String t)
    {
        // loading glfw basics
        if(!glfwInit())
        {
            exit(-1);
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        this->m_glfwWindow = glfwCreateWindow(w, h, t.c_str(), nullptr, nullptr);
        this->SetActive();

        // loader glad check if everything is loaded for use 
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD Loader" << std::endl;
			exit(-2);
		}

        glfwSetWindowUserPointer(this->m_glfwWindow, this);
    }
}