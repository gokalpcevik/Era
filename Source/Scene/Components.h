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
#include "../Renderer/InputLayout.h"
#include <Windows.h>

namespace Era
{
	namespace DX = DirectX;

	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(DX::XMFLOAT3 Translation, DX::XMFLOAT4 Rotation, DX::XMFLOAT3 Scale)
			: Translation(Translation), Rotation(Rotation) , Scale(Scale) {}

		[[nodiscard]] auto GetTransform() const -> DX::XMMATRIX
		{
			auto const RotationAsVec = DX::XMLoadFloat4(&Rotation);
			return DX::XMMatrixScaling(Scale.x, Scale.y, Scale.z) * DX::XMMatrixRotationQuaternion(RotationAsVec) *
				DX::XMMatrixTranslation(Translation.x, Translation.y, Translation.z);
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
		CameraComponent(ProjectionType projectionType, float aspectRatio)
			: m_ProjectionType(projectionType),m_AspectRatio(aspectRatio)
		{}

		[[nodiscard]] auto GetViewProjection() const -> DX::XMMATRIX
		{
			return m_ViewMatrix * m_ProjectionMatrix;
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

		auto IsPrimary() const -> bool
		{
			return m_IsPrimary;
		}

	private:
		bool m_IsPrimary = true;
		ProjectionType m_ProjectionType = ProjectionType::Perspective;
		float m_PerspectiveFOV = 60.0f;
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;
		float m_OrthoWidth = 1600.0f;
		float m_OrthoHeight = 900.0f;
		float m_OrthographicNear = 0.02f, m_OrthographicFar = 1.0f;
		float m_AspectRatio = 1.0f;
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
		MeshRendererComponent(ID3D11Device* pDevice,const MeshAsset& meshAsset)
		{
			InitializePipelineObjects(pDevice,meshAsset);
		}

		struct Vertex
		{
			Vertex(DX::XMFLOAT3 Position,DX::XMFLOAT3 Normal) : Position(Position) , Normal(Normal) {}

			DX::XMFLOAT3 Position{};
			DX::XMFLOAT3 Normal{};
		};

		struct VSConstantBufferData
		{
			DX::XMMATRIX WorldViewProjection{};
			DX::XMFLOAT4 Color{1.0f,1.0f,1.0f,1.0f};
		};

		[[nodiscard]] const VertexBufferRef<Vertex>& GetVertexBuffer() const { return m_VertexBuffer; }
		[[nodiscard]] const ConstantBufferRef<VSConstantBufferData>& GetConstantBuffer() const { return m_VSConstantBuffer; }
		[[nodiscard]] auto GetIndexBuffer() const -> const IndexBufferRef& { return m_IndexBuffer; }
		[[nodiscard]] auto GetVertexShader() const -> const VertexShaderRef& { return m_VertexShader; }
		[[nodiscard]] auto GetPixelShader() const -> const PixelShaderRef& { return m_PixelShader; }
		[[nodiscard]] auto GetInputLayout() const -> const InputLayoutRef& { return m_InputLayout; }

		static void SetPrimitiveTopology(ID3D11DeviceContext* pContext)
		{
			pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
		void SetWorldViewProjection(ID3D11DeviceContext* pContext,const DX::XMMATRIX& WorldViewProjection)
		{
			m_VSConstantBufferData.WorldViewProjection = WorldViewProjection;
			m_VSConstantBuffer->Update(pContext, m_VSConstantBufferData);
		}

	private:
		void InitializePipelineObjects(ID3D11Device* pDevice, const MeshAsset& meshAsset)
		{
			std::vector<Vertex> vertices{};
			vertices.reserve(meshAsset.GetNumVertices());
			for(size_t i = 0; i < meshAsset.GetNumVertices(); ++i)
			{
				DX::XMFLOAT3 Pos{ meshAsset.GetVertexPositions()[i].x ,meshAsset.GetVertexPositions()[i].y ,meshAsset.GetVertexPositions()[i].z };
				DX::XMFLOAT3 Normal{ meshAsset.GetVertexNormals()[i].x,meshAsset.GetVertexNormals()[i].y,meshAsset.GetVertexNormals()[i].z };
				vertices.emplace_back(Pos, Normal);
			}
			constexpr D3D11_INPUT_ELEMENT_DESC elems[] =
			{
				{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
				{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
			};

			m_VertexShader     = std::make_shared<VertexShader>(pDevice, "Shaders/Unlit.vshader");
			if(m_VertexShader->GetBlob())
				m_InputLayout      = std::make_shared<InputLayout>(pDevice, elems, _countof(elems),
			                                              m_VertexShader->GetBlob()->GetBufferPointer()
			                                              , m_VertexShader->GetBlob()->GetBufferSize());
			m_PixelShader      = std::make_shared<PixelShader>(pDevice, "Shaders/Unlit.pshader");
			m_VertexBuffer     = std::make_shared<VertexBuffer<Vertex>>(pDevice, vertices.data(), vertices.size());
			m_IndexBuffer	   = std::make_shared<IndexBuffer>(pDevice, meshAsset.GetIndices().data(), meshAsset.GetNumIndices());
			m_VSConstantBuffer = std::make_shared<ConstantBuffer<VSConstantBufferData>>(pDevice, m_VSConstantBufferData, ConstantBufferType::Vertex);
		}
	private:
		VertexBufferRef<Vertex> m_VertexBuffer{};
		IndexBufferRef m_IndexBuffer;
		VertexShaderRef m_VertexShader;
		PixelShaderRef m_PixelShader;
		InputLayoutRef m_InputLayout;
		ConstantBufferRef<VSConstantBufferData> m_VSConstantBuffer{};
		VSConstantBufferData m_VSConstantBufferData{};
	};
}
