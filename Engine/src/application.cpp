#include "corepch.h"
#include "application.h"

#include "window/window_constants.h"
#include "util/logger.h"

namespace zeus
{
	Application* Application::s_Instance = nullptr;

	Application::Application(ApplicationProperties props)
	{
		LOG_ENGINE(Info, "Welcome to the Zeus Engine version 1.0");

		if (s_Instance != nullptr)
		{
			LOG_ENGINE(Error, "Application already exists!");
		}

		s_Instance = this;
		s_Instance->m_Properties = props;
		m_ImGui = std::make_shared<ImGUI>();
		m_ImGui->SetGamingMode(props.GameUI);
	}

	void Application::OnEvent(Event& evt)
	{
		for (auto layer : m_LayerQueue)
		{
			layer->OnEvent(evt);
		}
	}

	Application::~Application()
	{
		LOG_ENGINE(Trace, "Destructing application...");
	}

	Application* Application::GetInstance()
	{
		return s_Instance;
	}

	void Application::Init()
	{
		m_Window = Window::GetWindow();
		m_Window->CreateWindow({ m_Properties.Title, m_Properties.Width, m_Properties.Height });
		m_Window->VSyncEnable(true);
		m_ImGui->Init(m_Properties.GameUI);

		MenuItem fileMenu;
		fileMenu.ItemName = "menu$$File";
		MenuItem openItem;
		openItem.ItemName = "menu-item$$Open";
		openItem.Action = []() -> bool { LOG_ENGINE(Trace, "Open File"); };
		m_ImGui->AddMenuItem(fileMenu);
		m_ImGui->AddMenuItem(openItem);
	}

	void Application::Run()
	{
		m_Running = true;

		float currentTime = 0;
		float lastTime = 0;

		while (m_Running != false && m_Window->ShouldClose() != true)
		{
			currentTime = m_Window->GetTime();
			m_DeltaTime = (currentTime - lastTime);
			lastTime = currentTime;

			// m_Window->SetWindowTitle(m_Properties.Title + "|" + std::to_string(int(1.0f / m_DeltaTime)) + " FPS");
			m_Window->ProcessEvents();

			m_ImGui->Begin();
			for (Layer* layer : m_LayerQueue)
			{
				// TODO: OnUpdate invokes only for each 60 frames
				layer->OnUpdate(m_DeltaTime);
				// TODO: OnRender invokes for each iteration
				layer->OnRender();
			}
			m_ImGui->End();

			m_Window->SwapBuffers();
		}

		m_ImGui->Destroy();
		m_Window->TerminateWindow();
	}

	void Application::Stop()
	{
		LOG_ENGINE(Trace, "Stop called");
		m_Running = false;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerQueue.PushBack(layer);
	}

	void Application::SetVSync(bool enabled)
	{
		if (m_Window == nullptr)
		{
			LOG_ENGINE(Error, "Runtime Error: Trying to change window settings before initialization!");
		}
		m_Window->VSyncEnable(enabled);
	}

	std::shared_ptr<ImGUI> Application::GetUI()
	{
		return m_ImGui;
	}

	std::shared_ptr<Window> Application::GetWindow()
	{
		return m_Window;
	}

	int Application::GetFramerate() const
	{
		return (int)(1.0f / m_DeltaTime);
	}

	ApplicationProperties& Application::GetApplicationProperties()
	{
		return m_Properties;
	}
}
