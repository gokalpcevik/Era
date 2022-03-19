#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include "ErrorChecker.h"

namespace Era
{
	using Microsoft::WRL::ComPtr;

	class RasterizerState
	{
	public:
		RasterizerState(ID3D11Device* pDevice, const D3D11_RASTERIZER_DESC& desc);

		void Bind(ID3D11DeviceContext* pContext) const;
		void Reset(ID3D11Device* pDevice, const D3D11_RASTERIZER_DESC& desc);
	private:
		ComPtr<ID3D11RasterizerState> m_RasterizerState{};

	};

	using RasterizerStateRef = std::shared_ptr<RasterizerState>;
}