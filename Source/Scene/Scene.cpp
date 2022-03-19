#include "Scene.h"
#include "Entity.h"

namespace Era
{
	Scene::Scene(const std::shared_ptr<Renderer>& pRenderer)
		: m_Renderer(pRenderer)
	{
		pRenderer->GetWindow()->Subscribe(this);
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
		DirectionalLightComponent* pDirectionalLight = nullptr;

		for(auto const entity : cameraView)
		{
			auto&& cc = m_Registry.get<CameraComponent>(entity);
			if(cc.IsPrimary())
				pCamera = &cc;
		}

		auto dlcView = m_Registry.view<DirectionalLightComponent>();
		for(auto const entity : dlcView)
		{
			pDirectionalLight = &m_Registry.get<DirectionalLightComponent>(entity);
			break;
		}
		
		auto const meshView = m_Registry.view<MeshRendererComponent, TransformComponent>();

		auto* pContext = m_Renderer->GetGraphicsDevice()->GetD3D11DeviceContext().Get();
		for (auto const entity : meshView)
		{
			auto&& [mrc, tc] = m_Registry.get<MeshRendererComponent, TransformComponent>(entity);
			if(pCamera)
				mrc.SetWorldViewProjectionMatrices(pContext,
			                           DX::XMMatrixTranspose(tc.GetTransform() * pCamera->GetViewProjection()));
			mrc.SetWorldMatrix(pContext,DX::XMMatrixTranspose(tc.GetTransform()));

			if(pDirectionalLight)
			{
				PSDefaultCBufferData data{};
				DX::XMStoreFloat4(&data.CameraPosition, pCamera->GetCameraPosition());
				data.LightDirection = pDirectionalLight->LightDirection;
				data.AmbientLightColor = pDirectionalLight->AmbientLightColor;
				data.DiffuseLightColor = pDirectionalLight->DiffuseLightColor;
				data.SpecularLightColor = pDirectionalLight->SpecularLightColor;
				data.Shininess = pDirectionalLight->Shininess;
				mrc.GetMaterial()->UpdateLightData(pContext, data);
			}
			m_Renderer->DrawMesh(mrc);
		}
	}

	void Scene::OnWindowResized(SDL_Window* resizedWindow, uint32_t width, uint32_t height)
	{
		auto const cameraView = m_Registry.view<CameraComponent>();
		for (auto const entity : cameraView)
		{
			auto&& cc = m_Registry.get<CameraComponent>(entity);
			cc.SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
		}
	}
}
