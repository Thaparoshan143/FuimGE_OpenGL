#pragma once

#include"../../includes/GLAD/glad.h"
#include"../../includes/GLFW/glfw3.h"

#include"../../Core/Base/Window.h"

namespace OpenGL
{
	class OpenGL_Win : public Abstract::Window
	{
		public:
		OpenGL_Win(WindowInfo &wi);
		~OpenGL_Win()	{	glfwTerminate();	}
		
		inline static GLFWwindow* GetWindow() {	return m_window;	}
		void SetActive() override {	glfwMakeContextCurrent(this->m_window);	}
		void SetColor(Color col) override;
		inline bool ShouldCloseWindow() override {	return glfwWindowShouldClose(this->m_window);	};
		void SwapFrameBuffer() override	{	glfwSwapBuffers(this->m_window);	};

		static GLFWwindow *m_window;

		void initializeOpenGLWindow(int w, int h, String t);
	};

    // All the input handling function will be static as we need to query all the poll event continuously for entire application life time
    static void static_mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
	static void static_mouse_position_callback(GLFWwindow *window, double xpos, double ypos);
	static void static_mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
	static void static_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void static_charkey_callback(GLFWwindow *window, uint charKey);

	static void static_framesize_callback(GLFWwindow* window, int width, int height);
}

