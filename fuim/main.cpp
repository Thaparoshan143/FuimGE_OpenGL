#include"./include_headers.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

// temporary frame rate locker for now
#define MAX_FRAME_RATE 1/60.0

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
		Shader tShader("../res/Shaders/Projection/", "ver.shader", "frag.shader");
		Shader texShader("../res/Shaders/Texture/", "ver.shader", "frag.shader");
		tShader.CompileProgram();
		texShader.CompileProgram();
		Texture tex("../res/Images/temp1.jpg");

		VertexArrayObject VAO(BufferFormat::PPP_RGB_UV);
		VAO.RawBind();
	
		VertexBufferObject VBO;
		IndexBufferObject IBO;
		IBO.AppendBuffer(get_quad_index(), QUAD_IND_VERTEX_COUNT);
		VBO.AppendBuffer(get_quad_bufferind_uv({0,0},{1.5, 1.2}, {1, 1, 1}), QUAD_IND_VERTEX_COUNT + 2*4);
		VBO.Offload();
		IBO.Offload();
		VAO.AppendBufferLink(&VBO, &IBO);
		VAO.EnableVertexAttrib();
		tex.Offload();

		TextRenderer tempTextRenderer;
		tempTextRenderer.SetRenderTarget(*std::make_shared<Interface::IWindow*>(*m_window.get()));
		FreetypeTextProp tempPropFT;
		tempPropFT.m_textProperties = {Color(0), TextAlignment(CENTER)};
		tempPropFT.m_textFont = {String("Roboto"), float(1)};
		tempPropFT.m_textTransform = {fVec3(-0.8), fVec3(0)};

		tempTextRenderer.AddToQueue("Roshan Thapa", tempPropFT);
		tempPropFT.m_textTransform = {fVec3(0.8), fVec3(0)};
		tempTextRenderer.AddToQueue("Roshan Thapa2", tempPropFT);

		Shader textShader("../res/Shaders/Text/", "ver.shader", "frag.shader");
		textShader.CompileProgram();

		while (!(*m_window)->ShouldCloseWindow())
		{
			float startTime = glfwGetTime();
			float delta = 0;

			startTime = glfwGetTime();

			(*m_window)->SetBgColor(Color(1, 0.5, 0.2, 1));

			texShader.UseProgram();
			VAO.Bind();
			tex.Bind();
			Mat4 projection = Mat4(1);
			texShader.SetUniformMat4("projection", projection);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			
			tempTextRenderer.RenderText();

			(*m_window)->SwapFrameBuffer();
			glfwPollEvents();

			while(delta <= MAX_FRAME_RATE)
			{
				delta = glfwGetTime() - startTime;
			}
			tempTextRenderer.SetQueueText(std::to_string(1/delta), 0);
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