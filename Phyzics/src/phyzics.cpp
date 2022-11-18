#include "phyzics.h"

#include <iostream>

namespace zeus
{
	PhyzicsStat Phyzics::s_Stat = {};

	void Phyzics::AddStaticBody(std::shared_ptr<PhyzicalBody> body)
	{
		m_StaticBodies.push_back(body);
	}

	void Phyzics::AddDynamicBody(std::shared_ptr<PhyzicalBody> body)
	{
		m_DynamicBodies.push_back(body);
	}

	// Note:
	// Below collision detection is very badly designed.
	// TODO: This will be well designed in future
	int i = 0;
	void Phyzics::Step(float dt)
	{
		int staticBodiesCount = (int)m_StaticBodies.size();
		int dynamicBodiesCount = (int)m_DynamicBodies.size();

		if (staticBodiesCount > 0)
		{
			int item = i % (int)m_StaticBodies.size();
			const auto& body = m_StaticBodies[item];

			body->Step(dt);

			for (const auto& body2 : m_StaticBodies)
			{
				if (body != body2 && body->IsCollide(body2))
				{
					body->OnCollision(body2);
				}
			}
		}

		if (dynamicBodiesCount > 0)
		{
			int item = i % (int)m_DynamicBodies.size();
			const auto& body = m_DynamicBodies[item];

			body->Step(dt);

			if (body == nullptr || body->IsDead)
			{
				m_DynamicBodies.erase(m_DynamicBodies.begin() + item);
			}

			for (const auto& body2 : m_DynamicBodies)
			{
				if (body != body2 && body->IsCollide(body2))
				{
					body->OnCollision(body2);
				}
			}
		}

		s_Stat.DynamicBodies = m_DynamicBodies.size();
		s_Stat.StaticBodies = m_StaticBodies.size();

		i = (i + 1) % (staticBodiesCount > dynamicBodiesCount ? staticBodiesCount : dynamicBodiesCount);
	}

	const PhyzicsStat& Phyzics::GetStatistics()
	{
		return s_Stat;
	}
}
