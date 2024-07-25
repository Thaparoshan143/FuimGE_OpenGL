#pragma once

#include<math.cpp>
#include<vector>


namespace Interface
{
    class IRenderableEntity
    {
        public: 
        IRenderableEntity() {   }
        virtual void Render() = 0;
        virtual uint32_t GetBufferLayout() = 0;
        
        protected:
        std::string name;
        bool isActive;
        Math::Transform transform;
    };
}