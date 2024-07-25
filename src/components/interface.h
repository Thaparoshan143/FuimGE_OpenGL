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
        
        protected:
        std::string name;
        bool isActive;
        Math::Transform transform;
    };

    class IRenderableGUI
    {
        public:
        virtual void Render() = 0;
    };
}