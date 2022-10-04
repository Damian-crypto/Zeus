#include "game_layer.h"

#include "imgui.h"

GameLayer::GameLayer(const char* name)
{
	m_Camera = std::make_shared<zeus::Camera>();
	m_Camera->GetProperties().Position = { -WIDTH / 2.0f, -HEIGHT / 2.0f, 0.0f };
	m_Camera->GetProperties().MovementSpeed = 85.0f;

	m_TexManager = zeus::TextureManager::GetInstance();
	m_TexManager->PutSpritesheet({ "person_sheet", "assets/textures/spritesheets/tilemap_packed.png", 16, true });
	m_TexManager->PutSpritesheet({ "building_sheet", "assets/textures/spritesheets/roguelike_spritesheet.png", 17, true });

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

glm::vec2 platformPos{ 0.0f, 0.0f };
void GameLayer::OnRender()
{
	zeus::Renderer::Start(m_Camera);

	for (int x = 0; x <= 256; x += 32)
	{
		for (int y = 0; y <= 256; y += 32)
		{
			zeus::Renderer::DrawTexturedQuad({ platformPos.x + x, platformPos.y + y, 0.0f }, { 16, 16, 0 }, m_TexManager->GetSubTexture("building_sheet", 0, 0));
		}
	}
	zeus::Renderer::DrawTexturedQuad(m_CharacterSprite, { 16, 16, 0 }, m_TexManager->GetSubTexture("person_sheet", 24 + m_CharacterSprite.x + m_CharacterSprite.w, 16 + m_CharacterSprite.y));

	ImGui::Begin("character");
	{
		ImGui::DragFloat("z value", &m_CharacterSprite.z, 0.01f);
		ImGui::End();
	}

	zeus::Renderer::Flush(m_TexManager);
}

float speed = 0.5f;
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
		if (m_Keys[KEY_A])
		{
			m_CharacterSprite.w = 0.0f;
			m_CharacterSprite.y += 0.1f;
			platformPos.x += speed;
		}
		if (m_Keys[KEY_D])
		{
			m_CharacterSprite.w = 3.0f;
			m_CharacterSprite.y += 0.1f;
			platformPos.x -= speed;
		}
		if (m_Keys[KEY_W])
		{
			m_CharacterSprite.w = 2.0f;
			m_CharacterSprite.y += 0.1f;
			platformPos.y -= speed;
		}
		if (m_Keys[KEY_S])
		{
			m_CharacterSprite.w = 1.0f;
			m_CharacterSprite.y += 0.1f;
			platformPos.y += speed;
		}
		if (!(m_Keys[KEY_A] || m_Keys[KEY_D] || m_Keys[KEY_W] || m_Keys[KEY_S]))
		{
			m_CharacterSprite.y = 2.0f;
		}
	}

	if (m_CharacterSprite.y > 2.0f)
		m_CharacterSprite.y = 0.0f;
}
