#pragma once

#include <memory>
#include <unordered_map>
#include <string>

namespace zeus
{
	struct Component
	{
	};

	class Entity
	{
	private:
		std::string m_Name{"default entity"};
		std::unordered_map<size_t, std::shared_ptr<Component>> m_Components;

	public:
		Entity();
		Entity(const std::string& name);

		template <typename T>
		const std::shared_ptr<T>& GetComponent();

		template <typename T>
		void AddComponent(const std::shared_ptr<T>& comp);
	};
}
