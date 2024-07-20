#ifndef MATH_CPP
#define MATH_CPP

#include<structure.cpp>
// #define MATH_3D_IMPLEMENTATION
// #include<math_3d.h>

namespace Math
{
    #define DEGTORADIANVAL 0.0174533
    // convert angle value into radian conrresponding based on factor
    #define ToRadian(x) (x * DEGTORADIANVAL)
    // convert radian value into angle conrresponding based on factor
    #define ToAngle(x) (x/DEGTORADIANVAL)

    // Transformation 
    typedef fVec3 Position;
    typedef fVec3 Scale;
    typedef fVec3 Rotation;

    static fVec3 normalize(fVec3 val)
    {
        // take power of two of all component and find squareroot for normalizing
        float allsqr = sqrt(pow(val.x,2) + pow(val.y,2) + pow(val.z,2));
        fVec3 result(val.x/allsqr, val.y/allsqr, val.z/allsqr);
        return result;
    }

    static float dot_product(fVec3 a, fVec3 b)
    {
        return (a.x*a.y + a.y*b.y + a.z*b.z);
    }

    static fVec3 cross_product(fVec3 a, fVec3 b)
    {
        fVec3 result;
        result.x = a.y * b.z - b.y * a.z;
        result.y = a.x * b.z - b.x * a.z;
        result.z = a.x * b.y - b.x * a.y;
        return result;
    }

    struct Transform
    {
        public:
        Transform(){    position = rotation = fVec3(0); scale = fVec3(1);   }
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


    // The matrix here is the implementation of other library with wrapper so have to inspect if something breaks (source : check ./src/external/math/math_3d.h)
    struct Mat4
    {
        public:
        Mat4(float val = 1)
        {
            allInitialize(val);
        }

        Mat4(   float m00, float m10, float m20, float m30,
	            float m01, float m11, float m21, float m31,
	            float m02, float m12, float m22, float m32,
	            float m03, float m13, float m23, float m33)
        {
            row[0][0] = m00, row[1][0] = m10, row[2][0] = m20, row[3][0] = m30,
            row[0][1] = m01, row[1][1] = m11, row[2][1] = m21, row[3][1] = m31,
            row[0][2] = m02, row[1][2] = m12, row[2][2] = m22, row[3][2] = m32,
            row[0][3] = m03, row[1][3] = m13, row[2][3] = m23, row[3][3] = m33;
        }



        Mat4 operator=(Mat4 &rhs)
        {
            for(int i=0;i<16;++i)
            {
                *(data + i) = *(rhs.data + i);
            }
            return *this;
        }
        friend Mat4 operator*(float v, Mat4 &rhs);
        Mat4 operator*(Mat4 &rhs)
        {
            Mat4 result;
            for(int i=0;i<4;++i)
            {
                for(int j=0;j<4;++j)
                {
                    result.row[i][j] = 0;
                    for(int k=0;k<4;++k)
                    {
                        result.row[i][j] += this->row[i][k] * rhs.row[k][j];
                    }
                }
            }
            return result;
        }

        Mat4 operator*=(Mat4 &rhs)
        {
            Mat4 result = *this;
            for(int i=0;i<4;++i)
            {
                for(int j=0;j<4;++j)
                {
                    for(int k=0;k<4;++k)
                    {
                        this->row[i][j] += result.row[i][k] * rhs.row[k][j];
                    }
                }
            }
            return *this;
        }

        // protected:
        union {
        // The first index is the column index, the second the row index. The memory
        // layout of nested arrays in C matches the memory layout expected by OpenGL.
        float data[16];
        float row[4][4];
        // OpenGL expects the first 4 floats to be the first column of the matrix.
        // So we need to define the named members column by column for the names to
        // match the memory locations of the array elements.
        struct {    float m00, m01, m02, m03;
                    float m10, m11, m12, m13;
                    float m20, m21, m22, m23;
                    float m30, m31, m32, m33;   };
        };

        void allInitialize(float v)
        {
            for(int i=0;i<16;++i)
            {
                *(data + i) = 0;
            }
            *(data + 0) = v;
            *(data + 5) = v;
            *(data + 10) = v;
            *(data + 15) = v;
        }
    };

    Mat4 operator*(float v, Mat4 &rhs)
    {
        Mat4 result;
        for(int i=0;i<16;++i)
        {
            result.data[i] = rhs.data[i] * v;
        }
        return result;
    }

    static Mat4 ScaleMat(Mat4 &mat, fVec3 scale)
    {
        Mat4 result;
        result.row[0][0] = scale.x;
        result.row[1][1] = scale.y;
        result.row[2][2] = scale.z;
        return result;
    }

    static Mat4 TranslateMat(Mat4 &mat, fVec3 trans)
    {
        Mat4 result;
        result.row[0][3] = trans.x;
        result.row[1][3] = trans.y;
        result.row[2][3] = trans.z;
        return result;
    }

    static Mat4 RotateMatX(float angle)
    {
        float s = sinf(angle), c = cosf(angle);
        return Mat4(
            1,  0,  0,  0,
            0,  c, -s,  0,
            0,  s,  c,  0,
            0,  0,  0,  1
        );
    }

    static Mat4 RotateMatY(float angle)
    {
        float s = sinf(angle), c = cosf(angle);
        return Mat4(
            c,  0,  s,  0,
            0,  1,  0,  0,
            -s,  0,  c,  0,
            0,  0,  0,  1
        );
    }

    static Mat4 RotateMatZ(float angle)
    {
        float s = sinf(angle), c = cosf(angle);
        return Mat4(
            c,  0,  s,  0,
            0,  1,  0,  0,
            -s,  0,  c,  0,
            0,  0,  0,  1
        );
    }

    static Mat4 RotateMat(float angle, fVec3 axis)
    {
        fVec3 normalized_axis = Math::normalize(axis);
        float x = normalized_axis.x, y = normalized_axis.y, z = normalized_axis.z;
        float c = cosf(angle), s = sinf(angle);
        
        return Mat4(
                c + x*x*(1-c),          x*y*(1-c) - z*s,        x*z*(1-c) + y*s,    0,
                y*x*(1-c) + z*s,        c + y*y*(1-c),          y*z*(1-c) - x*s,    0,
                z*x*(1-c) - y*s,        z*y*(1-c) + x*s,        c + z*z*(1-c),      0,
                0,                      0,                      0,                  1
            );
    }

    static Mat4 TransposeMat(Mat4 &mat)
    {
        return Mat4(
            mat.m00, mat.m01, mat.m02, mat.m03,
            mat.m10, mat.m11, mat.m12, mat.m13,
            mat.m20, mat.m21, mat.m22, mat.m23,
            mat.m30, mat.m31, mat.m32, mat.m33
	    );
    }

    static Mat4 IdentityMat()
    {
        return Mat4(
            1,  0,  0,  0,
            0,  1,  0,  0,
            0,  0,  1,  0,
            0,  0,  0,  1
        );
    }

    static Mat4 Ortho(float left, float right, float bottom, float top, float back, float front)
    {
        float l = left, r = right, b = bottom, t = top, n = front, f = back;
        float tx = -(r + l) / (r - l);
        float ty = -(t + b) / (t - b);
        float tz = -(f + n) / (f - n);
        return Mat4(
            2 / (r - l),  0,            0,            tx,
            0,            2 / (t - b),  0,            ty,
            0,            0,            2 / (f - n),  tz,
            0,            0,            0,            1
        );
    }

    static Mat4 Prespective(float fov, float aspectRatio, float near, float far)
    {
        float fovy_in_rad = fov;
        float f = 1.0f / tanf(fovy_in_rad / 2.0f);
        float ar = aspectRatio;
        float nd = near, fd = far;
        
        return Mat4(
            f / ar,           0,                0,                0,
            0,                f,                0,                0,
            0,                0,               (fd+nd)/(nd-fd),  (2*fd*nd)/(nd-fd),
            0,                0,               -1,                0
        );
    }

    static Mat4 LookAt(fVec3 from, fVec3 to, fVec3 up)
    {
        fVec3 z = Math::normalize(to - from) * -1;
        fVec3 x = Math::normalize(Math::cross_product(up, z));
        fVec3 y = Math::cross_product(z, x);
        
        return Mat4(
            x.x, x.y, x.z, -Math::dot_product(from, x),
            y.x, y.y, y.z, -Math::dot_product(from, y),
            z.x, z.y, z.z, -Math::dot_product(from, z),
            0,   0,   0,    1
        );
    }
}
#endif