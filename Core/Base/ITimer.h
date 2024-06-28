#pragma once

#include"./Types.h"

namespace Interface
{
    class ITimer
    {
        public:
        ITimer();

        // by default return raw delta (microseconds) if any operation required override; 
        inline static double GetDelta() { return (m_delta = (m_end - m_start)); }

        protected:
        // Here all the time should be in microseconds
        static double m_start, m_end, m_delta;
    };

    double ITimer::m_start, ITimer::m_end, ITimer::m_delta;
}
