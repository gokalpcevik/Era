#include "InputLayout.h"

namespace Era
{
	InputLayout::InputLayout(ID3D11Device* pDevice,
	    const D3D11_INPUT_ELEMENT_DESC* elements,
		uint32_t size,
		const void* pShaderBytecodeWithInputSignature,
		size_t byteCodeLength)
	  : m_Device(pDevice),
		m_ElementSize(size),
		m_ShaderBytecodeWithInputSignature(pShaderBytecodeWithInputSignature),
		m_ByteCodeLength(byteCodeLength)
	{
		DX_RESULT(pDevice->CreateInputLayout(elements, size, pShaderBytecodeWithInputSignature,
			byteCodeLength, &m_InputLayout));
	}
	void InputLayout::Bind(ID3D11DeviceContext* pContext) const
	{
		pContext->IASetInputLayout(m_InputLayout.Get());
	}
}