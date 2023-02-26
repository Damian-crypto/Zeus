		std::ofstream file(m_LevelPath, std::ios_base::out);

		std::string line;

		// Writing columns count
		line = "cols: " + std::to_string(m_LevelCols);
		file << line << '\n';

		// Reading rows count
		line = "rows: " + std::to_string(m_LevelRows);
		file << line << '\n';

		// Reading cell size
		line = "cellsize: " + std::to_string(m_CellSize);
		file << line << '\n';

		// Reading cell gap
		line = "cellgap: " + std::to_string(m_CellGap);
		file << line << '\n';

		// Escape blank line
		file << '\n';

		file << "levelmap:" << '\n';

		// Saving map
		int idx = 0;
		for (uint32_t y = 0; y < m_LevelRows; y++)
		{
			for (uint32_t x = 0; x < m_LevelCols; x++)
			{
				idx = x + y * m_LevelCols;
				if (idx < (int)m_Map.size())
				{
					file << "(" << m_Map[idx].TexCoords.x << ", ";
					file << m_Map[idx].TexCoords.y << ", ";
					file << (int)m_Map[idx].Type << ", ";
					file << m_Map[idx].idx << ") ";
				}
			}
			file << '\n';
		}
		file << '\n';