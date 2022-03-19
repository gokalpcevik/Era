#pragma once
#include <d3d11_3.h>
#include <filesystem>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include "ErrorChecker.h"

namespace Era
{
	using Microsoft::WRL::ComPtr;
	class Texture2D
	{
	public:
		Texture2D() = default;
		Texture2D(ID3D11Device* pDevice, const std::filesystem::path& path);
		Texture2D(ID3D11Device* pDevice,const uint8_t* data,size_t size);

		DirectX::XMUINT2 GetDimensions();
		void BindToVertexShader(ID3D11DeviceContext* pContext);
		void BindToPixelShader(ID3D11DeviceContext* pContext);
		void BindToPixelShader(ID3D11DeviceContext* pContext,const UINT startSlot);
		[[nodiscard]] auto GetShaderResourceView() const -> const ComPtr<ID3D11ShaderResourceView>& { return m_pSRV; }
		[[nodiscard]] auto GetResource() const -> const ComPtr<ID3D11Resource>& { return m_pResource; }

	private:
		ComPtr<ID3D11Resource> m_pResource;
		ComPtr<ID3D11ShaderResourceView> m_pSRV;
	};

	using Texture2DRef = std::shared_ptr<Texture2D>;
	using Texture2DUniqueRef = std::unique_ptr<Texture2D>;
}