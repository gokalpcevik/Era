#pragma once
#include <cstdint>
#include <d3d11_3.h>
#include <wrl/client.h>
#include "ErrorChecker.h"

namespace Era
{
	using Microsoft::WRL::ComPtr;

	class IndexBuffer
	{
	public:
		IndexBuffer(ID3D11Device3* pDevice, uint32_t* indices, uint32_t size);
		void Bind(ID3D11DeviceContext* pContext) const;
	private:

		ComPtr<ID3D11Buffer> m_indexBufferD3D = nullptr;
	};

	using IndexBufferRef = std::shared_ptr<IndexBuffer>;
}
