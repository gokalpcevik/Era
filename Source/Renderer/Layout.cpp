#include "Layout.h"

namespace Era
{
	auto constexpr GetShaderFormatSize(ShaderDataFormat type) -> uint32_t
	{
		switch(type) { case ShaderDataFormat::None: break;
		case ShaderDataFormat::Float: return  4;
		case ShaderDataFormat::Float2: return 4 * 2;
		case ShaderDataFormat::Float3: return 4 * 3;
		case ShaderDataFormat::Float4: return 4 * 4;
        case ShaderDataFormat::UInt: return 4;
        case ShaderDataFormat::UInt2: return 4 * 2;
        case ShaderDataFormat::UInt3: return 4 * 3;
        case ShaderDataFormat::UInt4: return 4 * 4;
        case ShaderDataFormat::Int: return 4;
        case ShaderDataFormat::Int2: return 4 * 2;
        case ShaderDataFormat::Int3: return 4 * 3;
        case ShaderDataFormat::Int4: return 4 * 4;
		}
		return UINT32_MAX;
	}

	auto ShaderDataFormatToDXGIFormat(ShaderDataFormat format) -> DXGI_FORMAT
	{
        switch(format)
        {
        case ShaderDataFormat::None: return DXGI_FORMAT_UNKNOWN;
        case ShaderDataFormat::Float: return DXGI_FORMAT_R32_FLOAT;
        case ShaderDataFormat::Float2: return DXGI_FORMAT_R32G32_FLOAT;
        case ShaderDataFormat::Float3: return DXGI_FORMAT_R32G32B32_FLOAT;
        case ShaderDataFormat::Float4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case ShaderDataFormat::UInt: return DXGI_FORMAT_R32_UINT;
        case ShaderDataFormat::UInt2: return DXGI_FORMAT_R32G32_UINT;
        case ShaderDataFormat::UInt3: return DXGI_FORMAT_R32G32B32_UINT;
        case ShaderDataFormat::UInt4: return DXGI_FORMAT_R32G32B32A32_UINT;
        case ShaderDataFormat::Int: return DXGI_FORMAT_R32_SINT;
        case ShaderDataFormat::Int2: return DXGI_FORMAT_R32G32_SINT;
        case ShaderDataFormat::Int3: return DXGI_FORMAT_R32G32B32_SINT;
        case ShaderDataFormat::Int4: return DXGI_FORMAT_R32G32B32A32_SINT;
        }
        return DXGI_FORMAT_UNKNOWN;
	}


	VertexLayout::VertexLayout(ID3DBlob* pBlob)
	{
		ID3D11ShaderReflection* pReflection = nullptr;
		D3DReflect(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), IID_ID3D11ShaderReflection,
			reinterpret_cast<void**>(&pReflection));

		D3D11_SHADER_DESC shaderDesc{};
		pReflection->GetDesc(&shaderDesc);
		auto type = static_cast<D3D11_SHADER_VERSION_TYPE>(D3D11_SHVER_GET_TYPE(shaderDesc.Version));
		if(type != D3D11_SHVER_VERTEX_SHADER)
		{
			ERA_ERROR("Vertex Layout can only be created if the given shader is a vertex shader.");
			return;
		}

        uint32_t alignedByteOffset = 0;

        for (UINT i = 0; i < shaderDesc.InputParameters; i++)
        {
            D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
            pReflection->GetInputParameterDesc(i, &paramDesc);
            LayoutElement element{};

            if (paramDesc.Mask == 1)
            {
                if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                {
                    element.Format = ShaderDataFormat::UInt;
                }
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                {
                    element.Format = ShaderDataFormat::Int;
                }
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                {
                    element.Format = ShaderDataFormat::Float;
                }
            }
            else if (paramDesc.Mask <= 3)
            {
                if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                {
                    element.Format = ShaderDataFormat::UInt2;
                }
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                {
                    element.Format = ShaderDataFormat::Int2;

                }
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                {
                    element.Format = ShaderDataFormat::Float2;
                }
            }
            else if (paramDesc.Mask <= 7)
            {
                if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                {
                    element.Format = ShaderDataFormat::UInt3;
                }
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                {
                    element.Format = ShaderDataFormat::Int3;
                }
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                {
                    element.Format = ShaderDataFormat::Float3;
                }
            }
            else if (paramDesc.Mask <= 15)
            {
                if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                {
                    element.Format = ShaderDataFormat::UInt4;
                }
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                {
                    element.Format = ShaderDataFormat::Int4;
                }
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                {
                    element.Format = ShaderDataFormat::Float4;
                }
            }

            element.AlignedByteOffset = alignedByteOffset;
            element.Size = GetShaderFormatSize(element.Format);
            element.SemanticName = paramDesc.SemanticName;
            element.SemanticIndex = paramDesc.SemanticIndex;
            alignedByteOffset += element.Size;
            m_Stride += element.Size;
            m_Elements.push_back(element);
        }

		pReflection->Release();
	}

}
