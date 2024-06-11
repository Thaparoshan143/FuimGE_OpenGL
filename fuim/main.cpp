#include"./include_headers.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

class FuimApp : public Application
{
	public:
	FuimApp(ApplicationInfo appInfo):OpenGL_Application(appInfo) {	}
	~FuimApp() {		}

	void Initialize() override 
	{

	}

	void Loop() override 
	{
		
		while (!(*m_window)->ShouldCloseWindow())
		{
            (*m_window)->SetBgColor(Color(1, 0.5, 0.2, 1));

			(*m_window)->SwapFrameBuffer();
			glfwPollEvents();
		}
	}
};

int main()
{
	ApplicationInfo appInfo(SCR_WIDTH, SCR_HEIGHT, "FUIM GE");
    Window *mainWindow = new Window(appInfo);
	FuimApp mainApp(appInfo);
	mainApp.SetWindow(mainWindow);

	mainApp.Initialize();
	mainApp.Loop();

	return 0;
}