#include<structure.cpp>
#include<application.cpp>
#include<buffer.cpp>
#include<vertexgenerator.cpp>
#include<shader.cpp>
#include<log.cpp>
#include<entity.cpp>
#include<timer.cpp>
#define FAST_OBJ_IMPLEMENTATION
#include<fast_obj.h>

class GRApplication : public Component::Application
{
	public:
	GRApplication(Component::ApplicationProp appProp) : Application(appProp)	{	}

	void Initialize() override	
	{	
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);   
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

		while(!m_window.ShouldCloseWindow())
		{
			m_window.SetBgColor(DEFAULT_WINDOW_BG);
			
			vao.Bind();
			shader.UseProgram();
			glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0);
	
			m_window.SwapFrameBuffer();
			delay();
			glfwPollEvents();
		}
	}
};


int main()
{
	Component::ApplicationProp appProp(800, 600, "GE");
	GRApplication *mainApplication = new GRApplication(appProp);
	
	mainApplication->Initialize();
	mainApplication->Loop();

	delete mainApplication;

	return 0;
}