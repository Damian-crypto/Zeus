#include "enemy_registry.h"

#include <cstring>

EnemyRegistry::EnemyRegistry(std::shared_ptr<zeus::TextureManager> texManager)
{
	m_TexManager = texManager;
}

std::shared_ptr<Enemy> EnemyRegistry::CreateEnemy(EnemyType type)
{
	std::shared_ptr<Enemy> enemy = nullptr;
	switch (type)
	{
		case EnemyType::Human:
		{
			enemy = std::make_shared<HumanEnemy>();
			enemy->SetTextureManager(m_TexManager);
			enemy->SetPosition({ 500.0f, 300.0f, 0.09f });
			enemy->SetSprite({ { 24.0f, 13.0f, 2.0f }, { 32.0f, 32.0f, 0.0f }, "person_sheet" });
			enemy->SetWeapon(WeaponType::Gun);
			m_Enemies.push_back(enemy);

			if (m_Phyzics == nullptr)
			{
				LOG(Error, "Runtime Error: Phyzics engine is not initialized for calculations!");
			}

			enemy->GetPhyzicalBody()->InternalData = (void*)"enemy";
			enemy->GetPhyzicalBody()->CollideFunction = [&](const std::shared_ptr<zeus::PhyzicalBody> body) {
				std::shared_ptr<Enemy> ptr = m_Enemies.back();
				if (strcmp((const char*)body->InternalData, "bullet") == 0)
				{
					ptr->IsDead = true;
				}
			};

			m_Phyzics->AddDynamicBody(enemy->GetPhyzicalBody());

			return enemy;
		}
	}

	LOG(Error, "Runtime Error: You specified invalid enemy type!");

	return nullptr;
}

void EnemyRegistry::OnUpdate(float dt)
{
	for (size_t i = 0; i < m_Enemies.size(); i++)
	{
		auto& enemy = m_Enemies[i];
		if (!enemy->IsDead)
		{
			enemy->GetPhyzicalBody()->Position = enemy->GetPosition();
			enemy->OnUpdate(dt);
		}
		else
		{
			enemy->GetPhyzicalBody()->IsDead = true;
			m_Enemies.erase(m_Enemies.begin() + i);
		}
	}
}

void EnemyRegistry::OnRender()
{
	for (auto& enemy : m_Enemies)
	{
		enemy->OnRender();
	}
}
