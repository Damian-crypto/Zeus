#pragma once

#include <string>
#include <memory>

#include "layer_queue.h"
#include "window/window.h"
#include "glm/glm.hpp"
#include "util/imgui_ui.h"

namespace zeus
{
	struct ApplicationProperties
	{
		std::string Title{ "" };
		uint32_t Width{ 1280 };
		uint32_t Height{ 720 };
		bool GameUI{ true };
		bool Resizable { true };
	};

	class Application
	{
	private:
		// Private fields
		std::shared_ptr<Window> m_Window;
		std::shared_ptr<ImGUI> m_ImGui;
		ApplicationProperties m_Properties;
		LayerQueue m_LayerQueue;
		float m_DeltaTime = 0.0f;
		bool m_Running = false;

		// Static fields
		static Application* s_Instance;

	public:
		//Application();
		Application(ApplicationProperties props);
		~Application();

		/**
		 * Returns the active instance of the application.
		 * 
		 * @return current application pointer
		 */
		static Application* GetInstance();

		/**
		 * All tools will be initialized here
		 */
		void Init();

		/**
		 * Start the main loop
		 */
		void Run();

		/**
		 * Stop the main loop
		 */
		void Stop();

		/**
		 * This will execute all evt type events in all layers
		 * 
		 * @param evt - specific event to be called
		 */
		void OnEvent(Event& evt);

		/**
		 * Pushes a layer into the layer queue of the application
		 * and the layer will be executed in every frame.
		 * 
		 * @param layer - pointer to the layer
		 */
		void PushLayer(Layer* layer);

		/**
		 * Enable vertical synchronization of the engine.
		 * This will limit the frames generated at a second to 60 FPS(default).
		 * 
		 * @param enabled - is vsync enabled?
		 */
		void SetVSync(bool enabled = true);

		/**
		 * Returns the active window
		 * 
		 * @return platform agnostic current window will be returned.
		 */
		std::shared_ptr<Window> GetWindow();

		/**
		 * Engine's immediate mode custom gui handler will be returned
		 * 
		 * @return ImGui based custom class will be returned.
		 */
		std::shared_ptr<ImGUI> GetUI();

		/**
		 * Engine's current framerate will be returned.
		 * Default should be 60 FPS.
		 * 
		 * @return current framerate of the engine will be returned
		 */
		int GetFramerate() const;

		/**
		 * Current application properties will be returned.
		 * 
		 * @return returns a struct of application properties.
		 */
		ApplicationProperties& GetApplicationProperties();
	};
}
