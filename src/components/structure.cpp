#ifndef STRUCTURE_CPP
#define STRUCTURE_CPP

#include<iostream>

typedef struct fVec2
{
    public:
    fVec2(float val)   {    x = val;   y = val;   }
    fVec2(float _x, float _y)   {   x = _x; y = _y; }
    fVec2(fVec2 &rhs)   {   x = rhs.x;  y = rhs.y; }

    fVec2& operator=(const fVec2 &rhs)
    {
        x = rhs.x;  y = rhs.y;
        return *this;
    }

    union
    {
        struct  {   float u, v; };
        struct  {   float x, y; };
    };
}fVec2;

typedef struct fVec3
{
    public:
    fVec3(float val)   {  x = val;   y = val;   z = val;   }
    fVec3(float _x, float _y, float _z = 1)   {  x = _x; y = _y; z = _z; }
    fVec3(fVec3 &rhs)   {  x = rhs.x;  y = rhs.y;  z = rhs.z;; } 

    fVec3& operator=(const fVec3 &rhs)
    {
        x = rhs.x;  y = rhs.y;  z = rhs.z;;  
        return *this;
    }

    union
    {
        struct  {   float r, g, b;   };
        struct  {   float x, y, z;   };
    };
}fVec3;

typedef struct fVec4
{
    public:
    fVec4(float val)   {  x = val;   y = val;   z = val;   w = val;   }
    fVec4(float _x, float _y, float _z = 1, float _w = 1)   {  x = _x; y = _y; z = _z; w = _w; }
    fVec4(fVec4 &rhs)   {   x = rhs.x;  y = rhs.y;  z = rhs.z;  w = rhs.w;  } 

    fVec4& operator=(const fVec4 &rhs)
    {
        x = rhs.x;  y = rhs.y;  z = rhs.z;  w = rhs.w;  
        return *this;
    }

    union
    {
        struct  {   float r, g, b, a;   };
        struct  {   float x, y, z, w;   };
    };
}fVec4;

typedef fVec3 Color3;
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