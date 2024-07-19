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
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void Loop() override
	{
		Component::VertexArrayObject vao(BufferFormat::PPP);
		Component::VertexBufferObject vbo;
		Component::IndexBufferObject ibo;
		Component::Shader shader("./res/Shaders/Basic/", "ver.shader", "frag.shader");
		shader.CompileProgram();

		Math::Transform transform;
		transform.position = fVec3(0, 0);
		transform.rotation = fVec3(0, 0);
		transform.scale = fVec3(0.5);
		Color4 col(1, 0, 0.2, 1);
		Component::Triangle triangle(transform, col);
		triangle.Offload();

		External::fastObjMesh *mesh = External::fast_obj_read("./res/Models/cube.obj");

		float *position = mesh->positions;
		std::cout << "Position count : " << mesh->position_count << std::endl;
		Util::print_float_xyz(position, mesh->position_count*3);
		std::cout << "------------------------------- -------------------------------" << std::endl;
		// float *normal = mesh->normals;
		// std::cout << "Normal count : " << mesh->normal_count << std::endl;
		// Util::print_float_xyz(normal, mesh->normal_count*3);
		// std::cout << "------------------------------- -------------------------------" << std::endl;
		// float *texcoord = mesh->texcoords;
		// std::cout << "tex coord count : " << mesh->texcoord_count << std::endl;
		// Util::print_float_xy(texcoord, mesh->texcoord_count*2);
		// std::cout << "------------------------------- -------------------------------" << std::endl;

		// float *color = mesh->colors;
		// std::cout << "Color count : " << mesh->color_count << std::endl;
		// Util::print_float_xyz(color, mesh->color_count*3);
		// std::cout << "------------------------------- -------------------------------" << std::endl;
		
		External::fastObjIndex *index = mesh->indices;
		std::cout << "Index : " << mesh->index_count*3 << std::endl;
		for(int i=0;i<mesh->index_count;i++)
		{
			std::cout << std::setprecision(5) << (*(index + i)).p << ",\t" << (*(index + i)).t << ",\t" << (*(index + i)).n << std::endl;
		}
		std::cout << "------------------------------- -------------------------------" << std::endl;

		vao.Bind();
		vbo.AppendBuffer(position, mesh->position_count*3);
		vbo.Offload();

		uint32_t *flattenIndex = (uint32_t*)index;
		ibo.AppendBuffer(flattenIndex, mesh->index_count*3);
		ibo.Offload();
		vao.EnableVertexAttrib();

		while(!m_window.ShouldCloseWindow())
		{
			m_window.SetBgColor(DEFAULT_WINDOW_BG);
			
			// triangle.Render();
			vao.Bind();
			shader.UseProgram();
			glDrawElements(GL_TRIANGLES, mesh->index_count*3, GL_UNSIGNED_INT, 0);
	
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