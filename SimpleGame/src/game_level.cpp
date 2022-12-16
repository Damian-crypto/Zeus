#include "game_level.h"

#include <memory>
#include <iostream>
#include <fstream>

glm::ivec2 platformPos{ -230, -96 };

void BeginLevel::OnStart()
{
	LoadLevel("assets/levels/level1.data");
}

void BeginLevel::Draw()
{
	float gap = m_CellSize * m_CellGap;
	static float leftBoundary	= 0.0f;
	static float rightBoundary	= m_LevelWidth + m_CellSize * 2;
	static float topBoundary	= m_LevelHeight + m_CellSize * 2;
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

			if (idx < m_Map.size())
			{
				m_Map[idx].PhysicalBody->Position = { xx, yy, 0 };
				quad.SetPosition({ xx, yy, 0.0f });
				quad.SetSize({ m_CellSize, m_CellSize, 0.0f });
				quad.SetRotation(0.0f);
				quad.SetSubTexture(m_LevelResources.TexManager->GetSubTexture("building_sheet", m_Map[idx].TexCoords.x, m_Map[idx].TexCoords.y));
				quad.SetEntityID(idx);

				zeus::Renderer::DrawTexturedQuad(quad);
			}

			// const auto& tile = m_Map[idx];
			// std::cout << "(" << tile.TexCoords.x << ", " << tile.TexCoords.y << ", " << (int)tile.Type << ") ";

			//zeus::Renderer::DrawQuad(quad);
		}
		// std::cout << '\n';
	}
}

void BeginLevel::SetEnemyRegistry(std::shared_ptr<EnemyRegistry> enemyRegistry)
{
	m_EnemyRegistry = enemyRegistry;
}

// String related utility functions for removing white spaces of a string(trailing/leading)
static inline void left_trim(std::string& s)
{
	s.erase(s.begin(), std::find_if(
			s.begin(),
			s.end(),
			[](unsigned char c) { return not std::isspace(c); }
		)
	);
}

static inline void right_trim(std::string& s)
{
	s.erase(std::find_if(
			s.rbegin(),
			s.rend(),
			[](unsigned char c) { return not std::isspace(c); }
		).base(),
	s.end());
}

static inline void trim(std::string& s)
{
	left_trim(s);
	right_trim(s);
}

void BeginLevel::LoadLevel(const std::string& filepath)
{
	m_LevelPath = filepath;

	std::ifstream file(m_LevelPath);

	if (!file.is_open())
	{
		LOG(Error, "Level file couldn't be found!");
		return;
	}

	long int begin = file.tellg();
	file.seekg(0, std::ios::end);
	long int filesize = file.tellg();
	file.seekg(std::ios::beg);
	if (filesize <= 0)
	{
		LOG(Warn, "LevelMap is empty -> %s", m_LevelPath.c_str());
		return;
	}

	try
	{
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

		// Reading cell gap
		getline(file, line);
		beginPos = line.find(':');
		endPos = line.size();
		m_CellGap = std::stof(line.substr(beginPos + 1, beginPos - endPos - 1));

		// Escape blank line
		std::getline(file, line);

		// Escape levelmap: line
		std::getline(file, line);

		// Reading map
		for (int row = 0; row <= m_LevelRows; row++)
		{
			std::getline(file, line);
			// std::cout << "map line: " << line << "\n";

			size_t lastPos = 0u;
			for (size_t k = 0; k < line.size(); k++)
			{
				if (lastPos + 14 > line.size() || k + 14 > line.size())
				{
					break;
				}

				beginPos = line.find('(', lastPos);
				size_t midPos1 = line.find(',', beginPos);
				size_t midPos2 = line.find(',', midPos1 + 1);
				size_t midPos3 = line.find(',', midPos2 + 1);
				// size_t midPos4 = line.find(',', midPos3 + 1);
				endPos = line.find(')', beginPos + 1);

				int texX = stoi(line.substr(beginPos + 1, midPos1 - beginPos - 1));
				int texY = stoi(line.substr(midPos1 + 1, midPos2 - midPos1 - 1));
				int mode = stoi(line.substr(midPos2 + 1, midPos3 - midPos2 - 1));
				int idx = stoi(line.substr(midPos3 + 1, endPos - midPos3 - 1));
				// int posY = stoi(line.substr(midPos4 + 1, endPos - midPos3 - 1));

				std::cout << "(" << texX << "," << texY << "," << mode << "," << idx << ") ";

				Tile tile;
				tile.TexCoords = { texX, texY };
				tile.Type = (TileType)mode;
				// tile.Position = { posX, posY, 0 };
				tile.idx = idx;
				tile.PhysicalBody = std::make_shared<zeus::PhyzicalBody>();
				tile.PhysicalBody->SetWidth(m_CellSize);
				tile.PhysicalBody->SetHeight(m_CellSize);
				if (mode == 1) {
					tile.PhysicalBody->InternalData = (void*)"rock";
					m_Phyzics->AddBody(tile.PhysicalBody);
				} else {
					tile.PhysicalBody->InternalData = (void*)"none";
				}

				m_Map.emplace_back(tile);

				lastPos = endPos + 1;
				k = lastPos;
			}
			std::cout << '\n';
		}

		std::getline(file, line);
		if (line.substr(0, 5) == "enemy")
		{
			std::getline(file, line);
			EnemyType type = EnemyType::None;
			if (line.size() >= 4 && line.substr(0, 4) == "type")
			{
				beginPos = line.find(':');
				endPos = line.size();
				std::string typeStr = line.substr(beginPos + 1, endPos);
				trim(typeStr);
				if (typeStr == "human")
				{
					type = EnemyType::Human;
				}
				else if (typeStr == "animal")
				{
					type = EnemyType::Animal;
				}
				else
				{
					type = EnemyType::None;
				}
			}

			if (m_EnemyRegistry == nullptr)
			{
				LOG(Error, "Runtime Error: Enemy registry is not initialized for operation!");
			}

			auto enemy = m_EnemyRegistry->CreateEnemy(type);
			std::getline(file, line);
			if (line.size() >= 3 && line.substr(0, 3) == "pos")
			{
				beginPos = line.find('(');
				size_t midPos = line.find(',');
				endPos = line.size();

				float x = stof(line.substr(beginPos + 1, midPos - beginPos - 1));
				float y = stof(line.substr(midPos + 1, endPos - midPos - 1));
				enemy->SetPosition({ x, y, 0.1f });

				m_Enemies.push_back(enemy);
			}

			std::getline(file, line);
			if (line.size() >= 6 && line.substr(0, 6) == "weapon")
			{
				beginPos = line.find(':');
				endPos = line.size();
				std::string weaponStr = line.substr(beginPos + 1, endPos);
				trim(weaponStr);
				if (weaponStr == "gun")
				{
					enemy->SetWeapon(WeaponType::Gun);
				}
			}
		}
	}
	catch (std::exception e)
	{
		LOG(Error, "Runtime Error: when loading the level -> %s", e.what());
	}
}

void BeginLevel::SetLevelSize(uint32_t width, uint32_t height)
{
	m_LevelWidth = width;
	m_LevelHeight = height;
}

void BeginLevel::SetPhyzicsEngine(std::shared_ptr<zeus::Phyzics> phyzics)
{
	m_Phyzics = phyzics;
}
