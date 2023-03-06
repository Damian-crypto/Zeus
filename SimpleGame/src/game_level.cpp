#include "game_level.h"

#include <memory>
#include <iostream>
#include <fstream>

glm::ivec2 platformPos{ -230, -96 };

void BeginLevel::OnStart()
{
	m_Serializer = std::make_shared<zeus::Zerializer>();
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
	}
}

void BeginLevel::SetEnemyRegistry(std::shared_ptr<EnemyRegistry> enemyRegistry)
{
	m_EnemyRegistry = enemyRegistry;
}

void BeginLevel::LoadLevel(const std::string& filepath)
{
	m_LevelPath = filepath;

	zeus::SerialInfo info;
	info.SerializationType = zeus::SerialType::PROPERTIES;
	try
	{
		m_Serializer->StartDeserialize(filepath, info);
	}
	catch (std::runtime_error& e)
	{
		LOG(Error, e.what());
		return;
	}

	m_LevelCols = m_Serializer->DeserializeInt("cols");
	m_LevelRows = m_Serializer->DeserializeInt("rows");
	m_CellSize = m_Serializer->DeserializeInt("cellsize");
	m_CellGap = m_Serializer->DeserializeDbl("cellgap");
	
    const std::function<Tile(const std::string&)> sToTile = [](const std::string& s) {
        Tile tile;
        std::vector<int> res;
        std::stringstream ss(s);
        std::string token;
        size_t pos;

        std::cout << s << std::endl;

        while (ss >> token)
        {
            pos = token.find(",");
            if (pos != std::string::npos)
                token.erase(pos, 1);
            res.push_back(std::stoi(token));
        }

        tile.TexCoords = { res[0], res[1] };
        tile.Type = (TileType)res[2];
        tile.idx = res[3];

        return tile;
    };
    std::vector<Tile> levelmap = m_Serializer->DeserializeVec<Tile>("levelmap", sToTile);

	// Reading map
	for (Tile& tile : levelmap)
	{
		tile.PhysicalBody = std::make_shared<zeus::PhyzicalBody>();
		tile.PhysicalBody->SetWidth(m_CellSize);
		tile.PhysicalBody->SetHeight(m_CellSize);
		if (tile.Type == TileType::Rock) {
			tile.PhysicalBody->InternalData = (void*)"rock";
			m_Phyzics->AddBody(tile.PhysicalBody);
		} else {
			tile.PhysicalBody->InternalData = (void*)"none";
		}

		m_Map.emplace_back(tile);
	}

	// Reading enemy
	if (m_Serializer->ReadHeader("enemy1"))
	{
		std::string enemyType = m_Serializer->DeserializeStr("type");
		EnemyType type = EnemyType::None;
		if (enemyType == "human")
		{
			type = EnemyType::Human;
		}
		else if (enemyType == "animal")
		{
			type = EnemyType::Animal;
		}
		else
		{
			type = EnemyType::None;
		}

		if (m_EnemyRegistry == nullptr)
		{
			LOG(Error, "Runtime Error: Enemy registry is not initialized for operation!");
		}

		// Reading enemy position
		auto enemy = m_EnemyRegistry->CreateEnemy(type);

		const std::function<int(const std::string&)> sToInt = [](const std::string& s) {
			return std::stoi(s);
		};
		std::vector<int> pos = m_Serializer->DeserializeVec<int>("pos", sToInt);
		enemy->SetPosition({ pos[0], pos[1], 0.1f });
		m_Enemies.push_back(enemy);
		
		// Reading enemy weapon
		std::string weaponName = m_Serializer->DeserializeStr("weapon");
		if (weaponName == "gun")
		{
			enemy->SetWeapon(WeaponType::Gun);
		}
		m_Serializer->EndHeader();
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
