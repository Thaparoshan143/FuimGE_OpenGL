#ifndef WINDOW_CPP
#define WINDOW_CPP

#include<glad.h>
#include<glfw3.h>
#include<structure.cpp>
#include<renderer.cpp>
#include <logger.h>

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600
#define DEFAULT_WINDOW_TITLE "Graphics Engine"
#define DEFAULT_WINDOW_BG {0.11, 0.11, 0.11, 1}
// #define DEFAULT_WINDOW_BG {0.55, 0, 0.9, 1}

namespace Component
{
    struct WindowProp
    {
        public:
        WindowProp(int w = DEFAULT_WINDOW_WIDTH, int h = DEFAULT_WINDOW_HEIGHT, const char *t = DEFAULT_WINDOW_TITLE) : width(w), height(h), title(t) {   }
        WindowProp(WindowProp &winProp) {   width = winProp.width;  height = winProp.height;    title = winProp.title;  }

        int width, height;
        String title;
    };

    class Window
    {
        public:
        Window(WindowProp &winProp) : m_winProp(winProp)
        {
            Logger& logger = Logger::getInstance();
            logger.log(Logger::LogLevel::NEWLOG, "Log Started");
            if(!glfwInit())   {   std::cout << "GLFW initialize error" << std::endl;  exit(-1);  logger.log(Logger::LogLevel::ERROR, "GLFW Initialize Error");}
            logger.log(Logger::LogLevel::INFO, "GLFW Initialized");
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            #ifdef __APPLE__
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            #endif

            m_window = glfwCreateWindow(m_winProp.width, m_winProp.height, m_winProp.title.GetRawChar(), NULL, NULL);
            glfwMakeContextCurrent(m_window);

            // loader glad check if everything is loaded for use 
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))    {   std::cout << "Failed to initialize GLAD Loader" << std::endl;   exit(-2);   }

            glfwSetWindowUserPointer(this->m_window, this);

            // callbacks
            glfwSetKeyCallback(this->m_window, static_key_callback);
            glfwSetScrollCallback(this->m_window, static_scroll_callback);
            glfwSetMouseButtonCallback(this->m_window, static_mousekey_callback);
            glfwSetCursorPosCallback(this->m_window, static_cursor_pos_callback);
            glfwSetCursorEnterCallback(this->m_window, static_cursor_enter_callback);
            glfwSetWindowSizeCallback(this->m_window, static_window_size_change);
            m_color = fVec3(0.1);
        }
        ~Window()   {   glfwTerminate();    }

        GLFWwindow* GetWindowHandle()   {   return   m_window;   }
        WindowProp GetWindowProp()  {   return m_winProp;    }
        inline bool ShouldCloseWindow() {   return glfwWindowShouldClose(m_window);   }
        void BgColor() {   SetBgColor(m_color.x, m_color.y, m_color.z, 1);   }
        void SetBgColor(Color4 col) {   SetBgColor(col.r, col.g, col.b, col.a);   }
        void SetBgColor(float r, float g, float b, float a) {   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); glClearColor(r, g, b, a);   }
        void SwapFrameBuffer()   {   glfwSwapBuffers(m_window);  }

        GLFWwindow *m_window;
        WindowProp m_winProp;
        fVec3 m_color;
    };
}
#endif