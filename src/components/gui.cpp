#ifndef GUI_CPP
#define GUI_CPP

#include<imgui.h>
#include<iostream>
#include<interface.h>
#include<structure.cpp>
#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>
#include<application.cpp>

namespace GUI
{
    #define THRESHOLD_FLOAT 10000000.0f
    #define FONT_SIZE 16
    #define DEFAULT_UI_FLAGS (ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)
    // These are just the wrapper for the imgui
    class Vec3InputField : public Interface::IRenderableGUI
    {
        public:
        Vec3InputField(std::string lab, fVec3 v3 = fVec3())
        {
            label = lab;
            vec3 = v3;
            min = THRESHOLD_FLOAT * -1;
            max = THRESHOLD_FLOAT;
        }

        void Render() override
        {
            ImGuiIO& io = ImGui::GetIO();
            auto boldFont = io.Fonts->Fonts[0];
            ImGui::Text(label.c_str());
            ImGui::SameLine();

            ImVec2 buttonSize = { 25, 25 };

            ImGui::Button("X", buttonSize);

            ImGui::PushItemWidth(40);
            ImGui::SameLine();
            ImGui::DragFloat(("##X" + label).c_str(), &vec3.x, 0.1f, min, max, "%.2f");
            ImGui::SameLine();

            ImGui::Button("Y", buttonSize);

            ImGui::SameLine();
            ImGui::DragFloat(("##Y" + label).c_str(), &vec3.y, 0.1f, min, max, "%.2f");
            ImGui::SameLine();

            ImGui::Button("Z", buttonSize);

            ImGui::SameLine();
            ImGui::DragFloat(("##Z" + label).c_str(), &vec3.z, 0.1f, min, max, "%.2f");
            ImGui::PopItemWidth();
            ImGui::Separator();
        }

        std::string label;
        fVec3 vec3;
        float min, max;
    };

    class GUIManager
    {
        public:
        GUIManager(Component::Application &app) : m_app(app), m_cam(m_app.m_renderer->GetCamera())
        {
            m_showInspector = true;
        }

        ~GUIManager()
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }
        void Init()  
        {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            m_io = ImGui::GetIO();
            m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
            m_io.Fonts->AddFontFromFileTTF("./res/Fonts/Roboto.ttf", FONT_SIZE);
            ImGui::StyleColorsDark();
            // Setup Platform/Renderer backends
            ImGui_ImplGlfw_InitForOpenGL(m_app.m_window.GetWindowHandle(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
            ImGui_ImplOpenGL3_Init("#version 330"); 
            initializeGUIEntity();
        }

        void Render()
        {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if(m_showInspector)
            {
                ImGui::SetNextWindowSize(ImVec2(350, ImGui::GetMainViewport()->Size.y));
                ImGui::Begin("Menu", NULL, DEFAULT_UI_FLAGS | ImGuiWindowFlags_NoBringToFrontOnFocus);
                ImGui::Text("Inspector");
                ImGui::Spacing();ImGui::Spacing();
                ImGui::SeparatorText("Transform");
                ImGui::SetWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - ImGui::GetWindowSize().x, 0));
                for(const auto &entity : m_GUIEntity)
                {
                    entity->Render();
                }
                if(ImGui::IsWindowFocused())
                {
                    m_cam.m_isNavActive = false;
                    m_cam.m_isActive = false;
                }
                else
                {
                    m_cam.m_isNavActive = true;
                }
                ImGui::End();
            }

            ImGui::Begin("Inspector Toggle", NULL, DEFAULT_UI_FLAGS);
            ImGui::SetWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - ImGui::GetWindowSize().x, 5));
            ImGui::Checkbox(m_showInspector ? "Hide" : "Show", &m_showInspector);
            ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        protected:
        void initializeGUIEntity()
        {
            Vec3InputField *GUIentry = new Vec3InputField("Position : ");
            m_GUIEntity.push_back(GUIentry);
            GUIentry = new Vec3InputField("Rotation : ");
            m_GUIEntity.push_back(GUIentry);
            GUIentry = new Vec3InputField("Scale Fc : ");
            m_GUIEntity.push_back(GUIentry);
        }

        protected:
        ImGuiIO m_io;
        Component::Application &m_app;
        Component::Camera &m_cam;
        bool m_showInspector;
        std::vector<Interface::IRenderableGUI*> m_GUIEntity;
    };
}
#endif