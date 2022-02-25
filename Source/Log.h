#pragma once
#define SPDLOG_WCHAR_TO_UTF8_SUPPORT
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>


namespace Era
{
    class Log
    {
    private:
        static std::shared_ptr<spdlog::logger> s_Logger;    
    public:
        Log() = default;
        static void Init();
        static auto GetLogger() -> std::shared_ptr<spdlog::logger>& { return s_Logger; }
    };   
   
} // namespace Era

#define ERA_TRACE(...)    ::Era::Log::GetLogger()->trace(__VA_ARGS__)
#define ERA_INFO(...)     ::Era::Log::GetLogger()->info(__VA_ARGS__)
#define ERA_WARN(...)     ::Era::Log::GetLogger()->warn(__VA_ARGS__)
#define ERA_ERROR(...)    ::Era::Log::GetLogger()->error(__VA_ARGS__)
#define ERA_CRITICAL(...) ::Era::Log::GetLogger()->critical(__VA_ARGS__)