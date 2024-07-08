#pragma once

#include"./Types.h"

namespace Transformation
{
    typedef enum Movement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    }Movement;

    typedef enum Speed
    {
        SLOW = 1,
        MEDIUM = 1 << 2,
        FAST = 1 << 3
    }Speed;

    typedef enum Sensitivity
    {
        LOW = 1,
        AVERAGE = 2, 
        HIGH = 3,
        VERYHIGH = 4
    }Sensitivity;

    typedef fVec3 Position;
    typedef fVec3 Scale;
    // for angle based or euler
    typedef fVec3 Rotation;
    // For the quaternion based 
    typedef fVec4 Quaternion;

    class Transform
    {
        public:
        Transform()
        {
            position = rotation = fVec3(0.0);
            scale = fVec3(1.0);
        }

        Transform(Position pos, Rotation rot, Scale sca)
        {
            SetTransform(pos, rot, sca);
        }

        void SetTransform(Position pos, Rotation rot, Scale sca)
        {
            position = pos;
            rotation = rot;
            scale = sca;
        }

        void SetTransform(Transform &trans)
        {
            SetTransform(trans.position, trans.rotation, trans.scale);
        }

        const inline Transform& GetTransform() {   return *this;    }

        union{
            struct
            {
                Position position;
                Rotation rotation;
                Scale scale;
            };
            float transform[9];
        };
    };

}