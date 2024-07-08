#pragma once

#include"../../Core/Base/ICamera.h"
#include"../../Core/Base/Transformation.hpp"
#include"../../Util/Maths.h"

namespace OpenGL
{
    class OpenGL_Camera : public Interface::ICamera
    {
        public:
        OpenGL_Camera(CameraProp &camProp);

        protected:
        void processKeyboardNavgiation(Transformation::Movement direction, float delta = 1/60.0) override;
        void processMouseScroll(float scrollLength) override;
        void processMouseMovement(float xOff, float yOff, bool constrainPitch = true) override;
    };
}