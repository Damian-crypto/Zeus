#include "game_layer.h"

#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "game_level.h"

#include <exception>
#include <iostream>

extern glm::ivec2 platformPos;

GameLayer::GameLayer(const char* name)
{
	// Initializing Phyzics engine
	m_Phyzics = std::make_shared<zeus::Phyzics>();

	// Initializing Camera
	m_Camera = std::make_shared<zeus::Camera>();
	m_Camera->GetProperties().Position = { 32.0f, 32.0f, 0.0f };
	m_Camera->GetProperties().MovementSpeed = 85.0f;

	// Initializing Texture manager
	m_TexManager = zeus::TextureManager::GetInstance();
	m_TexManager->PutTexture({ "wood", "assets/textures/wood.png" });
	m_TexManager->PutSpritesheet({ "person_sheet", "assets/textures/spritesheets/tilemap_packed.png", 16, true });
	m_TexManager->PutSpritesheet({ "building_sheet", "assets/textures/spritesheets/roguelike_spritesheet.png", 17, true });
	m_TexManager->PutSpritesheet({ "bullet_sheet", "assets/textures/spritesheets/explosion.png", 240, true });

	// Initializing Enemy registry
	m_EnemyReg = std::make_shared<EnemyRegistry>(m_TexManager);
	m_EnemyReg->SetPhyzicsEngine(m_Phyzics);
	m_EnemyReg->CreateEnemy(EnemyType::Human);

	// Initializing Main player
	m_Player = std::make_shared<Player>();
	m_Player->SetPhyzicsEngine(m_Phyzics);
	m_Player->SetTextureManager(m_TexManager);
	m_Player->SetPosition({ WIDTH / 2, HEIGHT / 2, 0.11f });
	m_Player->SetSprite({ { 24.0f, 18.0f, 2.0f }, { 32.0f, 32.0f, 0.0f }, "person_sheet" });
	m_Player->SetWeapon(WeaponType::Gun);

	// Initializing Level manager
	const auto& lvl1 = std::make_shared<BeginLevel>();
	lvl1->SetLevelSize(WIDTH, HEIGHT);
	m_LevelManager.AddLevel(m_CurrentLevel, lvl1);
	m_LevelManager.GetLevel(m_CurrentLevel)->GetLevelResources().TexManager = m_TexManager;

	// Initializing Renderer
	zeus::Renderer::Init();
	zeus::Renderer::SetBackgroundColor(GREY);
	zeus::Renderer::SetColorBlending(true);
	zeus::Renderer::SetDepthTest(true);
}

void GameLayer::OnEvent(zeus::Event& evt)
{
	zeus::EventDispatcher dispatcher(evt);

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

		switch (keyCode)
		{
		case KEY_1:
			m_Player->SetWeapon(WeaponType::Gun);
			break;
		case KEY_ESCAPE:
			zeus::Application::GetInstance()->Stop();
			break;
		}

		return true;
	};

	std::function<bool()> mousePressed = [&]() -> bool {
		auto mouseEvent = (zeus::MousePressedEvent&)evt;
		int button = mouseEvent.GetMouseButton();

		switch (button)
		{
		case MOUSE_LEFT:
			const auto& playerPos = m_Player->GetPosition();
			float dir = std::atan2(((HEIGHT - m_MousePos.y) - playerPos.y), (m_MousePos.x - playerPos.x));
			m_Player->Attack(dir);
			break;
		}

		return true;
	};

	std::function<bool()> mouseMoved = [&]() -> bool {
		auto mouseEvent = (zeus::MouseMovedEvent&)evt;
		
		m_MousePos = mouseEvent.GetMousePosition();

		return true;
	};

	dispatcher.Dispatch(zeus::EventType::MouseMoved, mouseMoved);
	dispatcher.Dispatch(zeus::EventType::MousePressed, mousePressed);
	dispatcher.Dispatch(zeus::EventType::KeyPressed, keyPressed);
	dispatcher.Dispatch(zeus::EventType::KeyReleased, keyReleased);
}

int minRate = 60, maxRate = 60;
void GameLayer::OnRender()
{
	zeus::Renderer::Start(m_Camera);

	m_LevelManager.SetActiveLevel(m_CurrentLevel);
	m_LevelManager.Draw();

	m_EnemyReg->OnRender();
	m_Player->OnRender();

	auto& cam = m_Camera->GetProperties();
	ImGui::Begin("info");
	{
		ImGuiIO& io = ImGui::GetIO();
		if (ImGui::IsWindowFocused())
		{
			io.WantCaptureKeyboard = true;
			io.WantCaptureMouse = true;
		}

		ImGui::DragFloat3("camera position", glm::value_ptr(cam.Position));
		int* tmp = (int*)&platformPos;
		ImGui::DragInt2("platform position", tmp);
		ImGui::DragFloat3("character position", glm::value_ptr(m_Player->GetPosition()), 0.1f);

		ImGui::Text("Framerate: %f", io.Framerate);

		ImGui::Text("Enemies count: %d", m_EnemyReg->GetEnemies().size());
		const auto& phyzStat = m_Phyzics->GetStatistics();
		ImGui::Text("Dynamic bodies: %d", phyzStat.DynamicBodies);
		ImGui::Text("Static bodies: %d", phyzStat.StaticBodies);

		ImGui::End();
	}

	zeus::Renderer::Flush(m_TexManager);
}

void GameLayer::OnUpdate(float dt)
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
			m_Camera->ProcessKeyboard(zeus::CameraMovement::UP, dt);
		}
		else if (m_Keys[KEY_S])
		{
			m_Camera->ProcessKeyboard(zeus::CameraMovement::DOWN, dt);
		}
	}
	else
	{
		float velocity = 150.0f * dt;
		if (m_Keys[KEY_A])
		{
			if (m_Player->GetPosition().x - 300.0f < platformPos.x)
			{
				platformPos.x -= velocity;
				m_Player->Move(-velocity, 0.0f, 0.0f);
			}
			else
			{
				m_Player->Move(-velocity, 0.0f, 0.0f);
			}
		}
		if (m_Keys[KEY_D])
		{
			if (m_Player->GetPosition().x + 300.0f > platformPos.x + WIDTH)
			{
				platformPos.x += velocity;
				m_Player->Move(velocity, 0.0f, 0.0f);
			}
			else
			{
				m_Player->Move(velocity, 0.0f);
			}
		}
		if (m_Keys[KEY_W])
		{
			if (m_Player->GetPosition().y + 300.0f > platformPos.y + HEIGHT)
			{
				platformPos.y += velocity;
				m_Player->Move(0.0f, velocity, 0.0f);
			}
			else
			{
				m_Player->Move(0.0f, velocity);
			}
		}
		if (m_Keys[KEY_S])
		{
			if (m_Player->GetPosition().y - 300.0f < platformPos.y)
			{
				platformPos.y -= velocity;
				m_Player->Move(0.0f, -velocity, 0.0f);
			}
			else
			{
				m_Player->Move(0.0f, -velocity);
			}
		}
	}

	m_Player->OnUpdate(dt);
	m_EnemyReg->OnUpdate(dt);
	m_Phyzics->Step(dt);
}
