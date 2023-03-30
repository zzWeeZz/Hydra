#include "HYpch.h"
#include "VulkanImage.h"
#include "Hydra/API/Vulkan/Backend/VulkanDevice.h"

namespace Hydra
{
	VulkanImage::VulkanImage(ImageSpecification& specs, Ptr<VulkanDevice> device)
	{
		m_Specs = specs;
		m_Device = device;
		Validate();
	}
	void VulkanImage::Validate()
	{
		auto vkDevice = std::reinterpret_pointer_cast<VulkanDevice>(m_Device.lock());
		uint32_t bufferSize = 0; 
		if (m_Specs.memSize > 0)
		{
			bufferSize = m_Specs.memSize;
		}
		else
		{
			bufferSize = m_Specs.width * m_Specs.height;
		}

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = bufferSize;
		bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VmaAllocationCreateInfo allocInfo{};
		allocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
		AllocatedBuffer cpuBuffer;

		VulkanAllocator::Allocate(cpuBuffer, &bufferInfo, &allocInfo);

		if (m_Specs.ImageData)
		{
			void* mappedData = nullptr;
			VulkanAllocator::MapMemory(cpuBuffer, mappedData);
			memcpy_s(mappedData, bufferSize, m_Specs.ImageData, bufferSize);
			VulkanAllocator::UnMapMemory(cpuBuffer);
		}

		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = (m_Specs.width);
		imageInfo.extent.height = (m_Specs.height);
		imageInfo.extent.depth = m_Specs.depth;
		imageInfo.mipLevels = m_Specs.mipLevels;
		imageInfo.arrayLayers = m_Specs.arrayLayers;
		imageInfo.format = GetVkFormat(m_Specs.format);
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = 0; // Optional

		VmaAllocationCreateInfo imgAllocInfo{};
		imgAllocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

		VulkanAllocator::Allocate(m_Image, &imageInfo, &imgAllocInfo);

		vkDevice->ImmediateSubmit([&](VkCommandBuffer buffer)
			{
				VkImageSubresourceRange framebufferRange = {};
				framebufferRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				framebufferRange.baseArrayLayer = 0;
				framebufferRange.baseMipLevel = 0;
				framebufferRange.layerCount = 1;
				framebufferRange.levelCount = 1;

				TransitionImageLayout(buffer, m_Image.Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, framebufferRange);

				VkBufferImageCopy copyRegion{};
				copyRegion.bufferOffset = 0;
				copyRegion.bufferRowLength = 0;
				copyRegion.bufferImageHeight = 0;

				copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				copyRegion.imageSubresource.mipLevel = 0;
				copyRegion.imageSubresource.baseArrayLayer = 0;
				copyRegion.imageSubresource.layerCount = 1;
				copyRegion.imageExtent = imageInfo.extent;

				vkCmdCopyBufferToImage(buffer, cpuBuffer.buffer, m_Image.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);


				TransitionImageLayout(buffer, m_Image.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, framebufferRange);
			});
		
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = m_Image.Image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = GetVkFormat(m_Specs.format);
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = m_Specs.mipLevels;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		HY_VK_CHECK(vkCreateImageView(vkDevice->GetHandle(), &viewInfo, nullptr, &m_View));
		VulkanAllocator::CustomDeletion([=]
			{
				vkDestroyImageView(vkDevice->GetHandle(), m_View, nullptr);
			});
		VulkanAllocator::DeAllocate(cpuBuffer);
	}
}