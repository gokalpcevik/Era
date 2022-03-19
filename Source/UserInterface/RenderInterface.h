#pragma once
#include <RmlUi/Core/RenderInterface.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <DirectXTK/WICTextureLoader.h>
#include "../Renderer/VertexBuffer.h"
#include "../Renderer/ConstantBuffer.h"
#include "../Renderer/IndexBuffer.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Shader.h"
#include "../Renderer/Renderer.h"

namespace Era
{
	namespace UI
	{
		using Microsoft::WRL::ComPtr;
		namespace DX = DirectX;

		struct UIVertex
		{
			DX::XMFLOAT4 Position;
			DX::XMFLOAT2 UV;
			std::array<uint8_t, 4> Color;
		};

		struct UIVSConstantBuffer
		{
			DX::XMFLOAT4X4 Transform{};
		};

		struct CompiledGeometry
		{
			VertexBufferRef<UIVertex> VertexBuffer{};
			ConstantBufferRef<UIVSConstantBuffer> ConstantBuffer{};
			IndexBufferRef IndexBuffer{};
			VertexShaderRef VertexShader{};
			PixelShaderRef PixelShader{};
			uintptr_t TextureHandle{};
		};

		class RenderInterface : public Rml::RenderInterface , public IWindowListener
		{
		public:
			RenderInterface(ID3D11Device* pDevice,const DX::XMFLOAT2& viewportSize,ID3D11DeviceContext* pContext);

			void RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices,
			                    Rml::TextureHandle texture, const Rml::Vector2f& translation) override;

			auto CompileGeometry(Rml::Vertex* vertices, int num_vertices, int* indices,
			                     int num_indices, Rml::TextureHandle texture) -> Rml::CompiledGeometryHandle override;
			virtual auto RenderCompiledGeometry(Rml::CompiledGeometryHandle geometry,
			                                    const Rml::Vector2f& translation) -> void override;
			void ReleaseCompiledGeometry(Rml::CompiledGeometryHandle geometry) override;
			void EnableScissorRegion(bool enable) override;
			void SetScissorRegion(int x, int y, int width, int height) override;
			auto GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions)
			-> bool override;
			auto LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions,
			                 const Rml::String& source) -> bool override;
			void SetTransform(const Rml::Matrix4f* transform) override;
			void ReleaseTexture(Rml::TextureHandle texture) override;

		private:
			void OnWindowResized(SDL_Window* resizedWindow, uint32_t width, uint32_t height) override;

		private:
			DX::XMFLOAT2 m_WindowSize{ 1600,900 };
			ID3D11Device* m_Device{ nullptr };
			ID3D11DeviceContext* m_DeviceContext{ nullptr };
		};
	}
}