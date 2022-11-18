#include "body.h"

namespace zeus
{
	void QuadShape::Step(float dt)
	{
	}

	bool QuadShape::IsCollide(BodyType type, BodyShape shape)
	{
		//switch (type)
		//{
		//	case BodyType::Quad:
		//	{
		//		QuadShape tmp = *(QuadShape*)&shape;
		//		bool x = Position.x + Width / 2 > tmp.Position.x && Position.x - Width / 2 < tmp.Position.x;
		//		bool y = Position.y + Height / 2 > tmp.Position.y && Position.y - Height / 2 < tmp.Position.y;
		//		
		//		return x && y;
		//	}
		//}

		return shape.Position.x == Position.x && shape.Position.y == Position.y;
	}
}
