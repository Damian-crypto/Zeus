#pragma once

#include "character.h"

class Enemy : public Character
{
protected:
	std::shared_ptr<Character> m_Target = nullptr;

public:
	virtual void Attack(float direction) = 0;
	virtual void OnUpdate(float dt) {};

	void SetTarget(const std::shared_ptr<Character> target);
};

class HumanEnemy : public Enemy
{
public:
	HumanEnemy();

	void Attack(float direction);
	void OnUpdate(float dt);
};
