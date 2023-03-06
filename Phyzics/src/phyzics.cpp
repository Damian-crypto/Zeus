#include "phyzics.h"

#include <iostream>
#include <cstring>

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
					const char* data = (const char*)body->InternalData;
					// std::cout << data << " is dead\n";
					m_Bodies.erase(m_Bodies.begin() + i);
					continue;
				}

				for (const auto& body2 : m_Bodies)
				{
					if (body != body2 && body->IsCollide(body2))
					{
						// std::cout << ((const char*)body->InternalData) << " is collided with " << ((const char*)body2->InternalData) << '\n';
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
		return;
	}

	const PhyzicsStat& Phyzics::GetStatistics()
	{
		return s_Stat;
	}
}
