#pragma once
#include <d3d11_3.h>
#include <filesystem>
#include <DirectXTK/WICTextureLoader.h>
#include <wrl/client.h>
#include "ErrorChecker.h"

namespace Era
{
	using Microsoft::WRL::ComPtr;
	class Texture2D
	{
	public:
		Texture2D() = default;
		Texture2D(ID3D11Device* pDevice,const std::filesystem::path& path);

		void PSSetShaderResources(ID3D11DeviceContext* pContext);
		[[nodiscard]] auto GetShaderResourceView() const -> const ComPtr<ID3D11ShaderResourceView>& { return m_pSRV; }
		[[nodiscard]] auto GetResource() const -> const ComPtr<ID3D11Resource>& { return m_pResource; }

	private:
		ComPtr<ID3D11Resource> m_pResource;
		ComPtr<ID3D11ShaderResourceView> m_pSRV;
	};
}