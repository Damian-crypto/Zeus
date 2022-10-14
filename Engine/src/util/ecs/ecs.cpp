#include "corepch.h"
#include "ecs.h"

#include "component.h"

namespace zeus
{
	Entity::Entity()
	{
	}

	Entity::Entity(const std::string& name)
		: m_Name(name)
	{
	}

	template <typename T>
	const std::shared_ptr<T>& Entity::GetComponent()
	{
		//static_assert(false, "Compiletime Error: Trying get invalid component!");
	}

	template<typename T>
	void Entity::AddComponent(const std::shared_ptr<T>& comp)
	{
		//static_assert(false, "Compiletime Error: Trying add invalid component!");
	}

	template <>
	const std::shared_ptr<SpriteComponent>& Entity::GetComponent()
	{
		return (std::shared_ptr<SpriteComponent>&)m_Components[typeid(SpriteComponent).hash_code()];
	}

	template<>
	void Entity::AddComponent(const std::shared_ptr<SpriteComponent>& comp)
	{
		m_Components[typeid(SpriteComponent).hash_code()] = comp;
	}

	template <>
	const std::shared_ptr<TransformComponent>& Entity::GetComponent()
	{
		return (std::shared_ptr<TransformComponent>&)m_Components[typeid(TransformComponent).hash_code()];
	}

	template<>
	void Entity::AddComponent(const std::shared_ptr<TransformComponent>& comp)
	{
		m_Components[typeid(TransformComponent).hash_code()] = comp;
	}
}
