#include "HYpch.h"
#include "VulkanCommandBuffer.h"
#include <Hydra/API/Vulkan/Backend/VulkanDevice.h>
#include <Hydra/API/Vulkan/CommandSubmiting/VulkanCommandQueue.h>
#include <Hydra/API/Vulkan/Resources/VulkanFramebuffer.h>
#include <Hydra/API/Vulkan/Backend/VulkanSwapchain.h>
namespace Hydra
{
	VulkanCommandBuffer::VulkanCommandBuffer(CommandBufferSpecification& specs) : CommandBuffer(specs)
	{
		m_CommandBuffer = {};
		auto vulkanDevice = std::reinterpret_pointer_cast<VulkanDevice>(specs.device.lock());
		auto vulkanCommandQueue = std::reinterpret_pointer_cast<VulkanCommandQueue>(specs.queue.lock());

		VkCommandBufferLevel vklevel = VK_COMMAND_BUFFER_LEVEL_MAX_ENUM;

		if (specs.level == CommandBufferLevel::Primary)
		{
			vklevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		}
		else if (specs.level == CommandBufferLevel::Secondary)
		{
			vklevel = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
		}
		else
		{
			HY_CORE_ASSERT(false, "Vulkan: Commandbuffer needs to have a valid CommadBufferLevel");
		}

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = vulkanCommandQueue->GetHandle();
		allocInfo.level = vklevel;
		allocInfo.commandBufferCount = 1;
		HY_VK_CHECK(vkAllocateCommandBuffers(vulkanDevice->GetHandle(), &allocInfo, &m_CommandBuffer));
	}

	void VulkanCommandBuffer::Begin()
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional
		HY_VK_CHECK(vkBeginCommandBuffer(m_CommandBuffer, &beginInfo));
	}

	void VulkanCommandBuffer::End()
	{
		vkEndCommandBuffer(m_CommandBuffer);
	}

	void VulkanCommandBuffer::BeginFramebuffer(uint32_t frameIndex, Ref<Framebuffer>& framebuffer, float color[4])
	{
		auto vulkanFramebuffer = std::reinterpret_pointer_cast<VulkanFramebuffer>(framebuffer);
		memcpy(vulkanFramebuffer->m_Attachments[frameIndex][0].clearValue.color.float32, color, sizeof(color));
		const VkRenderingInfo renderInfo{
			.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
			.renderArea = vulkanFramebuffer->m_Rect,
			.layerCount = 1,
			.colorAttachmentCount = static_cast<uint32_t>(vulkanFramebuffer->m_Attachments[frameIndex].size()),
			.pColorAttachments = (vulkanFramebuffer->m_Attachments[frameIndex].data()),
		};

		vkCmdBeginRendering(m_CommandBuffer, &renderInfo);
	}

	void VulkanCommandBuffer::EndFramebuffer(uint32_t frameIndex, Ref<Framebuffer>& framebuffer)
	{
		vkCmdEndRendering(m_CommandBuffer);
	}

	void VulkanCommandBuffer::CopyFramebufferToSwapchain(uint32_t frameIndex, Ref<Framebuffer>& framebuffer, Ref<Swapchain> swapchain)
	{
		auto vkFramebuffer = std::reinterpret_pointer_cast<VulkanFramebuffer>(framebuffer);
		auto vkSwapchain = std::reinterpret_pointer_cast<VulkanSwapchain>(swapchain);


		VkImageMemoryBarrier image_memory_barrier
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
			.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			.image = vkFramebuffer->m_Images[frameIndex][0].Image,
			.subresourceRange =
			{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			}
		};


		VkImageMemoryBarrier imagememorybarrier2
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
			.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			.image = vkSwapchain->GetImage(frameIndex),
			.subresourceRange =
			{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			}
		};

		std::array<VkImageMemoryBarrier, 2> barrier = { image_memory_barrier, imagememorybarrier2 };

		vkCmdPipelineBarrier(
			m_CommandBuffer,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,  // srcStageMask
			VK_PIPELINE_STAGE_TRANSFER_BIT, // dstStageMask
			0,
			0,
			nullptr,
			0,
			nullptr,
			barrier.size(), // imageMemoryBarrierCount
			barrier.data() // pImageMemoryBarriers
		);

		VkImageCopy imageCopyRegion{};
		imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageCopyRegion.srcSubresource.layerCount = 1;
		imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageCopyRegion.dstSubresource.layerCount = 1;
		imageCopyRegion.extent.width = 1280;
		imageCopyRegion.extent.height = 720;
		imageCopyRegion.extent.depth = 1;

		vkCmdCopyImage(m_CommandBuffer, vkFramebuffer->m_Images[frameIndex][0].Image, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vkSwapchain->GetImage(frameIndex), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopyRegion);


		image_memory_barrier =
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
			.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			.image = vkFramebuffer->m_Images[frameIndex][0].Image,
			.subresourceRange =
			{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			}
		};

		imagememorybarrier2 = 
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
			.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			.image = vkSwapchain->GetImage(frameIndex),
			.subresourceRange =
			{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			}
		};
		barrier = { image_memory_barrier, imagememorybarrier2 };
		vkCmdPipelineBarrier(
			m_CommandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT,  // srcStageMask
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, // dstStageMask
			0,
			0,
			nullptr,
			0,
			nullptr,
			barrier.size(), // imageMemoryBarrierCount
			barrier.data() // pImageMemoryBarriers
		);

	}

	void VulkanCommandBuffer::Free()
	{
		auto vulkanDevice = std::reinterpret_pointer_cast<VulkanDevice>(m_Specs.device.lock());
		auto vulkanCommandQueue = std::reinterpret_pointer_cast<VulkanCommandQueue>(m_Specs.queue.lock());
		vkFreeCommandBuffers(vulkanDevice->GetHandle(), vulkanCommandQueue->GetHandle(), 1, &m_CommandBuffer);
	}
}