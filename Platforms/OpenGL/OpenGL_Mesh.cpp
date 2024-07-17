#include"./OpenGL_Mesh.h"

namespace OpenGL
{
    OpenGL_Mesh::OpenGL_Mesh(String name, bool isActive) : IRenderableEntity(name, isActive)
    {
        m_textures = new std::vector<Texture*>;
        m_VAO.AppendBufferLink(&m_VBO, &m_IBO);
    }

    void OpenGL_Mesh::Render()
    {
        // bind appropriate textures
        unsigned int diffuseNr  = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr   = 1;
        unsigned int heightNr   = 1;

        for(unsigned int i = 0; i < (*m_textures).size(); i++)
        {
            std::cout << "Cannot go inside" << std::endl;
            (*m_textures)[i]->SetActiveSlot(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            String number;
            if((*m_textures)[i]->IsOfType("texture_diffuse"))
                number = std::to_string(diffuseNr++);
            else if((*m_textures)[i]->IsOfType("texture_specular"))
                number = std::to_string(specularNr++); // transfer unsigned int to string
            else if((*m_textures)[i]->IsOfType("texture_normal"))
                number = std::to_string(normalNr++); // transfer unsigned int to string
             else if((*m_textures)[i]->IsOfType("texture_height"))
                number = std::to_string(heightNr++); // transfer unsigned int to string

            // now set the sampler to the correct texture unit
            m_shader->SetUniformInt(((*m_textures)[i]->m_type + number), i);
            // and finally bind the texture
            (*m_textures)[i]->Bind();
        }
        
        // draw mesh
        m_VAO.Bind();
        glDrawElements(GL_TRIANGLES, m_IBO.m_buffer.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }
}