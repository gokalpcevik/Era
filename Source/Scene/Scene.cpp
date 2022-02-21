#include "Scene.h"
#include "Entity.h"

namespace Era
{
	Scene::Scene(const std::shared_ptr<Renderer>& pRenderer)
		: m_Renderer(pRenderer)
	{

	}

	Entity Scene::CreateEntity()
	{
		return {this,m_Registry.create()};
	}

	void Scene::Destroy(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::Update()
	{

	}

	void Scene::Draw()
	{
		auto cameraView = m_Registry.view<CameraComponent>();
		
	}

	void Scene::OnResize(uint32_t width, uint32_t height)
	{
		auto const cameraView = m_Registry.view<CameraComponent>();
		for(auto const entity : cameraView)
		{
			auto&& cc = m_Registry.get<CameraComponent>(entity);
			cc.SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
		}
		auto const meshView = m_Registry.view<MeshRendererComponent,TransformComponent>();

		for(auto const entity : meshView)
		{
			auto&& [mrc,tc] = m_Registry.get<MeshRendererComponent, TransformComponent>(entity);
			m_Renderer->DrawMesh(tc.GetTransform(), mrc);
		}
	}
}
