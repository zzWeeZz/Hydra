#pragma once

#include <Array>
#include "Hydra/API/ApiUtils.h"
#include "Hydra/API/ApiFormats.h"
#include "Hydra/API/Vulkan/VulkanAllocator.h"
inline std::string errorString(VkResult errorCode)
{
	switch (errorCode)
	{
#define STR(r) case VK_ ##r: return #r
		STR(NOT_READY);
		STR(TIMEOUT);
		STR(EVENT_SET);
		STR(EVENT_RESET);
		STR(INCOMPLETE);
		STR(ERROR_OUT_OF_HOST_MEMORY);
		STR(ERROR_OUT_OF_DEVICE_MEMORY);
		STR(ERROR_INITIALIZATION_FAILED);
		STR(ERROR_DEVICE_LOST);
		STR(ERROR_MEMORY_MAP_FAILED);
		STR(ERROR_LAYER_NOT_PRESENT);
		STR(ERROR_EXTENSION_NOT_PRESENT);
		STR(ERROR_FEATURE_NOT_PRESENT);
		STR(ERROR_INCOMPATIBLE_DRIVER);
		STR(ERROR_TOO_MANY_OBJECTS);
		STR(ERROR_FORMAT_NOT_SUPPORTED);
		STR(ERROR_SURFACE_LOST_KHR);
		STR(ERROR_NATIVE_WINDOW_IN_USE_KHR);
		STR(SUBOPTIMAL_KHR);
		STR(ERROR_OUT_OF_DATE_KHR);
		STR(ERROR_INCOMPATIBLE_DISPLAY_KHR);
		STR(ERROR_VALIDATION_FAILED_EXT);
		STR(ERROR_INVALID_SHADER_NV);
#undef STR
	default:
		return "UNKNOWN_ERROR";
	}
}

#define HY_VK_CHECK(X) do { VkResult error = X; if(error) { HY_CORE_ERROR("Fatal : VkResult is {0} in {1} at line {2}", errorString(error).c_str(), __FILE__, __LINE__); exit(1); } } while(0)

namespace Hydra
{
	VkFormat GetVkFormat(ImageFormat image)
	{
		VkFormat format = {};
		switch (image)
		{
		case Hydra::ImageFormat::R8UN:
			format = VK_FORMAT_R8_UNORM;
			break;
		case Hydra::ImageFormat::RGBA8UN:
			format = VK_FORMAT_R8G8B8A8_UNORM;
			break;
		case Hydra::ImageFormat::R32UI:
			format = VK_FORMAT_R32_UINT;
			break;
		case Hydra::ImageFormat::R32F:
			format = VK_FORMAT_R32_SFLOAT;
			break;
		case Hydra::ImageFormat::R32S:
			format = VK_FORMAT_R32_SINT;
			break;
		case Hydra::ImageFormat::RG32F:
			format = VK_FORMAT_R32G32_SFLOAT;
			break;
		case Hydra::ImageFormat::RG32S:
			format = VK_FORMAT_R32G32_SINT;
			break;
		case Hydra::ImageFormat::RG32UI:
			format = VK_FORMAT_R32G32_UINT;
			break;
		case Hydra::ImageFormat::RGB32F:
			format = VK_FORMAT_R32G32B32_SFLOAT;
			break;
		case Hydra::ImageFormat::RGB32S:
			format = VK_FORMAT_R32G32B32_SINT;
			break;
		case Hydra::ImageFormat::RGB32UI:
			format = VK_FORMAT_R32G32B32_UINT;
			break;
		case Hydra::ImageFormat::RGBA32F:
			format = VK_FORMAT_R32G32B32A32_SFLOAT;
			break;
		case Hydra::ImageFormat::RGBA32UI:
			format = VK_FORMAT_R32G32B32A32_UINT;
			break;
		case Hydra::ImageFormat::RGBA32SI:
			format = VK_FORMAT_R32G32B32A32_SINT;
			break;
		case Hydra::ImageFormat::RGBA16F:
			break;
		case Hydra::ImageFormat::RGBA16UN:
			break;
		case Hydra::ImageFormat::RGBA16UI:
			break;
		case Hydra::ImageFormat::RGBA16SN:
			break;
		case Hydra::ImageFormat::RGBA16SI:
			break;
		case Hydra::ImageFormat::Depth32:
			break;
		case Hydra::ImageFormat::Depth24:
			break;
		default:
			break;
		}
	}
}