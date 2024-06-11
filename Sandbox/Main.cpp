#include"../Platforms/OpenGL/OpenGL_Application.h"
#include"../Platforms/OpenGL/OpenGL_Window.h"

#define DEFAULT_SCR_WIDTH 800
#define DEFAULT_SCR_HEIGHT 600

using namespace OpenGL;

class SandboxApp : public OpenGL_Application
{
    public:
    SandboxApp(ApplicationInfo appInfo) : OpenGL_Application(appInfo) {  }
    ~SandboxApp()
    {

    }

    void Initialize() override {    }
    void Loop() override 
    { 
        while(!(*m_window)->ShouldCloseWindow())
        {
            (*m_window)->SetBgColor(Color(1));

            (*m_window)->SwapFrameBuffer();
            glfwPollEvents();
        } 
    }

    private:
    // std::vector<Abstract::Object*> m_objectList;
};

// Entry point of main Application for sandbox
int main(int argc, char *args[])
{
    ApplicationInfo SandboxAppInfo(DEFAULT_SCR_WIDTH, DEFAULT_SCR_HEIGHT, "Sandbox");

    OpenGL_Window *mainWindow = new OpenGL_Window(SandboxAppInfo);
    SandboxApp *mainApplication = new SandboxApp(SandboxAppInfo);
    mainApplication->SetWindow(mainWindow);

    if(mainApplication!=nullptr)
    {
        mainApplication->Initialize();
        mainApplication->Loop();
    }
    else
    {
        std::cout << "Failed to create Application. Try Again!!";
    }

    delete mainWindow;
    delete mainApplication;

    return 0;
}