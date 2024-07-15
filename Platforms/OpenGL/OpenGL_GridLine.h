#pragma once

#include"../../Core/Base/Types.h"
#include"../../Core/Base/IEntity.h"

#include"./OpenGL_Buffer.h"

#define TESTMODE
#include"../../Util/Util.cpp"

namespace OpenGL
{
    struct GridProp
    {
        GridProp(fVec2 s = fVec2(-1, 1), fVec2 e = fVec2(1, -1), fVec2 gc = fVec2(9))
        {
            start = s;
            end = e;
            gridCount = gc;
        }

        fVec2 start, end, gridCount;
    };

    class GridLine : public Interface::IEntity
    {
        public:
        GridLine(GridProp gridProp) : m_gridProp(gridProp), m_VAO(PPP)
        {
            float *temp = get_grid_pointxy_ind(m_gridProp.start, m_gridProp.end, m_gridProp.gridCount);
            m_VBO.AppendBuffer(temp, get_grid_xy_size(m_gridProp.gridCount));
            m_VBO.Offload();
            m_VAO.AppendBufferLink(&m_VBO);
            m_VAO.EnableVertexAttrib();
        }

        void Draw()
        {
            m_VAO.Bind();
            glDrawArrays(GL_LINES, 0, m_VBO.m_buffer.size());
        }

        VertexArrayObject m_VAO;
        VertexBufferObject m_VBO;

        GridProp m_gridProp;
    };
}