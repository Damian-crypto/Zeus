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
	float MovementSpeed = 30.0f;
} defaultOrthoCamera;

static const struct DefaultPerspectiveCamera
{
	glm::vec3 Position = { 0.0f, 0.0f, 600.0f };
	glm::vec3 Direction = { 0.0f, 0.0f, -1.0f };
	glm::vec3 Right = { 0.0f, 0.0f, 0.0f };
	float MovementSpeed = 0.3f;
} defaultPerspectiveCamera;

glm::vec3 spritePos{ 0.0f, 0.0f, 0.1f };
std::shared_ptr<zeus::TextureManager> textureManager;
class SandboxLevel : public zeus::Level
{
public:
	void OnStart() override
	{
	}

	void Draw() override
	{
		for (int y = 0; y < 1024; y += 128)
		{
			for (int x = 0; x < 1024; x += 128)
			{
				zeus::Renderer::DrawTexturedQuad({ x, y, 0 }, { 64, 64, 0 }, textureManager->GetSubTexture("building_sheet", 11, 1));
				//zeus::Renderer::DrawQuad({ x, y, 0.0f }, { 20.0f, 20.0f, 0.0f }, 0.0f, { dist(mt) / 255.0f, dist(mt) / 255.0f, dist(mt) / 255.0f, 1.0f });
			}
		}

		zeus::Renderer::DrawTexturedQuad(spritePos, {20.0f, 20.0f, 0.0f}, textureManager->GetTexture("wood_tex"));
	}
};

class SandboxLayer : public zeus::Layer
{
private:
	std::string m_Name;

	bool m_Keys[65536] = { false };
	bool m_CameraMode = false;

	std::mt19937 mt;
	std::uniform_int_distribution<int> dist{ 0, 255 };

	std::shared_ptr<zeus::Camera> m_Camera;

	zeus::LevelManager m_LevelManager;

public:
	SandboxLayer(const char* name)
		: m_Name(name)
	{
		m_Camera = std::make_shared<zeus::Camera>();
		m_Camera->GetProperties().Position = defaultOrthoCamera.Position;
		m_Camera->GetProperties().MovementSpeed = defaultOrthoCamera.MovementSpeed;

		textureManager = zeus::TextureManager::GetInstance();
		textureManager->PutTexture({ "wood_tex", "assets/textures/wood.png" });
		textureManager->PutTexture({ "window_tex", "assets/textures/window.png" });
		textureManager->PutSpritesheet({ "person_sheet", "assets/textures/tilemap_packed.png", 16, true });
		textureManager->PutSpritesheet({ "building_sheet", "assets/textures/roguelike_spritesheet.png", 17, true });

		const auto& lvl1 = std::make_shared<SandboxLevel>();
		m_LevelManager.AddLevel(1, lvl1);

		zeus::Renderer::Init();
		zeus::Renderer::SetBackgroundColor(GREY);
		zeus::Renderer::SetColorBlending(true);
		zeus::Renderer::SetDepthTest(true);
	}

	~SandboxLayer()
	{
	}

	float speed = 2.5f, lastX = WIDTH / 2.0f, lastY = HEIGHT / 2.0f;
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
				spritePos.x -= speed;
			}
			if (m_Keys[KEY_D])
			{
				spritePos.x += speed;
			}
			if (m_Keys[KEY_W])
			{
				spritePos.y += speed;
			}
			if (m_Keys[KEY_S])
			{
				spritePos.y -= speed;
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

	float zVal = 0.0f, angle = 0.0f;
	glm::vec3 scaler{ 50.0f, 50.0f, 0.0f };
	int texX = 24, texY = 18;
	void OnRender() override
	{
		zeus::Renderer::Start(m_Camera);

		RenderUI();

		//for (int y = 0; y < 256; y += 32)
		//{
		//	for (int x = 0; x < 256; x += 32)
		//	{
		//		zeus::Renderer::DrawTexturedQuad({ x, y, 0 }, { 16, 16, 0 }, textureManager->GetSubTexture("building_sheet", 11, 1));
		//		//zeus::Renderer::DrawQuad({ x, y, 0.0f }, { 20.0f, 20.0f, 0.0f }, 0.0f, { dist(mt) / 255.0f, dist(mt) / 255.0f, dist(mt) / 255.0f, 1.0f });
		//	}
		//}

		m_LevelManager.SetActiveLevel(1);
		m_LevelManager.Draw();

		zeus::Renderer::Flush(textureManager);
	}

	void RenderUI()
	{
		ImGui::Begin("Camera Properties");
		{
			auto& cam = m_Camera->GetProperties();
			ImGui::SameLine();
			ImGui::DragFloat("zoom", &cam.ZoomLevel);
			ImGui::SameLine();
			ImGui::Checkbox("lock zoom", &lockZoom);

			ImGui::DragFloat("speed", &cam.MovementSpeed);
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
			int step = 1;
			ImGui::InputScalar("index x", ImGuiDataType_S32, &texX, &step, NULL, "%d");
			ImGui::InputScalar("index y", ImGuiDataType_S32, &texY, &step, NULL, "%d");
			ImGui::DragFloat2("box position", glm::value_ptr(spritePos), 0.1f);
			ImGui::DragFloat("box depth value", &spritePos.z, 0.01f, -1.0f, 1.0f);
			ImGui::DragFloat("angle", &angle);
			ImGui::DragFloat3("scale", glm::value_ptr(scaler));
			ImGui::End();
		}
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

class EngineInfoLayer : public zeus::Layer
{
private:
	zeus::Application* app = nullptr;

public:
	EngineInfoLayer(const char* name)
	{
		app = zeus::Application::GetInstance();
	}

	bool showDemo = true;
	void OnUpdate(float dt) override
	{
		ImGui::ShowDemoWindow(&showDemo);

		ImGui::Begin("Renderer Info");
		{
			ImGui::Text("Maximum texture slots supported: %d", zeus::TextureManager::GetTextureMaxSlotsCount());
			const auto& stat = zeus::Renderer::GetRendererStat();
			ImGui::Text("Quads drawn: %d / %d", stat.QuadsDrawn, zeus::Renderer::MaxQuads);
			ImGui::Text("Draw calls: %d", stat.DrawCalls);
			ImGui::End();
		}

		ImGui::Begin("Mouse");
		{
			const auto& [mouseX, mouseY] = zeus::Input::GetMousePosition();
			ImGui::Text("cursor position: %f, %f", mouseX, mouseY);
			ImGui::End();
		}

		ImGui::Begin("Application");
		{
			static bool vsyncOn = true;
			ImGui::Checkbox("v-sync enabled", &vsyncOn);
			app->SetVSync(vsyncOn);
			ImGui::End();
		}
	}

	void OnRender() override
	{
	}

	void OnEvent(zeus::Event& e) override
	{
	}
};

int main()
{
	try
	{
		auto app = new zeus::Application({ "Sandbox", WIDTH, HEIGHT });
		app->Init();
		app->PushLayer(new SandboxLayer("sandbox"));
		app->PushLayer(new EngineInfoLayer("engine"));
		app->Run();
	}
	catch (std::exception e)
	{
		std::cerr << e.what() << std::endl;
	}
}
