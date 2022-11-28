#include "phyzics.h"

#include <iostream>

namespace zeus
{
	PhyzicsStat Phyzics::s_Stat = {};

	void Phyzics::AddBody(std::shared_ptr<PhyzicalBody> body)
	{
		m_Bodies.push_back(body);
	}

	// Note:
	// Below collision detection is very badly designed.
	// TODO: This will be well designed in future
	// int i = 0;
	void Phyzics::Step(float dt)
	{
		if (!m_Bodies.empty())
		{
			for (size_t i = 0; i < m_Bodies.size(); i++)
			{
				const auto& body = m_Bodies[i];

				body->Step(dt);

				if (body == nullptr || body->IsDead)
				{
					m_Bodies.erase(m_Bodies.begin() + i);
				}

				for (const auto& body2 : m_Bodies)
				{
					if (body != body2 && body->IsCollide(body2))
					{
						body->OnCollision(body2);
						body2->OnCollision(body);
					}
				}
			}
		}

		// i++;
		// if (i >= (int)m_Bodies.size())
		// {
		// 	i = 0;
		// }
		s_Stat.Bodies = m_Bodies.size();
	}

	const PhyzicsStat& Phyzics::GetStatistics()
	{
		return s_Stat;
	}
}
