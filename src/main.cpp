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
	GEApplication(Component::ApplicationProp appProp) : Application(appProp)	{	}

	~GEApplication()	
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Initialize() override	
	{	
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);   
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
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
		Component::VertexArrayObject vao(BufferFormat::PPP);
		Component::VertexBufferObject vbo;
		Component::IndexBufferObject ibo;
		Component::Shader shader("./res/Shaders/Basic/", "ver.shader", "frag.shader");
		shader.CompileProgram();

		External::FastObjWrapper tempObj("./res/Models/cube.obj");

		vao.Bind();
		std::vector<float> flattenVert = tempObj.GetVertexPosition();
		std::cout << "Position Count : " << flattenVert.size() << std::endl;
		Util::print_float_xyz(flattenVert.data(), flattenVert.size());
		std::cout << "------------------------------- -------------------------------" << std::endl;
		vbo.AppendBuffer(flattenVert);
		vbo.Offload();

		std::vector<uint32_t> flattenIndex = tempObj.GetIndex();
		std::cout << "Index : " << flattenIndex.size() << std::endl;
		for(int i=0;i<flattenIndex.size();i+=3)
		{
			std::cout << std::setprecision(5) << *(flattenIndex.data() + i) << ",\t" << *(flattenIndex.data() + i + 1) << ",\t" << *(flattenIndex.data() + i + 2) << std::endl;
		}
		std::cout << "------------------------------- -------------------------------" << std::endl;
		std::cout << "Index Count : " << flattenIndex.size() << std::endl;
		ibo.AppendBuffer(flattenIndex);
		ibo.Offload();
		vao.EnableVertexAttrib();

		Component::CameraProp camProp;
		camProp.worldUp = fVec3(0, 1, 0);
		camProp.pitch = 0;
		camProp.yaw = -90;
		camProp.speed = 0.5f;
		camProp.sensitivity = 0.2f;
		camProp.zoom = 45;
		Component::Renderer mainRenderer(camProp);
		mainRenderer.m_camera.m_transform.position = fVec3(0, 0, 8);

		bool show_demo_window = true;

		Util::_print_mat4(mainRenderer.m_camera.GetProjectionMatrix().data);
		std::cout << "------------------------------- -------------------------------" << std::endl;
		Util::_print_mat4(mainRenderer.m_camera.GetViewMatrix().data);

		while(!m_window.ShouldCloseWindow())
		{
        	glfwPollEvents();
			// (Your code calls glfwPollEvents())
			// ...
			// Start the Dear ImGui frame
			if(ImGui::IsKeyDown(ImGuiKey_Space))
			{
				std::cout << "Space pressed" << std::endl;
			}
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::ShowDemoWindow(); // Show demo window! :)

			// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

				ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
				ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

				if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				ImGui::End();
			}
				
			ImGui::Render();
			glfwGetFramebufferSize(m_window.GetWindowHandle(), &m_window.m_winProp.width, &m_window.m_winProp.height);
        	glViewport(0, 0, m_window.m_winProp.width, m_window.m_winProp.height);
			m_window.SetBgColor(DEFAULT_WINDOW_BG);
			vao.Bind();
			shader.UseProgram();
			shader.SetUniformMat4("view", mainRenderer.m_camera.GetViewMatrix());
			shader.SetUniformMat4("projection", mainRenderer.m_camera.GetProjectionMatrix());
			shader.SetUniformMat4("model", mainRenderer.m_camera.GetModelMatrix());
			glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0);
			// ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			// (Your code calls glfwSwapBuffers() etc.)
			m_window.SwapFrameBuffer();
		}
	}

	protected:
	ImGuiIO m_io;
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