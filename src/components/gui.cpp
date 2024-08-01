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

#define THEME_COLOR IM_COL32(160,34,240,255)

namespace GUI
{
    #define THRESHOLD_FLOAT 10000000.0f
    #define FONT_SIZE 16
    #define DEFAULT_UI_FLAGS (ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)

    // static helper functions
    static void static_render_vec3inputField(std::string &label, fVec3 &vec3, float min = -THRESHOLD_FLOAT, float max = THRESHOLD_FLOAT)
    {
        ImGui::TextUnformatted(label.c_str());
        ImGui::SameLine();

        ImVec2 buttonSize = { 20, 20 };

        ImGui::Button("X", buttonSize);

        ImGui::PushItemWidth(35);
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

    static void static_render_vec3inputFieldSep(std::string &label, fVec3 &vec3, float min = -THRESHOLD_FLOAT, float max = THRESHOLD_FLOAT)
    {
        ImGui::TextUnformatted(label.c_str());

        ImVec2 buttonSize = { 20, 20 };

        ImGui::Button("X", buttonSize);

        ImGui::PushItemWidth(35);
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

    class GUIManager
    {
        public:
        GUIManager()
        {
            m_showInspector = false;
            m_showOutliner = false;
            m_activeObject = nullptr;
        }

        ~GUIManager()
        {
            for(auto &entity : m_GUIEntity)
            {
                delete entity;
            }
            m_activeRenderer = nullptr;
            m_activeObject = nullptr;
            m_cam = nullptr;
            m_app = nullptr;
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }

        void Init(Component::Application &app)  
        {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            m_io = ImGui::GetIO();
            m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
            m_io.Fonts->AddFontFromFileTTF("./res/Fonts/Roboto.ttf", FONT_SIZE);
            ImGui::StyleColorsDark();
            // Setup Platform/Renderer backends
            ImGui_ImplGlfw_InitForOpenGL(app.m_window.GetWindowHandle(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
            ImGui_ImplOpenGL3_Init("#version 330"); 
            initializeGUIEntity();
            m_cam = &app.m_renderer->GetCamera();
            m_activeRenderer = app.m_renderer;
            m_app = &app;
        }

        void Render()
        {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

            if(m_showOutliner)
            {
                ImGui::SetNextWindowSize(ImVec2(225, ImGui::GetMainViewport()->Size.y));
                ImGui::Begin("Outliner", NULL, DEFAULT_UI_FLAGS | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoScrollbar);
                ImGui::TextUnformatted("Outliner");
                ImGui::Spacing();ImGui::Spacing();

                if(ImGui::IsWindowFocused())
                {
                    m_cam->m_isNavActive = false;
                    m_cam->m_isActive = false;
                }
                else
                {
                    m_cam->m_isNavActive = true;
                }

                ImGui::Separator();
                ImGui::Spacing();ImGui::Spacing();
                ImGui::PushStyleColor(ImGuiCol_Text, THEME_COLOR);
                ImGui::SeparatorText("World");
                ImGui::PopStyleColor();
                static std::string appbg("Background");
                ImGui::ColorEdit3("WColor", &m_app->m_window.m_color.x);
                ImGui::Checkbox("Grid Enable", &m_activeRenderer->m_gridActive);
                ImGui::Checkbox("Wireframe", &m_activeRenderer->m_wireframeEnable);
                if(ImGui::Button("-Default-", ImVec2(ImGui::GetWindowSize().x - 25, 25)))
                {
                    m_app->m_window.m_color = fVec3(0.1);
                    m_activeRenderer->m_gridActive = true;
                    m_activeRenderer->m_wireframeEnable = false;
                }
                ImGui::Separator();

                ImGui::Spacing();ImGui::Spacing();
                ImGui::PushStyleColor(ImGuiCol_Text, THEME_COLOR);
                ImGui::SeparatorText("Camera Properties");
                ImGui::PopStyleColor();

                static std::string _camtransform[1] = {"Position"};
                static_render_vec3inputFieldSep(_camtransform[0], m_cam->m_transform.position, -10, 10);
                ImGui::SliderFloat(": Sens.", &m_cam->m_camProp.sensitivity, 0.05, 0.8);
                ImGui::Separator();
                ImGui::SliderFloat(": Speed", &m_cam->m_camProp.speed, 75, 150);
                ImGui::Separator();
                ImGui::SliderFloat(": Zoom", &m_cam->m_camProp.zoom, 15, 90);
                ImGui::Spacing();ImGui::Spacing();
                if(ImGui::Button("Reset", ImVec2(ImGui::GetWindowSize().x - 25, 25)))
                {
                    m_cam->m_transform.position = fVec3(5, 2, 5);
                    m_cam->m_camProp.sensitivity = Math::Sensitivity::HIGH * CAM_SENMULTIPLIER;
                    m_cam->m_camProp.speed = Math::Speed::FAST * CAM_SPEEDMULTIPLIER;
                    m_cam->m_camProp.zoom = 45;
                }
                ImGui::Separator();

                ImGui::Spacing();ImGui::Spacing();
                ImGui::PushStyleColor(ImGuiCol_Text, THEME_COLOR);
                ImGui::SeparatorText("Light Properties");
                ImGui::PopStyleColor();
                static std::string lightProp[1] = {"Direction : "};
                static_render_vec3inputFieldSep(lightProp[0], m_activeRenderer->m_dirLight.direction);
                ImGui::ColorEdit3("LColor", &m_activeRenderer->m_dirLight.color.x);
                if(ImGui::Button("-Reset-", ImVec2(ImGui::GetWindowSize().x - 25, 25)))
                {
                    m_activeRenderer->m_dirLight.direction = fVec3(-0.5);
                    m_activeRenderer->m_dirLight.color = fVec3(1);
                }
                ImGui::Separator();

                ImGui::SetWindowPos(ImVec2(0, 0));
                ImGui::End();
            }

			if(m_showInspector)
            {
                ImGui::SetNextWindowSize(ImVec2(350, ImGui::GetMainViewport()->Size.y));
                ImGui::Begin("Inspector", NULL, DEFAULT_UI_FLAGS | ImGuiWindowFlags_NoBringToFrontOnFocus);
                ImGui::SetWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - ImGui::GetWindowSize().x, 0));

                if(ImGui::IsWindowFocused())
                {
                    m_cam->m_isNavActive = false;
                    m_cam->m_isActive = false;
                }
                else
                {
                    m_cam->m_isNavActive = true;
                }
                ImGui::PushStyleColor(ImGuiCol_Text, THEME_COLOR);
                ImGui::SeparatorText("Objects List");
                ImGui::PopStyleColor();

                for(const auto &renderItem : m_activeRenderer->m_renderQueue)
                {
                    if(ImGui::Button(renderItem->GetName().c_str(), ImVec2(ImGui::GetWindowSize().x - 35, 25)))
                    {
                        std::cout << "Clicked button : " << renderItem->GetName() << std::endl;
                        m_activeObject = renderItem;
                    }
                }

                if((m_activeObject != nullptr))
                {
                    ImGui::Separator();ImGui::Separator();
                    ImGui::Spacing();ImGui::Spacing();
                    ImGui::TextUnformatted("Inspector");
                    ImGui::Spacing();ImGui::Spacing();
                    ImGui::PushStyleColor(ImGuiCol_Text, THEME_COLOR);
                    ImGui::TextUnformatted("Name : ");
                    ImGui::PopStyleColor();
                    ImGui::SameLine();
                    ImGui::TextUnformatted(m_activeObject->GetName().c_str());
                    ImGui::Spacing();ImGui::Spacing();
                    ImGui::PushStyleColor(ImGuiCol_Text, THEME_COLOR);
                    ImGui::SeparatorText("Transform");
                    ImGui::PopStyleColor();

                    static std::string _transform[3] = {"Position : ", "Rotation : ", "Scale Fc : "};
                    static_render_vec3inputField(_transform[0], m_activeObject->GetPosition());
                    static_render_vec3inputField(_transform[1], m_activeObject->GetRotation(), -360, 360);
                    static_render_vec3inputField(_transform[2], m_activeObject->GetScale(), 0, 50);

                    if(ImGui::Button("Recenter", ImVec2(100, 25)))
                    {
                        m_activeObject->SetTransform(fVec3(0), fVec3(0), fVec3(1));
                    }
                    ImGui::Separator();

                    ImGui::Spacing();ImGui::Spacing();
                    ImGui::PushStyleColor(ImGuiCol_Text, THEME_COLOR);
                    ImGui::SeparatorText("Object Materials");
                    ImGui::PopStyleColor();
                    ImGui::ColorEdit4("Color", &m_activeObject->material.color.x);
                    ImGui::Spacing();ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::SliderFloat(" : Specular", &m_activeObject->material.specular, 0, 1);
                    ImGui::Separator();
                    ImGui::SliderFloat(" : Diffuse", &m_activeObject->material.diffuse, 0, 1);
                    ImGui::Separator();
                    ImGui::SliderFloat(" : Ambient", &m_activeObject->material.ambient, 0, 1);
                    ImGui::Separator();
                    ImGui::SliderFloat(" : Roughness", &m_activeObject->material.roughness, 0, 1);
                    ImGui::Separator();

                    if(ImGui::Button("-RESET-", ImVec2(100, 25)))
                    {
                        m_activeObject->material.specular = 0.8;
                        m_activeObject->material.diffuse = 0.4;
                        m_activeObject->material.ambient= 0.1;
                        m_activeObject->material.roughness = 0.5;
                    }
                }
                else
                {
                    ImGui::Spacing();ImGui::Spacing();
                    ImGui::Separator();
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                    ImGui::Text("No Active Object !");
                    ImGui::Text("Select object to open inspector !");
                    ImGui::PopStyleColor();
                    ImGui::Separator();
                }
                ImGui::End();
            }

            ImGui::Begin("Inspector Toggle", NULL, DEFAULT_UI_FLAGS);
            ImGui::SetWindowSize({45, 45});
            ImGui::SetWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - ImGui::GetWindowSize().x, 5));
            // ImGui::Checkbox(m_showInspector ? "Hide" : "Show", &m_showInspector);
            if(ImGui::ArrowButton("Inspector Toggle Arrow", m_showInspector?ImGuiDir_Right:ImGuiDir_Left))
            {
                m_showInspector = !m_showInspector;
            }
            ImGui::End();

            ImGui::Begin("Outliner Toggle", NULL, DEFAULT_UI_FLAGS);
            ImGui::SetWindowSize({45, 45});
            ImGui::SetWindowPos(ImVec2(5, ImGui::GetMainViewport()->Size.y - 50));
            // ImGui::Checkbox("Outliner", &m_showOutliner);
            if(ImGui::ArrowButton("Inspector Toggle Arrow", m_showOutliner?ImGuiDir_Left:ImGuiDir_Right))
            {
                m_showOutliner = !m_showOutliner;
            }
            ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        protected:
        void initializeGUIEntity()
        {

        }

        protected:
        ImGuiIO m_io;
        Component::Camera *m_cam;
        Component::Application *m_app;
        bool m_showInspector, m_showOutliner;
        std::vector<Interface::IRenderableGUI*> m_GUIEntity;
        Component::Renderer *m_activeRenderer;
        Interface::IRenderableEntity *m_activeObject;
    };
}
#endif