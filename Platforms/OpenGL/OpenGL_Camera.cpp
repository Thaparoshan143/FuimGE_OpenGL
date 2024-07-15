#include"./OpenGL_Camera.h"

namespace OpenGL
{
    OpenGL_Camera::OpenGL_Camera(CameraProp &camProp) : ICamera(camProp)
    {

    }

    void OpenGL_Camera::processKeyboardNavgiation(Transformation::Movement direction, float delta)
    {
        if(!m_isActive)
        {
            return;
        }
        float vel = m_camProp.speed * delta;

        switch(direction)
        {
            case Transformation::FORWARD:
                m_transform.position += m_camProp.front * vel;
            break;
            case Transformation::BACKWARD:
                m_transform.position -= m_camProp.front * vel;
            break;
            case Transformation::LEFT:
                m_transform.position -= m_camProp.right * vel;
            break;
            case Transformation::RIGHT:
                m_transform.position += m_camProp.right * vel;
            break;
            case Transformation::UP:
                m_transform.position += m_camProp.up * vel;
            break;
            case Transformation::DOWN:
                m_transform.position -= m_camProp.up * vel;
            break;
            default:
                std::cout << "Unknow direction to process ! Try again" << std::endl;
            break;
        }
        std::cout << m_transform.position.x << ", " << m_transform.position.y << ", " << m_transform.position.z << std::endl;
    }

    void OpenGL_Camera::processMouseScroll(float scrollLength)
    {
        if(!m_isActive)
        {
            return;
        }
        m_camProp.zoom -= scrollLength;
        m_camProp.zoom = Maths::_clamp<float>(m_camProp.zoom, CAM_ZOOM_LOW, CAM_ZOOM_HIGH);
    }

    void OpenGL_Camera::processMouseMovement(float xOff, float yOff, bool constrainPitch)
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
            m_camProp.pitch = Maths::_clamp<float>(m_camProp.pitch, CAM_PITCH_LOW, CAM_PITCH_HIGH);
        }

        updateCameraVectors();
    }
}