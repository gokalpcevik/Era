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

	InputLayout::InputLayout(ID3D11Device* pDevice, ID3DBlob* pShaderBlob, VertexLayout layout) : m_Layout(
		std::move(layout))
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc{};
		inputLayoutDesc.reserve(m_Layout.GetElements().size());
		D3D11_INPUT_ELEMENT_DESC desc{};
		for (const auto& elem : m_Layout.GetElements())
		{
			desc.AlignedByteOffset = elem.AlignedByteOffset;
			desc.SemanticIndex = elem.SemanticIndex;
			desc.InputSlot = elem.InputSlot;
			desc.SemanticName = elem.SemanticName.c_str();
			desc.Format = ShaderDataFormatToDXGIFormat(elem.Format);
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;
			inputLayoutDesc.push_back(desc);
		}


        DX_RESULT(pDevice->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), &m_InputLayout));
	}

	void InputLayout::Bind(ID3D11DeviceContext* pContext) const
	{
		pContext->IASetInputLayout(m_InputLayout.Get());
	}
}