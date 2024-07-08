#pragma once

#include"./IWindow.h"

// Currently using same Infobased
typedef WindowInfo ApplicationInfo;

#if 0
// one of the possible ApplicationInfo Structure
struct ApplicationInfo : WindowInfo 
{
    public:
    ApplicationInfo(int w, int h, wString t, int fps): WindowInfo(w, h, t), m_fps(fps)  {   }
}
#endif

namespace Interface
{
    class IApplication
    {
        public:
        IApplication(ApplicationInfo appInfo) : m_appInfo(appInfo)   {   }
        virtual ~IApplication() {   }

        void SetWindow(IWindow *window) {   m_window = window;  }
        inline IWindow* GetWindow()    {   return m_window;    }
        inline ApplicationInfo& GetAppInfo()    {   return m_window->GetWindowInfo();  }

        // Initial run of application
        virtual void Initialize() = 0;
        // per fps running 
        virtual void Loop() = 0;

        protected:
        ApplicationInfo m_appInfo;
        IWindow* m_window;
    };
}