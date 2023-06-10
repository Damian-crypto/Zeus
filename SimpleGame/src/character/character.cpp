#include "character.h"
#include "phyzicsall.h"

#include <cstring>
#include <iostream>

Character::Character()
{
	m_PhysicalBody = std::make_shared<zeus::PhyzicalBody>();
	m_PhysicalBody->Type = zeus::BodyShape::Quad;
	m_PhysicalBody->Position = m_Position;
	m_PhysicalBody->InternalData = (void*)"dummy body";
	m_PhysicalBody->SetWidth(90.0f);
	m_PhysicalBody->SetHeight(100.0f);
	m_Behaviour = Behaviour::None;
}

float step = 0.0f; // Speed of changing the sprite (animation)
const int NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3; // Direction index of the player in sprite sheet
void Character::Move(glm::vec3 velocity)
{
	m_LastPosition = m_Position;
	
	if (velocity.x == 0 && velocity.y == 0 && velocity.z == 0)
	{
		velocity = GetVelocity();
	}

	bool moving = (velocity.x != 0 || velocity.y != 0 || velocity.z != 0);

	velocity.y *= 0.5f;
	if (moving)
	{
		m_Position += velocity;
	}

	if (velocity.x != 0)
	{
		m_SpriteCoords.x = m_Sprite.Coords.x + (velocity.x > 0 ? WEST : NORTH);
	}

	if (velocity.y != 0)
	{
		m_SpriteCoords.x = m_Sprite.Coords.x + (velocity.y > 0 ? SOUTH : EAST);
	}

	if (moving)
	{
		m_SpriteCoords.y = m_Sprite.Coords.y + step;
	}

	m_PhysicalBody->Position = m_Position;
	m_Weapon->SetPosition(m_Position);

	step += 0.03f;
	if (step > 2.8f)
	{
		step = 0.0f;
	}
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

void Character::SetBehaviour(Behaviour behave)
{
	m_Behaviour = behave;
	if (m_Behaviour == Behaviour::WalkUpDown)
		SetVelocity({ 0, 1, 0 });
	else if (m_Behaviour == Behaviour::WalkLeftRight)
		SetVelocity({ 1, 0, 0 });
}

void Character::SetPhyzicsEngine(std::shared_ptr<zeus::Phyzics> phyzics)
{
	if (phyzics == nullptr)
	{
		LOG(Error, "Runtime Error: Phyzics engine is not initialized for calculations!");
		return;
	}

	m_Phyzics = phyzics;
	m_Phyzics->AddBody(m_PhysicalBody);
}

void Character::OnUpdate(float dt)
{
	m_Weapon->OnUpdate(dt);
}

void Character::OnRender()
{
	static zeus::QuadData characterQuad;
	characterQuad.SetPosition(m_Position);
	characterQuad.SetSubTexture(m_TexManager->GetSubTexture(m_Sprite.Name, m_SpriteCoords.x, m_SpriteCoords.y));
	characterQuad.SetSize(m_Sprite.Size);
	zeus::Renderer::DrawTexturedQuad(characterQuad);

	m_Weapon->OnRender();

	m_SpriteCoords.y = m_Sprite.Coords.y + 2; // otherwise player will stop at random positions :)
}

void Character::SetSprite(const Sprite& sprite)
{
	m_Sprite = sprite;
	m_Size = { sprite.Size.x, sprite.Size.y };
	m_SpriteCoords = sprite.Coords;
}
