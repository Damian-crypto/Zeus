#include "weapon.h"

Gun::Gun()
{
}

Gun::Gun(std::shared_ptr<zeus::TextureManager> texManager, const glm::vec3& position)
{
	m_TexManager = texManager;
	m_Position = position;
}

void Gun::OnUpdate(float dt)
{
	static const auto& app = zeus::Application::GetInstance();
	for (auto& bullet : m_Bullets)
	{
		//bullet.Position += bullet.Direction * dt;
		bullet.Position.x += std::cos(bullet.Direction) * 10;
		bullet.Position.y += std::sin(bullet.Direction) * 10;
		if (bullet.PhysicalBody != nullptr)
		{
			bullet.PhysicalBody->Position = bullet.Position;
		}
		if (bullet.Position.x > app->GetApplicationProperties().Width ||
			bullet.Position.x < 0 ||
			bullet.Position.y > app->GetApplicationProperties().Height ||
			bullet.Position.y < 0
			)
		{
			m_Bullets.begin()->PhysicalBody->IsDead = true;
			m_Bullets.erase(m_Bullets.begin());
		}
	}
}

void Gun::Attack(float direction)
{
	Bullet bullet;
	bullet.Direction = direction;
	bullet.Position = m_Position; // TODO: bullet hitting me
	bullet.PhysicalBody = std::make_shared<zeus::PhyzicalBody>();
	bullet.PhysicalBody->Type = zeus::BodyShape::Quad;
	bullet.PhysicalBody->SetWidth(32);
	bullet.PhysicalBody->SetHeight(32);
	bullet.PhysicalBody->InternalData = (void*)"bullet";
	bullet.PhysicalBody->CollideFunction = CollisionFn;
	if (m_Phyzics == nullptr)
	{
		LOG(Error, "Runtime Error: Phyzics engine is not initialized for calculations!");
	}
	m_Phyzics->AddBody(bullet.PhysicalBody);
	m_Bullets.push_back(bullet);
}

void Gun::OnRender()
{
	static zeus::QuadData quad;
	for (const auto& bullet : m_Bullets)
	{
		quad.SetPosition({ bullet.Position.x, bullet.Position.y, 0.1f });
		quad.SetSize({ 20, 20, 0 });
		const auto& tex = m_TexManager->GetSubTexture("bullet_sheet", 4, 6);
		quad.SetSubTexture(tex);
		zeus::Renderer::DrawTexturedQuad(quad);
	}
}
