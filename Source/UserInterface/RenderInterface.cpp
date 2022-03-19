#include "RenderInterface.h"

namespace Era
{
	namespace UI
	{
		RenderInterface::RenderInterface(ID3D11Device* pDevice,const DX::XMFLOAT2& viewportSize, ID3D11DeviceContext* pContext) : m_Device(pDevice),
			m_DeviceContext(pContext)
		{

		}

		void RenderInterface::RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices,
			Rml::TextureHandle texture, const Rml::Vector2f& translation)
		{
			
		}

		auto RenderInterface::CompileGeometry(Rml::Vertex* vertices, int num_vertices,
		                                      int* indices, int num_indices, Rml::TextureHandle texture) -> Rml::CompiledGeometryHandle
		{
			auto* compiledGeometry = new CompiledGeometry();

			std::vector<UIVertex> uiVertices{}; uiVertices.reserve(num_vertices);
			std::vector<uint32_t> uiIndices{}; uiIndices.reserve(num_vertices);

			for (size_t i = 0; i < num_vertices; ++i)
			{
				UIVertex vertex{};
				auto&& pos = vertices[i].position;
				auto&& color = vertices[i].colour;
				auto&& uv = vertices[i].tex_coord;

				vertex.Position = { pos.x,pos.y,0.0f,1.0f };
				vertex.Color = { color.red,color.green,color.blue,color.alpha };
				vertex.UV = { uv.x,uv.y };
				uiVertices.push_back(std::move(vertex));
			}

			for (size_t i = 0; i < num_indices; ++i)
			{
				uiIndices.push_back(indices[i]);
			}


			UIVSConstantBuffer cb{};
			DX::XMStoreFloat4x4(&cb.Transform, DX::XMMatrixOrthographicLH(m_WindowSize.x, m_WindowSize.y, 0.0f, 1.0f));

			compiledGeometry->VertexBuffer = std::make_shared<VertexBuffer<UIVertex>>(m_Device, uiVertices.data(), uiVertices.size());
			compiledGeometry->IndexBuffer = std::make_shared<IndexBuffer>(m_Device, uiIndices.data(), uiIndices.size());
			compiledGeometry->ConstantBuffer = std::make_shared<ConstantBuffer<UIVSConstantBuffer>>(m_Device, cb, ConstantBufferType::Vertex);
			compiledGeometry->TextureHandle = texture;

			return reinterpret_cast<uintptr_t>(compiledGeometry);
		}

		void RenderInterface::RenderCompiledGeometry(Rml::CompiledGeometryHandle geometry,
			const Rml::Vector2f& translation)
		{
			const auto* const pGeometry = reinterpret_cast<CompiledGeometry*>(geometry);

			pGeometry->ConstantBuffer->Bind(m_DeviceContext);
			pGeometry->IndexBuffer->Bind(m_DeviceContext);
			pGeometry->ConstantBuffer->Bind(m_DeviceContext);
			pGeometry->VertexBuffer->Bind(m_DeviceContext);
			pGeometry->VertexShader->Bind(m_DeviceContext);
			pGeometry->PixelShader->Bind(m_DeviceContext);
			m_DeviceContext->DrawIndexed(pGeometry->IndexBuffer->GetCount(), 0u, 0u);
		}

		void RenderInterface::ReleaseCompiledGeometry(Rml::CompiledGeometryHandle geometry)
		{
			delete reinterpret_cast<CompiledGeometry*>(geometry);
		}

		void RenderInterface::EnableScissorRegion(bool enable)
		{
		}

		void RenderInterface::SetScissorRegion(int x, int y, int width, int height)
		{
		}

		auto RenderInterface::GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source,
		                                      const Rml::Vector2i& source_dimensions) -> bool
		{
			return Rml::RenderInterface::GenerateTexture(texture_handle, source, source_dimensions);
		}

		auto RenderInterface::LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions,
		                                  const Rml::String& source) -> bool
		{
			return Rml::RenderInterface::LoadTexture(texture_handle, texture_dimensions, source);
		}

		void RenderInterface::SetTransform(const Rml::Matrix4f* transform)
		{
			
		}

		void RenderInterface::ReleaseTexture(Rml::TextureHandle texture)
		{
			Rml::RenderInterface::ReleaseTexture(texture);
		}

		void RenderInterface::OnWindowResized(SDL_Window* resizedWindow, uint32_t width, uint32_t height)
		{
			m_WindowSize = { static_cast<float>(width),static_cast<float>(height) };
		}
	}
}