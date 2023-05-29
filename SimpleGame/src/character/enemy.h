#pragma once

#include "character.h"

class Enemy : public Character
{
protected:
	std::shared_ptr<Character> m_Target = nullptr;

public:
	virtual void Attack(float direction, float offset = 1.0f) = 0;
	virtual void OnUpdate(float dt) {};

	void SetTarget(const std::shared_ptr<Character> target);
};

class HumanEnemy : public Enemy
{
public:
	HumanEnemy();

	void Attack(float direction, float offset) override;
	void OnUpdate(float dt) override;
};
