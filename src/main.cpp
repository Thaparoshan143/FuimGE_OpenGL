#include<application.cpp>
#include<log.cpp>
#include<entity.cpp>
#include<renderer.cpp>
#include<gui.cpp>

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
		m_GUIManager = new GUI::GUIManager(*this);
	}

	~GEApplication()	
	{
		delete m_renderer;
	}

	void Initialize() override	
	{	
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);  
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		m_GUIManager->Init();
	}

	void Loop() override
	{
		Component::Shader shader("./res/Shaders/Texture/", "ver.shader", "frag.shader");
		shader.CompileProgram();

		using namespace Component;
		Model tempModel("./res/Models/sphere.obj");

		while(!m_window.ShouldCloseWindow())
		{
        	glfwPollEvents();

			m_window.SetBgColor(DEFAULT_WINDOW_BG);

			m_renderer->Render();
			shader.UseProgram();
			shader.SetUniformMat4("view", m_renderer->GetCamera().GetViewMatrix());
			shader.SetUniformMat4("projection", m_renderer->GetCamera().GetProjectionMatrix());
			shader.SetUniformMat4("model", m_renderer->GetCamera().GetModelMatrix());
			tempModel.Render();

			m_GUIManager->Render();
			m_window.SwapFrameBuffer();
		}
	}

	protected:
	ImGuiIO m_io;
	GUI::GUIManager *m_GUIManager;
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