#ifndef ENTITY_CPP
#define ENTITY_CPP

#include<interface.h>
#include<buffer.cpp>
#include<shader.cpp>
#include<vertexgenerator.cpp>
#include<Importer.hpp>
#include<scene.h>
#include<postprocess.h>

namespace Component
{
    #define COMP_COUNT 3
    
    class Mesh
    {
        public:
        Mesh()  {   }

        void Render()
        {
            m_VBO.Bind();
            m_IBO.Bind();
            glDrawElements(GL_TRIANGLES, m_IBO.GetBufferSize(), GL_UNSIGNED_INT, 0);
        }

        friend class Model;
        protected:
        VertexBufferObject m_VBO;
        IndexBufferObject m_IBO;
    };

    class DuplicateModel : public Interface::IRenderableEntity
    {
        public:
        DuplicateModel(IRenderableEntity *parent, Math::Transform newTransform)
        {
            m_origin = parent;
            transform = newTransform;
            name = parent->GetName() + "_" + std::to_string(dupCount);
            dupCount++;
        }

        ~DuplicateModel()   {   m_origin = nullptr; }

        void Update() override              {   m_origin->Update(); }
        void Render() override              {   m_origin->Render(); }
        uint32_t GetBufferLayout() override {   return m_origin->GetBufferLayout();    }

        Interface::IRenderableEntity *m_origin;
        static int dupCount;
    };

    int DuplicateModel::dupCount = 0;

    class Model : public Interface::IRenderableEntity
    {
        Logger& logger = Logger::getInstance();
        public:
        Model(std::string path, uint32_t layout = PPP_UV_NNN, std::string _name = "Default", bool gamma = false) : m_VAO((BufferFormat)layout)
        {
            name = _name;
            loadModel(path);
            std::cout << " IBO Count : " << m_meshes[0]->m_IBO.GetBufferSize() << std::endl;
            logger.log(Logger::LogLevel::INFO, "IBO Count : " + std::string(std::to_string(m_meshes[0]->m_IBO.GetBufferSize())));
        }

        ~Model()
        {
            for(auto &mesh : m_meshes)
            {
                delete mesh;
            }
        }

        void Update() override  {   }

        void Render() override
        {
            m_VAO.Bind();
            for(uint i = 0;i<(uint)m_meshes.size();++i)
            {
                m_meshes[m_meshes.size() - i - 1]->Render();
            }
        }

        // just temporary for now
        uint32_t GetBufferLayout() override  {   return m_VAO.GetFormat();  }
        void SetMaterial(MaterialProp mat) {    material = mat; }

        protected:
        void loadModel(std::string &path)
        {
            // read file via ASSIMP
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
            // check for errors
            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
            {
                std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
                logger.log(Logger::LogLevel::ERROR, "ASSIMP::" + std::string(importer.GetErrorString()));
                return;
            }
            // retrieve the directory path of the filepath
            m_dir = path;

            // process ASSIMP's root node recursively
            processNode(scene->mRootNode, scene);
        }

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(aiNode *node, const aiScene *scene)
        {
            // process each mesh located at the current node
            for(uint i = 0; i < node->mNumMeshes; i++)
            {
                // the node object only contains indices to index the actual objects in the scene. 
                // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                m_meshes.push_back(processMesh(mesh, scene));
                std::cout << "Processing Main Node : " << i << std::endl;
                logger.log(Logger::LogLevel::INFO, "Processing Main Node : " + std::string(std::to_string(i)));
            }
            // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
            for(uint i = 0; i < node->mNumChildren; i++)
            {
                std::cout << "Processing Children Node : " << i << std::endl;
                logger.log(Logger::LogLevel::INFO, "Processing Children Node : " + std::string(std::to_string(i)));
                processNode(node->mChildren[i], scene);
            }
        }

        Mesh* processMesh(aiMesh *mesh, const aiScene *scene)
        {
            Mesh *temp = new Mesh;
            // walk through each of the mesh's vertices
            for(uint i = 0; i < mesh->mNumVertices; i++)
            {
                std::vector<float> vertex; // parsing from assimp struct to our glm:: based
                // VertexLayout vertex;
                // position
                vertex.push_back(mesh->mVertices[i].x);
                vertex.push_back(mesh->mVertices[i].y);
                vertex.push_back(mesh->mVertices[i].z);

                // texture coordinates
                if((m_VAO.GetFormat() & BUFFER_MASKTEX) && mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                    // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                    vertex.push_back(mesh->mTextureCoords[0][i].x); 
                    vertex.push_back(mesh->mTextureCoords[0][i].y);
                }
                else if((m_VAO.GetFormat() & BUFFER_MASKTEX))
                {
                    vertex.push_back(0);
                    vertex.push_back(0);
                }

                // normals
                if ((m_VAO.GetFormat() & BUFFER_MASKNORMAL) && mesh->HasNormals())
                {
                    vertex.push_back(mesh->mNormals[i].x);
                    vertex.push_back(mesh->mNormals[i].y);
                    vertex.push_back(mesh->mNormals[i].z);
                }
                temp->m_VBO.AppendBuffer(vertex);
            }

            // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
            for(uint i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                std::vector<uint> faceIndex;
                // retrieve all indices of the face and store them in the indices vector
                for(uint j = 0; j < face.mNumIndices; j++)
                    faceIndex.push_back(face.mIndices[j]);
                temp->m_IBO.AppendBuffer(faceIndex);
            }

            m_VAO.Bind();
            temp->m_VBO.Offload();
            temp->m_IBO.Offload();
            m_VAO.EnableVertexAttrib();
            // std::cout << "name : " << name << std::endl;
            // std::cout << "##Buffer handle : VBO : " << temp->m_VBO.GetBufferHandle() << std::endl;
            // std::cout << "##Buffer handle : IBO : " << temp->m_IBO.GetBufferHandle() << std::endl;

            // return a mesh object created from the extracted mesh data
            return temp;
        }

        protected:
        VertexArrayObject m_VAO;
        std::vector<Mesh*> m_meshes;
        std::string m_dir;
        bool m_gammaCorrection;
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

        uint32_t GetBufferLayout() override {   return m_VAO.GetFormat();   }

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
