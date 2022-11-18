#pragma once

#include "character.h"

class Player : public Character
{
public:
	Player();

	void Attack(float direction);
};
