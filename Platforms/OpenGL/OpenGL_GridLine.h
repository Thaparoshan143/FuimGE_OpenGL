#pragma once

#include"../../Core/Base/Types.h"
#include"../../Core/Base/IEntity.h"

#include"./OpenGL_Buffer.h"
#include"./OpenGL_Window.h"
#include"./OpenGL_Shader.h"

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
        Color color;
    };

    class GridLine : public Interface::IEntity
    {
        public:
        GridLine(Interface::IWindow *target, GridProp gridProp) : m_VAO(PPP), m_shader("../res/Shaders/Camera/", "ver.shader", "frag.shader"), m_gridProp(gridProp)
        {
            m_target = target;
            float *temp = get_grid_pointxy_ind(m_gridProp.start, m_gridProp.end, m_gridProp.gridCount);
            m_VBO.AppendBuffer(temp, get_grid_xy_size(m_gridProp.gridCount));
            m_VBO.Offload();
            m_VAO.AppendBufferLink(&m_VBO);
            m_VAO.EnableVertexAttrib();
            m_shader.CompileProgram();
        }

        void Draw()
        {
            m_VAO.Bind();
            m_shader.UseProgram();
            m_shader.SetUniformMat4("view", m_target->GetCamera()->GetViewMatrix());
            m_shader.SetUniformMat4("projection", m_target->GetCamera()->GetProjectionMatrix(m_target->GetWindowDim()));
            m_shader.SetUniformMat4("model", m_target->GetCamera()->GetModelMatrix());
            m_shader.SetUniformVec4("col", m_gridProp.color);
            glDrawArrays(GL_LINES, 0, m_VBO.m_buffer.size());
        }

        VertexArrayObject m_VAO;
        VertexBufferObject m_VBO;
        OpenGL_Shader m_shader;

        GridProp m_gridProp;
        Interface::IWindow *m_target;
    };
}