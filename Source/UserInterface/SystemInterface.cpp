#include "SystemInterface.h"

namespace Era
{
	namespace UI
	{
		auto SystemInterface::GetElapsedTime() -> double
		{
			return SDL_GetTicks() / 1000.0;
		}

		auto SystemInterface::LogMessage(Rml::Log::Type type, const Rml::String& message) -> bool
		{
			switch(type) {
				case Rml::Log::LT_ALWAYS:
				{
					ERA_TRACE(message);
					return true;
				}
				case Rml::Log::LT_ERROR:
				{
					ERA_ERROR(message);
					return true;
				}
				case Rml::Log::LT_ASSERT:
				{
					ERA_CRITICAL(message);
					return true;
				}
				case Rml::Log::LT_WARNING:
				{
					ERA_WARN(message);
					return true;
				}
				case Rml::Log::LT_INFO:
				{
					ERA_INFO(message);
					return true;
				}
				case Rml::Log::LT_DEBUG:
				{
					ERA_DEBUG(message);
					return true;
				}
				case Rml::Log::LT_MAX:
				{
					ERA_ERROR(message);
					return true;
				}
			default: ;
			}
		
		}
	}
}