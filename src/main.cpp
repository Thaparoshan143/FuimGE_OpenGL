#include<application.cpp>
#include<log.cpp>
#include<entity.cpp>
#include<renderer.cpp>
#include<gui.cpp>
#include<logger.h>

Logger& logger = Logger::getInstance("application.log");
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
		m_GUIManager = new GUI::GUIManager;
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
		glEnable(GL_DEPTH_TEST);
		glEnable( GL_POLYGON_SMOOTH );
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		m_GUIManager->Init(*this);
	}

	void Loop() override
	{
		using namespace Component;
		Model tempModel2("./res/Models/cube.obj", PPP_NNN, "Cube");
		Model tempModel("./res/Models/sphere.obj", PPP_NNN, "Sphere");
		tempModel2.SetTransform(fVec3(-5, 0, 1), fVec3(0, 20, 0), fVec3(1.1));
		tempModel.SetTransform(fVec3(1, 0, -5), fVec3(0, 0, 0), fVec3(2));
		m_renderer->AddEntity(&tempModel2);
		m_renderer->AddEntity(&tempModel);
		m_renderer->AddDuplicateEntity(&tempModel, 2, true);
		m_renderer->AddDuplicateEntity(&tempModel2, 2, true);
		

		while(!m_window.ShouldCloseWindow())
		{
        	glfwPollEvents();

			m_window.BgColor();
			logger.setStreamOutput(std::cout, Logger::LogLevel::WARNING);
			logger.setStreamOutput(std::cout, Logger::LogLevel::INFO);
			logger.setStreamOutput(std::cerr, Logger::LogLevel::ERROR);
			m_renderer->Update();
			m_renderer->Render();

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
	logger.log(Logger::LogLevel::INFO, "Application started");
	mainApplication->Initialize();
	mainApplication->Loop();

	delete mainApplication;

	return 0;
}