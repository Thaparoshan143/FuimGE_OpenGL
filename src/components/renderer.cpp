#ifndef RENDERER_CPP
#define RENDERER_CPP

#include<interface.h>
#include<math.cpp>
#include<glfw3.h>
#include<buffer.cpp>
#include<shader.cpp>
#include<vertexgenerator.cpp>
#include<event.cpp>
#include<imgui.h>
#include<map>

namespace Component
{
    #define CAM_ZOOM_LOW 1.0
    #define CAM_ZOOM_HIGH 45.0
    #define CAM_PITCH_LOW -89.0
    #define CAM_PITCH_HIGH 89.0
    #define CAM_Z_NEAR 0.1f
    #define CAM_Z_FAR 100.0f

    #define CAM_SENMULTIPLIER 0.01
    #define CAM_SPEEDMULTIPLIER 2.5f

    struct CameraProp
    {
        // These are different vector param in normalized form
        fVec3 front, up, right, worldUp;
        // speed of camera, sensitivity of camera, zoom represents FOV or normal zoom
        float pitch, yaw, speed, sensitivity, zoom; 
    };

    class Camera 
    {
        public:
        Camera(CameraProp &camProp)
        {
            m_camProp = camProp;
            updateCameraVectors();
            m_keyNavMap[GLFW_KEY_W] = Math::Movement::FORWARD;
            m_keyNavMap[GLFW_KEY_S] = Math::Movement::BACKWARD;
            m_keyNavMap[GLFW_KEY_A] = Math::Movement::LEFT;
            m_keyNavMap[GLFW_KEY_D] = Math::Movement::RIGHT;
            m_keyNavMap[GLFW_KEY_Q] = Math::Movement::UP;
            m_keyNavMap[GLFW_KEY_E] = Math::Movement::DOWN;
        }

        static Math::Mat4 GetViewMatrix()
        {
            return Math::LookAt(m_transform.position, m_transform.position + m_camProp.front, m_camProp.up);
        }

        static Math::Mat4 GetProjectionMatrix(fVec2 screenSize = fVec2(800, 600))
        {
            return Math::Prespective(ToRadian(m_camProp.zoom), screenSize.x/(float)screenSize.y, CAM_Z_NEAR, CAM_Z_FAR);
        }

        static Math::Mat4 GetModelMatrix()
        {
            return Math::Mat4(  m_transform.scale.x, 0, 0, 0,
                                0, m_transform.scale.y, 0, 0,
                                0, 0, m_transform.scale.z, 0,
                                0, 0, 0,                   1);
        }

        static void ProcessKeyNav(Input::KeyEvent key)
        {
            if(!m_isActive)
            {
                return;
            }

            float vel = m_camProp.speed * ImGui::GetIO().DeltaTime;
            Math::Movement dir = m_keyNavMap[key.GetKey()];
            // std::cout << "Key got here : " << char(key.GetKey()) << std::endl;
            switch(dir)
            {
                case Math::Movement::FORWARD:
                    m_transform.position += m_camProp.front * vel;
                break;
                case Math::Movement::BACKWARD:
                    m_transform.position -= m_camProp.front * vel;
                break;
                case Math::Movement::LEFT:
                    m_transform.position -= m_camProp.right * vel;
                break;
                case Math::Movement::RIGHT:
                    m_transform.position += m_camProp.right * vel;
                break;
                case Math::Movement::UP:
                    m_transform.position += m_camProp.up * vel;
                break;
                case Math::Movement::DOWN:
                    m_transform.position -= m_camProp.up * vel;
                break;
                default:
                    std::cout << "Unknow direction to process ! Try again" << std::endl;
                break;
            }
            // std::cout << m_transform.position.x << ", " << m_transform.position.y << ", " << m_transform.position.z << std::endl;
        }

        static void ProcessMouseScroll(float x, float y)
        {
            if(!m_isActive)
            {
                return;
            }
            m_camProp.zoom -= sqrt(x*x + y*y);
            m_camProp.zoom = Math::Clamp<float>(m_camProp.zoom, CAM_ZOOM_LOW, CAM_ZOOM_HIGH);
        }

        static void ProcessMouseMovement(float xOff, float yOff, bool constrainPitch = true)
        {
            if(!m_isActive)
            {
                return;
            }

            xOff *= m_camProp.sensitivity;
            yOff *= m_camProp.sensitivity;

            m_camProp.yaw += xOff;
            m_camProp.pitch += yOff;

            if(constrainPitch)
            {
                m_camProp.pitch = Math::Clamp<float>(m_camProp.pitch, CAM_PITCH_LOW, CAM_PITCH_HIGH);
            }

            updateCameraVectors();
        }

        private:
        static void updateCameraVectors()
        {
            fVec3 front;
            front.x = cos(ToRadian(m_camProp.yaw)) * cos(ToRadian(m_camProp.pitch));
            front.y = sin(ToRadian(m_camProp.pitch));
            front.z = sin(ToRadian(m_camProp.yaw)) * cos(ToRadian(m_camProp.pitch));
            m_camProp.front = Math::normalize(front);
            // std::cout << "Front vector : " << m_camProp.front.x << ", " << m_camProp.front.y << ", " << m_camProp.front.z << std::endl;

            m_camProp.right = Math::normalize(Math::cross_product(m_camProp.front, m_camProp.worldUp));
            // std::cout << "Right vector : " << m_camProp.right.x << ", " << m_camProp.right.y << ", " << m_camProp.right.z << std::endl;
            m_camProp.up = Math::normalize(Math::cross_product(m_camProp.right, m_camProp.front));  
            // std::cout << "Up vector : " << m_camProp.up.x << ", " << m_camProp.up.y << ", " << m_camProp.up.z << std::endl;
        }

        friend class Renderer;
        public:
        static Math::Transform m_transform;
        static CameraProp m_camProp;
        static bool m_isActive;
        static std::map<int, Math::Movement> m_keyNavMap;
    };

    Math::Transform Camera::m_transform;
    CameraProp Camera::m_camProp;
    bool Camera::m_isActive = true;
    std::map<int, Math::Movement> Camera::m_keyNavMap;

    struct GridProp
    {
        GridProp(fVec2 s = fVec2(-1, 1), fVec2 e = fVec2(1, -1), fVec2 gc = fVec2(9), Color4 col = Color4(1, 1, 1, 1))
        {
            start = s;
            end = e;
            gridCount = gc;
            color = col;
        }

        fVec2 start, end, gridCount;
        Color4 color;
    };

    class GridLine : Interface::IRenderableEntity
    {
        public:
        GridLine(GridProp gridProp = GridProp()) : m_VAO(PPP), m_shader("./res/Shaders/Line/", "ver.shader", "frag.shader"), m_gridProp(gridProp)
        {
            float *temp = Util::get_grid_pointxz_ind(m_gridProp.start, m_gridProp.end, m_gridProp.gridCount);
            m_VBO.AppendBuffer(temp, Util::get_grid_xz_size(m_gridProp.gridCount));
            m_VBO.Offload();
            m_VAO.EnableVertexAttrib();
            m_shader.CompileProgram();
            delete[] temp;
        }

        void Render()
        {
            m_VAO.Bind();
            m_VBO.Bind();
            m_shader.UseProgram();
            m_shader.SetUniformMat4("view", Camera::GetViewMatrix());
            m_shader.SetUniformMat4("projection", Camera::GetProjectionMatrix(fVec2(800, 600)));
            m_shader.SetUniformMat4("model", Camera::GetModelMatrix());
            m_shader.SetUniformVec4("col", m_gridProp.color);
            glDrawArrays(GL_LINES, 0, m_VBO.GetBufferSize());
        }

        uint32_t GetBufferLayout() override {   return m_VAO.GetFormat();   }

        VertexArrayObject m_VAO;
        VertexBufferObject m_VBO;
        Shader m_shader;

        GridProp m_gridProp;
    };

    class Renderer 
    {
        public:
        Renderer(CameraProp &camProp, GridProp &gridProp) : m_camera(camProp), m_grid(gridProp)
        {
            m_renderQueue.clear();
        }

        void AddEntity(Interface::IRenderableEntity *entity)
        {
            if(entity == nullptr)
            {
                std::cout << "The pointer is null ! Push next vaid entity" << std::endl;
                return;
            }
            m_renderQueue.push_back(entity);
            m_bufferLayoutMap.insert({entity->GetBufferLayout(), VertexArrayObject((BufferFormat)entity->GetBufferLayout())});
        }

        void Render()
        {
            m_grid.Render();
            for(int i=0;i<m_renderQueue.size();++i)
            {
                m_bufferLayoutMap[m_renderQueue[i]->GetBufferLayout()].EnableVertexAttrib();
                m_renderQueue[i]->Render();
            }
        }

        Camera& GetCamera()  {   return m_camera;    }
        
        protected:
        Camera m_camera;
        GridLine m_grid;
        std::vector<Interface::IRenderableEntity*> m_renderQueue;
        std::map<uint32_t, VertexArrayObject> m_bufferLayoutMap;
    };
}

// static callback functions
static void static_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // std::cout << "Key pressed : " << char(key) << std::endl;
    // std::cout << "Action : " << action << " | Scancode : " << scancode << " | Mod : " << mods << std::endl;
    Input::KeyEvent keyEv(key, action);
    Component::Camera::ProcessKeyNav(keyEv);
}

static void static_scroll_callback(GLFWwindow *window, double x, double y)
{
    // std::cout << "Scroll : " << x << ", " << y << std::endl;
    Component::Camera::ProcessMouseScroll(x, y);
}

static void static_mousekey_callback(GLFWwindow *window, int button, int action, int mods)
{
    // std::cout << "Mouse Button press : " << button << " | Action : " << action << " | Mod : " << mods << std::endl;
}

static void static_cursor_pos_callback(GLFWwindow *window, double x, double y)
{
    // std::cout << "Position changed : " << x << ", " << y << std::endl;
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

    Component::Camera::ProcessMouseMovement(xoffset, yoffset);
}

static void static_cursor_enter_callback(GLFWwindow *window, int status)
{
    // std::cout << "Cursor enter the window status : " << status << std::endl;
    if(status)
    {
        Component::Camera::m_isActive = true;
    }
    else
    {
        Component::Camera::m_isActive = false;
    }
}

#endif