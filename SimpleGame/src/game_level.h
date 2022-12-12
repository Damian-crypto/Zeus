#pragma once

#include "zeus.h"
#include "phyzicsall.h"
#include "character/enemy_registry.h"

enum class TileType
{
	Water = 0,
	Rock,
	Tree,
	None
};

struct Tile
{
	int idx = -1;
	glm::vec3 Position;
	glm::ivec2 TexCoords;
	TileType Type;
	std::shared_ptr<zeus::PhyzicalBody> PhysicalBody;
};

class BeginLevel : public zeus::Level
{
protected:
	std::string m_LevelPath;

	uint32_t m_LevelCols{ 0u };
	uint32_t m_LevelRows{ 0u };
	uint32_t m_CellSize{ 0u };
	uint32_t m_LevelWidth{ 1280u };
	uint32_t m_LevelHeight{ 720u };
	float m_CellGap{ 2.0f };

	std::vector<Tile> m_Map;

private:
	std::shared_ptr<zeus::Phyzics> m_Phyzics;
	std::shared_ptr<EnemyRegistry> m_EnemyRegistry;
	std::vector<shared_ptr<Enemey>> m_Enemies;

public:
	void OnStart() override;
	void Draw() override;

	void LoadLevel(const std::string& filepath);
	void SetLevelSize(uint32_t width, uint32_t height);
	void SetPhyzicsEngine(std::shared_ptr<zeus::Phyzics> phyzics);
	void SetEnemyRegistry(std::shared_ptr<EnemyRegistry> enemyRegistry);
};
