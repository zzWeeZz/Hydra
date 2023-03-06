#ifndef HYDRA_PCH
#define HYDRA_PCH

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
//API includes
#include <windows.h>
#include <wrl.h>

#include "Hydra/Utils/Memory.h"
#include "Hydra/Utils/Log.h"

#define HY_CORE_TRACE(...) ::Hydra::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HY_CORE_INFO(...) ::Hydra::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HY_CORE_WARN(...) ::Hydra::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HY_CORE_ERROR(...) ::Hydra::Log::GetCoreLogger()->error(__VA_ARGS__)

#define HY_TRACE(...) ::Hydra::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HY_INFO(...) ::Hydra::Log::GetClientLogger()->info(__VA_ARGS__)
#define HY_WARN(...) ::Hydra::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HY_ERROR(...) ::Hydra::Log::GetClientLogger()->error(__VA_ARGS__)

#endif