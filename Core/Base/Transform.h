#pragma once

#include"./Types.h"

namespace Abstract
{
    class Transform
    {
        public:
        Transform(fVec3 pos = fVec3(0), fVec3 rot = fVec3(0), fVec3 sca = fVec3(1))
        {
            m_position = pos;
            m_rotation = rot;
            m_scale = sca;
        }   

        friend std::ostream& operator << (std::ostream &out, const Transform &rhs);
        fVec3 m_position, m_rotation, m_scale;
    };

    std::ostream& operator << (std::ostream &out, const Transform &rhs)
    {
        out << "Position => X : " << rhs.m_position.x << " | Y : " << rhs.m_position.y << " | Z : " << rhs.m_position.z << "\n";
        out << "Rotation => X : " << rhs.m_rotation.x << " | Y : " << rhs.m_rotation.y << " | Z : " << rhs.m_rotation.z << "\n";
        out << "Scale => X : " << rhs.m_scale.x << " | Y : " << rhs.m_scale.y << " | Z : " << rhs.m_scale.z << "\n";
        return out;
    }
}