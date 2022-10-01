#include "zeus.h"

#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <random>
#include <exception>

static constexpr int WIDTH = 1280;
static constexpr int HEIGHT = 720;

const glm::vec4 RED = { 1.0f, 0.0f, 0.0f, 1.0f };
const glm::vec4 BLUE = { 0.0f, 0.0f, 1.0f, 1.0f };
const glm::vec4 GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
const glm::vec4 GREY = { 0.3f, 0.3f, 0.3f, 1.0f };

static const struct DefaultOrthographicCamera
{
	glm::vec3 Position = { -WIDTH / 2.0f, -HEIGHT / 2.0f, 0.0f };
	glm::vec3 Direction = { 0.0f, 0.0f, -1.0f };
	glm::vec3 Right = { 0.0f, 0.0f, 0.0f };
	float MovementSpeed = 0.3f;
} defaultOrthoCamera;

static const struct DefaultPerspectiveCamera
{
	glm::vec3 Position = { 0.0f, 0.0f, 600.0f };
	glm::vec3 Direction = { 0.0f, 0.0f, -1.0f };
	glm::vec3 Right = { 0.0f, 0.0f, 0.0f };
	float MovementSpeed = 0.3f;
} defaultPerspectiveCamera;


class SandboxLayer : public zeus::Layer
{
private:
	std::string m_Name;

	glm::vec3 m_BoxPos{ 0.0f };

	bool m_Keys[65536] = { false };
	bool m_CameraMode = false;

	std::mt19937 mt;
	std::uniform_int_distribution<int> dist{ 0, 255 };

	std::shared_ptr<zeus::Texture> m_WoodTex;
	std::shared_ptr<zeus::Texture> m_WindowTex;

	std::shared_ptr<zeus::Camera> m_Camera;

public:
	SandboxLayer(const char* name)
		: m_Name(name)
	{
		m_Camera = std::make_shared<zeus::Camera>();
		m_Camera->GetProperties().Position = defaultOrthoCamera.Position;
		m_Camera->GetProperties().MovementSpeed = defaultOrthoCamera.MovementSpeed;

		m_WoodTex = zeus::Texture::CreateTexture("assets/textures/wood.png");
		m_WoodTex->Bind(0);
		m_WindowTex = zeus::Texture::CreateTexture("assets/textures/window.png");
		m_WindowTex->Bind(1);

		zeus::Renderer::Init();
		zeus::Renderer::SetBackgroundColor(GREY);
		zeus::Renderer::SetColorBlending(true);
		zeus::Renderer::SetDepthTest(true);
	}

	~SandboxLayer()
	{
	}

	float speed = 0.1f, lastX = WIDTH / 2.0f, lastY = HEIGHT / 2.0f;
	bool firstMove = true, lockZoom = true;
	void OnUpdate(float dt) override
	{
		auto const& cam = m_Camera->GetProperties();
		if (m_Keys[KEY_LEFT_CONTROL])
		{
			if (m_Keys[KEY_D])
			{
				m_Camera->ProcessKeyboard(zeus::CameraMovement::RIGHT, dt);
			}
			else if (m_Keys[KEY_A])
			{
				m_Camera->ProcessKeyboard(zeus::CameraMovement::LEFT, dt);
			}
			else if (m_Keys[KEY_W])
			{
				if (cam.Type == zeus::CameraType::Orthographic)
					m_Camera->ProcessKeyboard(zeus::CameraMovement::UP, dt);
				else
					m_Camera->ProcessKeyboard(zeus::CameraMovement::FORWARD, dt);
			}
			else if (m_Keys[KEY_S])
			{
				if (cam.Type == zeus::CameraType::Orthographic)
					m_Camera->ProcessKeyboard(zeus::CameraMovement::DOWN, dt);
				else
					m_Camera->ProcessKeyboard(zeus::CameraMovement::BACKWARD, dt);
			}

			if (cam.Type == zeus::CameraType::Perspective)
			{
				auto [mouseX, mouseY] = zeus::Input::GetMousePosition();
				if (firstMove)
				{
					lastX = mouseX;
					lastY = mouseY;
					firstMove = false;
				}

				float xoffset = mouseX - lastX;
				float yoffset = lastY - mouseY; // reversed since y-coordinates go from bottom to top

				lastX = mouseX;
				lastY = mouseY;

				m_Camera->ProcessMouse(xoffset, yoffset);
			}
		}
		else
		{
			if (m_Keys[KEY_A])
			{
				m_BoxPos.x -= speed * dt;
			}
			if (m_Keys[KEY_D])
			{
				m_BoxPos.x += speed * dt;
			}
			if (m_Keys[KEY_W])
			{
				m_BoxPos.y += speed * dt;
			}
			if (m_Keys[KEY_S])
			{
				m_BoxPos.y -= speed * dt;
			}
		}
	}

	void ResetCamera(zeus::CameraType type)
	{
		auto& cam = m_Camera->GetProperties();
		if (type == zeus::CameraType::Orthographic)
		{
			cam.Position = defaultOrthoCamera.Position;
			cam.MovementSpeed = defaultOrthoCamera.MovementSpeed;
			cam.Direction = defaultOrthoCamera.Direction;
			cam.Right = defaultOrthoCamera.Right;
		}
		else if (type == zeus::CameraType::Perspective)
		{
			cam.Position = defaultPerspectiveCamera.Position;
			cam.MovementSpeed = defaultPerspectiveCamera.MovementSpeed;
			cam.Direction = defaultPerspectiveCamera.Direction;
			cam.Right = defaultPerspectiveCamera.Right;
		}
	}

	void ChangeCameraType(zeus::CameraType type)
	{
		static zeus::CameraType currentType = m_Camera->GetProperties().Type;
		auto& cam = m_Camera->GetProperties();
		if (type != currentType)
		{
			cam.Type = type;
			currentType = type;

			if (type == zeus::CameraType::Orthographic)
			{
				cam.Position = defaultOrthoCamera.Position;
			}
			else
			{
				cam.Position = defaultPerspectiveCamera.Position;
			}
		}
	}

	bool showDemo = true;
	float zVal = 0.0f, angle = 0.0f;
	glm::vec3 scaler{ 20.0f, 20.0f, 0.0f };
	void OnRender() override
	{
		zeus::Renderer::Start(m_Camera);

		ImGui::ShowDemoWindow(&showDemo);

		ImGui::Begin("Camera Properties");
		{
			auto& cam = m_Camera->GetProperties();
			ImGui::SameLine();
			ImGui::DragFloat("zoom", &cam.ZoomLevel);
			ImGui::SameLine();
			ImGui::Checkbox("lock zoom", &lockZoom);

			ImGui::DragFloat("speed", &cam.MovementSpeed, 0.001f);
			ImGui::DragFloat3("position", glm::value_ptr(cam.Position));
			ImGui::DragFloat3("looking at", glm::value_ptr(cam.Direction));
			ImGui::DragFloat3("right", glm::value_ptr(cam.Right));

			static const char* items[] = { "Orthographic", "Perspective" };
			static int item_current_idx = 0;
			const char* combo_preview_value = items[item_current_idx];
			if (ImGui::BeginCombo("Camera Type", combo_preview_value, 0))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(items[n], is_selected))
						item_current_idx = n;

					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (item_current_idx == 0)
				ChangeCameraType(zeus::CameraType::Orthographic);
			else
				ChangeCameraType(zeus::CameraType::Perspective);

			ImGui::Checkbox("camera mode", &m_CameraMode);
			zeus::Application::GetInstance()->GetWindow()->SetCameraMode(m_CameraMode);
			
			if (ImGui::Button("Reset"))
			{
				ResetCamera(cam.Type);
			}

			ImGui::End();
		}

		ImGui::Begin("Textured Quad Handle");
		{
			ImGui::DragFloat2("box position", glm::value_ptr(m_BoxPos), 0.1f);
			ImGui::DragFloat("box depth value", &m_BoxPos.z, 0.01f, -1.0f, 1.0f);
			ImGui::DragFloat("angle", &angle);
			ImGui::DragFloat3("scale", glm::value_ptr(scaler), 0.01f);
			ImGui::End();
		}

		ImGui::Begin("Mouse Handle");
		{
			const auto& [mouseX, mouseY] = zeus::Input::GetMousePosition();
			ImGui::Text("cursor position: %f, %f", mouseX, mouseY);
			ImGui::End();
		}

		zeus::Renderer::DrawTexturedQuad(m_BoxPos, scaler, m_WoodTex, angle);
		for (float y = 20; y <= 460.0f; y += 40.0f)
		{
			for (float x = 20; x <= 620.0f; x += 40.0f)
			{
				zeus::Renderer::DrawTexturedQuad({ x, y, 0.0f }, { 20.0f, 20.0f, 0.0f }, m_WindowTex);
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

			switch (keyCode)
			{
				case KEY_LEFT_ALT:
					m_CameraMode = !m_CameraMode;
					break;
			}

			return true;
		};

		std::function<bool()> keyReleased = [&]() -> bool {
			auto keyEvent = (zeus::KeyReleasedEvent&)evt;
			int keyCode = keyEvent.GetKeycode();
			m_Keys[keyCode] = false;

			switch (keyCode)
			{
				case KEY_ESCAPE:
					zeus::Application::GetInstance()->Stop();
					break;
			}

			return true;
		};

		std::function<bool()> mouseMoved = [&]() -> bool {
			auto mouseEvent = (zeus::MouseMovedEvent&)evt;
			auto pos = mouseEvent.GetMousePosition();

			return true;
		};

		std::function<bool()> mouseScrolled = [&]() -> bool {
			auto mouseEvent = (zeus::MouseScrolledEvent&)evt;
			auto scroll = mouseEvent.GetMouseScroll();
			m_Camera->ProcessMouseScroll(scroll.y, lockZoom);

			return true;
		};

		dispatcher.Dispatch(zeus::EventType::MousePressed, mousePressed);
		dispatcher.Dispatch(zeus::EventType::MouseScrolled, mouseScrolled);
		dispatcher.Dispatch(zeus::EventType::MouseMoved, mouseMoved);
		dispatcher.Dispatch(zeus::EventType::KeyPressed, keyPressed);
		dispatcher.Dispatch(zeus::EventType::KeyReleased, keyReleased);
	}
};

int main()
{
	try
	{
		auto app = new zeus::Application({ "Village game", WIDTH, HEIGHT });
		app->Init();
		app->PushLayer(new SandboxLayer("sandbox"));
		app->Run();
	}
	catch (std::exception e)
	{
		std::cerr << e.what() << std::endl;
	}
}
