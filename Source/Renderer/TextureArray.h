#pragma once
#include "Texture2D.h"

namespace Era
{
	class TextureArray
	{
	public:
		void Add(const Texture2DRef& texture, size_t index);
		void Erase(size_t index);
		void Push(const Texture2DRef& texture);
		void Pop();
		void BindToPixelShader(ID3D11DeviceContext* pContext) const;
		void UnbindFromPixelShader(ID3D11DeviceContext* pContext);

		[[nodiscard]] auto GetTexture(size_t index) const -> const Texture2DRef&;
	private:
		std::list<Texture2DRef> m_Textures;
	};

	using TextureArrayRef = std::shared_ptr<TextureArray>;
}