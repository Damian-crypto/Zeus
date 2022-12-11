#include "enemy.h"
#include "glm/glm.hpp"

#include <iostream>

void Enemy::SetTarget(const std::shared_ptr<Character> target)
{
	m_Target = target;
}

HumanEnemy::HumanEnemy()
{
}

void HumanEnemy::Attack(float direction)
{
	if (GetPhyzicsEngine() == nullptr)
	{
		LOG(Error, "Runtime Error: Phyzics engine is not initialized!");
		return;
	}
	if (m_Weapon == nullptr)
	{
		LOG(Error, "Runtime Error: Weapon is not selected but trying to shoot!");
	}
	else
	{
		m_Weapon->Attack(direction);
	}
}

void HumanEnemy::OnUpdate(float dt)
{
	m_Weapon->OnUpdate(dt);

	if (GetPhyzicalBody()->IsDead)
	{
		IsDead = true;
	}

	if (Collided && CollideObject == "rock")
	{
		auto& velo = GetVelocity();
		velo.y = -velo.y;
		SetVelocity(velo);
		SetPosition(m_LastPosition);
		Collided = false;
		CollideObject = "";
	}
	else
	{
		m_LastPosition = GetPosition();
	}

	glm::vec3 targetPos = m_Target->GetPosition();
	glm::vec2 targetSize = m_Target->GetSize();
	zeus::Rectangle targetArea(targetPos.x - targetSize.x / 2, targetPos.y - targetSize.y / 2, targetSize.x, targetSize.y);
	
	glm::vec3 pos = GetPosition();
	glm::vec2 size = GetSize();
	zeus::Rectangle area(pos.x - size.x / 2, pos.y, size.x, size.y);

	if (area.IsIntersectingWith(targetArea, 100))
	{
		float dir = atan2(targetPos.y - pos.y, targetPos.x - pos.x);
		Attack(dir);
	}
	else
	{
		Move();
	}
}
