#include "character.h"

Character::Character()
{
	m_PhysicalBody = std::make_shared<zeus::PhyzicalBody>();
	m_PhysicalBody->Type = zeus::BodyShape::Quad;
	m_PhysicalBody->Position = m_Position;
	m_PhysicalBody->InternalData = (void*)"player";
	m_PhysicalBody->SetWidth(80.0f);
	m_PhysicalBody->SetHeight(100.0f);
	m_PhysicalBody->CollideFunction = [&](const std::shared_ptr<zeus::PhyzicalBody> body) {
		const auto& pos = body->Position;
		QUICK_LOG(Trace, "%s hit by %s at %f %f", (const char*)m_PhysicalBody->InternalData, (const char*)body->InternalData, pos.x, pos.y);
	};
}

float step = 0.0f;
void Character::Move(float x, float y, float z, bool lock)
{
	m_Moving = true;

	// Direction of the player looking at
	//		0 - North
	//		1 - East
	//		2 - South
	//		3 - West
	if (x != 0)
	{
		m_SpriteCoords.x = m_Sprite.Coords.x + (x > 0 ? 3 : 0);
		if (!lock)
			m_Position.x += x;
	}

	if (y != 0)
	{
		m_SpriteCoords.x = m_Sprite.Coords.x + (y > 0 ? 2 : 1);
		if (!lock)
			m_Position.y += y;
	}

	if (z != 0)
	{
		m_Position.z += z;
	}

	m_SpriteCoords.y = m_Sprite.Coords.y - step;

	if (!lock)
		m_SpriteCoords.z -= m_Sprite.Coords.z;

	m_Weapon->SetPosition(m_Position);

	step -= 0.1f;
	if (step < 0.0f)
		step = 2.0f;
}

void Character::SetWeapon(WeaponType type)
{
	switch (type)
	{
	case WeaponType::Gun:
		m_Weapon = std::make_shared<Gun>(m_TexManager, m_Position);
		m_Weapon->SetPhyzicsEngine(m_Phyzics);
		break;
	}
}

void Character::SetPhyzicsEngine(std::shared_ptr<zeus::Phyzics> phyzics)
{
	if (phyzics == nullptr)
	{
		LOG(Error, "Runtime Error: Phyzics engine is not initialized for calculations!");
	}

	m_Phyzics = phyzics;
	m_Phyzics->AddDynamicBody(m_PhysicalBody);
}

void Character::OnUpdate(float dt)
{
	m_PhysicalBody->Position = m_Position;
	m_Weapon->OnUpdate(dt);
}

void Character::OnRender()
{
	if (!m_Moving)
	{
		m_SpriteCoords.y = m_Sprite.Coords.y;
	}

	static zeus::QuadData quad;
	quad.SetPosition(m_Position);
	quad.SetSubTexture(m_TexManager->GetSubTexture(m_Sprite.Name, m_SpriteCoords.x, m_SpriteCoords.y));
	quad.SetSize(m_Sprite.Size);
	zeus::Renderer::DrawTexturedQuad(quad);

	m_Weapon->OnRender();

	m_Moving = false;
}
