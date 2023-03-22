#include "HYpch.h"
#include "VulkanFramebuffer.h"
#include <Hydra/API/Vulkan/Backend/VulkanDevice.h>

namespace Hydra
{
	VulkanFramebuffer::VulkanFramebuffer(FramebufferSpecification& specs, Ptr<Device> device) : Framebuffer(specs, device)
	{
		for (size_t i = 0; i < g_FramesInFlight; i++)
		{
			m_Images[i].resize(m_Specs.formats.size());
			m_Views[i].resize(m_Specs.formats.size());
			m_Attachments[i].resize(m_Specs.formats.size());
		}
		Validate();
	}
	
	void VulkanFramebuffer::CleanUp()
	{
		for (size_t i = 0; i < g_FramesInFlight; ++i)
		{
			for (size_t imageFormatIndex = 0; imageFormatIndex < m_Specs.formats.size(); ++imageFormatIndex)
			{
				vkDestroyImageView(std::reinterpret_pointer_cast<VulkanDevice>(m_Device.lock())->GetHandle(), m_Views[i][imageFormatIndex], nullptr);
			}
		}
		for (size_t i = 0; i < g_FramesInFlight; ++i)
		{
			for (size_t imageFormatIndex = 0; imageFormatIndex < m_Specs.formats.size(); ++imageFormatIndex)
			{
				VulkanAllocator::DeAllocate(m_Images[i][imageFormatIndex]);
			}
		}
	}

	void VulkanFramebuffer::Resize(const size_t width, const size_t height)
	{
		if (width == 0 || height == 0)
		{
			HY_CORE_ASSERT(false, "resize failed, width or height were 0!");
		}
		auto vulkanDevice = std::reinterpret_pointer_cast<VulkanDevice>(m_Device.lock());
		vulkanDevice->WaitForIdle();

		m_Specs.height = static_cast<uint32_t>(height);
		m_Specs.width = static_cast<uint32_t>(width);
		CleanUp();
		Validate();
	}

	void VulkanFramebuffer::Validate()
	{
		m_Rect.extent.width = static_cast<uint32_t>(m_Specs.width);
		m_Rect.extent.height = static_cast<uint32_t>(m_Specs.height);
		m_Rect.offset = { 0,0 };
		auto vkDevice = std::reinterpret_pointer_cast<VulkanDevice>(m_Device.lock());
		for (size_t i = 0; i < g_FramesInFlight; ++i)
		{
			for (size_t imageFormatIndex = 0; imageFormatIndex < m_Specs.formats.size(); ++imageFormatIndex)
			{
				VkImageCreateInfo imageInfo{};
				imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
				imageInfo.imageType = VK_IMAGE_TYPE_2D;
				imageInfo.extent.width = static_cast<uint32_t>(m_Specs.width);
				imageInfo.extent.height = static_cast<uint32_t>(m_Specs.height);
				imageInfo.extent.depth = 1;
				imageInfo.mipLevels = 1;
				imageInfo.arrayLayers = 1;
				imageInfo.format = GetVkFormat(m_Specs.formats[imageFormatIndex]);
				imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
				imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
				imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
				imageInfo.flags = 0; // Optional

				VmaAllocationCreateInfo imgAllocInfo{};
				imgAllocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

				VulkanAllocator::Allocate(m_Images[i][imageFormatIndex], &imageInfo, &imgAllocInfo);

				VkImageViewCreateInfo viewInfo{};
				viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				viewInfo.image = m_Images[i][imageFormatIndex].Image;
				viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				viewInfo.format = GetVkFormat(m_Specs.formats[imageFormatIndex]);
				viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				viewInfo.subresourceRange.baseMipLevel = 0;
				viewInfo.subresourceRange.levelCount = 1;
				viewInfo.subresourceRange.baseArrayLayer = 0;
				viewInfo.subresourceRange.layerCount = 1;

				vkDevice->ImmediateSubmit([&](VkCommandBuffer buffer)
					{
						VkImageSubresourceRange framebufferRange = {};
						framebufferRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
						framebufferRange.baseArrayLayer = 0;
						framebufferRange.baseMipLevel = 0;
						framebufferRange.layerCount = 1;
						framebufferRange.levelCount = 1;

						TransitionImageLayout(buffer, m_Images[i][imageFormatIndex].Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, framebufferRange);
					});


				HY_VK_CHECK(vkCreateImageView(vkDevice->GetHandle(), &viewInfo, nullptr, &m_Views[i][imageFormatIndex]));
				VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
				const VkRenderingAttachmentInfo colorAttachmentInfo
				{
					.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
					.imageView = m_Views[i][imageFormatIndex],
					.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
					.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
					.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
					.clearValue = clearColor,
				};

				m_Attachments[i][imageFormatIndex] = colorAttachmentInfo;
			}
		}
	}
}