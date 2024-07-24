#include<application.cpp>
#include<log.cpp>
#include<entity.cpp>
#include<timer.cpp>
#include<renderer.cpp>
#define FAST_OBJ_IMPLEMENTATION
#include<fast_obj.h>
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
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Initialize() override	
	{	
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);   
		// glEnable(GL_CULL_FACE);
		// glCullFace(GL_BACK);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    	glfwSwapInterval(1); // Enable vsync
		// Initializing the ImGui and its context 
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
		bool show_demo_window = true;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		float *pos = new float[3];
		float *rot = new float[3];
		float *scale = new float[3];
		std::string objectName = "Default";
		bool show_inspector = true;

		External::FastObjWrapper tempObj("./res/Models/sphere.obj");
		Component::VertexArrayObject vao(BufferFormat::PPP);
		Component::VertexBufferObject vbo;
		Component::IndexBufferObject ibo;
		Component::Shader shader("./res/Shaders/Basic/", "ver.shader", "frag.shader");
		shader.CompileProgram();
		vao.Bind();
		auto tempVert = tempObj.GetVertexPosition();
		vbo.AppendBuffer(tempVert);
		vbo.Offload();
		auto tempIbo = tempObj.GetIndex();
		ibo.AppendBuffer(tempIbo);
		ibo.Offload();
		vao.EnableVertexAttrib();


		while(!m_window.ShouldCloseWindow())
		{
        	glfwPollEvents();

			glfwGetFramebufferSize(m_window.GetWindowHandle(), &m_window.m_winProp.width, &m_window.m_winProp.height);
        	glViewport(0, 0, m_window.m_winProp.width, m_window.m_winProp.height);
			m_window.SetBgColor(DEFAULT_WINDOW_BG);

			vao.Bind();
			shader.UseProgram();
			shader.SetUniformMat4("view", m_renderer->GetCamera().GetViewMatrix());
			shader.SetUniformMat4("projection", m_renderer->GetCamera().GetProjectionMatrix());
			shader.SetUniformMat4("model", m_renderer->GetCamera().GetModelMatrix());
			glDrawElements(GL_TRIANGLES, tempObj.GetIndexCount(), GL_UNSIGNED_INT, 0);
			m_renderer->Render();
			// (Your code calls glfwSwapBuffers() etc.)
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