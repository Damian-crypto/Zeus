#include "body.h"

#include <iostream>
#include <stdexcept>

namespace zeus
{
	void PhyzicalBody::OnCollision(std::shared_ptr<PhyzicalBody> body)
	{
		if (body == nullptr)
		{
			throw std::runtime_error("Runtime Error: colliding with null body");
		}

		this->Collision.CollisionBody = body;
	}

	bool PhyzicalBody::IsCollide(std::shared_ptr<PhyzicalBody> shape)
	{
		switch (shape->Type)
		{
			case BodyShape::Quad:
			{
				std::vector<glm::vec2> corners = {
					{ Position.x - HalfWidth, Position.y - HalfHeight },
					{ Position.x + HalfWidth, Position.y - HalfHeight },
					{ Position.x + HalfWidth, Position.y + HalfHeight },
					{ Position.x - HalfWidth, Position.y + HalfHeight }
				};

				//                   | |
				// shape.x - half <--|*|--> shape.x + half
				//                   | |
				//                   |*| pos
				//                   | |
				//                   | |
				//                   | |
				// Below conditions check whether "this body" is in the range of
				// "shape's" body (above illustration only for x-axis; same for y-axis)
				
				for (const glm::vec2& point : corners)
				{
					bool xCollision = shape->Position.x + shape->HalfWidth >= point.x && shape->Position.x - shape->HalfWidth <= point.x;
					bool yCollision = shape->Position.y + shape->HalfHeight >= point.y && shape->Position.y - shape->HalfHeight <= point.y;
				
					if (xCollision && yCollision)
					{
						return true;
					}
				}
				break;
			}
		}

		return false;
	}

	void PhyzicalBody::SetWidth(float width)
	{
		this->Width = width;
		this->HalfWidth = width / 2.0f;
	}

	void PhyzicalBody::SetHeight(float height)
	{
		this->Height = height;
		this->HalfHeight = height / 2.0f;
	}
}
