#include "player.h"

#include <cstring>

Player::Player()
{
	// GetPhyzicalBody()->CollideFunction = [&](const std::shared_ptr<zeus::PhyzicalBody> body) {
	// 	const auto& pos = body->Position;
	// 	if (strcmp((const char*)body->InternalData, "rock") == 0)
	// 	{
	// 		SetPosition(m_LastPosition);
	// 	}
	// 	if (strcmp((const char*)body->InternalData, "enemy") == 0)
	// 	{
	// 		SetPosition(m_LastPosition);
	// 	}
	// 	// QUICK_LOG(Trace, "%s hit by %s at %f %f", (const char*)m_PhysicalBody->InternalData, (const char*)body->InternalData, pos.x, pos.y);
	// };
}

void Player::Attack(float direction, float offset)
{
	m_Weapon->Attack(direction, offset);
}
