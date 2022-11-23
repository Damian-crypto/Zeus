#pragma once

#include "event/event.h"

namespace zeus
{
	class Layer
	{
	public:
		Layer() = default;
		Layer(const char* name);
		virtual ~Layer() {}

		virtual void OnUpdate(float dt) = 0;
		virtual void OnRender() = 0;
		virtual void OnEvent(Event& e);
	};
}
