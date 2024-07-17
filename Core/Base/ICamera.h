#pragma once

#include"./Types.h"
#include"./IEntity.h"

#include"../../includes/GLM/glm/gtc/matrix_transform.hpp"

struct CameraProp
{
    // These are the vectors representing the different param in normalized form
    fVec3 front, up, right, worldUp;
    // speed of camera, sensitivity of camera, zoom represents FOV or normal zoom
    float pitch, yaw, speed, sensitivity, zoom; 
};

#define CAM_ZOOM_LOW 1.0
#define CAM_ZOOM_HIGH 45.0
#define CAM_PITCH_LOW -89.0
#define CAM_PITCH_HIGH 89.0
#define CAM_Z_NEAR 1.0f
#define CAM_Z_FAR 100.0f

#define CAM_SENMULTIPLIER 0.01
#define CAM_SPEEDMULTIPLIER 2.5f

namespace Interface
{
    class ICamera : public Interface::IEntity
    {
        public:
        ICamera(CameraProp &camProp) : IEntity("Camera")   
        {   
            SetCameraProp(camProp);
        }

        virtual ~ICamera()  {   }

        Mat4 GetViewMatrix()
        {
            return glm::lookAt(m_transform.position, m_transform.position + m_camProp.front, m_camProp.up);
        }

        Mat4 GetProjectionMatrix(fVec2 screenSize = fVec2(800))
        {
            return glm::perspective(glm::radians(m_camProp.zoom), screenSize.x/(float)screenSize.y, CAM_Z_NEAR, CAM_Z_FAR);
        }

        Mat4 GetModelMatrix()
        {
            return Mat4(m_transform.scale.x, 0, 0, 0,
                        0, m_transform.scale.y, 0, 0,
                        0, 0, m_transform.scale.z, 0,
                        0, 0, 0,                   1);
        }

        void SetCameraProp(CameraProp &camProp)
        {
            m_camProp = camProp;
            updateCameraVectors();
        }

        virtual void processKeyboardNavgiation(Transformation::Movement direction, float delta = 1/60.0) = 0;
        virtual void processMouseScroll(float scrollLength) = 0;
        virtual void processMouseMovement(float xOff, float yOff, bool constrainPitch = true) = 0;

        CameraProp m_camProp;

        protected:
        void updateCameraVectors()
        {
            fVec3 front;
            front.x = cos(glm::radians(m_camProp.yaw)) * cos(glm::radians(m_camProp.pitch));
            front.y = sin(glm::radians(m_camProp.pitch));
            front.z = sin(glm::radians(m_camProp.yaw)) * cos(glm::radians(m_camProp.pitch));
            m_camProp.front = glm::normalize(front);

            m_camProp.right = glm::normalize(glm::cross(m_camProp.front, m_camProp.worldUp));
            m_camProp.up = glm::normalize(glm::cross(m_camProp.right, m_camProp.front));
        }
    };
}