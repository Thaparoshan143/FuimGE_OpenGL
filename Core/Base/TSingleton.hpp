#pragma once

#include"./Types.h"

namespace Singleton
{   
    template<class T>
    class TISingleton
    {
        protected:
        TISingleton(T *parent)
        {
            if(parent != nullptr)
            {
                m_instance = dynamic_cast<T*>(parent);
            }
            else
            {
                std::cout << "Instance can't be null" << std::endl;
            }
        }

        virtual ~TISingleton()  {   }

        public:
        static T* GetInstance()    {   return m_instance;  }

        protected:
        static T *m_instance;
    };

    template<class T>
    T* TISingleton<T>::m_instance = nullptr;
}