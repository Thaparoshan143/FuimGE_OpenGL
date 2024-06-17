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
		Shader tShader("../res/Shaders/Basic/", "ver.shader", "frag.shader");
		tShader.CompileProgram();

		VertexArrayObject VAO;
		VAO.RawBind();
		VertexBufferObject VBO;
		IndexBufferObject IBO;
		IBO.AppendBuffer(get_quad_index(), QUAD_IND_VERTEX_COUNT);
		VBO.AppendBuffer(get_quad_bufferind({0,0},{1.5, 1.2}, {1, 0, 0.5}), QUAD_IND_VERTEX_COUNT);
		VBO.Offload();
		IBO.Offload();
		VAO.AppendBufferLink(&VBO, &IBO);
		VAO.EnableVertexAttrib();

		while (!(*m_window)->ShouldCloseWindow())
		{
            (*m_window)->SetBgColor(Color(1, 0.5, 0.2, 1));

			tShader.UseProgram();
			// glDrawArrays(GL_TRIANGLES, 0, 6);
			glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

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