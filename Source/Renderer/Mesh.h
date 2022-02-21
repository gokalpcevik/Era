#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <string_view>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include <DirectXMath.h>
#include <wrl/client.h>
#include "GraphicsDevice.h"
#include "DeviceContext.h"

namespace Era
{
	/*
	namespace DX = DirectX;
	using Microsoft::WRL::ComPtr;

	struct MeshVertex
	{
		DX::XMFLOAT3 Position;
	};

	class Mesh
	{
	public:
		Mesh(const GraphicsDevice& device,std::string_view path);

		void Draw(const DeviceContext& context) const;
	private:
		std::shared_ptr<VertexBuffer<MeshVertex>> m_VertexBuffer = nullptr;
		std::shared_ptr<IndexBuffer> m_IndexBuffer = nullptr;
		std::shared_ptr<InputLayout> m_InputLayout = nullptr;
		VertexShaderRef m_VertexShader = nullptr;
		PixelShaderRef m_PixelShader = nullptr;
		std::vector<MeshVertex> m_Vertices{};
		std::vector<uint32_t> m_Indices{};
	};
	*/
}
