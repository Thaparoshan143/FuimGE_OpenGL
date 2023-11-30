#include"OpenGL_Win.h"

namespace OpenGL
{
	GLFWwindow* OpenGL_Win::m_window = nullptr;
	OpenGL_Win::OpenGL_Win(WindowInfo &wi) : Window(wi)
	{
		initializeOpenGLWindow(wi.dim.x, wi.dim.y, wi.title);
	}

	void OpenGL_Win::SetColor(Color col)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(col.r, col.g, col.b, col.a);
	}

	void OpenGL_Win::initializeOpenGLWindow(int w, int h, String t)
	{
		// loading basic glfw library and more
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef __APPLE__
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif
		if(!glfwInit())
		{
			exit(-1);
		}

		this->m_window = glfwCreateWindow(w, h, t.c_str(), NULL, NULL);
		this->SetActive();

		// loader glad check if everything is loaded for use 
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			exit(-2);
		}
		
		glfwSetWindowUserPointer(this->m_window, this);

		glfwSetMouseButtonCallback(this->m_window, static_mouse_button_callback);
		glfwSetCursorPosCallback(this->m_window, static_mouse_position_callback);
		glfwSetScrollCallback(this->m_window, static_mouse_scroll_callback);
		glfwSetKeyCallback(this->m_window, static_key_callback);
		glfwSetCharCallback(this->m_window, static_charkey_callback);

		glfwSetFramebufferSizeCallback(this->m_window, static_framesize_callback);
	}

	// All the static methods for input definition 
	static void static_mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
    {
		#ifdef DEBUG_MODE
		std::cout << "Mouse button registered || Button : " << button << " || Action : " << action << " || Mods : " << mods << std::endl;
		#endif
	}

	static void static_mouse_position_callback(GLFWwindow *window, double xpos, double ypos)
	{
		#ifdef DEBUG_MODE
		std::cout << "Mouse position changed to || Xpos : " << xpos << " || Ypos : " << ypos << std::endl;
		#endif
	}

	static void static_mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
	{
		#ifdef DEBUG_MODE
		std::cout << "Mouse scroll detected with offset || xoffset : " << xoffset << " || yoffset : " << yoffset << std::endl;
		#endif
	}

	static void static_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		#ifdef DEBUG_MODE
		std::cout << "Mod based key pressed from keyboard || key : " << key << " || scancode : " << scancode << " || action : " << " || mods : " << mods << std::endl;
		#endif
	}

	static void static_charkey_callback(GLFWwindow *window, uint charKey)
	{
		#ifdef DEBUG_MODE
		std::cout << "Normal Ascii key pressed... ||  Ascii code : " << charKey << " ||  Letter : " << char(charKey) << std::endl;
		#endif
	}

	static void static_framesize_callback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
		#ifdef DEBUG_MODE
		std::cout << "Frame size changed to || Width : " << width << " || Height : " << height << std::endl;
		#endif
	}
}