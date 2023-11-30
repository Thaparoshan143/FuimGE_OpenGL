#pragma once

#include"./Types.h"

struct WindowInfo
{
	iVec2 dim;
	String title;

	WindowInfo(int w = 800, int h = 600, String t = "App")
    {
        dim.x = w;  dim.y = h; title = t;	
    } 
};

namespace Abstract
{
    class Window
    {
        public:
        Window(WindowInfo &winInfo) {   m_winInfo = winInfo;    }
        virtual void SetActive() = 0;
        virtual inline bool ShouldCloseWindow() = 0;
        virtual void SetColor(Color col) = 0;
        virtual void SwapFrameBuffer() = 0;
        inline iVec2 GetWindowSize() { return m_winInfo.dim; }
        inline WindowInfo GetWindowInfo() { return m_winInfo; }

        WindowInfo m_winInfo;
    };
}