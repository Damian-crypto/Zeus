#include "enemy.h"
#include "glm/glm.hpp"

#include <iostream>

HumanEnemy::HumanEnemy()
{
}

void HumanEnemy::Attack(float direction)
{
	m_Weapon->Attack(direction);
}

float factor = 1.0f;
void HumanEnemy::OnUpdate(float dt)
{
	float velocity = 150.0f * dt;

	Move(0, factor * velocity, 0);

	if (Collided && CollideObject == "rock")
	{
		// auto pos = GetPosition();
		// LOG(Trace, "Last good pos %f %f", m_LastPosition.x, m_LastPosition.y);
		// LOG(Trace, "Collided at %f %f", pos.x, pos.y);
		// pos.y -= 50;
		// SetPosition(pos);
		factor = -factor;
		SetPosition(m_LastPosition);
		Collided = false;
		CollideObject = "";
	}
	else
	{
		// TODO: cannot find "Last Good Position"
		m_LastPosition = GetPosition();
	}
}
