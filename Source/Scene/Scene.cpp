#include "Scene.h"
#include "Entity.h"

namespace Era
{
	Scene::Scene(const std::shared_ptr<Renderer>& pRenderer)
		: m_Renderer(pRenderer)
	{

	}

	auto Scene::CreateEntity() -> Entity
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

		CameraComponent* pCamera = nullptr;

		for(auto const entity : cameraView)
		{
			auto&& cc = m_Registry.get<CameraComponent>(entity);
			if(cc.IsPrimary())
				pCamera = &cc;
		}

		auto const meshView = m_Registry.view<MeshRendererComponent, TransformComponent>();

		for (auto const entity : meshView)
		{
			auto&& [mrc, tc] = m_Registry.get<MeshRendererComponent, TransformComponent>(entity);
			if(pCamera)
				mrc.SetWorldViewProjection(m_Renderer->GetGraphicsDevice()->GetD3D11DeviceContext().Get(),
			                           DX::XMMatrixTranspose(tc.GetTransform() * pCamera->GetViewProjection()));
			m_Renderer->DrawMesh(mrc);
		}
	}

	void Scene::OnResize(uint32_t width, uint32_t height)
	{
		auto const cameraView = m_Registry.view<CameraComponent>();
		for(auto const entity : cameraView)
		{
			auto&& cc = m_Registry.get<CameraComponent>(entity);
			cc.SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
		}
		
	}
}
