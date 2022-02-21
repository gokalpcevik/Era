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
		T& GetComponent();

		template<typename T,typename ...Args>
		T& AddComponent(Args&&... args);

		template<typename T>
		void RemoveComponent();

		operator entt::entity() const
		{
			return m_EntityHandle;
		}
	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene{ nullptr };
	};
	template<typename T>
	T& Entity::GetComponent()
	{
		return m_Scene->GetComponent<T>();
	}

	template <typename T, typename ... Args>
	T& Entity::AddComponent(Args&&... args)
	{
		return m_Scene->AddComponent<T>(m_EntityHandle,std::forward<Args>(args)...);
	}

	template <typename T>
	void Entity::RemoveComponent()
	{
		m_Scene->RemoveComponent<T>(m_EntityHandle);
	}
}
