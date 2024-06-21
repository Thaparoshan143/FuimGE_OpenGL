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
		Shader tShader("../res/Shaders/Basic/", "ver.shader", "frag.shader");
		tShader.CompileProgram();

		VertexArrayObject VAO;
		VAO.RawBind();
		VertexBufferObject VBO;
		IndexBufferObject IBO;
		IBO.AppendBuffer(get_quad_index(), QUAD_IND_VERTEX_COUNT);
		VBO.AppendBuffer(get_quad_bufferind({0,0},{1.5, 1.2}, {1, 1, 1}), QUAD_IND_VERTEX_COUNT);
		VBO.Offload();
		IBO.Offload();
		VAO.AppendBufferLink(&VBO, &IBO);
		VAO.EnableVertexAttrib();

		TextRenderer tempTextRenderer;
		tempTextRenderer.SetRenderTarget(*std::make_shared<Interface::IWindow*>(*m_window.get()));
		FreetypeTextProp tempPropFT;
		tempPropFT.m_textProperties = {Color(0), TextAlignment(CENTER)};
		tempPropFT.m_textFont = {String("Roboto"), float(1)};
		tempPropFT.m_textTransform = {fVec3(0), fVec3(0)};

		tempTextRenderer.AddToQueue("Roshan Thapa", tempPropFT);
		tempPropFT.m_textTransform = {fVec3(0.5), fVec3(0)};
		tempTextRenderer.AddToQueue("Roshan Thapa2", tempPropFT);

		Shader textShader("../res/Shaders/Text/", "ver.shader", "frag.shader");
		textShader.CompileProgram();

		while (!(*m_window)->ShouldCloseWindow())
		{
			// std::cout << "Main loop entered";
            (*m_window)->SetBgColor(Color(1, 0.5, 0.2, 1));

			tShader.UseProgram();
			// glDrawArrays(GL_TRIANGLES, 0, 6);
			VAO.Bind();
			glDrawElements(GL_TRIANGLES, 10, GL_UNSIGNED_INT, 0);
			tempTextRenderer.RenderText();

			// tempText.Rendertext(textShader.GetProgramHandle(), 0, 0, 0.6, Color3(0.8), false);

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