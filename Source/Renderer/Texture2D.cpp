#include "Texture2D.h"

namespace Era
{
	Texture2D::Texture2D(ID3D11Device* pDevice, const std::filesystem::path& path)
	{
		DX_RESULT(DirectX::CreateWICTextureFromFile(pDevice, path.c_str(), &m_pResource, &m_pSRV));
	}

	void Texture2D::PSSetShaderResources(ID3D11DeviceContext* pContext)
	{
		pContext->PSSetShaderResources(0u, 1u, m_pSRV.GetAddressOf());
	}
}
