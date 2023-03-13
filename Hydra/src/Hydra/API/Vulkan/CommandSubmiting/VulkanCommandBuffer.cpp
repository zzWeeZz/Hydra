#include "HYpch.h"
#include "VulkanCommandBuffer.h"
#include <Hydra/API/Vulkan/Backend/VulkanDevice.h>
#include <Hydra/API/Vulkan/CommandSubmiting/VulkanCommandQueue.h>
#include <Hydra/API/Vulkan/Resources/VulkanFramebuffer.h>
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

	void VulkanCommandBuffer::BeginFramebuffer(uint32_t frameIndex, Ref<Framebuffer>& framebuffer)
	{
		auto vulkanFramebuffer = std::reinterpret_pointer_cast<VulkanFramebuffer>(framebuffer);

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

	void VulkanCommandBuffer::Free()
	{
		auto vulkanDevice = std::reinterpret_pointer_cast<VulkanDevice>(m_Specs.device.lock());
		auto vulkanCommandQueue = std::reinterpret_pointer_cast<VulkanCommandQueue>(m_Specs.queue.lock());
		vkFreeCommandBuffers(vulkanDevice->GetHandle(), vulkanCommandQueue->GetHandle(), 1, &m_CommandBuffer);
	}
}