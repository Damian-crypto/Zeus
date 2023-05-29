#pragma once

#include "glm/glm.hpp"

#include <functional>
#include <memory>

namespace zeus
{
	enum class BodyShape
	{
		None,
		Circle,
		Quad
	};

	struct PhyzicalBody;
	struct PhyzicalCollision
	{
		std::shared_ptr<PhyzicalBody> CollisionBody;
	};

	struct PhyzicalBody
	{
	private:
		float HalfWidth;
		float HalfHeight;
		float Width;
		float Height;

	public:
		glm::vec3 Position;
		glm::vec3 Velocity;
		float MovementSpeed;
		float Radius;
		BodyShape Type = BodyShape::None;
		PhyzicalCollision Collision;
		bool IsDead = false;
		void* InternalData = nullptr;

		void Step(float dt) {};
		bool IsCollide(std::shared_ptr<PhyzicalBody> shape);
		void OnCollision(std::shared_ptr<PhyzicalBody> body);

		void SetWidth(float width);
		void SetHeight(float height);
	};
}
