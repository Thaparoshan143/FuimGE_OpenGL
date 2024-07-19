#ifndef ENTITY_CPP
#define ENTITY_CPP

#include<interface.h>
#include<buffer.cpp>
#include<shader.cpp>
#include<vertexgenerator.cpp>

namespace Component
{
    class Text : Interface::IRenderableEntity
    {

    };

    // This are just for quick testing, don't use otherwise
    class Triangle : Interface::IRenderableEntity
    {
        public:
        Triangle(Math::Transform trans, Color4 col) : m_VAO(BufferFormat::PPP_RGB), m_shader("./res/Shaders/", "ver.shader", "frag.shader")
        {
            float *temp = Util::get_triangle_buffer(transform.position, transform.scale, col);
            m_VBO.AppendBuffer(temp, TRIANGLE_VERTEX_COUNT);
            this->transform = trans;
            // delete[] temp;
            m_shader.CompileProgram();
        }

        void Offload()
        {
            m_VBO.Offload();
            m_VAO.EnableVertexAttrib();
        }

        void Render() override
        {
            m_shader.UseProgram();
            Math::Mat4 scale(1);
            m_shader.SetUniformMat4("proj", scale);
            m_VAO.Bind();
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        protected:
        VertexArrayObject m_VAO;
        VertexBufferObject m_VBO;
        Shader m_shader;
    };
}
#endif
