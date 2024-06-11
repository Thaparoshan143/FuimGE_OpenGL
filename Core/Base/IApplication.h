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

        void SetWindow(IWindow *window) {   m_window = std::make_unique<IWindow*>(window);  }
        inline IWindow* GetWindow()    {   return *m_window.get();    }
        inline ApplicationInfo& GetAppInfo()    {   return (*m_window.get())->GetWindowInfo();  }

        // Initial run of application
        virtual void Initialize() = 0;
        // per fps running 
        virtual void Loop() = 0;

        protected:
        ApplicationInfo m_appInfo;
        std::unique_ptr<IWindow*> m_window;
    };
}