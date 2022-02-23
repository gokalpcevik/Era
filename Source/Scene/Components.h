#pragma once
#include <DirectXMath.h>
#include <filesystem>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "../Renderer/Shader.h"
#include "../Renderer/ConstantBuffer.h"
#include <Windows.h>

namespace Era
{
	namespace DX = DirectX;

	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(DX::XMFLOAT3 Translation, DX::XMFLOAT4 Rotation, DX::XMFLOAT3 Scale)
			: Translation(Translation), Rotation(Rotation) , Scale(Scale) {}

		[[nodiscard]] DX::XMMATRIX GetTransform() const
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

		[[nodiscard]]DX::XMMATRIX GetViewProjection() const
		{
			return m_ViewMatrix * m_ProjectionMatrix;
		}

		float GetAspectRatio() const
		{
			return m_AspectRatio;
		}

		DX::XMMATRIX GetView() const
		{
			return m_ViewMatrix;
		}

		DX::XMMATRIX GetProjection()
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

		bool IsPrimary() const
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
		MeshRendererComponent(ID3D11Device* pDevice,const std::filesystem::path& meshPath)
		{
			InitializePipelineObjects(pDevice,meshPath);
		}

		struct Vertex
		{
			DX::XMFLOAT3 Position{};
		};

		struct ConstantBufferUnlit
		{
			DX::XMMATRIX WorldViewProjection{};
			DX::XMFLOAT4 Color{0.0f,1.0f,1.0f,1.0f};
		};

		[[nodiscard]] const VertexBufferRef<Vertex>& GetVertexBuffer() const { return m_VertexBuffer; }
		[[nodiscard]] const ConstantBufferRef<ConstantBufferUnlit>& GetConstantBuffer() const { return m_ConstantBuffer; }
		[[nodiscard]] const IndexBufferRef& GetIndexBuffer() const { return m_IndexBuffer; }
		[[nodiscard]] const VertexShaderRef& GetVertexShader() const { return m_VertexShader; }
		[[nodiscard]] const PixelShaderRef& GetPixelShader() const { return m_PixelShader; }
		[[nodiscard]] const InputLayoutRef& GetInputLayout() const { return m_InputLayout; }

		static void SetPrimitiveTopology(ID3D11DeviceContext* pContext)
		{
			pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
		void SetWorldViewProjection(ID3D11DeviceContext* pContext,const DX::XMMATRIX& WorldViewProjection)
		{
			m_CBUnlit.WorldViewProjection = DX::XMMatrixTranspose(WorldViewProjection);
			m_ConstantBuffer->Update(pContext, m_CBUnlit);
		}

	private:
		void InitializePipelineObjects(ID3D11Device* pDevice,const std::filesystem::path& meshPath)
		{
			
			m_VertexShader = std::make_shared<VertexShader>(pDevice, "Shaders/Unlit.vshader");
			m_PixelShader = std::make_shared<PixelShader>(pDevice, "Shaders/Unlit.pshader");
			constexpr D3D11_INPUT_ELEMENT_DESC elems[] =
			{
				{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0}
			};
			m_InputLayout = std::make_shared<InputLayout>(pDevice, elems, 1,
				m_VertexShader->GetBlob()->GetBufferPointer()
				, m_VertexShader->GetBlob()->GetBufferSize());
			
			Assimp::Importer importer;
			auto const* pScene = importer.ReadFile(meshPath.string().c_str(), aiProcess_Triangulate |
				aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded
			);
			auto const* pMesh = pScene->mMeshes[0];
			std::vector<Vertex> vertices;
			vertices.reserve(pMesh->mNumVertices);
			for(uint32_t i = 0; i < pMesh->mNumVertices; ++i)
			{
				vertices.push_back({ {pMesh->mVertices[i].x ,pMesh->mVertices[i].y ,pMesh->mVertices[i].z} });
			}
			std::vector<uint32_t> indices;
			indices.reserve(pMesh->mNumFaces * 3);
			for(uint32_t i = 0; i < pMesh->mNumFaces; ++i)
			{
				const auto& face = pMesh->mFaces[i];
				indices.push_back(face.mIndices[0]);
				indices.push_back(face.mIndices[1]);
				indices.push_back(face.mIndices[2]);
			}
			m_VertexBuffer = std::make_shared<VertexBuffer<Vertex>>(pDevice, vertices.data(), vertices.size());
			m_IndexBuffer = std::make_shared<IndexBuffer>(pDevice, indices.data(), indices.size());
			m_ConstantBuffer = std::make_shared<ConstantBuffer<ConstantBufferUnlit>>(pDevice, m_CBUnlit, ConstantBufferType::Vertex);
		}

	private:
		VertexShaderRef m_VertexShader;
		PixelShaderRef m_PixelShader;
		InputLayoutRef m_InputLayout;
		VertexBufferRef<Vertex> m_VertexBuffer{};
		ConstantBufferRef<ConstantBufferUnlit> m_ConstantBuffer{};
		IndexBufferRef m_IndexBuffer;
		ConstantBufferUnlit m_CBUnlit{};
	};
}
