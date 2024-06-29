#pragma once

#include"../../Core/Base/IEntity.h"

#include"./OpenGL_Buffer.h"
#include"./OpenGL_Shader.h"
#include"./OpenGL_Texture.h"

namespace OpenGL
{
    class OpenGL_Mesh : public Interface::IEntity
    {
        public:
        OpenGL_Mesh(String name = DEFAULT_ENTITY_NAME, bool isActive = true);

        void Draw();
        friend class OpenGL_Object;
        
        protected:
        VertexArrayObject m_VAO;
        VertexBufferObject m_VBO;
        IndexBufferObject m_IBO;
        std::vector<Texture*> *m_textures;
        OpenGL_Shader *m_shader;
    };
}