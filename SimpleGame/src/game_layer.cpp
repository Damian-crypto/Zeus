#include "game_layer.h"

#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

GameLayer::GameLayer(const char* name)
{
	m_Camera = std::make_shared<zeus::Camera>();
	m_Camera->GetProperties().Position = { 32.0f, 32.0f, 0.0f };
	m_Camera->GetProperties().MovementSpeed = 85.0f;

	m_TexManager = zeus::TextureManager::GetInstance();
	m_TexManager->PutSpritesheet({ "person_sheet", "assets/textures/spritesheets/tilemap_packed.png", 16, true });
	m_TexManager->PutSpritesheet({ "building_sheet", "assets/textures/spritesheets/roguelike_spritesheet.png", 17, true });

	m_CharacterPos = { WIDTH / 2, HEIGHT / 2, 0.1f };
	m_CharacterSprite = { 0.0f, 0.0f, 2.0f };

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
		case KEY_ESCAPE:
			zeus::Application::GetInstance()->Stop();
			break;
		}

		return true;
	};

	dispatcher.Dispatch(zeus::EventType::KeyPressed, keyPressed);
	dispatcher.Dispatch(zeus::EventType::KeyReleased, keyReleased);
}

vec2i platformPos{ 0, 0 };
void GameLayer::OnRender()
{
	zeus::Renderer::Start(m_Camera);

	for (int x = 0; x <= 1152; x += 64)
	{
		for (int y = 0; y <= 768; y += 64)
		{
			int xx = 0, yy = 0;
			if (platformPos.x + x > 0)
				xx = (platformPos.x + x) % 1152;
			else
				xx = platformPos.x + 1152 + x;

			if (platformPos.y + y > 0)
				yy = (platformPos.y + y) % 768;
			else
				yy = platformPos.y + 768 + y;

			zeus::Renderer::DrawTexturedQuad({ (float)xx, (float)yy, 0.0f}, { 32, 32, 0 }, m_TexManager->GetSubTexture("building_sheet", 0, 0));
		}
	}
	zeus::Renderer::DrawTexturedQuad(m_CharacterPos, { 32, 32, 0 }, m_TexManager->GetSubTexture("person_sheet", 24 + m_CharacterSprite.x + m_CharacterSprite.z, 16 + m_CharacterSprite.y));

	auto& cam = m_Camera->GetProperties();
	ImGui::Begin("settings");
	{
		ImGui::DragFloat3("camera position", glm::value_ptr(cam.Position));
		int* tmp = (int*)&platformPos;
		ImGui::DragInt2("platform position", tmp);
		ImGui::DragFloat3("character position", glm::value_ptr(m_CharacterPos));
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
			m_CharacterSprite.z = 0.0f;
			m_CharacterSprite.y += 0.1f;
			platformPos.x += velocity;
		}
		if (m_Keys[KEY_D])
		{
			m_CharacterSprite.z = 3.0f;
			m_CharacterSprite.y += 0.1f;
			platformPos.x -= velocity;
		}
		if (m_Keys[KEY_W])
		{
			m_CharacterSprite.z = 2.0f;
			m_CharacterSprite.y += 0.1f;
			platformPos.y -= velocity;
		}
		if (m_Keys[KEY_S])
		{
			m_CharacterSprite.z = 1.0f;
			m_CharacterSprite.y += 0.1f;
			platformPos.y += velocity;
		}
		if (!(m_Keys[KEY_A] || m_Keys[KEY_D] || m_Keys[KEY_W] || m_Keys[KEY_S]))
		{
			m_CharacterSprite.y = 2.0f;
		}
	}

	if (m_CharacterSprite.y > 2.0f)
		m_CharacterSprite.y = 0.0f;
}
