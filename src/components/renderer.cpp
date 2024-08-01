#ifndef RENDERER_CPP
#define RENDERER_CPP

#include<interface.h>
#include<math.cpp>
#include<glfw3.h>
#include<buffer.cpp>
#include<shader.cpp>
#include<entity.cpp>
#include<vertexgenerator.cpp>
#include<event.cpp>
#include<imgui.h>
#include<map>

// directory list map for the shader for corresponding buffer format;
static std::map<uint32_t, std::string> _shader_dir = {
    {PPP_UV_NNN, "./res/Shaders/Texture/"},
    {PPP_NNN, "./res/Shaders/Normal/"},
    {PPP_RGB, "./res/Shaders/"}
};

namespace Component
{
    #define CAM_ZOOM_LOW 1.0
    #define CAM_ZOOM_HIGH 45.0
    #define CAM_PITCH_LOW -89.0
    #define CAM_PITCH_HIGH 89.0
    #define CAM_Z_NEAR 0.1f
    #define CAM_Z_FAR 100.0f

    #define CAM_SENMULTIPLIER 0.04
    #define CAM_SPEEDMULTIPLIER 8.0f

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

        static Math::Mat4 GetProjectionMatrix()
        {
            return Math::Prespective(ToRadian(m_camProp.zoom), m_screenSize.x/(float)m_screenSize.y, CAM_Z_NEAR, CAM_Z_FAR);
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
            if(!m_isNavActive)
            {
                return;
            }
            Math::Movement dir = m_keyNavMap[key.GetKey()];
            float vel = m_camProp.speed * ImGui::GetIO().DeltaTime;
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
                    std::cout << "Unknown direction to process ! Try again" << std::endl;
                break;
            }
            // std::cout << m_transform.position.x << ", " << m_transform.position.y << ", " << m_transform.position.z << std::endl;
        }

        static void ProcessMouseScroll(float x, float y)
        {
            if(!m_isNavActive)
            {
                return;
            }
            m_camProp.zoom -= y;
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
        static fVec2 m_screenSize;
        static CameraProp m_camProp;
        static bool m_isActive, m_isNavActive;
        static std::map<int, Math::Movement> m_keyNavMap;
    };

    Math::Transform Camera::m_transform;
    CameraProp Camera::m_camProp;
    fVec2 Camera::m_screenSize = fVec2(800, 600);
    bool Camera::m_isActive = false;
    bool Camera::m_isNavActive = true;
    std::map<int, Math::Movement> Camera::m_keyNavMap;

    struct GridProp
    {
        GridProp(fVec2 s = fVec2(-1, 1), fVec2 e = fVec2(1, -1), fVec2 gc = fVec2(9), Color4 col = Color4(0.4, 0.4, 0.4))
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

        void Update() override
        {
            m_VAO.Bind();
            m_VBO.Bind();
            m_shader.UseProgram();
            m_shader.SetUniformMat4("view", Camera::GetViewMatrix());
            m_shader.SetUniformMat4("projection", Camera::GetProjectionMatrix());
            m_shader.SetUniformMat4("model", Camera::GetModelMatrix());
            m_shader.SetUniformVec4("col", m_gridProp.color);
        }

        void Render() override
        {
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
            updateVAOShaderInMap(PPP_RGB);
            m_dirLight.direction = fVec3(-0.5);
            m_dirLight.ambient = fVec3(1);
            m_dirLight.diffuse = fVec3(1);
            m_dirLight.specular = fVec3(1);
            m_gridActive = true;
        }

        void AddEntity(Interface::IRenderableEntity *entity)
        {
            if(entity == nullptr)
            {
                std::cout << "The pointer is null ! Push next vaid entity" << std::endl;
                return;
            }
            m_renderQueue.push_back(entity);

            updateVAOShaderInMap(entity->GetBufferLayout());
        }

        void AddDuplicateEntity(Interface::IRenderableEntity *origin, uint32_t count = 1, bool incTransform = false)
        {
            std::cout << "==> Duplicate entry register : " << origin->GetName() << std::endl;
            for(uint32_t i=0;i<count;++i)
            {
                Math::Transform newTras = origin->GetTransform();
                if(incTransform)
                    newTras.position += (newTras.position * (i+1));
                Interface::IRenderableEntity *temp = new DuplicateModel(origin, newTras);
                AddEntity(temp);
            }
        }

        void Update()
        {
            m_grid.Update();
            for(const auto &item : m_renderQueue)
            {
                item->Update();
            }
        }

        void Render()
        {
            if(m_gridActive)
                m_grid.Render();
            for(const auto &item : m_renderQueue)
            {
                Shader *tempShader = m_shaderMap[item->GetBufferLayout()];
                tempShader->UseProgram();
                Math::Mat4 model = m_camera.GetModelMatrix();
                tempShader->SetUniformMat4("view", m_camera.GetViewMatrix());
                tempShader->SetUniformMat4("projection", m_camera.GetProjectionMatrix());

                model = Math::RotateMatX(item->GetRotation().x) * model;
                model = Math::RotateMatY(item->GetRotation().y) * model;
                model = Math::RotateMatZ(item->GetRotation().z) * model;
                // model = Math::RotateMat(0, item->GetRotation()) * model;
                model = Math::ScaleMat(model, item->GetScale());
                model = Math::TranslateMat(model, item->GetPosition());
                tempShader->SetUniformMat4("model",  model);

                // light properties
                tempShader->SetUniformVec3("viewPos", m_camera.m_transform.position);
                tempShader->SetUniformVec3("light.direction", m_dirLight.direction);
                tempShader->SetUniformVec3("light.ambient", m_dirLight.ambient);
                tempShader->SetUniformVec3("light.diffuse", m_dirLight.diffuse);
                tempShader->SetUniformVec3("light.specular", m_dirLight.specular);
                tempShader->SetUniformVec3("light.color", m_dirLight.color);

                // material properties
                tempShader->SetUniformFloat("material.ambient", item->material.ambient);
                tempShader->SetUniformFloat("material.diffuse", item->material.diffuse);
                tempShader->SetUniformFloat("material.specular", item->material.specular);
                tempShader->SetUniformFloat("material.roughness", item->material.roughness);
                tempShader->SetUniformVec4("material.color", item->material.color);

                if(m_wireframeEnable)
		            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                else
		            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                item->Render();
            }
        }

        Camera& GetCamera()  {   return m_camera;    }

        protected:
        void updateVAOShaderInMap(uint32_t layout)
        {
            if(m_shaderMap.count(layout) == 0)
            {
                std::cout << "Layout is unknow adding the new map now" << std::endl;
                Component::VertexArrayObject *tempVAO = new VertexArrayObject((BufferFormat)layout);
                tempVAO->EnableVertexAttrib();
                Shader *tempShader = nullptr;
                try
                {
                    tempShader = new Shader(_shader_dir[layout], "ver.shader", "frag.shader");
                    tempShader->CompileProgram();
                    std::cout << "Dir added : " << _shader_dir[layout] << std::endl;
                    m_shaderMap[layout] = tempShader;
                }
                catch(...)
                {
                    std::cout << "Unable to find the shader from global map" << std::endl;
                }
            }
            else
            {
                std::cout << "Layout already predefined : " << _shader_dir[layout] << std::endl;
            }
        }

        public:
        std::vector<Interface::IRenderableEntity*> m_renderQueue;
        // temporarly directional light for now
        DirectionalLightProp m_dirLight;
        bool m_gridActive, m_wireframeEnable;
        protected:
        Camera m_camera;
        GridLine m_grid;
        // here the map is intact between the buffer layout along with the VAO and corresponding shader map 
        static std::map<uint32_t, Shader*> m_shaderMap;
    };
    std::map<uint32_t, Shader*> Renderer::m_shaderMap;
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
    if(action == GLFW_PRESS || action == GLFW_REPEAT)
        Component::Camera::m_isActive = true;
    else if(action == GLFW_RELEASE)
        Component::Camera::m_isActive = false;
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
        Component::Camera::m_isNavActive = true;
    }
    else
    {
        Component::Camera::m_isNavActive = false;
    }
}

static void static_window_size_change(GLFWwindow *window, int width, int height)
{
    Component::Camera::m_screenSize = fVec2(width, height);
}

#endif