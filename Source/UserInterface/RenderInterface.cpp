#include "RenderInterface.h"

namespace Era
{
	namespace UI
	{
		RenderInterface::RenderInterface(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const DX::XMFLOAT2& viewportSize) : m_Device(pDevice),
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
				vertex.Color = { color.red/255.0f,color.green/255.0f,color.blue/255.0f,color.alpha/255.0f };
				vertex.UV = { uv.x, uv.y };
				uiVertices.push_back(vertex);
			}

			for (size_t i = 0; i < num_indices; ++i)
			{
				uiIndices.push_back(indices[i]);
			}


			UIVSConstantBuffer cb{};
			DX::XMStoreFloat4x4(&cb.Transform, DX::XMMatrixTranspose(DX::XMMatrixOrthographicLH(m_WindowSize.x, m_WindowSize.y, 0.0f, 1.0f)));

			compiledGeometry->VertexBuffer = new VertexBuffer<UIVertex>(m_Device, uiVertices.data(), uiVertices.size());
			compiledGeometry->IndexBuffer = new IndexBuffer(m_Device, uiIndices.data(), uiIndices.size());
			compiledGeometry->ConstantBuffer = new ConstantBuffer<UIVSConstantBuffer>(m_Device, cb, ConstantBufferType::Vertex);
			compiledGeometry->VertexShader = new VertexShader(m_Device, ShaderLibrary::GetOrAddShaderBlob("Shaders/UI.vshader"));
			compiledGeometry->PixelShader = new PixelShader(m_Device, ShaderLibrary::GetOrAddShaderBlob("Shaders/UI.pshader"));
			compiledGeometry->Texture = (ID3D11ShaderResourceView*)texture;

			D3D11_RASTERIZER_DESC rasterizerDesc{};
			rasterizerDesc.CullMode = D3D11_CULL_NONE;
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;

			compiledGeometry->RasterizerState = new RasterizerState(m_Device, rasterizerDesc);
			VertexLayout layout(ShaderLibrary::GetOrAddShaderBlob("Shaders/UI.vshader").Blob);
			compiledGeometry->InputLayout = new InputLayout(
				m_Device, ShaderLibrary::GetOrAddShaderBlob("Shaders/UI.vshader").Blob, layout);
			return reinterpret_cast<uintptr_t>(compiledGeometry);
		}

		void RenderInterface::RenderCompiledGeometry(Rml::CompiledGeometryHandle geometry,
			const Rml::Vector2f& translation)
		{
			const auto* const pGeometry = reinterpret_cast<CompiledGeometry*>(geometry);

			UIVSConstantBuffer cb{};
			DX::XMStoreFloat4x4(&cb.Transform, DX::XMMatrixTranspose(m_Transform * DX::XMMatrixOrthographicLH(m_WindowSize.x, m_WindowSize.y, 0.0f, 1.0f)));
			cb.Translation = { translation.x,translation.y };
			pGeometry->ConstantBuffer->Update(m_DeviceContext, cb);
			pGeometry->ConstantBuffer->Bind(m_DeviceContext);
			pGeometry->IndexBuffer->Bind(m_DeviceContext);
			pGeometry->ConstantBuffer->Bind(m_DeviceContext);
			pGeometry->VertexBuffer->Bind(m_DeviceContext);
			pGeometry->VertexShader->Bind(m_DeviceContext);
			pGeometry->PixelShader->Bind(m_DeviceContext);
			pGeometry->InputLayout->Bind(m_DeviceContext);
			pGeometry->RasterizerState->Bind(m_DeviceContext);
			std::vector<ID3D11ShaderResourceView*> views;
			views.push_back(pGeometry->Texture);
			if (pGeometry->Texture)
				m_DeviceContext->PSSetShaderResources(0u, 1u, views.data());

			D3D11_SAMPLER_DESC sampDesc;
			ZeroMemory(&sampDesc, sizeof(sampDesc));
			sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			sampDesc.MinLOD = 0;
			sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
			auto st = std::make_shared<SamplerState>(m_Device, sampDesc, SamplerStateShaderType::Pixel);
			st->Bind(m_DeviceContext);

			ID3D11BlendState* d3dBlendState;
			D3D11_BLEND_DESC omDesc;
			ZeroMemory(&omDesc,sizeof(D3D11_BLEND_DESC));
			omDesc.RenderTarget[0].BlendEnable = true;
			omDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			omDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			omDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			omDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			omDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			omDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			omDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;


			m_Device->CreateBlendState(&omDesc, &d3dBlendState);
			m_DeviceContext->OMSetBlendState(d3dBlendState, 0, 0xffffffff);

			d3dBlendState->Release();
			m_DeviceContext->DrawIndexed(pGeometry->IndexBuffer->GetCount(), 0u, 0u);
		}

		void RenderInterface::ReleaseCompiledGeometry(Rml::CompiledGeometryHandle geometry)
		{
			auto* pGeometry = reinterpret_cast<CompiledGeometry*>(geometry);;
			delete pGeometry->ConstantBuffer;
			delete pGeometry->IndexBuffer;
			delete pGeometry->VertexShader;
			delete pGeometry->PixelShader;
			delete pGeometry->VertexBuffer;
			delete pGeometry->InputLayout;
			delete pGeometry->RasterizerState;
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
			
			ID3D11Texture2D* pTexture;

			D3D11_TEXTURE2D_DESC textureDesc;
			textureDesc.Width = source_dimensions.x;
			textureDesc.Height = source_dimensions.y;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			textureDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
			textureDesc.Usage = D3D11_USAGE_DYNAMIC;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			textureDesc.MiscFlags = 0;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;

			m_Device->CreateTexture2D(&textureDesc, nullptr, &pTexture);

			D3D11_MAPPED_SUBRESOURCE mappedTex;

			m_DeviceContext->Map(pTexture, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_WRITE_DISCARD, 0, &mappedTex);

			for (int x = 0; x < source_dimensions.x; ++x)
			{
				for (int y = 0; y < source_dimensions.y; ++y)
				{
					const byte* source_pixel = source + (source_dimensions.x * 4 * y) + (x * 4);
					byte* destination_pixel = ((byte*)mappedTex.pData) + mappedTex.RowPitch * y + x * 4;
					destination_pixel[0] = source_pixel[0];
					destination_pixel[1] = source_pixel[1];
					destination_pixel[2] = source_pixel[2];
					destination_pixel[3] = source_pixel[3];
				}
			}
			m_DeviceContext->Unmap(pTexture, D3D11CalcSubresource(0, 0, 1));

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format = textureDesc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
			srvDesc.Texture2D.MostDetailedMip = 0;

			ID3D11ShaderResourceView* srv;
			DX_RESULT(m_Device->CreateShaderResourceView(pTexture, &srvDesc, &srv));

			texture_handle = (Rml::TextureHandle)srv;
			return true;
		}

		auto RenderInterface::LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions,
		                                  const Rml::String& source) -> bool
		{
			return Rml::RenderInterface::LoadTexture(texture_handle, texture_dimensions, source);
		}

		void RenderInterface::SetTransform(const Rml::Matrix4f* transform)
		{
			if(transform == nullptr)
			{
				m_Transform = DX::XMMatrixIdentity();
			}
			else
			{
				DX::XMFLOAT4X4 t = *(DX::XMFLOAT4X4*)transform;
				m_Transform = DX::XMLoadFloat4x4(&t);
			}

		}

		void RenderInterface::ReleaseTexture(Rml::TextureHandle texture)
		{
			auto* srv = (ID3D11ShaderResourceView*)texture;
			srv->Release();
		}

		void RenderInterface::OnWindowResized(SDL_Window* resizedWindow, uint32_t width, uint32_t height)
		{
			m_WindowSize = { static_cast<float>(width),static_cast<float>(height) };
		}
	}
}