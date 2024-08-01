#pragma once

#include<math.cpp>
#include<vector>

namespace Component
{
    struct MaterialProp
    {
        public: 
        MaterialProp(Color4 col = Color4(0.5, 0.5, 0.5), float spe = 0.8, float diff = 0.4, float amb = 0.05, float rough = 0.5)
        {
            color = col;    specular = spe; diffuse = diff; ambient = amb;  roughness = rough;  
        }
        Color4 color;
        float specular, diffuse, ambient, roughness;
    };

    struct DirectionalLightProp
    {
        fVec3 ambient, diffuse, specular;
        fVec3 direction, color;
    };
}

namespace Interface
{
    class IRenderableEntity
    {
        public: 
        IRenderableEntity() {   }
        virtual void Update() = 0;
        virtual void Render() = 0;
        virtual uint32_t GetBufferLayout() = 0;
        inline std::string& GetName()  {   return name;  }
        Math::Transform& GetTransform() {   return transform;   }
        fVec3& GetPosition()    {   return transform.position;  }
        fVec3& GetRotation()    {   return transform.rotation;  }
        fVec3& GetScale()    {   return transform.scale;  }
        void SetTransform(fVec3 pos, fVec3 rot, fVec3 sca)  {   transform.position = pos;   transform.rotation = rot;   transform.scale = sca;  }

        Component::MaterialProp material;        
        protected:
        std::string name;
        bool isActive;
        Math::Transform transform;
    };

    class IRenderableGUI
    {
        public:
        virtual ~IRenderableGUI()   {   }
        virtual void Render() = 0;
    };
}