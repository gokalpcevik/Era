#include "Texture2D.h"

namespace Era
{
	Texture2D::Texture2D(ID3D11Device* pDevice, const std::filesystem::path& path)
	{
		DX_RESULT(DirectX::CreateWICTextureFromFile(pDevice, path.c_str(), &m_pResource, &m_pSRV));
	}

	Texture2D::Texture2D(ID3D11Device* pDevice,const uint8_t* data,size_t size)
	{
		DX_RESULT(DirectX::CreateWICTextureFromMemory(pDevice, data, size, &m_pResource, &m_pSRV));
	}


	auto Texture2D::GetDimensions() const -> DirectX::XMUINT2
	{
		ComPtr<ID3D11Texture2D> tex;
		DX_RESULT(m_pResource.As(&tex));
		D3D11_TEXTURE2D_DESC desc;
		tex->GetDesc(&desc);
		return { desc.Width,desc.Height };
	}

	void Texture2D::BindToVertexShader(ID3D11DeviceContext* pContext)
	{
		pContext->VSSetShaderResources(0u, 1u, m_pSRV.GetAddressOf());
	}

	void Texture2D::BindToPixelShader(ID3D11DeviceContext* pContext)
	{
		pContext->PSSetShaderResources(0u, 1u, m_pSRV.GetAddressOf());
	}

	void Texture2D::BindToPixelShader(ID3D11DeviceContext* pContext, const UINT startSlot)
	{
		pContext->PSSetShaderResources(startSlot, 1u, m_pSRV.GetAddressOf());
	}
}
