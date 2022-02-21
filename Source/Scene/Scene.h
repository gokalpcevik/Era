#pragma once
#include <entt/entt.hpp>
#include "../Renderer/Renderer.h"

namespace Era
{
	class Entity;

	class Scene
	{
	public:
		friend class Application;

		explicit Scene(const std::shared_ptr<Renderer>& pRenderer);

		Entity CreateEntity();
		void Destroy(Entity entity);

		void Update();
		void Draw();

		template<typename T>
		T& GetComponent(entt::entity entity);

		template<typename T, typename ...Args>
		T& AddComponent(entt::entity entity,Args&&... args);

		template<typename T>
		void RemoveComponent(entt::entity entity);
	private:
		void OnResize(uint32_t width, uint32_t height);
	private:
		entt::registry m_Registry{};
		std::shared_ptr<Renderer> m_Renderer;
	};
	template<typename T>
	inline T& Scene::GetComponent(entt::entity entity)
	{
		return m_Registry.get<T>(entity);
	}
	template<typename T, typename ...Args>
	inline T& Scene::AddComponent(entt::entity entity, Args && ...args)
	{
		return m_Registry.emplace<T>(entity,std::forward<Args>(args)...);
	}

	template <typename T>
	void Scene::RemoveComponent(entt::entity entity)
	{
		m_Registry.remove<T>(entity);
	}
}
