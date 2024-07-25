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
    
    class Mesh : Interface::IRenderableEntity
    {
        public:
        Mesh()
        {
            m_textures = new std::vector<Texture*>;
        }

        uint32_t GetBufferLayout() {   return m_VAO.GetFormat();   }

        void Render() override
        {
            // bind appropriate textures
            unsigned int diffuseNr  = 1;
            unsigned int specularNr = 1;
            unsigned int normalNr   = 1;
            unsigned int heightNr   = 1;


            // for(unsigned int i = 0; i < (*m_textures).size(); i++)
            // {
            //     std::cout << "Cannot go inside" << std::endl;
            //     (*m_textures)[i]->SetActiveSlot(GL_TEXTURE0 + i); // active proper texture unit before binding
            //     // retrieve texture number (the N in diffuse_textureN)
            //     std::string number;
            //     if((*m_textures)[i]->IsOfType("texture_diffuse"))
            //         number = std::to_string(diffuseNr++);
            //     else if((*m_textures)[i]->IsOfType("texture_specular"))
            //         number = std::to_string(specularNr++); // transfer unsigned int to string
            //     else if((*m_textures)[i]->IsOfType("texture_normal"))
            //         number = std::to_string(normalNr++); // transfer unsigned int to string
            //     else if((*m_textures)[i]->IsOfType("texture_height"))
            //         number = std::to_string(heightNr++); // transfer unsigned int to string

            //     // now set the sampler to the correct texture unit
            //     m_shader->SetUniformInt(((*m_textures)[i]->m_type + number), i);
            //     // and finally bind the texture
            //     (*m_textures)[i]->Bind();
            // }
            
            // draw mesh
            m_VAO.Bind();
            glDrawElements(GL_TRIANGLES, m_IBO.GetBufferSize(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // always good practice to set everything back to defaults once configured.
            glActiveTexture(GL_TEXTURE0);
        }

        friend class Model;
        protected:
        VertexBufferObject m_VBO;
        IndexBufferObject m_IBO;
        VertexArrayObject m_VAO;
        std::vector<Texture*> *m_textures;
        Shader *m_shader;
    };

    class Model : Interface::IRenderableEntity
    {
        public:
        Model(std::string path, bool gamma = false)
        {
            loadModel(path);
        }

        void Render() override
        {
            for(uint i = 0;i<(uint)m_meshes.size();++i)
            {
                m_meshes[m_meshes.size() - i - 1]->Render();
            }
        }

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
            }
            // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
            for(uint i = 0; i < node->mNumChildren; i++)
            {
                std::cout << "Processing Children Node : " << i << std::endl;
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
                if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                    // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                    vertex.push_back(mesh->mTextureCoords[0][i].x); 
                    vertex.push_back(mesh->mTextureCoords[0][i].y);
                }
                else
                {
                    vertex.push_back(0);
                    vertex.push_back(0);
                }

                // normals
                if (mesh->HasNormals())
                {
                    vertex.push_back(mesh->mNormals[i].x);
                    vertex.push_back(mesh->mNormals[i].y);
                    vertex.push_back(mesh->mNormals[i].z);
                }
                temp->m_VBO.AppendBuffer(vertex);
            }
            temp->m_VAO.SetFormat(BufferFormat::PPP_UV);
            if(mesh->HasNormals())
            {
                temp->m_VAO.SetFormat(BufferFormat::PPP_UV_NNN);
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

            temp->m_VAO.Bind();
            temp->m_VBO.Offload();
            temp->m_IBO.Offload();

            // process materials
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; 
            // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
            // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
            // Same applies to other texture as the following list summarizes:
            // diffuse: texture_diffuseN
            // specular: texture_specularN
            // normal: texture_normalN

            std::vector<Texture*> *maps = nullptr;
            // 1. diffuse maps
            maps = queryTextureIfNotLoad(material, aiTextureType_DIFFUSE, "texture_diffuse");
            if(maps)
                temp->m_textures->insert(temp->m_textures->end(), maps->begin(), maps->end());

            // 2. specular maps
            maps = queryTextureIfNotLoad(material, aiTextureType_SPECULAR, "texture_specular");
            if(maps)
                temp->m_textures->insert(temp->m_textures->end(), maps->begin(), maps->end());

            // 3. normal maps
            maps = queryTextureIfNotLoad(material, aiTextureType_NORMALS, "texture_normal");
            if(maps)
                temp->m_textures->insert(temp->m_textures->end(), maps->begin(), maps->end());

            // 4. height maps
            maps = queryTextureIfNotLoad(material, aiTextureType_HEIGHT, "texture_height");
            if(maps)
                temp->m_textures->insert(temp->m_textures->end(), maps->begin(), maps->end());
            
            // return a mesh object created from the extracted mesh data
            temp->m_VAO.EnableVertexAttrib();
            temp->m_VAO.Unbind();
            return temp;
        }

        bool loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
        {
            try
            {
                for(uint i = 0; i < mat->GetTextureCount(type); i++)
                {
                    aiString str;
                    mat->GetTexture(type, i, &str);
                    std::cout << "String recieved from GetTexture() : " << str.data << std::endl;
                    Texture *temp = new Texture(str.data);
                    temp->m_type = typeName;
                    temp->Bind();
                    temp->Offload();
                    
                    // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
                    m_texturesLoaded[typeName].push_back(temp);
                }
                return true;
            }
            catch(...)
            {
                std::cout << "Couldn't find texture" << std::endl;
                return false;
            }
        }

        std::vector<Texture*>* queryTextureIfNotLoad(aiMaterial *mat, aiTextureType type, std::string typeName)
        {
            if(m_texturesLoaded.count(typeName) == 0)
            {
                std::cout << "Not found! loading texture for model" << std::endl;
                if(!loadMaterialTextures(mat, type, typeName))
                    return nullptr;
            }
            // std::cout << "Trying to return" << std::endl;
            return (m_texturesLoaded.count(typeName) != 0)?&m_texturesLoaded[typeName]:nullptr;
        }
        
        protected:
        std::vector<Mesh*> m_meshes;
        std::map<std::string, std::vector<Texture*>> m_texturesLoaded;
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

        uint32_t GetBufferLayout() {   return m_VAO.GetFormat();   }

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
