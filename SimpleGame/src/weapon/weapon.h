#pragma once

#include "zeus.h"
#include "glm/glm.hpp"
#include "phyzicsall.h"

#include <vector>
#include <cstring>

class Weapon
{
protected:
	glm::vec3 m_Position{ 0.0f };

	std::shared_ptr<zeus::TextureManager> m_TexManager = nullptr;
	std::shared_ptr<zeus::Phyzics> m_Phyzics = nullptr;

public:
	virtual void Attack(float direction) = 0;
	virtual void OnUpdate(float dt) {}
	virtual void OnRender() {}

	void SetPosition(const glm::vec3& position) { m_Position = position; }
	void SetPhyzicsEngine(std::shared_ptr<zeus::Phyzics> phyzics) { this->m_Phyzics = phyzics; }
};

class Gun : public Weapon
{
protected:
	struct Bullet
	{
		float Direction 	{ 0.0f };
		float MaxDistance 	{ 100.0f };
		glm::vec3 Position 	{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Origin 	{ 0.0f, 0.0f, 0.0f };
		std::shared_ptr<zeus::PhyzicalBody> PhysicalBody = nullptr;
	};

	std::vector<Bullet> m_Bullets;
	std::function<void(const std::shared_ptr<zeus::PhyzicalBody>, zeus::PhyzicalBody*)> CollisionFn = [&](const std::shared_ptr<zeus::PhyzicalBody> body, zeus::PhyzicalBody* me) {
		const auto& pos = body->Position;
		if ((pos.x < m_Position.x - 100 || pos.x > m_Position.x + 100))
		{
			const char* data = (const char*)body->InternalData;
			if (strcmp(data, "enemy") == 0)
			{
				body->IsDead = true;
			}
		}
			// QUICK_LOG(Trace, "Fired at: %f %f", pos.x, pos.y);
	};

public:
	Gun();
	Gun(std::shared_ptr<zeus::TextureManager> texManager, const glm::vec3& position);

	void Attack(float direction) override;
	void OnUpdate(float dt) override;
	void OnRender() override;
};

class Sword : public Weapon
{
public:
	void Attack(float direction) override;
};

class Axe : public Weapon
{
public:
	void Attack(float direction) override;
};
