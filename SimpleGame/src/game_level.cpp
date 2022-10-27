#include "game_level.h"

#include <iostream>
#include <fstream>

vec2i platformPos{ 0, 0 };

void BeginLevel::OnStart()
{
	LoadLevel("assets/levels/level1.data");
}

void BeginLevel::Draw()
{
	static float gap = m_CellSize * 2;
	static float leftBoundary = 0.0f;
	static float rightBoundary = m_LevelWidth + m_CellSize * 2;
	static float topBoundary = m_LevelHeight + m_CellSize * 2;
	static float bottomBoundary = 0.0f;

	zeus::QuadData quad;
	for (int y = 0; y < m_LevelRows; y++)
	{
		for (int x = 0; x < m_LevelCols; x++)
		{
			int idx = x + y * m_LevelCols;
			float xx = x * gap - platformPos.x + 32.0f;
			float yy = y * gap - platformPos.y - 32.0f;

			if (xx < leftBoundary || xx > rightBoundary || yy < bottomBoundary || yy > topBoundary)
				continue;

			quad.SetPosition({ xx, yy, 0.0f });
			quad.SetSize({ m_CellSize, m_CellSize, 0.0f });
			quad.SetRotation(0.0f);
			quad.SetSubTexture(m_LevelResources.TexManager->GetSubTexture("building_sheet", m_Map[idx].x, m_Map[idx].y));
			quad.SetEntityID(idx);

			zeus::Renderer::DrawTexturedQuad(quad);
		}
	}
}

void BeginLevel::LoadLevel(const std::string& filepath)
{
	m_LevelPath = filepath;

	std::ifstream file(m_LevelPath);

	std::string line;

	// Reading columns count
	getline(file, line);
	size_t beginPos = line.find(':');
	size_t endPos = line.size();
	m_LevelCols = std::stoi(line.substr(beginPos + 1, beginPos - endPos - 1));

	// Reading rows count
	getline(file, line);
	beginPos = line.find(':');
	endPos = line.size();
	m_LevelRows = std::stoi(line.substr(beginPos + 1, beginPos - endPos - 1));

	// Reading cell size
	getline(file, line);
	beginPos = line.find(':');
	endPos = line.size();
	m_CellSize = std::stoi(line.substr(beginPos + 1, beginPos - endPos - 1));

	// Escape blank line
	std::getline(file, line);

	// Reading map
	while (std::getline(file, line))
	{
		size_t lastPos = 0u;
		for (size_t k = 0; k < line.size(); k++)
		{
			beginPos = line.find('(', lastPos);
			size_t midPos = line.find(',', beginPos);
			endPos = line.find(')', beginPos + 1);

			int x = stoi(line.substr(beginPos + 1, midPos - beginPos - 1));
			int y = stoi(line.substr(midPos + 1, endPos - midPos - 1));

			m_Map.push_back({ x, y });

			lastPos = endPos + 1;
			k = lastPos;
		}
	}
}

void BeginLevel::SetLevelSize(uint32_t width, uint32_t height)
{
	m_LevelWidth = width;
	m_LevelHeight = height;
}
