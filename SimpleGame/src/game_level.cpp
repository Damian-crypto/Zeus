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
	for (int y = 11; y >= 0; y--)
	{
		for (int x = 0; x < 18; x++)
		{
			int xx = x * 64 + platformPos.x;
			int yy = y * 64 + platformPos.y;

			zeus::Renderer::DrawTexturedQuad({ (float)xx, (float)yy, 0.0f }, { 32, 32, 0 }, m_LevelResources.TexManager->GetSubTexture("building_sheet", m_Map[y][x].x, m_Map[y][x].y));
		}
	}
}

void BeginLevel::LoadLevel(const std::string& filepath)
{
	std::ifstream file(filepath);

	std::string line;
	while (std::getline(file, line))
	{
		size_t lastPos = 0u;
		std::vector<vec2i> tmp;
		for (size_t k = 0; k < line.size(); k++)
		{
			size_t beginPos = line.find('(', lastPos);
			size_t midPos = line.find(',', beginPos);
			size_t endPos = line.find(')', beginPos + 1);

			int x = stoi(line.substr(beginPos + 1, midPos - beginPos - 1));
			int y = stoi(line.substr(midPos + 1, endPos - midPos - 1));

			tmp.push_back({ x, y });

			lastPos = endPos + 1;
			k = lastPos;
		}

		m_Map.push_back(tmp);
	}
}
