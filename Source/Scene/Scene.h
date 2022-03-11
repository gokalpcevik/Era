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

		auto CreateEntity() -> Entity;
		auto Destroy(Entity entity) -> void;
		auto Update() -> void;
		auto Draw() -> void;

		template<typename T>
		auto GetComponent(entt::entity entity) -> T&;

		template<typename T, typename ...Args>
		auto AddComponent(entt::entity entity,Args&&... args) -> T&;

		template<typename T>
		auto RemoveComponent(entt::entity entity) -> void;

		template<typename T>
		auto RemoveComponentIfExists(entt::entity entity) -> void;

	private:
		auto OnResize(uint32_t width, uint32_t height) -> void;
	private:
		entt::registry m_Registry{};
		std::shared_ptr<Renderer> m_Renderer;
	};
	template<typename T>
	inline auto Scene::GetComponent(entt::entity entity) -> T&
	{
		return m_Registry.get<T>(entity);
	}
	template<typename T, typename ...Args>
	inline auto Scene::AddComponent(entt::entity entity, Args && ...args) -> T&
	{
		return m_Registry.emplace<T>(entity,std::forward<Args>(args)...);
	}

	template <typename T>
	auto Scene::RemoveComponent(entt::entity entity) -> void
	{
		m_Registry.remove<T>(entity);
	}

	template <typename T>
	auto Scene::RemoveComponentIfExists(entt::entity entity) -> void
	{
		m_Registry.remove_if_exists<T>(entity);
	}
}
