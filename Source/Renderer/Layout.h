#pragma once
#include <cstdint>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <algorithm>
#include "../Core/Log.h"

namespace Era
{
	enum class ShaderDataFormat
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		UInt,
		UInt2,
		UInt3,
		UInt4,
		Int,
		Int2,
		Int3,
		Int4,
	};

	auto constexpr GetShaderFormatSize(ShaderDataFormat type) -> uint32_t;
	auto ShaderDataFormatToDXGIFormat(ShaderDataFormat format) -> DXGI_FORMAT;

	struct LayoutElement
	{
		ShaderDataFormat Format{};
		uint32_t Size{};
		uint32_t AlignedByteOffset{};
		uint32_t InputSlot{};
		uint32_t SemanticIndex{};
		std::string SemanticName{};

		LayoutElement() = default;

		LayoutElement(ShaderDataFormat type,std::string name, bool normalized = false)
			: Format(type), Size(GetShaderFormatSize(type)), AlignedByteOffset(0), SemanticName(std::move(name))
		{
		}
	};


	class VertexLayout
	{
	public:
		VertexLayout() = default;
		VertexLayout(ID3DBlob* pBlob);

		[[nodiscard]] auto GetStride() const -> uint32_t { return m_Stride; }
		[[nodiscard]] auto GetElements() const -> std::vector<LayoutElement> const& { return m_Elements; }
	private:
		std::vector<LayoutElement> m_Elements{0};
		uint32_t m_Stride = 0;
	};
}