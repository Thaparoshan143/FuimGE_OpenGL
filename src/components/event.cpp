#ifndef EVENT_CPP
#define EVENT_CPP

#include<glfw3.h>

namespace Input
{
    class KeyEvent
    {
        public:
        KeyEvent(int key, int status)
        {
            m_key = key;
            m_status = status;
        }

        bool IsPressed()    {   return m_status == GLFW_PRESS;  }
        bool IsReleased()   {   return m_status == GLFW_RELEASE;    }
        bool IsRepeat()     {   return m_status == GLFW_REPEAT; }
        bool IsKey(int key) {   return m_key == key;    }
        inline int GetKey() {   return m_key;   }

        protected:
        int m_key;
        bool m_status;
    };
}
#endif