#include "SamplerState.h"

namespace Era
{
	SamplerState::SamplerState(ID3D11Device* pDevice, const D3D11_SAMPLER_DESC& desc, SamplerStateShaderType type) : m_Type(type)
	{
		DX_RESULT(pDevice->CreateSamplerState(&desc, &m_SamplerState));
	}

	void SamplerState::Bind(ID3D11DeviceContext* pContext) const
	{
		switch (m_Type)
		{
		case SamplerStateShaderType::Vertex:
		{
			pContext->VSSetSamplers(0u, 1u, m_SamplerState.GetAddressOf());
			break;
		}
		case SamplerStateShaderType::Pixel:
		{
			pContext->PSSetSamplers(0u, 1u, m_SamplerState.GetAddressOf());
			break;
		}
		case SamplerStateShaderType::Compute:
		{
			pContext->CSSetSamplers(0u, 1u, m_SamplerState.GetAddressOf());
			break;
		}
		case SamplerStateShaderType::Geometry:
		{
			pContext->GSSetSamplers(0u, 1u, m_SamplerState.GetAddressOf());
			break;
		}
		}
	}
}
