#pragma once
#include <d3d11_3.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include "Layout.h"
#include "ErrorChecker.h"
#include <RmlUi/Core.h>

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
		InputLayout(ID3D11Device* pDevice, ID3DBlob* pShaderBlob, VertexLayout layout);

		void Bind(ID3D11DeviceContext* pContext) const;
		[[nodiscard]] auto GetD3D11InputLayout() const -> const ComPtr<ID3D11InputLayout>& { return m_InputLayout; }
	private:
		ComPtr<ID3D11InputLayout> m_InputLayout;
		[[maybe_unused]] ID3D11Device* m_Device = nullptr;
		[[maybe_unused]] size_t m_ElementSize = 0;
		[[maybe_unused]] const void* m_ShaderBytecodeWithInputSignature = nullptr;
		[[maybe_unused]] size_t m_ByteCodeLength = 0;
		VertexLayout m_Layout{};
	};

	using InputLayoutRef = std::shared_ptr<InputLayout>;
}
