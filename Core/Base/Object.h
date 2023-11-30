#pragma once

#include"./Types.h"
#include"./Transform.h"

namespace Abstract
{
    class Object
    {
        public:
        Object(Transform transform = Transform(), String name = "Default", String id = "Object", int layer = 0, bool enable = true)
        {
            m_transform = transform;
            m_name = name;
            m_id = id;
            m_layer = layer;
            m_enabled = enable;
        }

        void SetActive(bool stat)
        {
            this->m_enabled = stat;
        }

        void UpdateLayer(int layer)
        {
            this->m_layer = layer;
        }

        inline Transform GetTransform() {   return m_transform; }
        inline String GetName() { return m_name;  }
        inline String GetId() {   return m_id;    }
        inline int GetLayer() { return m_layer; }
        inline bool GetStatus() {   return m_enabled;   }

        Transform m_transform;
        String m_name, m_id;
        int m_layer;
        bool m_enabled;
    };
}