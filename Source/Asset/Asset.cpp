#include "Asset.h"

namespace Era
{
	Asset::Asset(std::filesystem::path path) : m_Path(std::move(path)), m_HasExtension(m_Path.has_extension())
	{
		if(m_HasExtension)
			m_Extension = m_Path.extension();
	}

	auto Asset::GetPath() -> std::filesystem::path
	{
		return m_Path;
	}

	auto Asset::GetFileName() -> std::filesystem::path
	{
		return m_Path.filename();
	}

	auto Asset::GetExtension() -> std::filesystem::path
	{
		return m_Extension;
	}

	auto Asset::HasExtension() -> bool
	{
		return m_HasExtension;
	}
}
