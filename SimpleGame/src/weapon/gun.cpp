#include "weapon.h"

Gun::Gun()
{
	Init();
}

Gun::Gun(std::shared_ptr<zeus::TextureManager> texManager, const glm::vec3& position)
{
	m_TexManager = texManager;
	m_Position = position;
	Init();
}

void Gun::Init()
{
}

void Gun::OnUpdate(float dt)
{
	// static const auto& app = zeus::Application::GetInstance();
	for (size_t i = 0; i < m_Bullets.size(); i++)
	{
		auto& bullet = m_Bullets[i];
		//bullet.Position += bullet.Direction * dt;
		bullet.Position.x += std::cos(bullet.Direction) * 80;
		bullet.Position.y += std::sin(bullet.Direction) * 80;
		// QUICK_LOG(Trace, "bullet %d going to %f", i, bullet.Direction);
		if (bullet.PhysicalBody != nullptr)
		{
			bullet.PhysicalBody->Position = bullet.Position;
		}
		if (bullet.Position.x > bullet.Origin.x + bullet.MaxDistance ||
			bullet.Position.x < bullet.Origin.x - bullet.MaxDistance ||
			bullet.Position.y > bullet.Origin.y + bullet.MaxDistance ||
			bullet.Position.y < bullet.Origin.y - bullet.MaxDistance
			)
		{
			bullet.PhysicalBody->IsDead = true;
			m_Bullets.erase(m_Bullets.begin() + i);
		}
	}
}

void Gun::Attack(float direction, float offset)
{
	Bullet bullet;
	bullet.Direction = direction;
	bullet.Origin = m_Position;
	float x = m_Position.x + offset * cos(direction);
	float y = m_Position.y + offset * sin(direction);
	bullet.Position = { x, y, 0.0f }; // TODO: bullet hitting me
	bullet.MaxDistance = 200.0f;
	bullet.PhysicalBody = std::make_shared<zeus::PhyzicalBody>();
	bullet.PhysicalBody->Type = zeus::BodyShape::Quad;
	bullet.PhysicalBody->SetWidth(32);
	bullet.PhysicalBody->SetHeight(32);
	bullet.PhysicalBody->InternalData = (void*)"bullet";
	if (m_Phyzics == nullptr)
	{
		LOG(Error, "Runtime Error: Phyzics engine is not initialized for calculations!");
		return;
	}
	m_Phyzics->AddBody(bullet.PhysicalBody);
	m_Bullets.push_back(bullet);
}

void Gun::OnRender()
{
	static zeus::QuadData quad;
	for (const auto& bullet : m_Bullets)
	{
		if (!bullet.PhysicalBody->IsDead)
		{
			quad.SetPosition({ bullet.Position.x, bullet.Position.y, 0.1f });
			quad.SetSize({ 20, 20, 0 });
			const auto& tex = m_TexManager->GetSubTexture("bullet_sheet", 4, 6);
			quad.SetSubTexture(tex);
			zeus::Renderer::DrawTexturedQuad(quad);
		}
	}
}
