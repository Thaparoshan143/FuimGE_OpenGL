#pragma once

#include"../../includes/GLAD/glad.h"
#include"../../includes/GLFW/glfw3.h"

#include"../../Core/Base/IWindow.h"

#include"./OpenGL_Camera.h"
#include"./OpenGL_KeyCode.h"

namespace OpenGL
{
    class OpenGL_Window : public Interface::IWindow
    {
        public:
        OpenGL_Window(WindowInfo winInfo);
        ~OpenGL_Window()    {   delete m_camera;    glfwTerminate();    }

        inline bool ShouldCloseWindow() override {   return glfwWindowShouldClose(this->m_glfwWindow);  }
        void SetActive() override {  glfwMakeContextCurrent(this->m_glfwWindow); }
        void SetBgColor(float r, float g, float b, float a) override {   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    glClearColor(r, g, b, a);   }

        inline void* GetWindowHandle() override   {   return (void*)m_glfwWindow;    }
        void SwapFrameBuffer() override {   glfwSwapBuffers(this->m_glfwWindow);    }
        void setActiveCamera() override;

        protected:
        GLFWwindow *m_glfwWindow;

        void initialize(int w, int h, String t);
    };
}