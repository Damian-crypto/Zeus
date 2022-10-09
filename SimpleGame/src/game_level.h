#pragma once

#include "zeus.h"

struct vec2i
{
	int x, y;
};

class BeginLevel : public zeus::Level
{
private:
	std::vector<std::vector<vec2i>> m_Map;

public:
	void OnStart() override;
	void Draw() override;

	void LoadLevel(const std::string& filepath);
};
