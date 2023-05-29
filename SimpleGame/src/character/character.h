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
protected:
	glm::vec2 m_Size{ 0.0f };
	glm::vec3 m_Position{ 0.0f };
	glm::vec3 m_Velocity{ 0.0f };
	glm::vec3 m_LastPosition { 0.0f };
	glm::vec3 m_SpriteCoords{ 0.0f };

	std::shared_ptr<Weapon> m_Weapon = nullptr;
	std::shared_ptr<zeus::TextureManager> m_TexManager = nullptr;

	Sprite m_Sprite;

private:
	std::shared_ptr<zeus::Phyzics> m_Phyzics = nullptr;
	std::shared_ptr<zeus::PhyzicalBody> m_PhysicalBody = nullptr;

public:
	Character();
	/* This move function is not taking the actual position on the scene.
	 * But it adds the value you entered to the existing character location.
	 * e.g. player.xCoord += -1
	 * In the above example player moving towards left not it's position is -1
	 * @param velocity The value which will be added to its current position in each update
	 */
	void Move(glm::vec3 velocity = glm::vec3(0.0f));
	virtual void Attack(float direction) = 0;
	void OnUpdate(float dt);
	void OnRender();

	void SetVelocity(const glm::vec3& velocity) { m_Velocity = velocity; }
	void SetWeapon(WeaponType type);
	void SetSprite(const Sprite& sprite);
	void SetSize(const glm::vec2& size) { m_Size = size; }
	void SetPosition(const glm::vec3 pos) { m_Position = pos; m_PhysicalBody->Position = pos; }
	void SetTextureManager(std::shared_ptr<zeus::TextureManager> texManager) { m_TexManager = texManager; }
	void SetPhyzicsEngine(std::shared_ptr<zeus::Phyzics> phyzics);
	void SetPhyzicalBody(std::shared_ptr<zeus::PhyzicalBody> body) { m_PhysicalBody = body; }

	Sprite& GetSprite() { return m_Sprite; }
	glm::vec3& GetPosition() { return m_Position; }
	glm::vec3& GetVelocity() { return m_Velocity; }
	glm::vec2 GetSize() { return m_Size; }
	std::shared_ptr<zeus::Phyzics> GetPhyzicsEngine() const { return m_Phyzics; }
	std::shared_ptr<zeus::PhyzicalBody> GetPhyzicalBody() const { return m_PhysicalBody; }
};
