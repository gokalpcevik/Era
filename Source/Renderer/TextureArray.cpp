#include "TextureArray.h"

namespace Era
{
	void TextureArray::Add(const Texture2DRef& texture, size_t index)
	{
		auto it = m_Textures.begin();
		std::advance(it, index);
		m_Textures.insert(it, texture);
	}

	void TextureArray::Erase(size_t index)
	{
		if (m_Textures.size() == 0) return;
		auto it = m_Textures.begin();
		std::advance(it, index);
		m_Textures.erase(it);
	}

	void TextureArray::Push(const Texture2DRef& texture)
	{
		m_Textures.push_back(texture);
	}

	void TextureArray::Pop()
	{
		m_Textures.pop_back();
	}

	void TextureArray::BindToPixelShader(ID3D11DeviceContext* pContext) const
	{
		if (m_Textures.size() <= 0)
			return;
		std::vector<ID3D11ShaderResourceView* const*> SRViews;
		for(auto& t : m_Textures)
		{
			SRViews.push_back(t->GetShaderResourceView().GetAddressOf());
		}
		pContext->PSSetShaderResources(0u, m_Textures.size(), *SRViews.data());
	}

	void TextureArray::UnbindFromPixelShader(ID3D11DeviceContext* pContext)
	{
		ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
		pContext->PSSetShaderResources(0, 1, nullSRV);
	}

	auto TextureArray::GetTexture(size_t index) const -> const Texture2DRef&
	{
		auto iter = m_Textures.begin();
		std::advance(iter, index);
		return *iter;
	}
}
