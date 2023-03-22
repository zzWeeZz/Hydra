#include "HYpch.h"
#include "VulkanCommandBuffer.h"
#include <Hydra/API/Vulkan/Backend/VulkanDevice.h>
#include <Hydra/API/Vulkan/CommandSubmiting/VulkanCommandQueue.h>
#include <Hydra/API/Vulkan/Resources/VulkanFramebuffer.h>
#include <Hydra/API/Vulkan/Backend/VulkanSwapchain.h>
#include <Hydra/API/Vulkan/Pipeline/VulkanGraphicsPipeline.h>
#include "Hydra/API/Vulkan/VulkanUtils.h"
#include "Hydra/API/Vulkan/Resources/VulkanBuffer.h"
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

		vkCmdPushDescriptorSetKHR = (PFN_vkCmdPushDescriptorSetKHR)vkGetDeviceProcAddr(vulkanDevice->GetHandle(), "vkCmdPushDescriptorSetKHR");
		if (!vkCmdPushDescriptorSetKHR) {
		}
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
		viewPort.height = static_cast<float>(vulkanFramebuffer->GetRect().extent.height);
		viewPort.width = static_cast<float>(vulkanFramebuffer->GetRect().extent.width);
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

	void VulkanCommandBuffer::DrawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
	{
		vkCmdDrawIndexed(m_CommandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}

	void VulkanCommandBuffer::BindGraphicsPipeline(uint32_t frameIndex, Ref<GraphicsPipeline>& pipeline)
	{
		auto vulkanPipeline = std::reinterpret_pointer_cast<VulkanGraphicsPipeline>(pipeline);
		m_CurrentPipeline = vulkanPipeline;
		vkCmdBindPipeline(m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline->GetHandle());
	}

	void VulkanCommandBuffer::BindVertexBuffer(uint32_t frameindex, Ref<Buffer>& buffer)
	{
		auto vulkanBuffer = std::reinterpret_pointer_cast<VulkanBuffer>(buffer);

		VkDeviceSize offset = 0;
		vkCmdBindVertexBuffers(m_CommandBuffer, 0, 1, &vulkanBuffer->GetAllocation().buffer, &offset);
	}

	void VulkanCommandBuffer::BindIndexBuffer(uint32_t frameindex, Ref<Buffer>& buffer)
	{
		auto vulkanBuffer = std::reinterpret_pointer_cast<VulkanBuffer>(buffer);

		VkDeviceSize offset = 0;
		vkCmdBindIndexBuffer(m_CommandBuffer, vulkanBuffer->GetAllocation().buffer, offset, VK_INDEX_TYPE_UINT16);
	}

	void VulkanCommandBuffer::BindConstantBuffer(uint32_t frameindex, uint32_t bindPoint, uint32_t space, Ref<Buffer>& buffer)
	{
		auto vulkanBuffer = std::reinterpret_pointer_cast<VulkanBuffer>(buffer);

		VkDescriptorBufferInfo info = {};
		info.buffer = vulkanBuffer->GetAllocation().buffer;
		info.offset = 0;
		info.range = vulkanBuffer->GetAllocation().sizeOfBuffer;

		VkWriteDescriptorSet set = {};
		set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		set.dstSet = 0;
		set.dstBinding = bindPoint;
		set.descriptorCount = 1;
		set.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		set.pBufferInfo = &info;

		vkCmdPushDescriptorSetKHR(m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_CurrentPipeline.lock()->GetLayout(), space, 1, &set);
	}

	void VulkanCommandBuffer::CopyFramebufferToSwapchain(uint32_t frameIndex, Ref<Framebuffer>& framebuffer, Ref<Swapchain> swapchain)
	{
		auto vkFramebuffer = std::reinterpret_pointer_cast<VulkanFramebuffer>(framebuffer);
		auto vkSwapchain = std::reinterpret_pointer_cast<VulkanSwapchain>(swapchain);

		VkImageSubresourceRange framebufferRange = {};
		framebufferRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		framebufferRange.baseArrayLayer = 0;
		framebufferRange.baseMipLevel = 0;
		framebufferRange.layerCount = 1;
		framebufferRange.levelCount = 1;

		TransitionImageLayout(m_CommandBuffer, vkFramebuffer->m_Images[frameIndex][0].Image, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, framebufferRange);
		TransitionImageLayout(m_CommandBuffer, vkSwapchain->GetImage(frameIndex), VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, framebufferRange);

		VkImageCopy imageCopyRegion{};
		imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageCopyRegion.srcSubresource.layerCount = 1;
		imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageCopyRegion.dstSubresource.layerCount = 1;
		imageCopyRegion.extent.width = vkFramebuffer->GetRect().extent.width;
		imageCopyRegion.extent.height = vkFramebuffer->GetRect().extent.height;
		imageCopyRegion.extent.depth = 1;
		if ((vkSwapchain->GetSpecs().height == vkFramebuffer->GetRect().extent.height) && (vkFramebuffer->GetRect().extent.width == vkSwapchain->GetSpecs().width))
		{
			vkCmdCopyImage(m_CommandBuffer, vkFramebuffer->m_Images[frameIndex][0].Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vkSwapchain->GetImage(frameIndex), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopyRegion);
		}
		TransitionImageLayout(m_CommandBuffer, vkFramebuffer->m_Images[frameIndex][0].Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, framebufferRange);
		TransitionImageLayout(m_CommandBuffer, vkSwapchain->GetImage(frameIndex), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, framebufferRange);
	}

	void VulkanCommandBuffer::Free()
	{
		auto vulkanDevice = std::reinterpret_pointer_cast<VulkanDevice>(m_Specs.device.lock());
		auto vulkanCommandQueue = std::reinterpret_pointer_cast<VulkanCommandQueue>(m_Specs.queue.lock());
		vkFreeCommandBuffers(vulkanDevice->GetHandle(), vulkanCommandQueue->GetHandle(), 1, &m_CommandBuffer);
	}
}