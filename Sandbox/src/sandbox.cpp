#include "zeus.h"

#include <iostream>
#include <random>
#include <exception>

const glm::vec4 RED = { 1.0f, 0.0f, 0.0f, 1.0f };
const glm::vec4 BLUE = { 0.0f, 0.0f, 1.0f, 1.0f };
const glm::vec4 GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
const glm::vec4 GREY = { 0.3f, 0.3f, 0.3f, 1.0f };

class SandboxLayer : public zeus::Layer
{
private:
	std::string m_Name;

	float xPos = 0.5f;
	float yPos = 0.0f;

	bool m_Keys[65536] = { false };

	std::mt19937 mt;
	std::uniform_int_distribution<int> dist{ 0, 255 };

	std::shared_ptr<zeus::Texture> m_WoodTex;

public:
	SandboxLayer(const char* name)
		: m_Name(name)
	{
		m_WoodTex = zeus::Texture::CreateTexture("assets/textures/wood.png");

		zeus::Renderer::Init();
		zeus::Renderer::SetBackgroundColor(GREY);
	}

	float speed = 0.1f;
	void OnUpdate(float dt) override
	{
		if (m_Keys[KEY_LEFT])
			xPos -= speed * dt;
		if (m_Keys[KEY_RIGHT])
			xPos += speed * dt;
		if (m_Keys[KEY_UP])
			yPos += speed * dt;
		if (m_Keys[KEY_DOWN])
			yPos -= speed * dt;
	}

	void OnRender() override
	{
		zeus::Renderer::Start();
		zeus::Renderer::DrawTexturedQuad({ xPos, yPos, 0.0f }, { 20.0f, 20.0f, 0.0f }, m_WoodTex);
		for (float y = 20; y <= 460.0f; y += 40.0f)
		{
			for (float x = 20; x <= 620.0f; x += 40.0f)
			{
				//zeus::Renderer::DrawQuad({ x, y, 0.0f }, { 20.0f, 20.0f, 0.0f }, { dist(mt) / 255.0f, dist(mt) / 255.0f, dist(mt) / 255.0f, 1.0f });
			}
		}
		zeus::Renderer::Flush();
	}

	void OnEvent(zeus::Event& evt) override
	{
		zeus::EventDispatcher dispatcher(evt);

		std::function<bool()> mousePressed = [&]() -> bool {
			auto [x, y] = zeus::Input::GetMousePosition();
			auto mouseEvent = (zeus::MousePressedEvent&)evt;

			return true;
		};

		std::function<bool()> keyPressed = [&]() -> bool {
			auto keyEvent = (zeus::KeyPressedEvent&)evt;
			int keyCode = keyEvent.GetKeycode();
			m_Keys[keyCode] = true;

			return true;
		};

		std::function<bool()> keyReleased = [&]() -> bool {
			auto keyEvent = (zeus::KeyReleasedEvent&)evt;
			int keyCode = keyEvent.GetKeycode();
			m_Keys[keyCode] = false;

			if (keyCode == KEY_ESCAPE)
				zeus::Application::GetInstance()->Stop();

			return true;
		};

		std::function<bool()> mouseMoved = [&]() -> bool {
			auto mouseEvent = (zeus::MouseMovedEvent&)evt;
			auto pos = mouseEvent.GetMousePosition();
			std::cout << "(" << pos.x << ", " << pos.y << ")\n";
			
			return true;
		};

		dispatcher.Dispatch(zeus::EventType::MousePressed, mousePressed);
		dispatcher.Dispatch(zeus::EventType::MouseMoved, mouseMoved);
		dispatcher.Dispatch(zeus::EventType::KeyPressed, keyPressed);
		dispatcher.Dispatch(zeus::EventType::KeyReleased, keyReleased);
	}
};

int main()
{
	try
	{
		auto app = new zeus::Application({ "Village game", 640, 480 });
		app->Init();
		app->PushLayer(new SandboxLayer("sandbox"));
		app->Run();
	}
	catch (std::exception e)
	{
		std::cerr << e.what() << std::endl;
	}
}
