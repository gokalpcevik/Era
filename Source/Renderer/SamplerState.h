#pragma once
#include <d3d11_3.h>
#include <wrl/client.h>
#include "ErrorChecker.h"

namespace Era
{
	using Microsoft::WRL::ComPtr;

	enum class SamplerStateShaderType
	{
		Vertex = 0,
		Pixel,
		Compute,
		Geometry
	};

	class SamplerState
	{
	public:
		SamplerState(ID3D11Device* pDevice,const D3D11_SAMPLER_DESC& desc,SamplerStateShaderType type);
		void Bind(ID3D11DeviceContext* pContext) const;
	private:
		ComPtr<ID3D11SamplerState> m_SamplerState{nullptr};
		SamplerStateShaderType m_Type{};
	};

	using SamplerStateRef = std::shared_ptr<SamplerState>;
}
