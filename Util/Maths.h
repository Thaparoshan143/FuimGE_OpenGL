#pragma once

namespace Maths
{
    template<typename T>
    T _clamp(T val, T low, T high)
    {
        return val < low ? low : val > high ? high : val;
    }
}