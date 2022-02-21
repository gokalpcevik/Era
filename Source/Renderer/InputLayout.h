#pragma once
#include <d3d11_3.h>
#include <wrl/client.h>
#include "ErrorChecker.h"

namespace Era
{
	using Microsoft::WRL::ComPtr;

	class InputLayout
	{
	public:
		InputLayout(ID3D11Device* pDevice,
		    const D3D11_INPUT_ELEMENT_DESC* elements, 
			uint32_t size,
			const void* pShaderBytecodeWithInputSignature,
			size_t byteCodeLength);

		void Bind(ID3D11DeviceContext* pContext) const;
		[[nodiscard]] const ComPtr<ID3D11InputLayout>& GetD3D11InputLayout() const { return m_InputLayout; }

	private:
		ComPtr<ID3D11InputLayout> m_InputLayout;
		ID3D11Device* m_Device = nullptr;
		size_t m_ElementSize = 0;
		const void* m_ShaderBytecodeWithInputSignature = nullptr;
		size_t m_ByteCodeLength = 0;
	};

	using InputLayoutRef = std::shared_ptr<InputLayout>;
}
