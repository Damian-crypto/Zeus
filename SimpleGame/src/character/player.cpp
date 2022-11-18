#include "player.h"

Player::Player()
{
}

void Player::Attack(float direction)
{
	m_Weapon->Attack(direction);
}
