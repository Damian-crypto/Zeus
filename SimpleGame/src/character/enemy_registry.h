#pragma once

#include "zeus.h"
#include "enemy.h"
#include "phyzicsall.h"

enum class EnemyType
{
	Human,
	Animal,
	None
};

class EnemyRegistry
{
private:
	std::shared_ptr<zeus::TextureManager> m_TexManager;
	std::vector<std::shared_ptr<Enemy>> m_Enemies;
	std::shared_ptr<zeus::Phyzics> m_Phyzics;
	std::shared_ptr<Character> m_Target;

public:
	EnemyRegistry() = default;
	EnemyRegistry(std::shared_ptr<zeus::TextureManager> texManager);

	/* Returns a new enemy pointer which is stored in enemy registry.
	 * Sprite for the specific enemy is already determined.
	 * @param type Type of the enemy will return (Human, Animal, None).
	 */
	std::shared_ptr<Enemy> CreateEnemy(EnemyType type);
	
	std::vector<std::shared_ptr<Enemy>>& GetEnemies() { return m_Enemies; };
	
	void SetPhyzicsEngine(std::shared_ptr<zeus::Phyzics> phyzics) { this->m_Phyzics = phyzics; }
	void SetTarget(std::shared_ptr<Character> target);

	void OnUpdate(float dt);
	void OnRender();
};
