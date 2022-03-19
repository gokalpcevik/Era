#include "RasterizerState.h"

namespace Era
{
	RasterizerState::RasterizerState(ID3D11Device* pDevice, const D3D11_RASTERIZER_DESC& desc) 
	{
		DX_RESULT(pDevice->CreateRasterizerState(&desc, &m_RasterizerState));
	}

	void RasterizerState::Bind(ID3D11DeviceContext* pContext) const
	{
		pContext->RSSetState(m_RasterizerState.Get());
	}

	void RasterizerState::Reset(ID3D11Device* pDevice, const D3D11_RASTERIZER_DESC& desc)
	{
		m_RasterizerState.Reset();
		DX_RESULT(pDevice->CreateRasterizerState(&desc, &m_RasterizerState));
	}
}
