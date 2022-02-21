#pragma once
#include <d3d11_3.h>
#include <string_view>
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
		Texture2D(ID3D11Device* pDevice,std::wstring_view path);

		void SetShaderResourcesPS(ID3D11DeviceContext* pContext);
		[[nodiscard]] const ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const { return m_pSRV; }
		[[nodiscard]] const ComPtr<ID3D11Resource>& GetResource() const { return m_pResource; }

	private:
		ComPtr<ID3D11Resource> m_pResource;
		ComPtr<ID3D11ShaderResourceView> m_pSRV;
	};
}