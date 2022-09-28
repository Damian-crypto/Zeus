#pragma once

#include "core.h"
#include "layer_queue.h"
#include "window/window.h"
#include "glm/glm.hpp"

namespace zeus
{
	struct ApplicationProperties
	{
		std::string Title{ "" };
		uint32_t Width{ 1280 };
		uint32_t Height{ 720 };
	};

	class Application
	{
	private:
		// Private fields
		std::shared_ptr<Window> m_Window;
		ApplicationProperties m_Properties;
		LayerQueue m_LayerQueue;
		float m_DeltaTime = 0.0f;
		bool m_Running = false;

		// Static fields
		static Application* s_Instance;

	public:
		Application(ApplicationProperties props);
		~Application();

		static Application* GetInstance();

		void Init();
		void Run();
		void Stop();
		void OnEvent(Event& evt);
		void PushLayer(Layer* layer);
		std::shared_ptr<Window> GetWindow();
		int GetDeltaTime() const;

		ApplicationProperties& GetApplicationProperties();
	};
}
