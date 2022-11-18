#include "enemy.h"

#include <iostream>

HumanEnemy::HumanEnemy()
{
}

void HumanEnemy::Attack(float direction)
{
	m_Weapon->Attack(direction);
}

void HumanEnemy::OnUpdate(float dt)
{
}
