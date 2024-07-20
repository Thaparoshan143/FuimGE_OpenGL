#ifndef RENDERER_CPP
#define RENDERER_CPP

#include<interface.h>
#include<math.h>

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
        }

        Math::Mat4 GetViewMatrix()
        {
            return Math::LookAt(m_transform.position, m_transform.position + m_camProp.front, m_camProp.up);
        }

        Math::Mat4 GetProjectionMatrix(fVec2 screenSize = fVec2(800, 600))
        {
            return Math::Prespective(ToRadian(m_camProp.zoom), screenSize.x/(float)screenSize.y, CAM_Z_NEAR, CAM_Z_FAR);
        }

        Math::Mat4 GetModelMatrix()
        {
            return Math::Mat4(  m_transform.scale.x, 0, 0, 0,
                                0, m_transform.scale.y, 0, 0,
                                0, 0, m_transform.scale.z, 0,
                                0, 0, 0,                   1);
        }

        private:
        void updateCameraVectors()
        {
            fVec3 front;
            front.x = cos(ToRadian(m_camProp.yaw)) * cos(ToRadian(m_camProp.pitch));
            front.y = sin(ToRadian(m_camProp.pitch));
            front.z = sin(ToRadian(m_camProp.yaw)) * cos(ToRadian(m_camProp.pitch));
            m_camProp.front = Math::normalize(front);

            m_camProp.right = Math::normalize(Math::cross_product(m_camProp.front, m_camProp.worldUp));
            m_camProp.up = Math::normalize(Math::cross_product(m_camProp.right, m_camProp.front));  
        }

        friend class Renderer;
        public:
        Math::Transform m_transform;
        CameraProp m_camProp;
        bool m_isActive;
    };


    class Renderer 
    {
        public:
        Renderer(CameraProp &camProp) : m_camera(camProp)
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
        }

        void Render()
        {
            for(int i=0;i<m_renderQueue.size();++i)
            {
                m_renderQueue[i]->Render();
            }
        }
        
        // protected:
        Camera m_camera;
        std::vector<Interface::IRenderableEntity*> m_renderQueue;
    };
}
#endif