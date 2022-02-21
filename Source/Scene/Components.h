#pragma once
#include <DirectXMath.h>
#include <filesystem>
#include "../Renderer/Shader.h"

namespace Era
{
	namespace DX = DirectX;

	struct TransformComponent
	{
		[[nodiscard]] DX::XMMATRIX GetTransform() const
		{
			auto const RotationAsVec = DX::XMLoadFloat4(&Rotation);
			return DX::XMMatrixTranslation(Translation.x, Translation.y, Translation.z)
			* DX::XMMatrixRotationQuaternion(RotationAsVec)
			* DX::XMMatrixScaling(Scale.x,Scale.y,Scale.z);
		}
		DX::XMFLOAT3 Translation{};
		DX::XMFLOAT4 Rotation{};
		DX::XMFLOAT3 Scale{};
	};

	enum class ProjectionType
	{
		Perspective,
		Orthographic
	};
	struct CameraComponent
	{
		CameraComponent(const DX::XMMATRIX& viewMatrix, const DX::XMMATRIX& projection,ProjectionType projectionType,
			float aspectRatio)
			: m_ProjectionType(projectionType),m_AspectRatio(aspectRatio),m_ViewMatrix(viewMatrix),m_ProjectionMatrix(projection)
		{}

		[[nodiscard]]DX::XMMATRIX GetViewProjection() const
		{
			return m_ProjectionMatrix * m_ViewMatrix;
		}

		void SetPerspectiveFOV(float fov)
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
		ProjectionType m_ProjectionType = ProjectionType::Perspective;
		float m_PerspectiveFOV = 45.0f;
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;
		float m_OrthoWidth = 1600.0f;
		float m_OrthoHeight = 900.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
		float m_AspectRatio = 0.0f;
		DX::XMMATRIX m_ViewMatrix = DX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
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

	struct MeshRendererComponent
	{
		MeshRendererComponent(ID3D11Device* pDevice,const std::filesystem::path& meshPath)
		{
			InitializePipelineObjects(pDevice,meshPath);
		}

		struct Vertex
		{
			DX::XMFLOAT3 Position;
		};

		[[nodiscard]] const VertexBufferRef<Vertex>& GetVertexBuffer() const { return m_VertexBuffer; }
		[[nodiscard]] const IndexBufferRef& GetIndexBuffer() const { return m_IndexBuffer; }
		[[nodiscard]] const VertexShaderRef& GetVertexShader() const { return m_VertexShader; }
		[[nodiscard]] const PixelShaderRef& GetPixelShader() const { return m_PixelShader; }
		[[nodiscard]] const InputLayoutRef& GetInputLayout() const { return m_InputLayout; }

		static void SetPrimitiveTopology(ID3D11DeviceContext* pContext)
		{
			pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
	private:
		void InitializePipelineObjects(ID3D11Device* pDevice,const std::filesystem::path& meshPath)
		{
			m_VertexShader = std::make_shared<VertexShader>(pDevice, "Shaders\\Unlit.vshader");
			m_PixelShader = std::make_shared<PixelShader>(pDevice, "Shaders\\Unlit.pshader");
			constexpr D3D11_INPUT_ELEMENT_DESC elems[] =
			{
				{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0}
			};
			m_InputLayout = std::make_shared<InputLayout>(pDevice, elems, 1,
				m_VertexShader->GetBlob()->GetBufferPointer()
				, m_VertexShader->GetBlob()->GetBufferSize());
		}

	private:
		VertexShaderRef m_VertexShader;
		PixelShaderRef m_PixelShader;
		InputLayoutRef m_InputLayout;
		VertexBufferRef<Vertex> m_VertexBuffer;
		IndexBufferRef m_IndexBuffer;
	};
}
