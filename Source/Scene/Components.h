#pragma once
#include <DirectXMath.h>
#include <filesystem>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "../Asset/MeshAsset.h"
#include "../Renderer/ConstantBuffer.h"
#include "../Renderer/VertexBuffer.h"
#include "../Renderer/IndexBuffer.h"
#include "../Renderer/Shader.h"
#include "../Renderer/ShaderLibrary.h"
#include "../Renderer/InputLayout.h"
#include <Windows.h>

namespace Era
{
	namespace DX = DirectX;

	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(const DX::XMVECTOR& Translation, const DX::XMVECTOR& Rotation,const DX::XMVECTOR& Scale)
		{
			DX::XMStoreFloat3(&this->Translation, Translation);
			DX::XMStoreFloat4(&this->Rotation, Rotation);
			DX::XMStoreFloat3(&this->Scale, Scale);
		}

		[[nodiscard]] auto GetTransform() const -> DX::XMMATRIX
		{
			auto const RotationAsVec = DX::XMLoadFloat4(&Rotation);
			return DX::XMMatrixScaling(Scale.x, Scale.y, Scale.z) * DX::XMMatrixRotationQuaternion(RotationAsVec) *
				DX::XMMatrixTranslation(Translation.x, Translation.y, Translation.z);
		}
		DX::XMFLOAT3 Translation{};
		DX::XMFLOAT4 Rotation{};
		DX::XMFLOAT3 Scale{1.0f,1.0f,1.0f};
	};

	enum class ProjectionType
	{
		Perspective,
		Orthographic
	};
	struct CameraComponent
	{
		CameraComponent(ProjectionType projectionType, float aspectRatio)
			: m_ProjectionType(projectionType),m_AspectRatio(aspectRatio)
		{}

		[[nodiscard]] auto GetCameraPosition() const -> DX::XMVECTOR
		{
			return m_InvViewMatrix.r[3];
		}

		[[nodiscard]] auto GetViewProjection() const -> DX::XMMATRIX
		{
			return m_ViewMatrix * m_ProjectionMatrix;
		}

		[[nodiscard]] auto IsPrimary() const -> bool
		{
			return m_IsPrimary;
		}

		auto GetAspectRatio() const -> float
		{
			return m_AspectRatio;
		}

		auto GetView() const -> DX::XMMATRIX
		{
			return m_ViewMatrix;
		}

		auto GetProjection() -> DX::XMMATRIX
		{
			return m_ProjectionMatrix;
		}
		void SetPrimary(bool primary)
		{
			m_IsPrimary = primary;
		}

		void SetViewMatrix(const DX::XMMATRIX& view)
		{
			m_ViewMatrix = view;
			m_InvViewMatrix = DX::XMMatrixInverse(nullptr, m_ViewMatrix);
		}

		void SetPerspectiveHalfAngleFOV(float fov)
		{
			m_PerspectiveFOV = fov; RecalculateProjection();
		}
		void SetPerspectiveNear(float perspectiveNear)
		{
			m_PerspectiveNear = perspectiveNear; RecalculateProjection();
		}
		void SetPerspectiveFar(float perspectiveFar)
		{
			m_PerspectiveFar = perspectiveFar; RecalculateProjection();
		}
		void SetOrthoWidth(float width)
		{
			m_OrthoWidth = width; RecalculateProjection();
		}
		void SetOrthoHeight(float height)
		{
			m_OrthoHeight = height; RecalculateProjection();
		}
		void SetAspectRatio(float aspectRatio)
		{
			m_AspectRatio = aspectRatio; RecalculateProjection();
		}
		void SetProjectionType(ProjectionType type)
		{
			m_ProjectionType = type; RecalculateProjection();
		}

	private:
		bool m_IsPrimary = true;
		ProjectionType m_ProjectionType = ProjectionType::Perspective;
		float m_PerspectiveFOV = 60.0f;
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;
		float m_OrthoWidth = 16.f;
		float m_OrthoHeight = 9.f;
		float m_OrthographicNear = 0.02f, m_OrthographicFar = 20.0f;
		float m_AspectRatio = 1.0f;
		DX::XMMATRIX m_ViewMatrix = DX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		DX::XMMATRIX m_InvViewMatrix = DX::XMMatrixIdentity();
		DX::XMMATRIX m_ProjectionMatrix = DX::XMMatrixPerspectiveLH(40.f, 40.f, 0.1f, 5000.0f);
	private:
		void RecalculateProjection()
		{
			if(m_ProjectionType == ProjectionType::Perspective)
			{
				m_ProjectionMatrix = DX::XMMatrixPerspectiveFovLH(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
			}
			else
			{
				m_ProjectionMatrix = DX::XMMatrixOrthographicLH(m_OrthoWidth, m_OrthoHeight, m_OrthographicNear, m_OrthographicFar);
			}
		}
	};

	struct DirectionalLightComponent
	{
		DX::XMFLOAT3 LightDirection{ 0.0f,0.0f,1.0f };
		DX::XMFLOAT4 AmbientLightColor{ 1.0f,1.0f,1.0f,1.0f }; 
		DX::XMFLOAT4 DiffuseLightColor{ 1.0f,1.0f,1.0f ,1.0f};;
		DX::XMFLOAT4 SpecularLightColor{ 1.0f,1.0f,1.0f ,1.0f};
		float Intensity = 10.0f;
	};
}
