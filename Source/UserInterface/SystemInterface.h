#pragma once
#include <RmlUi/Core/SystemInterface.h>
#include <SDL2/SDL.h>
#include "../Core/Log.h"

namespace Era
{
	namespace UI
	{
		class SystemInterface : public Rml::SystemInterface
		{
		public:
			SystemInterface() = default;
			~SystemInterface() override = default;

			auto GetElapsedTime() -> double override;
			auto LogMessage(Rml::Log::Type type, const Rml::String& message) -> bool override;
		};

	}
}