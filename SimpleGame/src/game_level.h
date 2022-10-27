#pragma once

#include "zeus.h"

struct vec2i
{
	int x, y;
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

	std::vector<glm::ivec2> m_Map;

public:
	void OnStart() override;
	void Draw() override;

	void LoadLevel(const std::string& filepath);
	void SetLevelSize(uint32_t width, uint32_t height);
};
