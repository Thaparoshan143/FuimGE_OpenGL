#include<application.cpp>
#include<log.cpp>
#include<entity.cpp>
#include<renderer.cpp>
#include<imgui.h>
#define __OBJC__
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>

#define FONT_SIZE 16

class GEApplication : public Component::Application
{
	public:
	GEApplication(Component::ApplicationProp appProp) : Application(appProp)
	{
		Component::CameraProp camProp;
		camProp.worldUp = fVec3(0, 1, 0);
		camProp.pitch = -25;
		camProp.yaw = -135;
		camProp.speed = Math::VERYFAST * CAM_SPEEDMULTIPLIER;
		camProp.sensitivity = Math::HIGH * CAM_SENMULTIPLIER;
		camProp.zoom = 45;
		Component::GridProp gridProp({-10, 10}, {10, -10}, {19, 19});
		m_renderer = new Component::Renderer(camProp, gridProp);
		m_renderer->GetCamera().m_transform.position = fVec3(5, 2, 5);
	}

	~GEApplication()	
	{
		delete m_renderer;
	}

	void Initialize() override	
	{	
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);  


		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		m_io = ImGui::GetIO();
		m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		m_io.Fonts->AddFontFromFileTTF("./res/Fonts/Roboto.ttf", FONT_SIZE);
   	 	ImGui::StyleColorsDark();
		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(this->m_window.GetWindowHandle(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
		ImGui_ImplOpenGL3_Init("#version 330"); 
	}

	void Loop() override
	{
		Component::Shader shader("./res/Shaders/Texture/", "ver.shader", "frag.shader");
		shader.CompileProgram();

		using namespace Component;
		Mesh mesh("./res/Models/Sphere.obj", BufferFormat::PPP_UV);
		mesh.Offload();

		Texture texture("./res/Images/temp1.jpg");
		texture.Offload();

		while(!m_window.ShouldCloseWindow())
		{
        	glfwPollEvents();

			m_window.SetBgColor(DEFAULT_WINDOW_BG);

			m_renderer->Render();
			texture.Bind();
			shader.UseProgram();
			shader.SetUniformMat4("view", m_renderer->GetCamera().GetViewMatrix());
			shader.SetUniformMat4("projection", m_renderer->GetCamera().GetProjectionMatrix());
			shader.SetUniformMat4("model", m_renderer->GetCamera().GetModelMatrix());
			mesh.Render();

			m_window.SwapFrameBuffer();
		}
	}

	protected:
	ImGuiIO m_io;
	Component::Renderer *m_renderer;
};


int main()
{
	Component::ApplicationProp appProp(800, 600, "GE");
	GEApplication *mainApplication = new GEApplication(appProp);
	
	mainApplication->Initialize();
	mainApplication->Loop();

	delete mainApplication;

	return 0;
}