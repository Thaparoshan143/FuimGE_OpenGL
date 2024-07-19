#ifndef APPLICATION_CPP
#define APPLICATION_CPP

#include<window.cpp>
#include<timer.cpp>

namespace Component
{
    // Later change with other property with inheriting from window prop for now typedef
    // struct ApplicationProp : public WindowProp   {   };
    typedef WindowProp ApplicationProp;

    class Application
    {
        public:
        Application(ApplicationProp appInfo) : m_window(appInfo) 
        {
            
        }

        virtual ~Application()  {   }
        virtual void Initialize()   {   }
        virtual void Loop()
        {
            while(!m_window.ShouldCloseWindow())
            {
                m_window.SetBgColor(DEFAULT_WINDOW_BG);
                
                m_window.SwapFrameBuffer();
                delay();
                glfwPollEvents();
            }
        }

        Window m_window;
    };
}
#endif