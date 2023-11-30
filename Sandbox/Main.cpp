#include"../Platforms/OpenGL/OpenGL_App.h"
#include"../Core/Base/Object.h"
#include"../Core/Base/Log.h"

#define DEFAULT_SCR_WIDTH 800
#define DEFAULT_SCR_HEIGHT 600

using namespace OpenGL;

class SandboxApp : public OpenGL_App
{
    public:
    SandboxApp(OpenGL_Win *win) : OpenGL_App(win) {  }
    ~SandboxApp()
    {
        m_objectList.clear();
    }

    void Initialize() override {    }
    void Loop() override 
    { 
        for(int i=0;i<5;i++)
        {
            Abstract::Object *temp = new Abstract::Object;
            temp->UpdateLayer(i);
            m_objectList.push_back(temp);
            Util::Log::PrintObjectInfo(*temp);
        }

        while(!m_window->ShouldCloseWindow())
        {
            m_window->SetColor(Color(1));

            m_window->SwapFrameBuffer();
            glfwPollEvents();
        } 
    }

    private:
    std::vector<Abstract::Object*> m_objectList;
};

// Entry point of main Application for sandbox
int main(int argc, char *args[])
{
    ApplicationInfo SandboxAppInfo(DEFAULT_SCR_WIDTH, DEFAULT_SCR_HEIGHT, "Sandbox");

    OpenGL_Win *mainWindow = new OpenGL_Win(SandboxAppInfo);
    SandboxApp *mainApplication = new SandboxApp(mainWindow);

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