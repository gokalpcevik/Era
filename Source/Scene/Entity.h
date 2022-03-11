#pragma once
#include <entt/entt.hpp>
#include "Scene.h"

namespace Era
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(Scene* pScene,entt::entity entity);

		template<typename T>
		auto GetComponent() -> T&;

		template<typename T,typename ...Args>
		auto AddComponent(Args&&... args) -> T&;

		template<typename T>
		void RemoveComponent();

		template<typename T>
		void RemoveComponentIfExists() const;

		operator entt::entity() const
		{
			return m_EntityHandle;
		}
	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene{ nullptr };
	};
	template<typename T>
	auto Entity::GetComponent() -> T&
	{
		return m_Scene->GetComponent<T>(m_EntityHandle);
	}

	template <typename T, typename ... Args>
	auto Entity::AddComponent(Args&&... args) -> T&
	{
		return m_Scene->AddComponent<T>(m_EntityHandle,std::forward<Args>(args)...);
	}

	template <typename T>
	void Entity::RemoveComponent()
	{
		m_Scene->RemoveComponent<T>(m_EntityHandle);
	}

	template <typename T>
	void Entity::RemoveComponentIfExists() const
	{
		m_Scene->RemoveComponentIfExists<T>(m_EntityHandle);
	}
}
