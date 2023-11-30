#pragma once

#include"./Types.h"

typedef enum NormalizeMode
{
    ZEROTOONE = 0,
    NEGTOPOSONE = 1,
}NormalizeMode;

namespace Util
{
    fVec2 _normalize2D(fVec2 curPos, fVec2 against, NormalizeMode mod = NEGTOPOSONE)
    {
        if(mod = NEGTOPOSONE)
            return fVec2((curPos.x - against.x/2.0)/(against.x/2.0), (curPos.y - against.y/2.0)/(against.y/2.0));
        else
            return fVec2(curPos.x/against.x, curPos.y/against.y);
    }

    fVec3 _normalize3D(fVec3 curPos, fVec3 against, NormalizeMode mod = NEGTOPOSONE)
    {
        if(mod = NEGTOPOSONE)
            return fVec3((curPos.x - against.x/2.0)/(against.x/2.0), (curPos.y - against.y/2.0)/(against.y/2.0), (curPos.z - against.z/2.0)/(against.z/2.0));
        else
            return fVec3(curPos.x/against.x, curPos.y/against.y, curPos.z/against.z);
    }
}