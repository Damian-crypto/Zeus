#include "body.h"

namespace zeus
{
	void PhyzicalBody::OnCollision(std::shared_ptr<PhyzicalBody> body)
	{
		if (CollideFunction != nullptr)
			CollideFunction(body);
	}

	bool PhyzicalBody::IsCollide(std::shared_ptr<PhyzicalBody> shape)
	{
		switch (shape->Type)
		{
			case BodyShape::Quad:
			{
				//                   | |
				// shape.x - half <--|*|--> shape.x + half
				//                   | |
				//                   |*| pos
				//                   | |
				//                   | |
				//                   | |
				// Below conditions check whether "this body" is in the range of
				// "shape's" body (above illustration only for x-axis; same for y-axis)

				bool xCollision = shape->Position.x + shape->HalfWidth >= Position.x
					&& shape->Position.x - shape->HalfWidth <= Position.x;
				bool yCollision = shape->Position.y + shape->HalfHeight >= Position.y// - 10
					&& shape->Position.y - shape->HalfHeight <= Position.y ;//+ 10;

				return xCollision && yCollision;
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
