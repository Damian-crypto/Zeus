#include "application.h"

#include "window/window_constants.h"

namespace zeus
{
	Application* Application::s_Instance = nullptr;

	Application::Application(ApplicationProperties props)
		: m_Properties(props)
	{
		if (s_Instance != nullptr)
		{
			throw std::runtime_error("Application already exists!");
		}

		s_Instance = this;
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
		std::cout << "Destruction application...\n";
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
	}

	void Application::Run()
	{
		m_Running = true;

		float currentTime = 0;
		float lastTime = 0;

		while (m_Running != false && m_Window->ShouldClose() != true)
		{
			currentTime = m_Window->GetTime();
			m_DeltaTime = 1.0f / (float)(currentTime - lastTime);
			lastTime = currentTime;

			m_Window->SetWindowTitle(m_Properties.Title + "|" + std::to_string((int)m_DeltaTime) + " FPS");
			m_Window->ProcessEvents();

			for (Layer* layer : m_LayerQueue)
			{
				layer->OnUpdate(m_DeltaTime);
				layer->OnRender();
			}

			m_Window->SwapBuffers();
		}

		m_Window->TerminateWindow();
	}

	void Application::Stop()
	{
		std::cout << "Stop called\n";
		m_Running = false;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerQueue.PushBack(layer);
	}

	std::shared_ptr<Window> Application::GetWindow()
	{
		return m_Window;
	}

	int Application::GetDeltaTime() const
	{
		return (int)m_DeltaTime;
	}

	ApplicationProperties& Application::GetApplicationProperties()
	{
		return m_Properties;
	}
}
