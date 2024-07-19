#ifndef MATH_CPP
#define MATH_CPP

#include<structure.cpp>

namespace Math
{
    // Transformation 
    typedef fVec3 Position;
    typedef fVec3 Scale;
    typedef fVec3 Rotation;

    struct Transform
    {
        public:
        Transform() : position(0), rotation(0), scale(1) {   }
        Transform(fVec3 p, fVec3 r, fVec3 s) : position(p), rotation(r), scale(s) {   }
        Transform(Transform &rhs)   {   position = rhs.position;    rotation = rhs.rotation;    scale = rhs.scale;  }

        Transform& operator=(Transform &rhs) {   position = rhs.position;    rotation = rhs.rotation;    scale = rhs.scale;  return *this;   }

        union
        {
            struct 
            {
                Position position;
                // currently using the 3 float vector, change it to quaternion later
                Rotation rotation;
                Scale scale;
            };
            float transform[9];
        };
    };

    struct Mat4
    {
        public:
        Mat4(float val)
        {
            allInitialize(val);
        }


        float data[16];
        protected:
        void allInitialize(float v)
        {
            for(int i=0;i<16;++i)
            {
                if (i == 15)
                    *(data + i) = 1;
                else if(i%5 == 0)
                    *(data + i) = v;
                else
                    *(data + i) = 0;
            }
        }
    };
}
#endif