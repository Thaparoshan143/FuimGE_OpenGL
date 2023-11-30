#pragma once

#include"./Types.h"
#include"./Window.h"

typedef WindowInfo ApplicationInfo;

namespace Abstract
{
    // using same info mode for both, contains width, height and the title of window

    class Application
    {
        public:
        Application(Window *window) {   m_window = window;  }
        ~Application() {    delete m_window;    }

        virtual void Initialize() = 0;
        virtual void Loop() = 0;

        protected:
        Window *m_window;
    };
}