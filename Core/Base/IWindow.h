#pragma once

#include"./Types.h"

struct WindowInfo
{
    public:
    WindowInfo(int w = 800, int h = 600, String t = String("Application")) : width(w), height(h), title(t)  {   }
    WindowInfo(WindowInfo &winInfo) {    width = winInfo.width;  height = winInfo.height;    title = winInfo.title;  }
    ~WindowInfo()   {   }
    
    int width, height;
    String title;
};

namespace Interface
{
    class IApplication;

    class IWindow
    {
        public:
        IWindow() { }
        IWindow(WindowInfo winInfo) : m_winInfo(winInfo) {  }
        virtual ~IWindow()  {   }

        virtual void SetActive() = 0;
        virtual inline bool ShouldCloseWindow() = 0;
        virtual void SwapFrameBuffer() = 0;
        virtual void SetBgColor(float r, float g, float b, float a = 1.0f) = 0;
        bool IsActive() const {   return m_isActive;  }
        void SetActive(bool status) {   m_isActive = status;    }
        WindowInfo& GetWindowInfo() {   return m_winInfo;   }
        void SetWindowInfo(WindowInfo &winInfo) {   m_winInfo = winInfo;    }
        void SetBgColor(Color col)  {   SetBgColor(col.r, col.g, col.b, col.a); }
        iVec2 GetWindowDim() const    {   return iVec2(m_winInfo.width, m_winInfo.height);    }
        void SetTargetApplication(std::weak_ptr<IApplication*> target) {   m_target = target;  }

        private:
        std::weak_ptr<IApplication*> m_target;
        WindowInfo m_winInfo;
        bool m_isActive;
    };
}
