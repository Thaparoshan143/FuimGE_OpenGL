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
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   		glEnable( GL_BLEND );
	}

	void Loop() override 
	{
		String modelPath = String("../res/Models/cube.fbx");
		String modelPath2 = String("../res/Models/icoSphere.fbx");
		
		OpenGL_Object tempObject(modelPath, "Cube");
		OpenGL_Object tempObject2(modelPath2, "Ico Sphere");

		Shader modelShader("../res/Shaders/Model/", "ver.shader", "frag.shader");
		modelShader.CompileProgram();

		while (!(*m_window)->ShouldCloseWindow())
		{	
			OpenGL::FrameTimer::StartTime();

			(*m_window)->SetBgColor(Color(1, 0.5, 0.2, 1));

			modelShader.UseProgram();
			glm::mat4 tempMat = glm::scale(glm::mat4(1), fVec3(0.0072));
			tempMat = glm::scale(tempMat, fVec3(0.8));

			modelShader.SetUniformMat4("projection", tempMat);
			tempObject.RenderObject();
			tempMat = glm::scale(tempMat, fVec3(0.4));
			modelShader.SetUniformMat4("projection", tempMat);
			tempObject2.RenderObject();

			(*m_window)->SwapFrameBuffer();
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