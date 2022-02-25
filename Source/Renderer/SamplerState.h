#pragma once
#include <d3d11.h>
#include <wrl/client.h>

namespace Era
{
	using Microsoft::WRL::ComPtr;

	class SamplerState
	{
	public:
		SamplerState(ID3D11Device* pDevice);

	private:
		ComPtr<ID3D11SamplerState> m_SamplerState{nullptr};
	};
}
