#include "body.h"

namespace zeus
{
	void CircleShape::Step(float dt)
	{
	}

	bool CircleShape::IsCollide(BodyType type, BodyShape shape)
	{
		//switch (type)
		//{
		//	case BodyType::Quad:
		//	{
		//		CircleShape tmp = *(CircleShape*)&shape;

		//		break;
		//	}
		//}

		return shape.Position.x == Position.x && shape.Position.y == Position.y;
	}
}
