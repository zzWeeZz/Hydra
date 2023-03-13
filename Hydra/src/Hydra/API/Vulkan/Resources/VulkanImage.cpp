#include "HYpch.h"
#include "VulkanImage.h"


namespace Hydra
{
	VulkanImage::VulkanImage(ImageSpecification& specs)
	{
		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = (specs.width);
		imageInfo.extent.height = (specs.height);
		imageInfo.extent.depth = specs.depth;
		imageInfo.mipLevels = specs.mipLevels;
		imageInfo.arrayLayers = specs.arrayLayers;
		imageInfo.format = GetVkFormat(specs.format);
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = 0; // Optional

		VmaAllocationCreateInfo imgAllocInfo{};
		imgAllocInfo.usage = GetVmaMemoryUsage(specs.memoryUsage);

		VulkanAllocator::Allocate(m_Image, &imageInfo, &imgAllocInfo);
	}
}