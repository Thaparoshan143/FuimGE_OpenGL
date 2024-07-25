#ifndef ENTITY_CPP
#define ENTITY_CPP

#include<interface.h>
#include<buffer.cpp>
#include<shader.cpp>
#include<vertexgenerator.cpp>
#define FAST_OBJ_IMPLEMENTATION
#include<fast_obj.h>

namespace Component
{
    #define COMP_COUNT 3
    
    class Mesh : Interface::IRenderableEntity
    {
        public:
        Mesh(std::string path, BufferFormat bf = BufferFormat::PPP) : m_VAO(bf)
        {
            m_mesh = External::fast_obj_read(path.c_str());
        }

        ~Mesh() {   delete m_mesh;  }

        void LoadMesh(std::string path, BufferFormat bf)
        {
            delete m_mesh;
            m_VAO.SetFormat(bf);
            m_mesh = External::fast_obj_read(path.c_str());
        }

        uint32_t GetBufferLayout() override {   return m_VAO.GetFormat();   }

        void Offload()
        {
            m_VAO.Bind();
            float *vert = m_mesh->positions;
            uint32_t *ind = (uint32_t*)(m_mesh->indices);
            float *norm = m_mesh->normals;
            std::vector<float> vertVec;

            switch(m_VAO.GetFormat())
            {
                case PPP:
                    m_VBO.AppendBuffer(vert, m_mesh->position_count*COMP_COUNT);
                break;
                case PPP_NNN:
                    vertVec.reserve(1024);
                    for(int i=0;i<m_mesh->position_count*COMP_COUNT;i+=3)
                    {
                        vertVec.push_back(*(m_mesh->positions+i));
                        vertVec.push_back(*(m_mesh->positions+i+1));
                        vertVec.push_back(*(m_mesh->positions+i+2));
                        vertVec.push_back(*(m_mesh->normals+i));
                        vertVec.push_back(*(m_mesh->normals+i+1));
                        vertVec.push_back(*(m_mesh->normals+i+2));
                    }
                    m_VBO.AppendBuffer(vertVec);
                break;
                case PPP_UV:
                    vertVec.reserve(1024);
                    std::cout << "Position COunt : " << m_mesh->position_count << std::endl;
                    std::cout << "Texcord count : " << m_mesh->texcoord_count << std::endl;
                    for(int i=0;i<m_mesh->position_count*COMP_COUNT;i+=3)
                    {
                        vertVec.push_back(*(m_mesh->positions+i));
                        vertVec.push_back(*(m_mesh->positions+i+1));
                        vertVec.push_back(*(m_mesh->positions+i+2));
                        for(int j=0;j<2;++j)
                        {
                            vertVec.push_back(*(m_mesh->texcoords + 2*(i/3) + j));
                        }
                        // std::cout << "u : " << *(m_mesh->texcoords + 2*(i/3) + 0) << "\t| v : " << *(m_mesh->texcoords + 2*(i/3) + 1) << std::endl;
                    }
                    m_VBO.AppendBuffer(vertVec);
                break;
                default:
                    std::cout << "unknow format" << std::endl;
                break;
            }

            m_IBO.AppendBuffer(ind, m_mesh->index_count*COMP_COUNT);
            m_VBO.Offload();
            m_IBO.Offload();
            m_VAO.EnableVertexAttrib();
        }

        void Render() override
        {
            m_VAO.Bind();
            glDrawElements(GL_TRIANGLES, m_IBO.GetBufferSize(), GL_UNSIGNED_INT, 0);
        }

        protected:
        VertexBufferObject m_VBO;
        IndexBufferObject m_IBO;
        VertexArrayObject m_VAO;
        External::fastObjMesh *m_mesh;
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
            delete[] temp;
            m_shader.CompileProgram();
        }

        void Offload()
        {
            m_VBO.Offload();
            m_VAO.EnableVertexAttrib();
        }

        uint32_t GetBufferLayout() override  {   return m_VAO.GetFormat();   }

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
