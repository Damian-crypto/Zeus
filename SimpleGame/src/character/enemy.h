#pragma once

#include "character.h"

class Enemy : public Character
{
public:
	std::string CollideObject;
	bool Collided = false;

public:
	virtual void Attack(float direction) = 0;
	virtual void OnUpdate(float dt) {};
};

class HumanEnemy : public Enemy
{
public:
	HumanEnemy();

	void Attack(float direction);
	void OnUpdate(float dt);
};
