#pragma once

#include "sandbox.h"

class SandboxLevel : public zeus::Level
{
public:
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
		glm::vec3 Position{ 0.0f };
		glm::ivec2 TexCoords{ 0.0f };
		TileType Type = TileType::None;

		std::string ToString() const
		{
			std::stringstream ss;
			ss << "Tile(texX: " << TexCoords.x;
			ss << ", texY: " << TexCoords.y;
			ss << ", type: " << (int)Type;
			ss << ", index: " << idx << ")";

			return ss.str();
		}
	};

private:
	std::string m_LevelPath;

	uint32_t m_LevelCols{ 0u };
	uint32_t m_LevelRows{ 0u };
	uint32_t m_CellSize{ 32u };
	float m_CellGap = 2.0f;

	std::vector<Tile> m_Map;
	std::shared_ptr<zeus::Zerializer> m_Serializer;
	std::unordered_map<int, bool> m_Selected;

public:
	void OnStart() override
	{
		m_Serializer = std::make_shared<zeus::Zerializer>();
		LoadLevel("assets/levels/level.data");
	}

	std::string GetLevelPath() const
	{
		return m_LevelPath;
	}

	std::string GetLevelName() const
	{
		size_t begin = m_LevelPath.find_last_of('/');
		size_t end = m_LevelPath.find_last_of('.');

		return m_LevelPath.substr(begin + 1, end - begin - 1);
	}

	zeus::QuadData quad;
	void Draw() override
	{
		float gap = m_CellSize * m_CellGap;
		static float leftBoundary = 0.0f;
		static float rightBoundary = WIDTH - m_CellSize;
		static float topBoundary = HEIGHT - m_CellSize << 1;
		static float bottomBoundary = 0.0f;

		for (uint32_t y = 0; y < m_LevelRows; y++)
		{
			for (uint32_t x = 0; x < m_LevelCols; x++)
			{
				// idx = col + row * cols <- to conver the 2D indexing to 1D indexing
				int idx = x + y * m_LevelCols;
				// Normalize cartisian coordinates to viewport coordinates
				float xx = x * gap - spritePos.x + 32.0f;
				float yy = y * gap - spritePos.y - 32.0f;

				// All the tiles outside the viewer's boundary should negelet (performance tip).
				// There should be one tile outside the boundary of user's view (smooth presentation).
				if (xx < leftBoundary || xx > rightBoundary || yy < bottomBoundary || yy > topBoundary)
					continue;

				if (idx < (int)m_Map.size())
				{
					// If this tile (idx) is selected then it will be rendered larger than the others
					if (m_Selected[idx])
					{
						float dx = m_CellSize * 0.2f;
						quad.SetSize({ m_CellSize + dx, m_CellSize + dx, 1.0f });
					}
					else
					{
						quad.SetSize({ m_CellSize, m_CellSize, 0.0f });
					}

					// Quad is the engine specific tile
					quad.SetPosition({ xx, yy, m_Map[idx].Position.z });
					quad.SetRotation(0.0f);
					quad.SetSubTexture(textureManager->GetSubTexture("building_sheet", m_Map[idx].TexCoords.x, m_Map[idx].TexCoords.y));
					quad.SetEntityID(idx);

					zeus::Renderer::DrawTexturedQuad(quad);
				}
			}
		}

		// Pivot to keep in track
		quad.SetPosition(spritePos);
		quad.SetSize(scaler);
		quad.SetRotation(angle);
		quad.SetEntityID(2546);
		quad.SetTexture(textureManager->GetTexture("wood_tex"));

		zeus::Renderer::DrawTexturedQuad(quad);
	}

	/**
	 * This function will increase the map size always (cannot reduce map size)
	 *
	 * @param size: side length of the map and size always +ve (entire map is a square)
	 */
	void ResizeMap(uint32_t size)
	{
		uint32_t newSize = size * size;
		uint32_t oldSize = m_LevelCols * m_LevelRows;
		uint32_t remainSize = newSize - oldSize;
		m_LevelCols = size;
		m_LevelRows = size;
		if (newSize < oldSize)
		{
			remainSize = -remainSize;
			ClearLevelMap();
		}

		Tile defaultTile;
		defaultTile.TexCoords = { -1, -1 };
		for (uint32_t i = 0; i < remainSize; i++)
		{
			m_Map.emplace_back(defaultTile);
		}
	}

	/**
	 * This will return the currently rendering map.
	 */
	std::vector<Tile>& GetLevelMap()
	{
		return m_Map;
	}

	/**
	 * This will change all old tiles to a specific type of tile
	 * 
	 * @param oldTex: old texture to be changed
	 * @param newType: new tile type to be applied
	 */
	void ChangeAllTiles(glm::ivec2 oldTex, TileType newType)
	{
		for (Tile& tile : m_Map)
		{
			if (tile.TexCoords == oldTex)
			{
				tile.Type = newType;
			}
		}
	}

	/**
	 * This will clear the entire level map to its default tile.
	 */
	void ClearLevelMap()
	{
		Tile defaultTile;
		defaultTile.TexCoords = { -1, -1 };
		for (auto& tile : m_Map)
		{
			tile = defaultTile;
		}
	}

	void SelectCell(int id)
	{
		m_Selected[id] = true;
	}

	void DeselectAll()
	{
		m_Selected.clear();
	}

	void SetCellsize(uint32_t size)
	{
		m_CellSize = size;
	}

	void SetCellgap(float gap)
	{
		m_CellGap = gap;
	}

	float GetCellgap()
	{
		return m_CellGap;
	}

	uint32_t GetCellsize()
	{
		return m_CellSize;
	}

	std::pair<uint32_t, uint32_t> GetDims() const
	{
		return { m_LevelCols, m_LevelRows };
	}

	void SaveLevel()
	{
		zeus::SerialInfo info;
		info.SerializationType = zeus::SerialType::PROPERTIES;
		m_Serializer->StartSerialize(m_LevelPath, info);

		// Tile(posX, posY, type, index)
		const std::function<std::string(const Tile&)> ivToString = [](const Tile& tile) {
			std::stringstream ss;
			ss << "(" << tile.TexCoords.x << ", ";
			ss << tile.TexCoords.y << ", ";
			ss << (int)tile.Type << ", ";
			ss << tile.idx << ") ";
			return ss.str();
		};

		m_Serializer->Serialize("#", m_LevelPath);
		m_Serializer->Serialize("cols", (int)m_LevelCols);
		m_Serializer->Serialize("rows", (int)m_LevelRows);
		m_Serializer->Serialize("cellsize", (int)m_CellSize);
		m_Serializer->Serialize("cellgap", (double)m_CellGap);
		m_Serializer->Serialize("levelmap", m_Map, ivToString);
		m_Serializer->Flush();
	}

	void LoadLevel(const std::string& filepath)
	{
		m_LevelPath = filepath;

		zeus::SerialInfo info;
		info.SerializationType = zeus::SerialType::PROPERTIES;
		m_Serializer->StartDeserialize(filepath, info);

		m_LevelCols = m_Serializer->DeserializeInt("cols");
		m_LevelRows = m_Serializer->DeserializeInt("rows");
		m_CellSize = m_Serializer->DeserializeInt("cellsize");
		m_CellGap = m_Serializer->DeserializeDbl("cellgap");

		const std::function<Tile(std::string)> StringToTile = [](std::string s) {
			Tile tile;
			std::vector<int> elements;
			std::stringstream ss;
			ss << s;

			std::string token;
			while (ss >> token)
			{
				elements.push_back(stoi(token));
			}

			tile.TexCoords = { elements[0], elements[1] };
			tile.Type = (TileType)elements[2];
			tile.idx = elements[3];

			return tile;
		};

		std::vector<Tile>& levelmap = m_Serializer->DeserializeVec<Tile>("levelmap", StringToTile);

		// Reading map
		for (const SandboxLevel::Tile& tile : levelmap)
		{
			m_Map.push_back(tile);
		}
	}
};
