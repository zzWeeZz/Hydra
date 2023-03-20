#include "HYpch.h"
#include "VulkanCommandBuffer.h"
#include <Hydra/API/Vulkan/Backend/VulkanDevice.h>
#include <Hydra/API/Vulkan/CommandSubmiting/VulkanCommandQueue.h>
#include <Hydra/API/Vulkan/Resources/VulkanFramebuffer.h>
#include <Hydra/API/Vulkan/Backend/VulkanSwapchain.h>
#include <Hydra/API/Vulkan/Pipeline/VulkanGraphicsPipeline.h>
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

		vkCmdSetScissor(m_CommandBuffer, 0, 1, &vulkanFramebuffer->GetRect());
		VkViewport viewPort = {};
		viewPort.height = vulkanFramebuffer->GetRect().extent.height;
		viewPort.width = vulkanFramebuffer->GetRect().extent.width;
		viewPort.minDepth = 0;
		viewPort.maxDepth = 1;
		viewPort.x = 0;
		viewPort.y = 0;
		vkCmdSetViewport(m_CommandBuffer, 0, 1, &viewPort);
	}

	void VulkanCommandBuffer::EndFramebuffer(uint32_t frameIndex, Ref<Framebuffer>& framebuffer)
	{
		vkCmdEndRendering(m_CommandBuffer);
	}

	void VulkanCommandBuffer::DrawInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
	{
		vkCmdDraw(m_CommandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void VulkanCommandBuffer::BindGraphicsPipeline(uint32_t frameIndex, Ref<GraphicsPipeline>& pipeline)
	{
		auto vulkanPipeline = std::reinterpret_pointer_cast<VulkanGraphicsPipeline>(pipeline);
		vkCmdBindPipeline(m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline->GetHandle());
	}

	void VulkanCommandBuffer::CopyFramebufferToSwapchain(uint32_t frameIndex, Ref<Framebuffer>& framebuffer, Ref<Swapchain> swapchain)
	{
		auto vkFramebuffer = std::reinterpret_pointer_cast<VulkanFramebuffer>(framebuffer);
		auto vkSwapchain = std::reinterpret_pointer_cast<VulkanSwapchain>(swapchain);


		VkImageMemoryBarrier image_memory_barrier
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT,
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
			.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
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
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // dstStageMask
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