#include "corepch.h"
#include "level_manager.h"

namespace zeus
{
	std::shared_ptr<Level> LevelManager::GetLevel(uint32_t key)
	{
		return m_Levels[key];
	}

	std::shared_ptr<Level> LevelManager::GetActiveLevel()
	{
		return m_Levels[m_ActiveLevel];
	}

	void LevelManager::SetActiveLevel(uint32_t lvl)
	{
		m_ActiveLevel = lvl;
	}

	void LevelManager::AddLevel(uint32_t key, std::shared_ptr<Level> level)
	{
		m_Levels[key] = level;
		m_Levels[key]->m_LevelID = key;
		m_Levels[key]->OnStart();
	}

	void LevelManager::RemoveLevel(uint32_t key)
	{
		if (m_Levels.find(key) == m_Levels.end())
		{
			throw std::runtime_error("Runtime Error: Trying to remove invalid level!");
		}

		m_Levels.erase(key);
	}

	void LevelManager::Draw()
	{
		m_Levels[m_ActiveLevel]->Draw();
	}

	LevelResource& Level::GetLevelResources()
	{
		return m_LevelResources;
	}

	uint32_t Level::GetLevelID()
	{
		return m_LevelID;
	}
}
