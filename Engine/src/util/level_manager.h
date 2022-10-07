#pragma once

#include <memory>

#include "texture_manager.h"
#include "glm/glm.hpp"

namespace zeus
{
	class Level
	{
	public:
		virtual void OnStart() = 0;
		virtual void Draw() = 0;
	};

	class LevelManager
	{
	private:
		uint32_t m_ActiveLevel{ 0 };
		std::unordered_map<uint32_t, std::shared_ptr<Level>> m_Levels;

	public:
		std::shared_ptr<Level> GetLevel(uint32_t key);
		std::pair<uint32_t, std::shared_ptr<Level>> GetActiveLevel();
		void SetActiveLevel(uint32_t lvl = 0);
		void AddLevel(uint32_t key, std::shared_ptr<Level> level);
		void RemoveLevel(uint32_t key);
		void Draw();
	};
}
