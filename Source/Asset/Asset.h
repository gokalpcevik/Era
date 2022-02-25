#pragma once
#include <filesystem>

namespace Era
{
	class Asset
	{
	public:
		Asset(std::filesystem::path path);
		virtual ~Asset() = default;

		[[nodiscard]] virtual auto GetPath() -> std::filesystem::path;
		[[nodiscard]] virtual auto GetExtension() -> std::filesystem::path;
		[[nodiscard]] virtual auto HasExtension() -> bool;
	protected:
		std::filesystem::path m_Path{};
		std::filesystem::path m_Extension{};
		bool m_HasExtension{true};
	};
}