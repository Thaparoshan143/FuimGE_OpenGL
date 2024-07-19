#ifndef LOG_CPP
#define LOG_CPP

#include<iostream>

namespace Util
{
    static void _print_mat4(float *data)
    {
        std::cout << "[";
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                std::cout << *(data + 4*i + j) << ",\t";
            }
            std::cout << std::endl;
        }
        std::cout << "]";
    }
}
#endif
