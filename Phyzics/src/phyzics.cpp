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
				const auto& body1 = m_Bodies[i];

				body1->Step(dt);

				if (body1 == nullptr || body1->IsDead)
				{
					const char* data = (const char*)body1->InternalData;
					m_Bodies.erase(m_Bodies.begin() + i);
					continue;
				}

				for (const auto& body2 : m_Bodies)
				{
					if (body1 != body2 && body1->IsCollide(body2))
					{
						body1->OnCollision(body2);
						body2->OnCollision(body1);
					}
				}
			}
		}

		s_Stat.Bodies = m_Bodies.size();
		return;
	}

	const PhyzicsStat& Phyzics::GetStatistics()
	{
		return s_Stat;
	}
}
