#pragma once

#include"./Types.h"

namespace Light
{
    struct ILightProp
    {
        fVec3 ambient, diffuse, specular;
    };

    struct DirectionLightProp : public ILightProp
    {
        fVec3 direction;
    };

    struct PointLightProp : public ILightProp
    {
        fVec3 position;

        float constant, linear, quadratic;
    };

    struct SpotLightProp : public PointLightProp
    {
        fVec3 direction;
        
        float cutOff, outerCutOff;
    };
}