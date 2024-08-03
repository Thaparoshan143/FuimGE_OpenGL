#ifndef APPLICATION_CPP
#define APPLICATION_CPP

#include<window.cpp>
#include<renderer.cpp>

#define DELAY_TIME 1/24.0
#define SEC_CLOCK()	((double)clock() / CLOCKS_PER_SEC)

void delay(double secs = DELAY_TIME)
{
	double end = SEC_CLOCK() + secs;
	while(SEC_CLOCK() < end);
}

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

        virtual ~Application()
        {   
		    delete m_renderer;
        }

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
	    
        Renderer *m_renderer;
        Window m_window;
    };
}
#endif