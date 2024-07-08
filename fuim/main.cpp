#include"./include_headers.h"
// #include <typeinfo>

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

class FuimApp : public Application
{
	public:
	FuimApp(ApplicationInfo appInfo):OpenGL_Application(appInfo) {	}
	~FuimApp() {		}

	void Initialize() override 
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   		glEnable(GL_BLEND);
	}

	void Loop() override 
	{
		String modelPath = String("../res/Models/cube.obj");
		
		OpenGL_Object tempObject(modelPath, "Cube");

		Shader modelShader("../res/Shaders/Model/", "ver.shader", "frag.shader");
		modelShader.CompileProgram();
		Shader cameraShader("../res/Shaders/Camera/", "ver.shader", "frag.shader");
		cameraShader.CompileProgram();

		OpenGL_Camera *tempCamRef = dynamic_cast<OpenGL_Camera*>(m_window->GetCamera());

		while (!m_window->ShouldCloseWindow())
		{	
			OpenGL::FrameTimer::StartTime();

			m_window->SetBgColor(Color(0.2, 0.2, 0.2, 1));
			cameraShader.UseProgram();
			// modelShader.UseProgram();
			cameraShader.SetUniformMat4("view", tempCamRef->GetViewMatrix());
			glm::mat4 tempMat = tempCamRef->GetProjectionMatrix(m_window->GetWindowDim());
			cameraShader.SetUniformMat4("projection", tempMat);
			cameraShader.SetUniformMat4("model", tempCamRef->GetModelMatrix());
			tempObject.RenderObject();

			m_window->SwapFrameBuffer();
			glfwPollEvents();

			OpenGL::FrameTimer::EndTime();
			OpenGL::FrameTimer::PassResidualDelta();
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