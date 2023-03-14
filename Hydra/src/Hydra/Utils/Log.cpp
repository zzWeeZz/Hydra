#include "HYpch.h"
#include "Log.h"

namespace Hydra
{
	Ref<spdlog::logger> Log::s_CoreLogger;
	Ref<spdlog::logger> Log::s_ClientLogger;

	void Log::Initialize()
	{
		spdlog::set_pattern("%^[%T] [%n]: %v%$");
		s_ClientLogger = spdlog::stdout_color_mt("CLIENT");
		s_ClientLogger->set_level(spdlog::level::trace);

		s_CoreLogger = spdlog::stdout_color_mt("HYDRA");
		s_CoreLogger->set_level(spdlog::level::trace);
	}
}
