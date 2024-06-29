#pragma once

#include "../../includes/assimp/Importer.hpp"
#include "../../includes/assimp/scene.h"
#include "../../includes/assimp/postprocess.h"

#include"./OpenGL_Mesh.h"

namespace OpenGL
{
    class OpenGL_Object : public Interface::IEntity
    {
        public:
        OpenGL_Object(const String &path, String name = DEFAULT_ENTITY_NAME, bool gamma = false);

        void RenderObject();

        protected:
        void loadModel(const String &path);
        void processNode(aiNode *node, const aiScene *scene);
        OpenGL_Mesh* processMesh(aiMesh *mesh, const aiScene *scene);
        bool loadMaterialTextures(aiMaterial *mat, aiTextureType type, String typeName);

        std::vector<OpenGL_Mesh*> m_meshes;
        std::map<String, std::vector<Texture*>> m_texturesLoaded;
        String m_dir;
        bool m_gammaCorrection;

        protected:
        std::vector<Texture*>* queryTextureIfNotLoad(aiMaterial *mat, aiTextureType type, String typeName);
    };
}