#include"../../Core/Base/ITimer.h"

#include"../../includes/GLAD/glad.h"
#include"../../includes/GLFW/glfw3.h"

#include <unistd.h> 

namespace OpenGL
{
    #define DEFAULT_FPS 60

    typedef void (*NoArgCallbackFunc)();

    class FrameTimer : public Interface::ITimer
    {
        public:
        FrameTimer()    {   }
        
        static void StartTime()
        {
            m_start = glfwGetTime();
        }

        static void Reset()
        {
            StartTime();
        }

        static void EndTime()
        {
            m_end = glfwGetTime();
            GetDelta();
        }

        static double GetTimeFromStart()
        {
            return glfwGetTime() - m_start;
        }

        static void PassResidualDelta()
        {
            if(m_delta >= m_responseTime)   {   return; }
            usleep((m_responseTime - m_delta) * pow(10, 6));
        }

        static void SetFPS(uint fps)   {   m_FPS = fps;    m_responseTime = 1/fps; }
        static float GetDeltaBasedFPS() {   return 1.0/GetDelta();  }
        static void SetLockFPS(bool lockFps)   {   m_FPSLock = lockFps;    }
        static bool IsFPSLock()    {   return m_FPSLock & true;   }
        // This contains the residual delta free time for given response time 
        static double GetFreeDeltaTime()   {   return (m_responseTime - GetDelta());    }

        protected:
        // This is the minimum FPS to lock into
        static uint m_FPS;
        static bool m_FPSLock;
        static double m_responseTime;
    };

    uint FrameTimer::m_FPS = DEFAULT_FPS;
    double FrameTimer::m_responseTime = 1.0/DEFAULT_FPS;
}