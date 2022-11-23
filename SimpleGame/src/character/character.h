#pragma once

#include "sprite.h"
#include "weapon/weapon.h"
#include "zeus.h"
#include "phyzicsall.h"
#include "glm/glm.hpp"

#include <memory>

enum class WeaponType
{
	Gun,
	Axe,
	Sword
};

class Character
{
public:
	bool IsDead = false;

protected:
	glm::vec3 m_Position{ 0.0f };
	glm::vec3 m_SpriteCoords{ 0.0f };

	std::shared_ptr<Weapon> m_Weapon = nullptr;
	std::shared_ptr<zeus::TextureManager> m_TexManager = nullptr;

	Sprite m_Sprite;

private:
	bool m_Moving = false;
	std::shared_ptr<zeus::Phyzics> m_Phyzics = nullptr;
	std::shared_ptr<zeus::PhyzicalBody> m_PhysicalBody = nullptr;

public:
	Character();
	// This move function is not taking the actual position on the scene.
	// But it adds the value you entered to the existing character location.
	// e.g. player.xCoord += -1
	// In the above example player moving towards left not it's position is -1
	void Move(float x, float y, float z = 0.0f, bool lock = false);
	virtual void Attack(float direction) = 0;
	void OnUpdate(float dt);
	void OnRender();

	void SetWeapon(WeaponType type);
	void SetPosition(const glm::vec3 pos) { m_Position = pos; }
	void SetSprite(const Sprite& sprite) { m_Sprite = sprite; m_SpriteCoords = sprite.Coords; }
	void SetTextureManager(std::shared_ptr<zeus::TextureManager> texManager) { m_TexManager = texManager; }
	void SetPhyzicsEngine(std::shared_ptr<zeus::Phyzics> phyzics);
	void SetPhyzicalBody(std::shared_ptr<zeus::PhyzicalBody> body) { m_PhysicalBody = body; }

	Sprite& GetSprite() { return m_Sprite; }
	glm::vec3& GetPosition() { return m_Position; }
	std::shared_ptr<zeus::PhyzicalBody> GetPhyzicalBody() { return m_PhysicalBody; }
};
