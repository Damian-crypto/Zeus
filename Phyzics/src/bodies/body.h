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
		std::function<void(const std::shared_ptr<PhyzicalBody> body, PhyzicalBody* me)> CollideFunction;
		bool IsDead = false;
		void* InternalData = nullptr;

		void Step(float dt) {};
		bool IsCollide(std::shared_ptr<PhyzicalBody> shape);
		void OnCollision(std::shared_ptr<PhyzicalBody> body);

		void SetWidth(float width);
		void SetHeight(float height);
	};
}
