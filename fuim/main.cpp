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
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK); 
	}

	void Loop() override 
	{
		String modelPath = String("../res/Models/Car.obj");
		
		OpenGL_Object tempObject(modelPath, "Cube");

		Shader gridShader("../res/Shaders/Camera/", "ver.shader", "frag.shader");
		gridShader.CompileProgram();
		Shader cameraShader("../res/Shaders/Lighting/Directional/", "ver.shader", "frag.shader");
		cameraShader.CompileProgram();

		OpenGL_Camera *tempCamRef = dynamic_cast<OpenGL_Camera*>(m_window->GetCamera());

		GridLine tempGrid(this->m_window, GridProp({-10, 10}, {10, -10}, {19, 19}));

		while (!m_window->ShouldCloseWindow())
		{	
			OpenGL::FrameTimer::StartTime();

			m_window->SetBgColor(Color(0.1, 0.1, 0.1, 1));

			tempGrid.Draw();

			cameraShader.UseProgram();
			cameraShader.SetUniformMat4("view", tempCamRef->GetViewMatrix());
			Mat4 tempMat = tempCamRef->GetProjectionMatrix(m_window->GetWindowDim());
			cameraShader.SetUniformMat4("projection", tempMat);
			cameraShader.SetUniformMat4("model", tempCamRef->GetModelMatrix());
			cameraShader.SetUniformVec3("col", fVec3(1, 0.5, 0.2));
			cameraShader.SetUniformVec3("light.direction", fVec3(-0.2f, -1.0f, -0.3f));
			cameraShader.SetUniformVec3("viewPos", tempCamRef->m_transform.position);

			// light properties
			cameraShader.SetUniformVec3("light.ambient", fVec3(0.05f));
			cameraShader.SetUniformVec3("light.diffuse", fVec3(0.5f));
			cameraShader.SetUniformVec3("light.specular", fVec3(1.0f));

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