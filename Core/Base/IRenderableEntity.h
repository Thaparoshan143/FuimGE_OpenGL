#pragma once

#include"./Types.h"
#include"./IEntity.h"

namespace Interface
{
    class IRenderableEntity : public IEntity
    {
        public:
        IRenderableEntity(String name = DEFAULT_ENTITY_NAME, bool isActive = true) : IEntity(name, isActive)  {   }

        virtual void Render() = 0;
    };
}