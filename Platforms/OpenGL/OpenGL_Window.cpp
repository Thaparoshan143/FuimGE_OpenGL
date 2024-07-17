#include"./OpenGL_Window.h"

namespace OpenGL
{
    static void static_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        if(window == nullptr)
        {
            std::cout << "Unable to capture the window att" << std::endl;
        }

        OpenGL_Window *temp = (OpenGL_Window*)glfwGetWindowUserPointer(window);
        
        if(action == Input::PRESS || action == Input::REPEAT)
        {
            if(key == Input::KEY_W)
            {
                temp->GetCamera()->processKeyboardNavgiation(Transformation::FORWARD);
            }
            else if(key == Input::KEY_S)
            {
                temp->GetCamera()->processKeyboardNavgiation(Transformation::BACKWARD);
            }
            else if(key == Input::KEY_A)
            {
                temp->GetCamera()->processKeyboardNavgiation(Transformation::LEFT);
            }
            else if(key == Input::KEY_D)
            {
                temp->GetCamera()->processKeyboardNavgiation(Transformation::RIGHT);
            }
            else if(key == Input::KEY_Q)
            {
                temp->GetCamera()->processKeyboardNavgiation(Transformation::UP);
            }
            else if(key == Input::KEY_E)
            {
                temp->GetCamera()->processKeyboardNavgiation(Transformation::DOWN);
            }
        }
        // std::cout << char(key) << std::endl;
    }

    static void static_mouse_btn_callback(GLFWwindow *window, int btn, int action, int mods)
    {       
        if(window == nullptr)
        {
            std::cout << "Unable to capture the window att" << std::endl;
        }

        if(btn == Input::KEY_LEFT && action == Input::PRESS)
        {
            std::cout << "Left" << std::endl;
        }
        else if(btn == Input::KEY_RIGHT && action == Input::PRESS)
        {
            std::cout << "Right" << std::endl;
        }

        OpenGL_Window *temp = (OpenGL_Window*)glfwGetWindowUserPointer(window);
        temp->GetCamera()->m_isActive = !temp->GetCamera()->m_isActive;
    }

    static void static_cursor_pos_callback(GLFWwindow *window, double x, double y)
    {
        float xpos = static_cast<float>(x);
        float ypos = static_cast<float>(y);

        static float lastX, lastY;
        static bool firstMouse = true;
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        if(window == nullptr)
        {
            std::cout << "Unable to capture the window att" << std::endl;
        }
        OpenGL_Window *temp = (OpenGL_Window*)glfwGetWindowUserPointer(window);
        temp->GetCamera()->processMouseMovement(xoffset, yoffset);
    }

    static void static_mouse_scroll(GLFWwindow *window, double xoff, double yoff)
    {
        if(window == nullptr)
        {
            std::cout << "Unable to capture the window att" << std::endl;
        }
        OpenGL_Window *temp = (OpenGL_Window*)glfwGetWindowUserPointer(window);
        temp->GetCamera()->processMouseScroll(yoff);
    }

    static void static_mouse_inside_window(GLFWwindow *window, int inside)
    {
        OpenGL_Window *temp = (OpenGL_Window*)glfwGetWindowUserPointer(window);

        if(inside)
        {
            temp->GetCamera()->m_isActive = true;
            std::cout << "I am inside" << std::endl;
        }
        else
        {
            temp->GetCamera()->m_isActive = false;
            std::cout << "I am outside" << std::endl;
        }
    }

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
        glfwSetKeyCallback(this->m_glfwWindow, static_key_callback);
        glfwSetMouseButtonCallback(this->m_glfwWindow, static_mouse_btn_callback);
        glfwSetCursorPosCallback(this->m_glfwWindow, static_cursor_pos_callback);
        glfwSetScrollCallback(this->m_glfwWindow, static_mouse_scroll);
        glfwSetCursorEnterCallback(this->m_glfwWindow, static_mouse_inside_window);

        setActiveCamera();
    }

    void OpenGL_Window::setActiveCamera()
    {
        CameraProp newCamProp;
        newCamProp.worldUp = fVec3(0, 1, 0);
        newCamProp.pitch = -30;
        newCamProp.yaw = -45.0;
        newCamProp.speed = Transformation::Speed::FAST * CAM_SPEEDMULTIPLIER;
        newCamProp.sensitivity = (Transformation::Sensitivity::VERYHIGH) * CAM_SENMULTIPLIER;
        newCamProp.zoom = 45;

        m_camera = new OpenGL_Camera(newCamProp);
        m_camera->m_transform.position = fVec3(-15, 10, 15);
    }
}