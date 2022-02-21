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
		//Member variables and parameters should be called count aswell but oh well
		uint32_t GetCount() const { return m_Size; }

	private:

		ComPtr<ID3D11Buffer> m_indexBufferD3D = nullptr;
		uint32_t m_Size = 0;
	};

	using IndexBufferRef = std::shared_ptr<IndexBuffer>;
}
