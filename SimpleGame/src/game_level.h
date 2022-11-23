#pragma once

#include "zeus.h"

enum class TileType
{
	Water = 0,
	Rock,
	Tree
};

struct Tile
{
	glm::vec3 Position;
	glm::ivec2 TexCoords;
	TileType Type;
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

	std::vector<Tile> m_Map;

public:
	void OnStart() override;
	void Draw() override;

	void LoadLevel(const std::string& filepath);
	void SetLevelSize(uint32_t width, uint32_t height);
};
