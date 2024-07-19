#ifndef STRUCTURE_CPP
#define STRUCTURE_CPP

#include<iostream>

template<typename T>
struct Vec2
{
    public:
    Vec2(T val)   {    x = val;   y = val;   }
    Vec2(T _x, T _y)   {   x = _x; y = _y; }
    Vec2(Vec2 &rhs)   {   x = rhs.x;  y = rhs.y; }

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
    Vec3(T val)   {  x = val;   y = val;   z = val;   }
    Vec3(T _x, T _y, T _z = 1)   {  x = _x; y = _y; z = _z; }
    Vec3(Vec3 &rhs)   {  x = rhs.x;  y = rhs.y;  z = rhs.z;; } 

    Vec3& operator=(const Vec3 &rhs)
    {
        x = rhs.x;  y = rhs.y;  z = rhs.z;;  
        return *this;
    }

    union
    {
        struct  {   T r, g, b;   };
        struct  {   T x, y, z;   };
    };
};

template<typename T>
struct Vec4
{
    public:
    Vec4(T val)   {  x = val;   y = val;   z = val;   w = val;   }
    Vec4(T _x, T _y, T _z = 1, T _w = 1)   {  x = _x; y = _y; z = _z; w = _w; }
    Vec4(Vec4 &rhs)   {   x = rhs.x;  y = rhs.y;  z = rhs.z;  w = rhs.w;  } 

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

typedef struct String
{
    public:
    String(const char *value = nullptr)
    {
        if(value != nullptr)
            data = allocateCharBuffer(value);
        else
            // std::cout << "Cannot make nullpointer allocation" << std::endl;
            data = nullptr;
            return;
        // std::cout << "--------------------" << std::endl;
    }

    ~String()   {   if(data!=nullptr)   delete[] data;  }

    u_int32_t GetLength()   {   return len; }
    char* GetRawChar()   {   return allocateCharBuffer(data);    }   
    char* operator=(const String &rhs) {   return operator=(rhs.data); }
    char* operator=(const char *rhs)
    {
        // std::cout << "Is pointer NUll : " << (rhs == nullptr) << std::endl;
        data = allocateCharBuffer(rhs);
        return data;
    }


    private:
    char* allocateCharBuffer(const char *val)
    {
        // std::cout << "Alocating char buffer" << std::endl;
        len = getCharSize(val);
        char *temp = new char[len];
        memcpy(temp, val, len);
        return temp;
    }

    u_int32_t getCharSize(const char *val)
    {
        u_int32_t count = 0;
        while(*val != '\0')  {   count++;    val++;  }
        return count;
    }

    private:
    char *data;
    int len;
}String;

// implementation of vector left
// template<class T>
// class Vector
// {
//     public:
//     Vector(uint32_t rsize)  {   }

//     void Push()
//     {

//     }

//     void Pop()
//     {

//     }

//     T *data;
//     uint32_t len;
// };

#endif