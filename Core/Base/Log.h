#pragma once

#include<iostream>

#include"./Object.h"

namespace Util
{
    template<typename T>
    void Print(String title, T value = T(0))
    {
        std::cout << title << value << std::endl;
    }

    class Log
    {
        public:

        static void PrintObjectInfo(Abstract::Object &obj)
        {
            #ifdef LOG_ENABLE
                std::cout << "==> Object Info <==" << std::endl;
                Print<String>("Object Name : ", obj.GetName());
                Print<String>("Object Id : ", obj.GetId());
                Print<int>("Object layer : ", obj.GetLayer());
                Print<bool>("Object Status : ", obj.GetStatus());
                Print<Abstract::Transform>("Transform\n", obj.GetTransform());
            #endif
        }
    };
}