#include"./include_headers.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

class SandboxApp : OpenGL_App
{
	public:
	SandboxApp(Window *instance):OpenGL_App(instance) {}
	~SandboxApp() {		}

	void Initialize() override 
	{

	}

	void Loop() override 
	{
		while (!m_window->ShouldCloseWindow())
		{
            m_window->SetColor(Color(1,1,1,1));
			m_window->SwapFrameBuffer();
			glfwPollEvents();
		}
	}
};

int main()
{
	ApplicationInfo appInfo(SCR_WIDTH, SCR_HEIGHT, "Sandbox");
    Window *mainWindow = new Window(appInfo);
	SandboxApp mainApp(mainWindow);

	mainApp.Initialize();
	mainApp.Loop();

	return 0;
}