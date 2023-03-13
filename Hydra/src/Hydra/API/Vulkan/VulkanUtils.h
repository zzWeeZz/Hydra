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
	inline VkFormat GetVkFormat(ImageFormat image)
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
			format = VK_FORMAT_R16G16B16A16_SFLOAT;
			break;
		case Hydra::ImageFormat::RGBA16UN:
			format = VK_FORMAT_R16G16B16A16_UNORM;
			break;
		case Hydra::ImageFormat::RGBA16UI:
			format = VK_FORMAT_R16G16B16A16_UINT;
			break;
		case Hydra::ImageFormat::RGBA16SN:
			format = VK_FORMAT_R16G16B16A16_SNORM;
			break;
		case Hydra::ImageFormat::RGBA16SI:
			format = VK_FORMAT_R16G16B16A16_SINT;
			break;
		case Hydra::ImageFormat::Depth32:
			format = VK_FORMAT_D32_SFLOAT_S8_UINT;
			break;
		case Hydra::ImageFormat::Depth24:
			format = VK_FORMAT_D24_UNORM_S8_UINT;
			break;
		default:
			break;
		}
		return format;
	}

	inline VmaMemoryUsage GetVmaMemoryUsage(MemoryUsage usage)
	{
		VmaMemoryUsage vma = {};
		switch (usage)
		{
		case Hydra::MemoryUsage::GPU_Only:
			vma = VMA_MEMORY_USAGE_GPU_ONLY;
			break;
		case Hydra::MemoryUsage::CPU_Only:
			vma = VMA_MEMORY_USAGE_CPU_ONLY;
			break;
		case Hydra::MemoryUsage::CPU_To_GPU:
			vma = VMA_MEMORY_USAGE_CPU_TO_GPU;
			break;
		case Hydra::MemoryUsage::GPU_To_CPU:
			vma = VMA_MEMORY_USAGE_GPU_TO_CPU;
			break;
		default:
			break;
		}
		return vma;
	}
}