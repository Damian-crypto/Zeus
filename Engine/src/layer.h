#pragma once

#include "core.h"
#include "event/event.h"

namespace zeus
{
	struct Component
	{
	};

	class Entity
	{
	private:
		std::vector<Component*> m_Components;

	public:
		Entity(const char* name);

		void OnUpdate(float dt);
		void OnRender();
		void OnEvent(Event& e);
	};

	class Layer
	{
	private:
		std::vector<Entity*> m_Entities;

	public:
		Layer() = default;
		Layer(const char* name);
		virtual ~Layer() {}

		virtual void OnUpdate(float dt) = 0;
		virtual void OnRender() = 0;
		virtual void OnEvent(Event& e);
	};
}
