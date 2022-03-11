#include "Asset.h"

namespace Era
{
	Asset::Asset(std::filesystem::path path) : m_Path(std::move(path))
	{
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
		return m_Path.extension();
	}

	auto Asset::HasExtension() -> bool
	{
		return m_Path.has_extension();
	}
}
