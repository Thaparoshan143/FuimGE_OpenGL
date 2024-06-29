#pragma once

#include"./Types.h"
#include"./Transformation.hpp"

namespace Interface
{
    #define DEFAULT_ENTITY_NAME "Default"
    
    class IEntity
    {
        public:
        IEntity(String name = DEFAULT_ENTITY_NAME, bool isActive = true) : m_name(name), m_isActive(isActive)    {   }

        void SetName(String name)   {   m_name = name;  }
        const String& GetName() {   return m_name;  }
        bool IsActive() {   return m_isActive & true;   }
        bool SetIsActive(bool isActive) {   return m_isActive;  }
        Transformation::Transform GetTransform()    {   m_transform.GetTransform(); }
        void SetTransform(Transformation::Transform transform)  {   m_transform = transform;    }

        // Later replace name with hashed string
        String m_name;
        bool m_isActive;
        Transformation::Transform m_transform;
    };
}