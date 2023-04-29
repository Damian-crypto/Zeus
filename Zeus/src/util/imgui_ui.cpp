#include "corepch.h"
#include "imgui_ui.h"

#include "application.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "util/logger.h"

namespace zeus
{
    bool ImGUI::m_Initialized = false;

	void ImGUI::Init(bool gameUI)
	{
        m_GameUI = gameUI;
        m_Initialized = true;

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        m_NativeWindow = (GLFWwindow*)Application::GetInstance()->GetWindow()->GetNativeWindow();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(m_NativeWindow, true);
        ImGui_ImplOpenGL3_Init("#version 410");

        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
        // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Read 'docs/FONTS.md' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
        //io.Fonts->AddFontDefault();
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
        //ImFont* font = io.Fonts->A
	}

	void ImGUI::Begin()
	{
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        // ImGui::SetNextWindowSize(ImVec2{ 100, 100 });
        
        if (!m_GameUI)
        {
            // If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
            // In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
            // In this specific demo, we are not using DockSpaceOverViewport() because:
            // - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
            // - we allow the host window to have padding (when opt_padding == true)
            // - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
            // TL;DR; this demo is more complicated than what you would normally use.
            // If we removed all the options we are showcasing, this demo would become:
            //void ShowExampleAppDockSpace()
            //{
            //    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
            //}

            static bool p_open = false;
            static bool opt_fullscreen = true;
            static bool opt_padding = false;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
            // because it would be confusing to have two docking targets within each others.
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar;
            if (opt_fullscreen)
            {
                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->WorkPos);
                ImGui::SetNextWindowSize(viewport->WorkSize);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }
            else
            {
                dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
            }

            // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
            // and handle the pass-thru hole, so we ask Begin() to not render a background.
            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
            // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
            // all active windows docked into it will lose their parent and become undocked.
            // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
            // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
            if (!opt_padding)
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Zues Workspace", &p_open, window_flags);
            if (!opt_padding)
                ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // Submit the DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyWorkspace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }
            else
            {
                LOG_ENGINE(Error, "Runtime Error: ImGui docking is not enabled!");
            }

            if (!m_GameUI)
            {
                if (ImGui::BeginMenuBar())
                {
                    const static std::regex menuRegex("menu\\$\\$(.*)");
                    const static std::regex menuItemRegex("menu-item\\$\\$(.*)");

                    for (size_t i = 0; i < m_MenuItems.size(); i++)
                    {
                        MenuItem item = m_MenuItems[i];
                        size_t nameStart = item.ItemName.find("$");
                        std::string name = item.ItemName.substr(nameStart + 2);
                        if (std::regex_match(item.ItemName, menuRegex))
                        {
                            if (ImGui::BeginMenu(name.c_str()))
                            {
                                for (size_t j = i; j < m_MenuItems.size(); j++)
                                {
                                    item = m_MenuItems[j];
                                    nameStart = item.ItemName.find("$");
                                    name = item.ItemName.substr(nameStart + 2);
                                    // LOG_ENGINE(Trace, name.c_str());
                                    if (std::regex_match(item.ItemName, menuItemRegex))
                                    {
                                        if (ImGui::MenuItem(name.c_str()))
                                        {
                                            item.Action();
                                        }
                                    }
                                }

                                ImGui::EndMenu();
                            }
                        }
                    }

                    // if (ImGui::BeginMenu("Options"))
                    // {
                    //     // Disabling fullscreen would allow the window to be moved to the front of other windows,
                    //     // which we can't undo at the moment without finer window depth/z control.
                    //     ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                    //     ImGui::MenuItem("Padding", NULL, &opt_padding);
                    //     ImGui::Separator();

                    //     if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
                    //     if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
                    //     if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
                    //     if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
                    //     if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
                    //     ImGui::Separator();

                    //     if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                    //     {
                    //         p_open = false;
                    //     }
                    //     ImGui::EndMenu();
                    // }

                    ImGui::EndMenuBar();
                }
            }

            ImGui::End();
        }
	}

	void ImGUI::End()
	{
        ImGuiIO& io = ImGui::GetIO();
        auto props = Application::GetInstance()->GetApplicationProperties();
        io.DisplaySize = ImVec2((float)props.Width, (float)props.Height);

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // This is for handle when imgui window is out of the main window frame
        // Otherwise it will throw an error. Save the current context for later.
        // And to update the renderer additional platform windows.
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
	}

    void ImGUI::Destroy()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGUI::AddMenuItem(const MenuItem& item)
    {
        m_MenuItems.push_back(item);
    }

    bool ImGUI::IsBlocking()
    {
        if (m_Initialized)
        {
            ImGuiIO& io = ImGui::GetIO();
            if (io.WantCaptureMouse || io.WantCaptureKeyboard)
            {
                return true;
            }
        }
        else
        {
            return true;
        }

        return false;
    }

    void ImGUI::SetGamingMode(bool gaming)
    {
        m_GameUI = gaming;
    }

    std::string ImGUI::ShowOpenFileDialog()
    {
        LOG(Trace, "Hello");
        // ImGui::BeginPopupModal("Open File", nullptr);
        // {
        //     ImGui::Text("File");

        //     ImGui::EndPopup();
        // }
        return "";
    }
}
