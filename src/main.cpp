#include<application.cpp>
#include<log.cpp>
#include<entity.cpp>
#include<renderer.cpp>
#include<gui.cpp>
#include<logger.h>
#include<filesystem>

namespace fs = std::__fs::filesystem;

Logger& logger = Logger::getInstance("application.log");
class GRApplication : public Component::Application
{
	public:
	GRApplication(Component::ApplicationProp appProp) : Application(appProp)
	{
		Component::CameraProp camProp;
		camProp.worldUp = fVec3(0, 1, 0);
		camProp.pitch = -20;
		camProp.yaw = -90;
		camProp.speed = Math::VERYFAST * CAM_SPEEDMULTIPLIER;
		camProp.sensitivity = Math::HIGH * CAM_SENMULTIPLIER;
		camProp.zoom = 45;
		Component::GridProp gridProp({-10, 10}, {10, -10}, {19, 19});
		m_renderer = new Component::Renderer(camProp, gridProp);
		m_renderer->GetCamera().m_transform.position = fVec3(0, 6, 17);
		m_GUIManager = new GUI::GUIManager;
	}

	~GRApplication()	
	{
		delete m_GUIManager;
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
		std::vector<Model*> modelEntry;
		int index = -4;

		for (const auto& entry : fs::directory_iterator("./res/Models")) 
		{ 
			if (fs::is_regular_file(entry)) 
			{ 
				std::cout << entry.path() << std::endl;
				std::string fileName(entry.path().filename());
				fileName = fileName.substr(0, fileName.find_last_of("."));
				Model *temp = new Model(entry.path(), PPP_NNN, fileName);
				temp->SetTransform(fVec3(index, 0.05, index%3), fVec3(0), fVec3(1));
				modelEntry.push_back(temp);
				index+=2;
			} 
    	}

		delay();

		for(auto &item : modelEntry)
		{
			m_renderer->AddEntity(item);
		}

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

		for(auto &item : modelEntry)
		{
			delete item;
		}
	}

	protected:
	ImGuiIO m_io;
	GUI::GUIManager *m_GUIManager;
};


int main()
{
	Component::ApplicationProp appProp(1200, 800, "FuimGR");
	GRApplication *mainApplication = new GRApplication(appProp);
	logger.log(Logger::LogLevel::INFO, "Application started");
	mainApplication->Initialize();
	mainApplication->Loop();

	delete mainApplication;

	return 0;
}