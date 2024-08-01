#ifndef STRUCTURE_CPP
#define STRUCTURE_CPP

#include<iostream>

template<typename T>
struct Vec2
{
    public:
    Vec2(T val = 1)   {    x = val;   y = val;   }
    Vec2(T _x, T _y)   {   x = _x; y = _y; }
    Vec2(const Vec2 &rhs)   {   x = rhs.x;  y = rhs.y; }

    Vec2& operator=(const Vec2 &rhs)
    {
        x = rhs.x;  y = rhs.y;
        return *this;
    }

    union
    {
        struct  {   T u, v; };
        struct  {   T x, y; };
    };
};

template<typename T>
struct Vec3
{
    public:
    Vec3(T val = 1)   {  x = val;   y = val;   z = val;   }
    Vec3(T _x, T _y, T _z = 1)   {  x = _x; y = _y; z = _z; }
    Vec3(const Vec3 &rhs)   {  x = rhs.x;  y = rhs.y;  z = rhs.z;; } 

    Vec3& operator=(const Vec3 &rhs)
    {
        x = rhs.x;  y = rhs.y;  z = rhs.z;;  
        return *this;
    }

    friend Vec3 operator*(float v, const Vec3 &rhs);
    Vec3 operator*(float v)
    {
        return Vec3(x*v, y*v, z*v);
    }

    Vec3 operator-(const Vec3 &rhs)
    {
        return Vec3(x-rhs.x, y-rhs.y, z-rhs.z);
    }

    Vec3 operator+(const Vec3 &rhs)
    {
        return Vec3(x+rhs.x, y+rhs.y, z+rhs.z);
    }

    Vec3 operator-=(const Vec3 &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    Vec3 operator+=(const Vec3 &rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    union
    {
        struct  {   T r, g, b;   };
        struct  {   T x, y, z;   };
    };
};

template<typename T>
Vec3<T> operator*(float v, const Vec3<T> &rhs)
{
    return Vec3<T>(rhs.x*v, rhs.y*v, rhs.z*v);
}

template<typename T>
struct Vec4
{
    public:
    Vec4(T val = 1)   {  x = val;   y = val;   z = val;   w = val;   }
    Vec4(T _x, T _y, T _z = 1, T _w = 1)   {  x = _x; y = _y; z = _z; w = _w; }
    Vec4(const Vec4 &rhs)   {   x = rhs.x;  y = rhs.y;  z = rhs.z;  w = rhs.w;  } 

    Vec4& operator=(const Vec4 &rhs)
    {
        x = rhs.x;  y = rhs.y;  z = rhs.z;  w = rhs.w;  
        return *this;
    }

    union
    {
        struct  {   T r, g, b, a;   };
        struct  {   T x, y, z, w;   };
    };
};

typedef Vec2<float> fVec2;
typedef Vec2<int> iVec2;
typedef Vec2<uint32_t> uiVec2;

typedef Vec3<float> fVec3;
typedef Vec3<int> iVec3;
typedef Vec3<uint32_t> uiVec3;
typedef fVec3 Color3;

typedef Vec4<float> fVec4;
typedef Vec4<int> iVec4;
typedef Vec4<uint32_t> uiVec4;
typedef fVec4 Color4;
#endif