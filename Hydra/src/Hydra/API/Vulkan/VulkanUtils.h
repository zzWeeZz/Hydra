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
#pragma region ConvertFormatFunctions
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

	inline size_t GetSizeOfFormat(VkFormat format)
	{
		size_t out = 0;
		switch (format)
		{
		case VK_FORMAT_R8_UNORM:
			out = sizeof(uint8_t);
			break;
		case VK_FORMAT_R8G8B8A8_UNORM:
			out = sizeof(uint8_t) * 4;
			break;
		case VK_FORMAT_R32_UINT:
			out = sizeof(uint32_t);
			break;
		case VK_FORMAT_R32_SFLOAT:
			out = sizeof(float);
			break;
		case VK_FORMAT_R32_SINT:
			out = sizeof(int32_t);
			break;
		case VK_FORMAT_R32G32_SFLOAT:
			out = sizeof(float) * 2;
			break;
		case VK_FORMAT_R32G32_SINT:
			out = sizeof(int32_t) * 2;
			break;
		case VK_FORMAT_R32G32_UINT:
			out = sizeof(uint32_t) * 2;
			break;
		case VK_FORMAT_R32G32B32_SFLOAT:
			out = sizeof(float) * 3;
			break;
		case VK_FORMAT_R32G32B32_SINT:
			out = sizeof(int32_t) * 3;
			break;
		case VK_FORMAT_R32G32B32_UINT:
			out = sizeof(uint32_t) * 3;
			break;
		case VK_FORMAT_R32G32B32A32_SFLOAT:
			out = sizeof(float) * 4;
			break;
		case VK_FORMAT_R32G32B32A32_UINT:
			out = sizeof(uint32_t) * 4;
			break;
		case VK_FORMAT_R32G32B32A32_SINT:
			out = sizeof(int32_t) * 4;
			break;
		case VK_FORMAT_R16G16B16A16_SFLOAT:
			out = sizeof(float) * 4 / 2;
			break;
		case VK_FORMAT_R16G16B16A16_UNORM:
			out = sizeof(uint16_t) * 4;
			break;
		case VK_FORMAT_R16G16B16A16_UINT:
			out = sizeof(uint16_t) * 4;
			break;
		case VK_FORMAT_R16G16B16A16_SNORM:
			out = sizeof(int16_t) * 4;
			break;
		case VK_FORMAT_R16G16B16A16_SINT:
			out = sizeof(int16_t) * 4;
			break;
		default:
			// Unsupported format
			assert(false);
			break;
		}
		return out;
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

	inline VkBufferUsageFlags GetVkBufferUsage(BufferUsage usage)
	{
		VkBufferUsageFlags flags = {};

		switch (usage)
		{
		case Hydra::BufferUsage::None:
			HY_CORE_ASSERT(false, "Buffer Format can not be None!");
			break;
		case Hydra::BufferUsage::VertexBuffer:
			flags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			break;
		case Hydra::BufferUsage::IndexBuffer:
			flags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			break;
		case Hydra::BufferUsage::ConstantBuffer:
			flags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			break;
		case Hydra::BufferUsage::StorageBuffer:
			flags = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
			break;
		case Hydra::BufferUsage::IndirectBuffer:
			flags = VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
			break;
		default:
			HY_CORE_ASSERT(false, "Buffer Format is Invalid!");
			break;
		}
		return flags;
	}

#pragma endregion
#pragma region Samplers

	
	inline VkSamplerMipmapMode FormatToVkFormat(const VulkanMipmapMode& address)
	{
		return static_cast<VkSamplerMipmapMode>(address);
	}
	inline VkFilter FormatToVkFormat(const VulkanFilter& address)
	{
		return static_cast<VkFilter>(address);
	}
	inline VkSamplerAddressMode FormatToVkFormat(const VulkanAddress& address)
	{
		return static_cast<VkSamplerAddressMode>(address);
	}

#pragma endregion
#pragma region TransferUtils
	inline void AccessMasksFromLayouts(VkImageLayout sourceLayout, VkImageLayout targetLayout, VkImageMemoryBarrier& outBarrier)
	{
		// Source layouts (old)
		// Source access mask controls actions that have to be finished on the old layout
		// before it will be transitioned to the new layout
		switch (sourceLayout)
		{
		case VK_IMAGE_LAYOUT_UNDEFINED:
			// Image layout is undefined (or does not matter)
			// Only valid as initial layout
			// No flags required, listed only for completeness
			outBarrier.srcAccessMask = 0;
			break;

		case VK_IMAGE_LAYOUT_PREINITIALIZED:
			// Image is preinitialized
			// Only valid as initial layout for linear images, preserves memory contents
			// Make sure host writes have been finished
			outBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			// Image is a color attachment
			// Make sure any writes to the color buffer have been finished
			outBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			// Image is a depth/stencil attachment
			// Make sure any writes to the depth/stencil buffer have been finished
			outBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:
			// Image is a depth/stencil attachment
			// Make sure any writes to the depth/stencil buffer have been finished
			outBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			// Image is a transfer source
			// Make sure any reads from the image have been finished
			outBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			break;

		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			// Image is a transfer destination
			// Make sure any writes to the image have been finished
			outBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			// Image is read by a shader
			// Make sure any shader reads from the image have been finished
			outBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;

		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
			// Image is read by a shader
			// Make sure any shader reads from the image have been finished
			outBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
			break;

		case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL:
			// Image is read by a shader
			// Make sure any shader reads from the image have been finished
			outBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
			break;

		case VK_IMAGE_LAYOUT_GENERAL:
			outBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
			outBarrier.dstAccessMask = 0;
			break;
		default:
			HY_CORE_ASSERT(false, "Layouts are not configured!");
			break;
		}

		// Target layouts (new)
		// Destination access mask controls the dependency for the new image layout
		switch (targetLayout)
		{
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			// Image will be used as a transfer destination
			// Make sure any writes to the image have been finished
			outBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			// Image will be used as a transfer source
			// Make sure any reads from the image have been finished
			outBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			break;

		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			// Image will be used as a color attachment
			// Make sure any writes to the color buffer have been finished
			outBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:
			outBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			// Image layout will be used as a depth/stencil attachment
			// Make sure any writes to depth/stencil buffer have been finished
			outBarrier.dstAccessMask = outBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			// Image will be read in a shader (sampler, input attachment)
			// Make sure any writes to the image have been finished
			outBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;

		case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL:
			outBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;

		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
			outBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;

		case VK_IMAGE_LAYOUT_GENERAL:
			outBarrier.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
			outBarrier.dstAccessMask = 0;
			break;

		default:
			HY_CORE_ASSERT(false, "Layouts are not configured!");
			break;
		}
	}

	inline std::pair<VkPipelineStageFlags, VkPipelineStageFlags> GetStageFlagsFromLayouts(VkImageLayout currentLayout, VkImageLayout targetLayout)
	{
		VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
		VkPipelineStageFlags destStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

		if (currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && targetLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && targetLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && targetLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL)
		{
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && targetLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			destStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && targetLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
		{
			sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			destStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && targetLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
		{
			sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			destStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && targetLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
		{
			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && targetLayout == VK_IMAGE_LAYOUT_GENERAL)
		{
			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL && targetLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL)
		{
			sourceStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
			destStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL && targetLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL)
		{
			sourceStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
			destStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && targetLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
		{
			sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			destStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL && targetLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL)
		{
			sourceStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
			destStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL && targetLayout == VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL)
		{
			sourceStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
			destStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && targetLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
		{
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_GENERAL && targetLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
		{
			sourceStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
			destStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && targetLayout == VK_IMAGE_LAYOUT_GENERAL)
		{
			sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			destStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && targetLayout == VK_IMAGE_LAYOUT_GENERAL)
		{
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && targetLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && targetLayout == VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL)
		{
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && targetLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			destStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && targetLayout == VK_IMAGE_LAYOUT_GENERAL)
		{
			sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
			destStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR && targetLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			sourceStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			destStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && targetLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
		{
			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && targetLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
		{
		sourceStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		destStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		}
		else
		{
			HY_CORE_ASSERT(false, "Layouts are not configured!");
		}

		return { sourceStage, destStage };
	}

	inline void TransitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout currentLayout, VkImageLayout targetLayout, VkImageSubresourceRange subresource)
	{
		VkImageMemoryBarrier imageMemoryBarrier = {};
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.oldLayout = currentLayout;
		imageMemoryBarrier.newLayout = targetLayout;
		imageMemoryBarrier.image = image;
		imageMemoryBarrier.subresourceRange = subresource;

		AccessMasksFromLayouts(currentLayout, targetLayout, imageMemoryBarrier);
		const auto [sourceStage, destStage] = GetStageFlagsFromLayouts(currentLayout, targetLayout);

		vkCmdPipelineBarrier(commandBuffer, sourceStage, destStage, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
	}
#pragma endregion
}