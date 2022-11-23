#pragma once

#include "zeus.h"
#include "enemy.h"
#include "phyzicsall.h"

enum class EnemyType
{
	Human,
	Animal
};

class EnemyRegistry
{
private:
	std::shared_ptr<zeus::TextureManager> m_TexManager;
	std::vector<std::shared_ptr<Enemy>> m_Enemies;
	std::shared_ptr<zeus::Phyzics> m_Phyzics;

public:
	EnemyRegistry() = default;
	EnemyRegistry(std::shared_ptr<zeus::TextureManager> texManager);

	std::shared_ptr<Enemy> CreateEnemy(EnemyType type);
	std::vector<std::shared_ptr<Enemy>>& GetEnemies() { return m_Enemies; };
	void SetPhyzicsEngine(std::shared_ptr<zeus::Phyzics> phyzics) { this->m_Phyzics = phyzics; }

	void OnUpdate(float dt);
	void OnRender();
};
