#pragma once

#include"./Types.h"

namespace Transformation
{
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

        protected:
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